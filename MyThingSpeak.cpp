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

#include "MyThingSpeak.h"

void MyThingSpeak::write_internal(float temparature, float moisture){
  if (isnan(moisture) || isnan(temparature))
  {
    Serial.println("Failed to read from sensors!");
    return;
  }

  Serial.println("ThingsSpeak write");
  Serial.println("=================");
  Serial.println("-----------------");
  Serial.print("Moisture: ");
  Serial.print(moisture);
  Serial.println("%");
  Serial.print("Temperature: ");
  Serial.print(temparature);
  Serial.println("C");
  Serial.println("-----------------");

  //send data to thingSpeak
  ThingSpeak.setField(field_temp,temparature);
  ThingSpeak.setField(field_mois,moisture);
  ThingSpeak.writeFields(channelNumber, writeAPIKey);        
}

void MyThingSpeak::setup(unsigned long channelNumber, const char *writeAPIKey, unsigned int field_temp, unsigned int field_mois, unsigned int update_interval){
   this->channelNumber = channelNumber;
   strncpy(this->writeAPIKey, String(writeAPIKey).c_str(), 17); 
   this->field_temp = field_temp;
   this->field_mois = field_mois;
   this->update_interval = update_interval;
}


void MyThingSpeak::write(float temperature, float moisture){

  if (thingSpeakUpdate.expired()) {
    thingSpeakUpdate.set(update_interval*1000); // set new interval period
    write_internal(temperature,moisture);
  }

}

