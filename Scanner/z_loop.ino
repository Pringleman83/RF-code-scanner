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
          scanner();
        }

        if (option == 2) {
          runScannedCodes();
        }

        if (option == 3) {
          runStoredCodes();
        }

        if (option == 4) {
          bruteForce();
        }

        if (option == 5) {
          deBruijn();
        }

        if (option == 6) {
          deBruijn2();
        }
      }
    }
  }
  
  if (option == 1) {
    display.setCursor(0,0);
    display.print(F("1) Scan"));
    display.setCursor(0,7);
    display.print(F("Next           Select"));  
  }

  if (option == 2) {
    display.setCursor(0,0);
    display.print(F("2) Scanned Codes"));
    display.setCursor(0,7);
    display.print(F("Next           Select")); 
  }

  if (option == 3) {
    display.setCursor(0,0);
    display.print(F("3) Stored Codes"));
    display.setCursor(0,7);
    display.print(F("Next           Select"));
  }

  if (option == 4) {
    display.setCursor(0,0);
    display.print(F("4) Brute Force"));
    display.setCursor(0,7);
    display.print(F("Next           Select"));
  }

    if (option == 5) {
    display.setCursor(0,0);
    display.print(F("5) De Bruijn"));
    display.setCursor(0,7);
    display.print(F("Next           Select")); 
  }

    if (option == 6) {
      display.setCursor(0,0);
      display.print(F("6) De Bruijn V2"));
      display.setCursor(0,7);
      display.print(F("Next           Select")); 
  }
  lastNavButtonState = navReading;
  lastSelButtonState = selReading;
}
