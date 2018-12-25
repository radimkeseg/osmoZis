#include "MyMoisture.h"

#include "Arduino.h" //not sure why but here it is needed

#define MEASURE_COUNT 5
float MyMoisture::measure(){
   Serial.print("Moisture: ");
   float value_moisture = 0;
   for(int i=0; i<MEASURE_COUNT; i++){
     digitalWrite(MEASURING, LOW);
     value_moisture+= analogRead(SENSE_PIN);
     delay(50);
   }
   value_moisture/=MEASURE_COUNT;

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

