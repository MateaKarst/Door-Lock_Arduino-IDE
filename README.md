# Door-Lock_Arduino-IDE
The Locker Door project is an Arduino-based security system designed to control the opening and closing of a locker door via a password input using a keypad. It features a servo motor for physical locking/unlocking and an I2C LCD display for feedback and messages. The system is simple, yet effective, with security features such as password entry, error handling, and locking functionality.

## Components Used
Arduino Uno - Acts as the microcontroller that controls all components.
Battery - Powers the Arduino and connected components.
Breadboard - Organizes the electrical components.
I2C LCD Display - Used for visual feedback and user interaction.
Keypad - Matrix keypad for password input.
Servo Motor - Opens and closes the locker door.
Jumper Wires - Used to connect the components.

## Libraries Used
Keypad Library (Keypad.h) - To interact with the keypad and read user input.
Wire Library (Wire.h) - Enables communication with the I2C LCD.
LiquidCrystal_I2C Library (LiquidCrystal_I2C.h) - Controls the I2C LCD display.
Servo Library (Servo.h) - Manages the servo motor for door opening and closing.
EEPROM Library (EEPROM.h) - Stores and retrieves password data from memory.
