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

