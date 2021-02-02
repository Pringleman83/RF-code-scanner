void bruteForce(void) {
  display.clear();
  display.setCursor(0,0);
  display.print(F("Brute Force"));
  display.setCursor(98,7);
  display.print(F("Stop"));
  int totalCombinations = 4096;
  int bitlength = 12; // Set to 12 for speed.
  int protocol = 1; // Set to 1 as believed to be the most common protocol.

  //If we're trying to find a 12 bit code, the number will be no higher than 4096.
  if (bitlength == 12){
    totalCombinations = 4097;
  }
  //It's a lot higher for 24 bits.
  else if (bitlength == 24){
    totalCombinations = 1677216;
  }
  mhz433t.setProtocol(protocol);
  
  //Stop repeats
  mhz433t.setRepeatTransmit(2);
  //Warm up
  for (int i = 0; i < 5; i++){
    mhz433t.send(1111,bitlength);
  }
  
    for(int code = 0; code < totalCombinations+1; code++){
      //Only display every 10th code (for speed)
      if(code%10 == 0){
      display.setCursor(0,0);
      display.print(F("Bitlength: "));
      display.print(bitlength);
      display.setCursor(0,1);
      display.print(F("Code: "));
      display.print(code);
      display.setCursor(0,2);
      display.print(F("Protocol: "));
      display.print(protocol);
      }
       
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
            display.print(F("Stopped        "));
            delay(2000);
            display.clear();
             //Restore repeats
             mhz433t.setRepeatTransmit(10);
            return;
          }
        }
      }
      lastSelButtonState = selReading;
    }
  //Restore repeats
  mhz433t.setRepeatTransmit(10);
  display.setCursor(41,3);
  display.print(F("Complete"));
  delay(4000);
  display.clear();
}
