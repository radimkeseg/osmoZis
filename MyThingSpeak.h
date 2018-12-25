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

