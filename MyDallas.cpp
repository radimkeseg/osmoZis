#include "MyDallas.h"

#include "Arduino.h" //not sure why but here it is needed

void MyDallas::begin(){
  sensors->begin();
}

float MyDallas::measure(){
  sensors->requestTemperatures(); // Send the command to get temperature readings 
  Serial.print("Temperature: "); 
  last_temp=sensors->getTempCByIndex(0); // Why "byIndex"?
  Serial.print(last_temp);   
  Serial.println("C");   

  return last_temp;
}
float MyDallas::getLastMeasured(){
  return last_temp;
}

