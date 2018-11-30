#pragma once

#define MEASURING 12 //=D6
#define SENSE_PIN  A0 // sensor input at Analog pin A0

#define MIN_RANGE 330
#define MAX_RANGE 960

class MyMoisture{
private:
  float last_mois = 0;

public:
  float getLastMeasured();
  float measure();
};

