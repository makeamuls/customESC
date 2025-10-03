# Arduino ESC PWM Controller

A simple Arduino sketch for manually controlling a standard hobby brushless motor Electronic Speed Controller (ESC) or servo via the Serial Monitor. This is useful for testing custom motors, calibrating ESCs, or understanding how PWM control works.

## Description

This code generates a 50Hz PWM signal on a specified pin. The pulse width of this signal, which determines the motor's speed, can be set by sending a number between 1000 and 2000 through the Arduino's Serial Monitor.

* **1000 µs pulse**: 0% throttle (motor stopped)
* **2000 µs pulse**: 100% throttle (motor at full speed)

The sketch includes a standard ESC arming sequence in the `setup()` function to ensure safe operation upon startup.

## Hardware Requirements

* An Arduino board (Uno, Nano, etc.)
* A standard 3-wire brushless ESC
* A brushless motor compatible with your ESC
* A power supply for the ESC and motor (e.g., a LiPo battery)
* A computer with the Arduino IDE installed

**⚠️ Safety First!** Always remove propellers or any other attachments from your motor before testing. Brushless motors can be very powerful and start unexpectedly.

## Wiring

1.  **ESC Signal:** Connect the ESC's signal wire (usually white or yellow) to **Pin 9** on the Arduino.
2.  **Ground:** Connect the ESC's ground wire (usually black or brown) to any **GND** pin on the Arduino. This is crucial for a stable signal.
3.  **Power:**
    * Do **NOT** power the Arduino through the ESC's 5V (BEC) wire (usually red) unless you are sure your setup requires it. It's often safer to power the Arduino via its USB connection.
    * Connect your main power supply (battery) to the ESC's power input leads.
    * Connect the three motor phase wires from the ESC to the motor.

## How to Use

1.  **Upload the Code:** Open the `esc_controller.ino` file in the Arduino IDE and upload it to your board.
2.  **Open Serial Monitor:** With the Arduino connected to your computer, open the Serial Monitor (Tools > Serial Monitor or `Ctrl+Shift+M`).
3.  **Set Baud Rate:** Make sure the baud rate in the Serial Monitor is set to **9600**.
4.  **Arm the ESC:** The code will automatically run the arming sequence. You should see "Initializing ESC..." followed by "ESC armed and ready." in the monitor. You might also hear a series of beeps from the motor as the ESC initializes.
5.  **Control the Motor:**
    * Type `1000` into the input box and press Enter. The motor should be stopped.
    * Slowly increase the value (e.g., `1100`, `1200`, ...) and press Enter each time to see the motor spin up.
    * Type `2000` for full throttle.
    * Type `1000` to stop the motor.

That's it! You now have direct manual control over your brushless motor.
