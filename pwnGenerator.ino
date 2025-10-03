/**
 * @file esc_controller.ino
 * @brief Manual ESC/Servo PWM Signal Generator
 * * This sketch generates a standard 50Hz PWM signal, typical for controlling hobby
 * Electronic Speed Controllers (ESCs) and servos. It allows for manual control
 * of the pulse width via the Arduino Serial Monitor.
 * * The standard for most ESCs is:
 * - 1000 microseconds (µs) pulse = 0% throttle (motor stop)
 * - 1500 microseconds (µs) pulse = 50% throttle
 * - 2000 microseconds (µs) pulse = 100% throttle
 * * IMPORTANT: Always remove propellers from your motor when testing!
 */

// --- Pin Definitions ---
const int ESC_PIN = 9; // The Arduino pin connected to the ESC's signal wire.

// --- PWM Signal Configuration ---
const int PWM_FREQUENCY_HZ = 50; // Standard frequency for ESCs is 50Hz.
const int PWM_PERIOD_MS = 1000 / PWM_FREQUENCY_HZ; // Total period of one cycle (20ms).

// --- Throttle Configuration (in microseconds) ---
const int MIN_THROTTLE_PULSE = 1000; // Pulse width for 0% throttle.
const int MAX_THROTTLE_PULSE = 2000; // Pulse width for 100% throttle.

// --- Global Variables ---
// Holds the current pulse width value. Initialized to the minimum throttle
// to ensure the motor doesn't start unexpectedly.
int currentPulseWidth = MIN_THROTTLE_PULSE;

/**
 * @brief Runs once when the Arduino is powered on or reset.
 */
void setup() {
  // Set the ESC signal pin as an output.
  pinMode(ESC_PIN, OUTPUT);
  
  // Start serial communication for user input.
  Serial.begin(9600);
  
  // --- ESC Initialization Sequence ---
  // Most ESCs require a "low throttle" signal at startup to arm themselves.
  // This prevents the motor from spinning unexpectedly if the throttle is high
  // when the ESC is powered on. We send this signal for 3 seconds.
  Serial.println("Initializing ESC: Sending low throttle signal...");
  
  unsigned long armingStartTime = millis();
  while (millis() - armingStartTime < 3000) {
    sendESCSignal(MIN_THROTTLE_PULSE);
  }
  
  Serial.println("ESC armed and ready.");
  Serial.println("Enter a value between 1000 and 2000 to set throttle.");
}

/**
 * @brief The main loop that runs repeatedly after setup().
 */
void loop() {
  // Check if the user has sent a new command over the serial port.
  if (Serial.available() > 0) {
    handleSerialInput();
  }
  
  // Continuously send the PWM signal with the current throttle setting.
  // This is required by the ESC; if the signal stops, the motor will stop.
  sendESCSignal(currentPulseWidth);
}

/**
 * @brief Reads, validates, and applies user input from the Serial Monitor.
 */
void handleSerialInput() {
  // Read the incoming text until a newline character is received.
  String input = Serial.readStringUntil('\n');
  
  // Convert the text to an integer.
  int newPulseWidth = input.toInt();
  
  // Validate that the new value is within the safe operating range.
  // We also check newPulseWidth is not 0, which toInt() returns on failure.
  if (newPulseWidth >= MIN_THROTTLE_PULSE && newPulseWidth <= MAX_THROTTLE_PULSE) {
    currentPulseWidth = newPulseWidth; // Update the throttle setting.
    Serial.print("Throttle set to: ");
    Serial.println(currentPulseWidth);
  } else if (newPulseWidth != 0) {
    // Inform the user if the value is out of range.
    Serial.print("Invalid value: ");
    Serial.print(newPulseWidth);
    Serial.println(". Please use a value between 1000 and 2000.");
  }
}

/**
 * @brief Generates a single PWM pulse on the ESC_PIN.
 * * This function manually creates the PWM signal using delays. This is known as
 * "bit-banging". It's simple and doesn't require using hardware timers.
 * * @param pulseWidth The duration in microseconds for the pulse to be HIGH.
 */
void sendESCSignal(int pulseWidth) {
  // Ensure the pulse width is within the total period.
  int pulseWidthUs = constrain(pulseWidth, MIN_THROTTLE_PULSE, MAX_THROTTLE_PULSE);
  
  // The total period in microseconds.
  long periodUs = PWM_PERIOD_MS * 1000;
  
  // 1. Set the pin HIGH to start the pulse.
  digitalWrite(ESC_PIN, HIGH);
  
  // 2. Wait for the desired pulse duration.
  delayMicroseconds(pulseWidthUs);
  
  // 3. Set the pin LOW to end the pulse.
  digitalWrite(ESC_PIN, LOW);
  
  // 4. Wait for the remainder of the period to maintain the 50Hz frequency.
  delayMicroseconds(periodUs - pulseWidthUs);
}
