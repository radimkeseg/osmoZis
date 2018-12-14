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
  ThingSpeak.setField(7,temparature);
  ThingSpeak.setField(8,moisture);
  ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);        
}

void MyThingSpeak::write(float temperature, float moisture){

  if (thingSpeakUpdate.expired()) {
    thingSpeakUpdate.set(THINGSPEAK_UPDATE_INTERVAL*1000); // set new interval period
    write_internal(temperature,moisture);
  }

}

