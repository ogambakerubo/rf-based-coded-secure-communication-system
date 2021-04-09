// Import ControlP5, Serial, Regex libraries
import processing.serial.*;
import controlP5.*;
import java.util.regex.*;
// Create Serial object
Serial port;
// Create ControlP5 object
ControlP5 controlP5;
// Create Textfield objects
Textfield messageField;
Textfield keyField;
// Create Textarea objects
Textarea errorField;
Textarea cipherText;
// Create PFont objects
PFont font;
PFont smallerFont;

String encryptionKey;
String message;
String errorMessage;
String dataSent;
String inBuffer = null;

int lf = 10; // Linefeed in ASCII

// Setup the processing programme
void setup() {
    size(700, 800); // Window size (width, height)
    font = createFont("Calibri", 30); // Set font type, sizes
    smallerFont = createFont("Calibri", 20);
    
    try {
        port = new Serial(this, Serial.list()[0], 57600); // Set Serial to first availble port, baudrate 57600
        port.clear(); // Clear serial
        inBuffer = port.readStringUntil(lf); // Throw out the first reading, in case we started reading 
        inBuffer = null; // in the middle of a string from the sender.
    } catch(Exception e) { // In case of unavailable Serial port
        errorMessage = "Please connect the device to the USB port and relaunch the application";
        println(errorMessage); // Print error message and error type in the Terminal
        println(e);
    }
    
    controlP5 = new ControlP5(this);
    
    keyField = controlP5
       .addTextfield("key") // Set name of textfield in quotes
       .setPosition(100, 100) // Set position (x, y)
       .setSize(500, 50) // Set size (width, height)
       .setFont(font) // Set font
       .setAutoClear(false); // Remove auto clear on ENTER
    messageField = controlP5
       .addTextfield("message") // Set name of textfield in quotes
       .setPosition(100, 200) // Set position (x, y)
       .setSize(500, 50) // Set size (width, height)
       .setFont(font) // Set font
       .setAutoClear(false); // Remove auto clear on ENTER
    // Set textareas
    errorField = controlP5
       .addTextarea("error") // Set name of textarea in quotes
       .setPosition(100, 300) // Set position (x, y)
       .setSize(500, 50) // Set size (width, height)
       .setFont(smallerFont); // Set font
    cipherText = controlP5
       .addTextarea("ciphertext") // Set name of textarea in quotes
       .setPosition(100, 600) // Set position (x, y)
       .setSize(500, 100) // Set size (width, height)
       .setFont(smallerFont) // Set font
       .setColorBackground(color(21,27,84));
    // Set label for textarea "cihertext"
    controlP5.addTextlabel("cipherLabel") // Set name of textlabel in quotes
       .setPosition(100, 700) // Set position (x, y)
       .setSize(500, 50) // Set size (width, height)
       .setFont(font) // Set font
       .setValue("CIPHERTEXT"); // Set textlabel value in quotes
    // Set buttons which call functions Send, Clear when clicked
    controlP5.addButton("Send") // Set name of button in quotes
       .setPosition(300, 400) // Set position (x, y)
       .setSize(100, 50) // Set size (width, height)
       .setFont(font); // Set font
    controlP5.addButton("Clear") // Set name of button in quotes
       .setPosition(300, 500) // Set position (x, y)
       .setSize(100, 50) // Set size (width, height)
       .setFont(font); // Set font
}

void draw() {
    try {
        background(180);
        while(port.available() > 0) {
            inBuffer = port.readStringUntil(lf); // Read string from buffer until new line feed   
            if (inBuffer != null) {
                println(inBuffer); // Display encrypted message that is currently transmitting
                cipherText.setText(inBuffer);
            }
        }       
    } catch(Exception e) { // In case of unavailable Serial port
        errorMessage = "Please connect the device to the USB port and relaunch the application";
        errorField.setText(errorMessage); // Display error message in error field
    }
}
//Process message to be sent
void Send() {
    encryptionKey = keyField.getText(); // Set strings from the textfields entries
    message = messageField.getText();
    // Check that the key, message are 16 characters long
    if (encryptionKey.length() == 16 && message.length() == 16) {
        // Check that the encryption key only contains 16 numbers
        if (Pattern.matches("[0-9]{16}", encryptionKey)) {
            dataSent = encryptionKey + "|" + message; // Data sent has the separator "|"
            port.write(dataSent);
            errorField.clear();
        } else { // Error message
            errorMessage = "Key must be an unsigned whole number";
            errorField.setText(errorMessage);
        }    
    } else { // Error message
        errorMessage = "Key must be 16 characters long & Message must be 16 characters long";
        errorField.setText(errorMessage);
    }
}
//Clear the key,message and error fields
void Clear() {
    keyField.clear();
    messageField.clear();
    errorField.clear();
}
