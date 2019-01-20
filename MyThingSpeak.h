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

#include "Interval.h"
#include <ThingSpeak.h>

#define THINGSPEAK_UPDATE_INTERVAL 10*60 //seconds

class MyThingSpeak{
private:
  // ThingSpeak
  Interval thingSpeakUpdate;

  void write_internal(float temperature, float moisture);

public:
  unsigned long channelNumber;// = 211292;
  char writeAPIKey[17];// = "CHF36K1C8B6YQUV4";
  unsigned int field_temp;// = 7;
  unsigned int field_mois;// = 8;
  unsigned int update_interval = THINGSPEAK_UPDATE_INTERVAL; //


   void begin(Client & client){
      ThingSpeak.begin(client);
   }

   void setup(unsigned long channelNumber, const char *writeAPIKey, unsigned int field_temp, unsigned int field_mois, unsigned int update_interval);
   void write(float temperature, float moisture);
};

