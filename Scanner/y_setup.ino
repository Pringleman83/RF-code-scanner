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
  display.setFont(System5x7);
  display.clear();
  display.setCursor(0,3);
  display.print(F("Code Scanner Project"));
  // Wait for 5 seconds
  delay(1000);
  // Clear the LCD
  display.clear();
  
  ///////////////////////////////////////////////////////
  // Stored codes///////////////////////////////////////
  //////////////////////////////////////////////////////

  storedCodes[0].number = 1;
  storedCodes[0].bitlength = 64;
  storedCodes[0].frequency = 433;
  storedCodes[0].value = 5393;
  storedCodes[0].protocol = 1;
  storedCodes[0].key = 12;

  storedCodes[1].number = 2;
  storedCodes[1].bitlength = 24;
  storedCodes[1].frequency = 433;
  storedCodes[1].value = 5393;
  storedCodes[1].protocol = 2;
  storedCodes[1].key = 24;

  storedCodes[2].number = 3;
  storedCodes[2].bitlength = 24;
  storedCodes[2].frequency = 433;
  storedCodes[2].value = 53;
  storedCodes[2].protocol = 1;
  storedCodes[2].key = 12;

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
  storedCodes[5].value = 11078670;
  storedCodes[5].protocol = 1;
  storedCodes[5].key = 24;

  storedCodes[6].number = 7;
  storedCodes[6].bitlength = 24;
  storedCodes[6].frequency = 433;
  storedCodes[6].value = 11591236;
  storedCodes[6].protocol = 2;
  storedCodes[6].key = 24;
  /////////////////////////////////////////////////////
}
