#pragma once

#define TEMP_BUS 14 //=D5 
#define NOT_MEASURED -128

#include <OneWire.h> 
#include <DallasTemperature.h>


class MyDallas{
private:
  float last_temp = NOT_MEASURED;

  OneWire *oneWire;
  DallasTemperature *sensors;
 
public:
  MyDallas(){
    oneWire = new OneWire(TEMP_BUS);
    sensors = new DallasTemperature(oneWire);
  }
  ~MyDallas(){
    delete sensors;
    delete oneWire;
  }
    
  void begin();
  float getLastMeasured();
  float measure();
};
