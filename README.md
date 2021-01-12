#RF Code Sniffer

This project is somethnigi did some time ago to experiment with RF signals and replay attacks.

It's an old project but I've revived it a little after going through another phase of interest.

I've just added an updated version that works with the tiny OLED (SSD1306 128 x 64) displays rather than the chunky 2 line LCD displays it was originally designed for.

There are 2 buttons; Navigate and Select. Pressing the navigate button cycles though options and the select button selects that option.

The options are:

Sniff

This puts the device in sniffing mode, waiting to detect codes.


Sniffed Codes

This allows you to cycle through the sniffed codes so that you can view and replay them.


Stored Codes

This allows you to cycle through hard coded codes so that you can view and replay them.


Brute Force

This is a silly brute forcer that cycles through codes. It's very basic and so poor it's pretty much useless. It was a place holder for something more elaborate. Maybe I'll put something more meaningful there some day (or if any of you would like to contribute, please do!).


It's set up for an Arduino Nano but i believe the pin configuration would be the same for an UNO too.

Here's a basic layout of the circuit:

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

I've only used this for testing. Anything else may require better components (however, this is here for educational purposes).

I believe there's some kind of bug when it comes to receiving or tansmitting multiple codes. I need to build a second device to fully test it though. Again, any help with that is greatly appreciated.

Thanks for looking. I hope you enjoy tinkering with this as much as I did.
    
