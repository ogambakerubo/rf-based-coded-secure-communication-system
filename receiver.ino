// Include Wire Library for I2C
#include <Wire.h>
// Include NewLiquidCrystal Library for I2C
#include <LiquidCrystal_I2C.h>
// Include RadioHead Amplitude Shift Keying Library
#include <RH_ASK.h>
// Include dependant SPI Library
#include <SPI.h>
// Include the Keypad library
#include <Keypad.h>
// Include AES Encryption Library
#include "AESLib.h"

// Length of password + 1 for null character
#define Password_Length 17
// Character to hold password input
char Data[Password_Length];
// Counter for character entries
byte data_count = 0;
// Character to hold key input
char customKey;

// Constants for row and column sizes
const byte ROWS = 4;
const byte COLS = 3;

// Array to represent keys on keypad
char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};

// Connections to Arduino
byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3};

// Create keypad object
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

// Define LCD pinout
const int  en = 2, rw = 1, rs = 0, d4 = 4, d5 = 5, d6 = 6, d7 = 7, bl = 3;

// Define I2C Address - change if reqiuired
const int i2c_addr = 0x3F;

LiquidCrystal_I2C lcd(i2c_addr, en, rw, rs, d4, d5, d6, d7, bl, POSITIVE);

// Create Amplitude Shift Keying Object
RH_ASK rf_driver;

void setup() {
  // put your setup code here, to run once:
  // Initialize ASK Object
  rf_driver.init();
  // Set display type as 20 char, 4 rows
  lcd.begin(20, 4);
  //  Setup Serial Monitor
  Serial.begin(57600);
  // Print on first row
  lcd.setCursor(0, 0);
  lcd.print("Up and Running!");

  // Wait 1 second
  delay(1000);

  // Print on second row
  lcd.setCursor(0, 1);
  lcd.print("Setting up ...");

  // Wait 4 seconds
  delay(4000);

  // Clear the display
  lcd.clear();
}

void loop() {
  // put your main code here, to run repeatedly:
  // Set buffer to size of expected message
  uint8_t buf[16];
  uint8_t buflen = sizeof(buf);
  // Initialize LCD and print
  lcd.setCursor(0, 0);
  lcd.print("Enter Password:");
  // Look for keypress
  customKey = customKeypad.getKey();
  if (customKey) {
    // Enter keypress into array and increment counter
    Data[data_count] = customKey;
    lcd.setCursor(data_count, 1);
    lcd.print(Data[data_count]);
    data_count++;
  }

  if (customKey == '*') {
    // '*' keypress to clear the LCD display, Data
    clearData();
  }

  // See if we have reached the password length
  if (data_count == Password_Length - 1) {
    delay(1000);
    lcd.setCursor(0, 3);
    lcd.print("Processing ...");
    // Check if received packet is correct size
    if (rf_driver.recv(buf, &buflen)) {
      uint8_t * key = (uint8_t *)Data;
      char * text = (char *)buf;
      Serial.print("encrypted:");
      Serial.println(text);
      Serial.print("key:");
      Serial.println((char *)key);
      // Message received is decrypted, displayed
      aes128_dec_single(key, text);
      Serial.print("decrypted:");
      Serial.println(text);
      // Clear the LCD display
      lcd.clear();
      lcd.setCursor(0, 0);
      // Display decrypted text
      lcd.print("Message Received: ");
      lcd.setCursor(0, 1);
      lcd.print(text);
      // Wait 30 seconds
      delay(30000);
      // Clear data and LCD display
      clearData();
    }
  } else if (data_count >= Password_Length) {
    // Clear LCD dispaly, Data if password exceeds 16 characters
    clearData();
  }
}

void clearData() {
  // Clear LCD display
  lcd.clear();

  // Go through array and clear data
  while (data_count != 0) {
    Data[data_count--] = 0;
  }
  return;
}
