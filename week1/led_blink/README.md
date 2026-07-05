# IoT LED Blink Project

## Hardware Required
* Arduino Uno / ESP32 Board
* 1x LED (Built-in Pin 13 or external)
* 1x 220-ohm Resistor (if using external LED)
* 1x 10k-ohm Potentiometer
* Breadboard and Jumper Wires

## Circuit Diagram Description
* **LED:** Connected to Digital Pin 13 through a 220-ohm current-limiting resistor to Ground.
* **Potentiometer:** Pin 1 (Left) connected to 5V, Pin 2 (Middle/Wiper) connected to Analog Input Pin A0, and Pin 3 (Right) connected to Ground.

## How to Upload Code (Step-by-Step)
1. Open the Arduino IDE.
2. Open the file `led_blink.ino`.
3. Connect your Arduino board to the computer via USB.
4. Select the correct Board and Port from the **Tools** menu.
5. Click on the **Upload** button (Right arrow icon).

## Expected Output
The LED on Pin 13 will blink. Rotating the potentiometer knob will modify the resistance, changing the delay and altering the blinking speed in real-time. The serial monitor will output the current blink count at a 9600 baud rate.

## Troubleshooting Tips
1. **Board not detected:** Check the USB cable connection and verify if the correct COM port is selected in Arduino IDE.
2. **Serial Monitor showing garbage values:** Ensure that the Serial Monitor baud rate matches `9600`.
3. **LED not blinking:** Double-check the polarity of the external LED (longer leg goes to Pin 13).
