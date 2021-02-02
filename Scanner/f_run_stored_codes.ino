void runStoredCodes(void) {
  boolean validCode = true;
  int code = 0;
  int oldCode = 1;
  display.clear();
  display.setCursor(0,0);
  display.print(F("Stored Codes"));
  
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
        display.print(F("No stored codes"));    
        validCode = false;
      }
      else{
        display.setCursor(0,0);
        display.print(F("End of codes"));    
        validCode = false;
      }
      // If there are codes in the list give the user the option of viewing again
      if (storedCodes[0].number != 0){
        display.setCursor(0,7);
        display.print(F("Beginning       Back"));
        
      }
      // If not, just give the user the option to go back
      else{ 
        display.setCursor(16,7);
        display.print(F("Back"));
      }
    }
      else{  
      display.setCursor(0,0);
      display.print(storedCodes[code].number);
      display.print(F(") "));
      display.print(storedCodes[code].value);
      display.setCursor(0,1);
      display.print(F("Bitlength: "));
      
      display.print(storedCodes[code].bitlength);
      
      display.setCursor(0,2);
      display.print(F("Protocol: "));
      display.print(storedCodes[code].protocol);
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
