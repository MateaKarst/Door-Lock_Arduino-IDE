//// Libraries ////
// These are the necessary libraries for my Arduino product
#include <Keypad.h>             // Enables the use of a keypad
#include <Wire.h>               // Allows communication over the I2C backpack on the led
#include <LiquidCrystal_I2C.h>  // Helps to control the I2C LCD display
#include <Servo.h>              // Allows to control the servo motor
#include <EEPROM.h>             // Helps to read and write data to the memory


//// Defining and Decalring ////
Servo myservo;                       // Declares a Servo object named myservo which will be used to control the servo motor
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Initializes the LCD display (I2C address 0x27, 16 columns, and 2 rows)

#define Password_Length 7      // Defines Password_Length to specify the maximum length of the password
#define EEPROM_Flag_Address 0  // During initialization, if a password has been previously set, the system assumes that it has already been configured;
// otherwise, the user is prompted to set up a password before enabling the door lock functionality


//// Storing and Tracking Information ////
int pos = 0;                              // Keeps track of the position of a number/letter in an array
char Data[Password_Length];               // Used to store keyed in data (password digits)
char Master[Password_Length] = "010702";  //  Stores the master password
byte data_count = 0;                      // Counts the number of characters entered by the user
char customKey;                           // Helps to store a single key pressed by the user on the keypad


//// Setting up the Keypad ////
const byte ROWS = 4;  // States the number of rows
const byte COLS = 3;  // States the number of columns
char keys[ROWS][COLS] = {
  { '1', '2', '3' },
  { '4', '5', '6' },
  { '7', '8', '9' },
  { '*', '0', '#' }
};                 // Defines the layout of rows against columns
bool door = true;  // This is a boolean which will help control the state of the door (locked or unlocked)

byte rowPins[ROWS] = { 1, 2, 3, 4 };  // Uno arduino digital pins 1 to 4
byte colPins[COLS] = { 5, 6, 7 };     // Uno arduino digital pins 5 to 7 are mapping the columns

Keypad customKeypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);  //Customising the keypad
// customKeypad is the given class
// rowPins and colPins are the respective pins that set the layout of the ROWS and COLS respectively


//// Initial Setup (Powering on / off) ////
void setup() {
  // Is called when the Arduino board is powered on or reset
  if (!i2CAddrTest(0x27)) {                // Checking the I2C led address
    lcd = LiquidCrystal_I2C(0x3F, 16, 2);  // My specific led type
  }
  lcd.init();       // Initializes the LCD display
  lcd.backlight();  // Turns on the backlight of the LCD display

  if (EEPROM.read(EEPROM_Flag_Address) != 1) {  // If the value is not equal to 1, then the code below is executed.
    //(Also means that a password has not been inputed yet so the door is locked)
    lcd.print("Locker Door !");            // Display text on led
    lcd.setCursor(0, 1);                   // First row, second column
    lcd.print("-- Arduino :D --");         // Display text on led
    delay(3000);                           // Stays in this state for a few seconds
    lcd.clear();                           // Clear the displayed text
    EEPROM.write(EEPROM_Flag_Address, 1);  // Indicates a 1 on the memory to show that the above code has already ben run
  }

  myservo.attach(9);  // My servo motor is on pin 9 of the Arduino uno board
  ServoClose();       // Sets the initial position of the servo motor on a closed door
}


//// Checks if the Door is Opened/Closed and Updates the States ////
void loop() {                           // Loop function (repetition)
  if (door == 0) {                      // If the door is closed (boolean value)
    customKey = customKeypad.getKey();  // Then the keys pressed on the keypad is given to the custom key

    if (customKey == '#') {         // If the user presses #
      lcd.clear();                  // Clear the led displayed text
      ServoClose();                 // Close the door
      lcd.print("Door is closed");  // Display the text telling that the door is closed
      delay(3000);                  // A short time delay
      door = 1;                     // Updates the state of the door as closed
    }
  } else {
    Open();  // Open the door when door is set to value 1
  }
}


//// Clearing the Data ////
void clearData() {         // Clear the array(void returns no value)
  while (data_count != 0)  // While the array count is not 0
  {
    Data[data_count--] = 0;  // Gives the data array a 0
  }
  return;  // Helps to control calling the code (nothing is returned as there is no value)
}


//// Servo Motor Positioning ////
void ServoOpen() {                    // Open door
  for (pos = 90; pos >= 0; pos -= 5)  // States the servo motor's opened angle position
  {
    myservo.write(pos);  // Sets the servo motor into position
    delay(15);           // Small delay to allow a smooth movement for the servo motor
  }
}

void ServoClose() {                   // Close door
  for (pos = 0; pos <= 90; pos += 5)  // States the servo motor's closed angle position
  {
    myservo.write(pos);  // Sets the servo motor into position
    delay(15);           // Small delay to allow a smooth movement for the servo motor
  }
}


//// Opening the Door with Correct Password / Does Not Open Door for Incorrect Passwords ////
void Open() {                   // Handles the execution of opening the door
  lcd.setCursor(0, 0);          // Sets the led position to first column and row
  lcd.print("Enter Password");  // Displayes text on led

  customKey = customKeypad.getKey();  // Reads the keys pressed on the keypad
  if (customKey) {                    // If it is not null then
    Data[data_count] = customKey;     // It gets stored as Data
    lcd.setCursor(data_count, 1);     // Sets the led position to the corresponding column and second row
    lcd.print(Data[data_count]);      // Displays the pressed key onto the led
    data_count++;                     // Helps to allow the next keys to be pressed
  }

  if (data_count == Password_Length - 1) {  // If the data_count reaches the maximum length
    if (!strcmp(Data, Master)) {            // strcmp function is used to compate the Data and Master values
      lcd.clear();                          // Clears the led text display
      ServoOpen();                          // If results match, open the door
      lcd.print("Door is Open");            // Displays opened door text
      door = 0;                             // Boolean for door open
    } else {
      lcd.clear();                  // Clears the led text display
      lcd.print("Wrong Password");  // If results don't match,
      // then the door stays closed and the led displays wron password test
      delay(1000);  // Small delay
      door = 1;     // Boolean for door closed
    }
    clearData();  // Clears the data for the next data entry
  }
}


//// Checking if the I2C Address is Connected Correctly ////
bool i2CAddrTest(uint8_t addr) {  // Checks if there is an I2C led address
  Wire.begin();                   // Allows for the communication of the Arduino uno board to the led

  Wire.beginTransmission(addr);       // Starts the data transfers
  if (Wire.endTransmission() == 0) {  // Checks if it was successful (0 means successfull without any errors)
    return true;                      // Transfer was successful
  }
  return false;  // An error occured
}
