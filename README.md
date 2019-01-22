# OsmoZis v2

... In other languages [česky](README.cs-CZ.md)

Do you need to dry out your house from rising water? Typically an older house or a house with problematic fundament insulation?

Well without too much of the theory, there is chance a small gadget like this would help.
![OsmoZis v2](img/OsmoZis_20190120_110924.jpg)

# How does it work?
Simply said, the equipment generates low frequency peaks which eliminate force existing in the water/wall solution and let the gravitation win the fight of various forces and drop the water down, back to the ground.

Sounds like magic, well, most likely it does, however it was successfully tested experimentally.

If you need more details or theory provided by experts, seach on the internet for articles or videos that might help you to understand the magic. One interesting article I have found is the following https://www.youtube.com/watch?v=hDw9_C7s-Mk.

# Does it exist on the market?
Of course it does, just search OSMO something ... The devices are far too expensive and some of them just show a led diode shining, without a chance to see whether it really works or not.

This simple and cheap device, which costs fraction of what you can find on the market, provides functions where you can:
* observe moisture on the display
* observe temperature on the display
* connect the device to your wifi
  * see IP address of the device on the display
  *  see actual time
* and additional settings
  * time zone offset
  * to send data to thinkgspeak to see moisture/temperature online every 10 minuts by default (customizable based on your preferences)


# What do I need to make my own device?
check the "img" folder, there you can find design of the circuitry, PCB gerber file and several pictures that might help to make the final product.

![setup](img/how_to_set_it_up.jpg)

# How to make it breathing?
Just use the piece of code and check the software, or in case you don't want to bother, just use the bin file and flash your own ESP directly.

If you are interested in the code, be aware of several external libraries that are needed, find the list below.

1. include <OneWire.h> 
2. include <DallasTemperature.h>

3. include <SPI.h>
4. include <Wire.h>
5. include <Adafruit_GFX.h>
6. include "Adafruit_SSD1306.h"

7. include <Interval.h>
8. include <ThingSpeak.h>

9. ESP8266 board and libraries
