#include "MyMoisture.h"

#include "Arduino.h" //not sure why but here it is needed

float MyMoisture::measure(){
   Serial.print("Moisture: ");
   digitalWrite(MEASURING, LOW);
   float value_moisture= analogRead(SENSE_PIN);

//experimentally checked range
   if(value_moisture<MIN_RANGE) value_moisture=MIN_RANGE;
   if(value_moisture>MAX_RANGE) value_moisture=MAX_RANGE;
   value_moisture = (1.0f-(value_moisture - MIN_RANGE)/(MAX_RANGE - MIN_RANGE))*100;
   
   Serial.print(value_moisture);Serial.println("%");
   delay(100);
   digitalWrite(MEASURING, HIGH);

   return last_mois = value_moisture;
}

float MyMoisture::getLastMeasured(){
  return last_mois;
}

