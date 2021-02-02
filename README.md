#RF Code Scanner

This project is something did some time ago to experiment with RF signals and replay attacks.

It's an old project but, with the input of Abhiram223344 I've revived it after going through another phase of interest.

It now works with the tiny OLED (SSD1306 128 x 64) displays rather than the chunky 2 line LCD displays it was originally designed for.

There are 2 buttons; Navigate and Select. Pressing the navigate button cycles though options and the select button selects that option.

The options are:

Scan

This puts the device in scanning mode, waiting to detect codes.


Scanned Codes

This allows you to cycle through the sniffed codes so that you can view and replay them.


Stored Codes

This allows you to cycle through hard coded codes so that you can view and replay them.


Brute Force

This is a silly brute forcer that cycles through codes. It's very basic and so poor it's pretty much useless. It was a place holder for something more elaborate. Maybe I'll put something more meaningful there some day (or if any of you would like to contribute, please do!).


De Bruijn
and
De Bruijn 2

These are alternatives to bruteforcing taking advantage of locks that use a shift register to check codes. For example, looking for a 4 bit code of 0011, sending 0000 would fail. Sending 00001 would also fail but it would be checking 0000 when those bits are sent and then 0001 when the last bit is sent as it'd incorporate the last three bits from the previous set of 4. 000011 would check 0000, 0001 and 0011. Of course, the last code would work.

A ebruijn sequence is a sequence of password characters that covers all combinations with the fewest characters. I haven't mastered this yet so these two options are a space for me to work for now.


It's set up for an Arduino Nano but i believe the pin configuration would be the same for an UNO too.

Here's a basic layout of the circuit (OLED version):

Arduino Pin

    D2        >>       Receiver Data
    D4        >>       Select Button      >>     GND
    D5        >>       Navigate Button    >>     GND
    D10       >>       Transmitter Data
    A4        >>       OLED SDA
    A5        >>       OLED SCL
    5V        >>       Receiver, Transmitter, OLED VCC
    GND       >>       Receiver, Transmitter, OLED, Buttons GND
    
    
You'll need the following libraries installed:

RC-Switch
SSD1306Ascii
    
Finally, I recommend that you avoid the very cheap 080408(?) receivers. Their range is ridiculously poor, even with an antenna attached. I've found the RXB8 to be much more useful and not much more expensive. I've not had any issues with the cheap transmitters though.

I've only used this for testing. Anything else may require better components.

Please note, this code is produced for educational purposes only! You can learn a lot by rading through the code and checking the security of your own devices. 

Big thanks to Abhiram223344 who has provided very useful input into this project and helped to design some very important testing devices. Thank you Abhiram!

Thanks for looking. I hope you enjoy tinkering with this as much as I did.
    
