/**The MIT License (MIT)

Copyright (c) 2019 Radim Keseg

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

#include "settings.h"

extern settings_t settins;

void CustomSettings::init() {
  //http://esp8266.github.io/Arduino/versions/2.0.0/doc/filesystem.html
  Serial.print("SPIFFS begin - ");
  if(SPIFFS.begin()){
    Serial.println("OK");
    //SPIFFS.format(); //uncomment if formating needed
    delay(100);
  }else{
    Serial.println("FAILED");
  }
}

void CustomSettings::write(){
    Serial.println("writing custom setting start");
    Serial.println("file: " + CUSTOM_SETTINGS);
    File f = SPIFFS.open(CUSTOM_SETTINGS, "w");
    if (f){
      f.write((uint8_t*) &settings, sizeof(settings_t));
    }else{
      Serial.println("open file for writing failed: " + CUSTOM_SETTINGS);
    }
    f.flush();
    f.close();
    Serial.println("writing custom setting end");    
}

void CustomSettings::read(){
    //read setting from SPIFF
    Serial.println("reading custom setting start");
    File f = SPIFFS.open(CUSTOM_SETTINGS, "r");
    if(f){
       f.read((uint8_t*) &settings, sizeof(settings_t));
    }else{
      Serial.println("open file for reading failed: " + CUSTOM_SETTINGS);
    }
    f.close();
    Serial.println("reading custom setting end");
}

void CustomSettings::print(){    
    Serial.print("UTC TimeOffset: "); Serial.println(settings.UTC_OFFSET);
    Serial.print("DST: "); Serial.println(settings.DST);
}

