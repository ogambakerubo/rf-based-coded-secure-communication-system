// Include AES Encryption Library
#include "AESLib.h"
// Include RadioHead Amplitude Shift Keying Library
#include <RH_ASK.h>
// Include dependant SPI Library
#include <SPI.h>

// Create Amplitude Shift Keying Object
RH_ASK rf_driver;

void setup() {
  // put your setup code here, to run once:
  // Initialize ASK Object
  rf_driver.init();
  // Setup Serial Monitor
  Serial.begin(57600);
}

void loop() {
  // put your main code here, to run repeatedly:
  uint8_t key[17]; // 16 characters + 1 for null character
  char text[17]; // 16 characters + 1 for null character

  if (Serial.available() > 0) {
    String msgString = Serial.readString(); // Read data string
    String strkey;
    String message;

    // Split string into two values
    for (int i = 0; i < msgString.length(); i++) {
      if (msgString.substring(i, i + 1) == "|") {
        strkey = msgString.substring(0, i);
        message = msgString.substring(i + 1);

        break;
      }
    }
    // Assign values to key, text
    strkey.toCharArray(key, strkey.length() + 1);
    message.toCharArray(text, message.length() + 1);
    // Encrypt message using the key with AES-128 ECB mode
    aes128_enc_single(key, text);
  }
  // Send output character
  rf_driver.send((uint8_t *)text, strlen(text));
  rf_driver.waitPacketSent();
  // Print encrypted text to Serial Monitor
  Serial.print("encrypted:");
  Serial.println(text);
  delay(1000);
}
