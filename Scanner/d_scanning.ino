//////////////////////////////////////////////////////////////////////////////////////////////////

int scan(void) {
  /* Start of test code
  display.setCursor(0,1);
  display.print(scannedCodes[0].frequency);
  display.setCursor(0,2);
  display.print(scannedCodes[0].value);
  display.setCursor(0,3);
  display.print(scannedCodes[0].protocol);
  // End of test code
  */

  // If a signal is received
  if (mhz433r.available()){
    if (mhz433r.getReceivedProtocol()==0){
      mhz433r.resetAvailable();
      return 0;
    }
    else{
    // Hold received code
    unsigned long value = mhz433r.getReceivedValue();
    int bitlength = mhz433r.getReceivedBitlength();
    int protocol = mhz433r.getReceivedProtocol();
    int key = 24;
    
    // Set received notification in top right corner of screen
    display.clear();
    display.setCursor(0,0);
    display.print(F("Code detected"));

    // Check if the code is already stored
    for (int scannedCode = 0; scannedCode < codeMax; scannedCode++){
      // If the value of the current scanned code is 0, we have reached the end of stored codes
      // No need to continue the for loop
      if (scannedCodes[scannedCode].value == 0){
        break;
      }
      // Check if the code in this position of the list matches the received code
      // If it does, mark it as not being a new code and break out of the for loop
      // otherwise mark it as a new code
      if (
      scannedCodes[scannedCode].value == value &&
      scannedCodes[scannedCode].bitlength == bitlength &&
      scannedCodes[scannedCode].protocol == protocol
      ){
        // Not a new code
        // Clear displayed information
        display.setCursor(0,2);
        display.print(F("Duplicate (code "));
        display.print(scannedCode + 1);
        display.print(F(")"));
        delay(2000);
        display.clear();
        // Set receiver ready to receive
        mhz433r.resetAvailable();
        return 0;
      }
    }

    // New code
    // Save Value, Bitlength and Protocol to list
    scannedCodes[recCount].number = recCount + 1;
    scannedCodes[recCount].value = value;
    scannedCodes[recCount].bitlength = bitlength;
    scannedCodes[recCount].protocol = protocol;
    scannedCodes[recCount].key = 24;

    // Display Value, Bitlength and Protocol
    display.setCursor(0,2);
    display.print(recCount);
    display.print(F(") "));  
    display.print(value);
    display.setCursor(0,3);
    display.print(bitlength);
    display.print(F(" bit"));
    display.setCursor(0,4);
    display.print(F("Protocol: "));
    display.print(protocol);
    delay(800);     
    // Increment the code counter
    recCount ++;
    // Set receiver ready to receive
    mhz433r.resetAvailable();
    // Clear displayed information
    display.clear();
    return 1;
  }
}
  return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

void scanner(void) {
  /*
  // Test code (add a dummy scanned code)
  scannedCodes[0].number = 1;
  scannedCodes[0].bitlength = 24;
  scannedCodes[0].frequency = 433;
  scannedCodes[0].value = 5393;
  scannedCodes[0].protocol = 1;
  scannedCodes[0].key = 24;
  // End of test code
  */

  // Check if we can store any more codes
  if (recCount == codeMax){
    display.clear();
    display.setCursor(0,2);
    display.print(F("Memory full"));
    delay(3000);
    display.clear();
    return;
  }
  display.clear();
  int newCodeCount = 0;
  while (true) {

    // Check if we can store any more codes
    if (recCount == codeMax){
      display.setCursor(0,2);
      display.print(F("Memory full"));
      delay(2000);
      display.clear();
      return;
    }

    if (millis() > scannerTimer + refreshAfter){
      display.setCursor(0,0);
      display.print(F("Scanning"));
      
      // Animate the full stop     
      if (pointCount == 0){
        display.print(F(".  "));
      }
      else if (pointCount == 1){
        display.print(F(" . "));
      }
      else{
        display.print(F("  ."));
      }
      pointCount++;
      if (pointCount>2){
      pointCount = 0;
      }
      display.setCursor(0,2);
      display.print(F("This session: "));
      display.print(newCodeCount);

      display.setCursor(0,3);
      display.print(F("Codes stored: "));
      display.print(recCount);

      display.setCursor(0,4);
      display.print(F("Saved: "));
      display.print(recCount);
      display.print(F("/"));
      display.print(codeMax);
      
      display.setCursor(0,7);
      display.print(F("                Back"));
      scannerTimer = millis();
    }
    // Add 1 to the newCodeCount if a new code is found and 0 if a new code is not
    newCodeCount+=scan();
    
    
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
