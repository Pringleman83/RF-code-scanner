#include <Wire.h>
#include <RCSwitch.h>
#include <SPI.h>
#include "SSD1306Ascii.h"
#include "SSD1306AsciiAvrI2c.h"

#define SCREEN_WIDTH 128  // Oled display width 
#define SCREEN_HEIGHT 64 //  OIled display hgeight 

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define I2C_ADDRESS 0x3C //0x3C or 0x3D depending on Oled configuration
SSD1306AsciiAvrI2c display;



// Create new RCSwitch instances
RCSwitch mhz433r = RCSwitch();
RCSwitch mhz433t = RCSwitch();

// Create the RF code type
typedef struct{
  int number;
  int frequency;
  unsigned long value;
  int bitlength;
  int protocol;
  int key; // Not sure if this is the same as bitlength
} rfCode;


// Create array to save sniffed codes
const int codeMax = 30; // maximum number of sniffed codes
rfCode sniffedCodes[codeMax];

// Create array to save stored codes
const int storedMax = 10; // maximum number of stored codes
rfCode storedCodes[storedMax];


unsigned long debounceDelay = 50;

const int selButton = 4;
int selButtonState;
int lastSelButtonState;
unsigned long lastSelDebounceTime = 0;
int selReading;

const int navButton = 5;
int navButtonState;
int lastNavButtonState;
unsigned long lastNavDebounceTime = 0;
int navReading;
int recCount = 0; // Not 0 when we have a test code set in the sniffer loop

int option = 1;
int numberOfOptions = 4;

//////////////////////////////////////////////////////////////////////////////////////////////////

void transmit(int protocol, int code, int bitlength){


  
  //display.setTextColor(WHITE);
  display.clear();
  display.setCursor(0,1);
  display.print("Transmitting...");
  
  mhz433t.setProtocol(protocol);
  mhz433t.send(code, bitlength);
  delay(125);
  mhz433r.resetAvailable();
}

//////////////////////////////////////////////////////////////////////////////////////////////////

void sniff() {
  boolean newCode = true;
  /* Start of test code
  display.setCursor(0,1);
  display.print(sniffedCodes[0].frequency);
  display.setCursor(0,2);
  display.print(sniffedCodes[0].value);
  display.setCursor(0,3);
  display.print(sniffedCodes[0].protocol);
  // End of test code
  */

  // If a signal is received
  if (mhz433r.available()) {
    // Hold received code
    unsigned long value = mhz433r.getReceivedValue();
    int bitlength = mhz433r.getReceivedBitlength();
    int protocol = mhz433r.getReceivedProtocol();
    int key = 24;
    
    // Set received notification in top right corner of screen
    display.setCursor(17,3);
    display.print("Rec");
    
    // Check if the code is already stored
    for (int sniffedCode = 0; sniffedCode < codeMax; sniffedCode++){
      // If the value of the current sniffed code is 0, we have reached the end of stored codes
      // No need to continue the for loop
      if (sniffedCodes[sniffedCode].value == 0){
        break;
      }

      // Check if the code in this position of the list matches the received code
      // If it does, mark it as not being a new code and break out of the for loop
      // otherwise mark it as a new code
      if (
      sniffedCodes[sniffedCode].value == value &&
      sniffedCodes[sniffedCode].bitlength == bitlength &&
      sniffedCodes[sniffedCode].protocol == protocol
      ){
        newCode = false;
        break;
      }
      else{
        newCode = true;
      }
    }

    // Save to list if new code received
    if (newCode){
      // Save Value, Bitlength and Protocol to list
      sniffedCodes[recCount].number = recCount + 1;
      sniffedCodes[recCount].value = value;
      sniffedCodes[recCount].bitlength = bitlength;
      sniffedCodes[recCount].protocol = protocol;
      sniffedCodes[recCount].key = 24;
  
      // Display Value, Bitlength and Protocol
      display.setCursor(0,0);
      display.print("                    ");
      display.setCursor(0,0);
      display.print(recCount);
      display.print(") ");
      
      display.print(mhz433r.getReceivedValue() );
      display.setCursor(0,1);
      display.print("                    ");
      
      display.setCursor(0,1);
      display.print(mhz433r.getReceivedBitlength() );
      display.print(" bit");
      
      display.setCursor(0,2);
      display.print("                ");
      
      display.setCursor(0,2);
      display.print("Protocol: ");
      display.print(mhz433r.getReceivedProtocol() );
      

      // Increment the code counter
      recCount ++;
    }

    // Clear received notification
    delay(100);
    display.setCursor(17,3);
    display.print("   ");
    
    
    // Set receiver ready to receive
    mhz433r.resetAvailable();
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

void sniffer() {
  /*
  // Test code (add a dummy sniffed code)
  sniffedCodes[0].number = 1;
  sniffedCodes[0].bitlength = 24;
  sniffedCodes[0].frequency = 433;
  sniffedCodes[0].value = 5393;
  sniffedCodes[0].protocol = 1;
  sniffedCodes[0].key = 24;
  // End of test code
  */
  
  display.clear();
  display.setCursor(0,0);
  display.print("Sniffing");
  display.setCursor(0,7);
  display.print("                Back");
  
  while (true) {
    sniff();
    selReading = digitalRead(selButton);
    
    if (selReading != lastSelButtonState) {
    lastSelDebounceTime = millis();
    }
    
    if ((millis() - lastSelDebounceTime) > debounceDelay) {
      
      if (selReading != selButtonState) {
        selButtonState = selReading;
  
        if (!selButtonState) {
          break;
        }
      }
    }
    lastSelButtonState = selReading;
  }
  display.clear();
}
  
///////////////////////////////////////////////////////////////////////////////////////////////////////

void runSniffedCodes() {
  boolean validCode = true;
  int code = 0;
  int oldCode = 1;
  display.clear();
  display.setCursor(0,0);
  display.print("Sniffed Codes");
  
  while (true) {
    navReading = digitalRead(navButton);
    
    if (navReading != lastNavButtonState) {
    lastNavDebounceTime = millis();
    }
    
    if ((millis() - lastNavDebounceTime) > debounceDelay) {
      
      if (navReading != navButtonState) {
        navButtonState = navReading;

        // Navigate through codes if button pressed
        if (!navButtonState) {
          if (validCode){
            code++;
          }
          else{
            code = 0;
          }
        }
      }
    }

    // Clear the screen if the code has changed
    if (code != oldCode){
      display.clear();
    }
    lastNavButtonState = navReading;

    // If end of list
    if (sniffedCodes[code].number == 0){
      // If there are no stored codes, advise the user.
      if (sniffedCodes[0].number == 0){
        display.setCursor(0,0);
        display.print("No sniffed codes");
        
        validCode = false;
      }
      else{
        display.setCursor(0,0);
        display.print("End of sniffed codes");
        
        validCode = false;
      }
      // If there are codes in the list give the user the option of viewing again
      if (sniffedCodes[0].number != 0){
        display.setCursor(0,7);
        display.print("Beginning       Back");
        
      }
      // If not, just give the user the option to go back
      else{
        display.setCursor(55,7);
        display.print("Back"); 
      }
    }
      else{
      display.setCursor(0,0);
      display.print(sniffedCodes[code].number);
      display.print(") ");
      display.print(sniffedCodes[code].value);
      display.setCursor(0,1);
      display.print("Bitlength: ");
      display.print(sniffedCodes[code].bitlength);
      display.setCursor(0,2);
      display.print("Protocol: ");
      display.print(sniffedCodes[code].protocol);
      display.setCursor(0,7);
      display.print("Next            Send");
      validCode = true;
    }
    
    selReading = digitalRead(selButton);
    
    if (selReading != lastSelButtonState) {
    lastSelDebounceTime = millis();
    }
    
    if ((millis() - lastSelDebounceTime) > debounceDelay) {
      
      if (selReading != selButtonState) {
        selButtonState = selReading;
  
        if (!selButtonState) {
          if (validCode){
            transmit(sniffedCodes[code].protocol, sniffedCodes[code].value, sniffedCodes[code].bitlength);
            display.clear();
          }
          if (!validCode){
            display.clear();
            break;
          }
        }
      }
    }
    lastSelButtonState = selReading;
    oldCode = code;
  }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

void runStoredCodes() {
  boolean validCode = true;
  int code = 0;
  int oldCode = 1;
  display.clear();
  display.setCursor(0,0);
  display.print("Stored Codes");
  
  while (true) {
    navReading = digitalRead(navButton);
    
    if (navReading != lastNavButtonState) {
    lastNavDebounceTime = millis();
    }
    
    if ((millis() - lastNavDebounceTime) > debounceDelay) {
      
      if (navReading != navButtonState) {
        navButtonState = navReading;

        // Navigate through codes if button pressed
        if (!navButtonState) {
          if (validCode){
            code++;
          }
          else{
            code = 0;
          }
        }
      }
    }

    // Clear the screen if the code has changed
    if (code != oldCode){
      display.clear();
    }
    lastNavButtonState = navReading;

    // If end of code list
    if (storedCodes[code].number == 0){
      // If there are no stored codes, advise the user.
      if (storedCodes[0].number == 0){
        display.setCursor(0,0);
        display.print("No stored codes");
        
        validCode = false;
      }
      else{
        display.setCursor(0,0);
        display.print("End of stored codes");
        
        validCode = false;
      }
      // If there are codes in the list give the user the option of viewing again
      if (storedCodes[0].number != 0){
        display.setCursor(0,7);
        display.print("Beginning       Back");
        
      }
      // If not, just give the user the option to go back
      else{ 
        display.setCursor(16,7);
        display.print("Back");
        
      }
    }
      else{  
      display.setCursor(0,0);
      display.print(storedCodes[code].number);
      display.print(") ");
      display.print(storedCodes[code].value);
      display.setCursor(0,1);
      display.print("Bitlength: ");
      
      display.print(storedCodes[code].bitlength);
      
      display.setCursor(0,2);
      display.print("Protocol: ");
      display.print(storedCodes[code].protocol);
      display.setCursor(0,7);
      display.print("Next            Send");
      validCode = true;
    }
    
    selReading = digitalRead(selButton);
    
    if (selReading != lastSelButtonState) {
    lastSelDebounceTime = millis();
    }
    
    if ((millis() - lastSelDebounceTime) > debounceDelay) {
      
      if (selReading != selButtonState) {
        selButtonState = selReading;
  
        if (!selButtonState) {
          if (validCode){
            transmit(storedCodes[code].protocol, storedCodes[code].value, storedCodes[code].bitlength);
            display.clear(); 
          }
          if (!validCode){
            display.clear();
            break;
          }
        }
      }
    }
    lastSelButtonState = selReading;
    oldCode = code;
  }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

void bruteForce() {
  //display.setTextColor(WHITE);
  display.clear();
  display.setCursor(0,0);
  display.print("Brute Force");
  
  display.setCursor(47,52);
  display.print("Stop");
  
  //i loop for (int bitlength = 0; bitlength < 100; bitlenght++){

    int bitlength = 24; // Set to 24 as believed to be likely bitlength at this stage.
    int protocol = 1; // Set to 1 as believed to be the most common protocol.
    
    for(int code = 0; code < 9999; code++){
      display.setCursor(0,0);
      display.print("Bitlength: ");
      display.print(bitlength);
      display.setCursor(0,1);
      display.print("Code: ");
      display.print(code);
      display.setCursor(0,2);
      display.print("Protocol: ");
      display.print(protocol);
      
      mhz433t.setProtocol(protocol);
      mhz433t.send(code, bitlength);
      selReading = digitalRead(selButton);
    
      if (selReading != lastSelButtonState) {
      lastSelDebounceTime = millis();
      }
      
      if ((millis() - lastSelDebounceTime) > debounceDelay) {
        
        if (selReading != selButtonState) {
          selButtonState = selReading;
    
          if (!selButtonState) {
            display.setCursor(45,7);
            display.print("Stopped");
            delay(2000);
            display.clear();
            
            return;
          }
        }
      }
      lastSelButtonState = selReading;
    }
  // Close bitlength loop }
  display.setCursor(41,3);
  display.print("Complete");
  delay(2000);
  display.clear();
  delay(1000);
}

///////////////////////////////////////////////////////

void setup() {

  Serial.begin(115200);
  
  pinMode(selButton, INPUT_PULLUP);
  pinMode(navButton, INPUT_PULLUP);
  mhz433r.enableReceive(0); //Interrupt 0 - Pin 2
  mhz433t.enableTransmit(10);

  mhz433t.setRepeatTransmit(10);
  
  #if OLED_RESET >= 0            
  display.begin(&Adafruit128x64, I2C_ADDRESS, RST_PIN);
  #else // OLED_RESET >= 0
  display.begin(&Adafruit128x64, I2C_ADDRESS);
  #endif // OLED_RESET >= 0
  // Call oled.setI2cClock(frequency) to change from the default frequency.
  display.setFont(System5x7);  //This sets the font. There must be others but I'm not sure what they are.
  display.clear();             //This clears the oled display.
  


  
  // Write to the LCD
  display.setCursor(0,3);
  //display.setTextColor(WHITE);
  display.print("Code Sniffer Project");
 
  // Wait for 5 seconds
  delay(1000);
 
  // Clear the LCD
  display.clear();

  //////////////////////////////////////////////////////
  // Set stored codes///////////////////////////////////
  //////////////////////////////////////////////////////

  storedCodes[0].number = 1;
  storedCodes[0].bitlength = 24;
  storedCodes[0].frequency = 433;
  storedCodes[0].value = 5393;
  storedCodes[0].protocol = 1;
  storedCodes[0].key = 24;

  storedCodes[1].number = 2;
  storedCodes[1].bitlength = 24;
  storedCodes[1].frequency = 433;
  storedCodes[1].value = 5393;
  storedCodes[1].protocol = 2;
  storedCodes[1].key = 24;

  storedCodes[2].number = 3;
  storedCodes[2].bitlength = 24;
  storedCodes[2].frequency = 433;
  storedCodes[2].value = 1111;
  storedCodes[2].protocol = 1;
  storedCodes[2].key = 24;

  storedCodes[3].number = 4;
  storedCodes[3].bitlength = 24;
  storedCodes[3].frequency = 433;
  storedCodes[3].value = 1111;
  storedCodes[3].protocol = 2;
  storedCodes[3].key = 24;

  storedCodes[4].number = 5;
  storedCodes[4].bitlength = 24;
  storedCodes[4].frequency = 433;
  storedCodes[4].value = 9999;
  storedCodes[4].protocol = 1;
  storedCodes[4].key = 24;

  storedCodes[5].number = 6;
  storedCodes[5].bitlength = 24;
  storedCodes[5].frequency = 433;
  storedCodes[5].value = 9999;
  storedCodes[5].protocol = 2;
  storedCodes[5].key = 24;

  storedCodes[6].number = 7;
  storedCodes[6].bitlength = 24;
  storedCodes[6].frequency = 433;
  storedCodes[6].value = 11591236;
  storedCodes[6].protocol = 2;
  storedCodes[6].key = 24;
  //////////////////////////////////////////////////////

}

//////////////////////////////////////////////////////

void loop() {

  navReading = digitalRead(navButton);
  selReading = digitalRead(selButton);

  if (navReading != lastNavButtonState) {
    lastNavDebounceTime = millis();
  }

  if (selReading != lastSelButtonState) {
  lastSelDebounceTime = millis();
  }

  if ((millis() - lastNavDebounceTime) > debounceDelay) {
      
    if (navReading != navButtonState) {
      navButtonState = navReading;
      
      if (!navButtonState) {
        display.clear();
        option++;
        if (option > numberOfOptions) {
          option = 1;
        }
      }
    }
  }

  if ((millis() - lastSelDebounceTime) > debounceDelay) {
    
    if (selReading != selButtonState) {
      selButtonState = selReading;

      if (!selButtonState) {
        if (option == 1) {
          sniffer();
        }

        if (option == 2) {
          runSniffedCodes();
        }

        if (option == 3) {
          runStoredCodes();
        }

        if (option == 4) {
          bruteForce();
        }
      }
    }
  }
  
  if (option == 1) {
    display.setCursor(0,0);
    display.print("1) Sniff");
    //display.setCursor(0,52);
    //display.print("Next         Select");
    
  }

  if (option == 2) {
    display.setCursor(0,0);
    display.print("2) Sniffed Codes");
    //display.setCursor(0,52);
    //display.print("Next          Select");
    
  }

  if (option == 3) {
    display.setCursor(0,0);
    display.print("3) Stored Codes");
    //display.setCursor(0,52);
    //display.print("Next          Select");
	
  }

  if (option == 4) {
    display.setCursor(0,0);
    display.print("4) Brute Force");
    //display.setCursor(0,52);
    //display.print("Next          Select");
    
  }

  lastNavButtonState = navReading;
  lastSelButtonState = selReading;
}
