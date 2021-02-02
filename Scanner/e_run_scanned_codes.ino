
void runScannedCodes(void) {
  boolean validCode = true;
  int code = 0;
  int oldCode = 1;
  display.clear();
  display.setCursor(0,0);
  display.print(F("Scanned Codes"));
  
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
    if (scannedCodes[code].number == 0){
      // If there are no stored codes, advise the user.
      if (scannedCodes[0].number == 0){
        display.setCursor(0,0);
        display.print(F("No codes"));
        
        validCode = false;
      }
      else{
        display.setCursor(0,0);
        display.print(F("End of codes"));
        
        validCode = false;
      }
      // If there are codes in the list give the user the option of viewing again
      if (scannedCodes[0].number != 0){
        display.setCursor(0,7);
        display.print(F("Beginning       Back"));
        
      }
      // If not, just give the user the option to go back
      else{
        display.setCursor(55,7);
        display.print(F("Back")); 
      }
    }
      else{
      display.setCursor(0,0);
      display.print(scannedCodes[code].number);
      display.print(F(") "));
      display.print(scannedCodes[code].value);
      display.setCursor(0,1);
      display.print(F("Bitlength: "));
      display.print(scannedCodes[code].bitlength);
      display.setCursor(0,2);
      display.print(F("Protocol: "));
      display.print(scannedCodes[code].protocol);
      display.setCursor(0,7);
      display.print(F("Next            Send"));
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
            transmit(scannedCodes[code].protocol, scannedCodes[code].value, scannedCodes[code].bitlength);
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
