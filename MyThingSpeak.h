#pragma once

#include <Interval.h>
#include <ThingSpeak.h>

#define THINGSPEAK_UPDATE_INTERVAL 10*60 //seconds

class MyThingSpeak{
private:
  // ThingSpeak
  unsigned long myChannelNumber = 211292;
  const char *myWriteAPIKey = "CHF36K1C8B6YQUV4";

  Interval thingSpeakUpdate;

  void write_internal(float temperature, float moisture);

public:
   void begin(Client & client){
      ThingSpeak.begin(client);
   }

   void write(float temperature, float moisture);
};

