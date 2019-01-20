/**The MIT License (MIT)

Copyright (c) 2019 by Radim Keseg

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#pragma once

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Adafruit_SSD1306.h"

// If using software SPI (the default case):
#define OLED_CLK   D0
#define OLED_MOSI  D3
#define OLED_RESET D2
#define OLED_DC    D1 
#define OLED_CS    -1

#define LOGO16_GLCD_HEIGHT 16 
#define LOGO16_GLCD_WIDTH  16 
static const unsigned char PROGMEM logo16_mois_bmp[] =
{ B00000000, B00000000,
  B00000001, B10000000,
  B00000010, B01000000,
  B00000010, B01000000,
  B00000100, B00100000,
  B00001000, B00010000,
  B00010000, B00001000,
  B00100100, B00000100,
  B00101000, B00000100,
  B00100000, B00000100,
  B00100000, B00000100,
  B00010000, B00001000,
  B00001000, B00010000,
  B00000100, B00100000,
  B00000011, B11000000,
  B00000000, B00000000 };

static const unsigned char PROGMEM logo16_temp_bmp[] =
{ B00000000, B00000000,
  B00000001, B10000000,
  B00000011, B11000000,
  B00000010, B01000000,
  B00000011, B01000000,
  B00000010, B01000000,
  B00000011, B01000000,
  B00000010, B01000000,
  B00000011, B01000000,
  B00000010, B01000000,
  B00000011, B01000000,
  B00000100, B00100000,
  B00001010, B00010000,
  B00001000, B00010000,
  B00000111, B11100000,
  B00000000, B00000000 };

static const unsigned char PROGMEM gauge_bmp[] =
{ B00000000, B00011111, B11111000, B00000000,
  B00000000, B11100000, B00000111, B00000000,
  B00000011, B00000000, B00000000, B11000000,
  B00000100, B00000000, B00000000, B00100000,
  B00011000, B00000000, B00000000, B00011000,
  B00010000, B00000000, B00000000, B00001000,
  B00100000, B00000000, B00000000, B00000100,
  B00100000, B00000000, B00000000, B00000100,
  B01000000, B00000000, B00000000, B00000010,
  B01000000, B00000000, B00000000, B00000010,
  B01000000, B00000000, B00000000, B00000010,
  B10000000, B00000000, B00000000, B00000001,
  B10000000, B00000000, B00000000, B00000001,
  B10000000, B00000000, B00000000, B00000001,
  B10000000, B00000000, B00000000, B00000001,
  B10000000, B00000000, B00000000, B00000001  };     

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif


class MyDisplay{
private:
     Adafruit_SSD1306 *display;
public:
  
  MyDisplay(){
     display = new Adafruit_SSD1306(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);
  }
  ~MyDisplay(){
    delete display;
  }
  void begin();
  
  void showDisplay();
  void clearDisplay();
  
  void write_intro(bool manual=false);

  void write_moisture(int x, int y, float moisture);
  void write_temp(int x, int y, float temperature);

  void write_time(int x, int y, const char* formatedTime);
  void write_IP(int x, int y, const char* formatedIP);
  void write_gauge(int x, int y, float value, bool showText = true, float min =0.00, float max=99.99); //32x16 + 8
  void write_progress(int x, int y, int width, int height, float value, bool showText, float min=0.00, float max=99.99);  

  void clear_rect(int x, int y, unsigned int width, unsigned int height);
};

