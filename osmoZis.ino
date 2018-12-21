/*********************************************************************
OsmoZis by Radim Keseg
*********************************************************************/


#include "MyMoisture.h"
MyMoisture myMoisture;

#include "MyDallas.h"
MyDallas myDallas;

#include "MyThingSpeak.h"
MyThingSpeak myThingSpeak;

#include "MyDisplay.h"
MyDisplay myDisplay;

#include "MyWifi.h"
MyWifi myWifi;

#include "TimeClient.h"
TimeClient timeClient(1);



//osmoZis generator 200Hz
volatile byte state = LOW;
volatile byte count = 0;
void ICACHE_RAM_ATTR blink() {
  if(count == 0) state = !state;
  if(count == 1) state = !state;
  if(count == 9) count = 0; //==9 if CHANGE or ==4 if RISING
  else count++;
  digitalWrite(D8, state);
}

static String stFncHandleData(){
  String res;
  res = "{\"temp\":\"";
  res += myDallas.getLastMeasured();
  res += "\",\"mois\":\"";
  res += myMoisture.getLastMeasured();
  res += "\"}";
  return res;
}

void setup()   {                
  Serial.begin(9600);

  // the trick to generate impulses using PWM - 1kHz
  pinMode(MEASURING, OUTPUT);
  pinMode(D6, OUTPUT);
  pinMode(D7, INPUT);
  pinMode(D8, OUTPUT);
  analogWrite(D6, 128);
  attachInterrupt(digitalPinToInterrupt(D7), blink, CHANGE);//CHANGE or RISING

  myDisplay.begin();
  myDallas.begin();
  myDisplay.write_intro();

  myWifi.setup("osmoZis",180);
  myWifi.setDataHandler( stFncHandleData );

  myThingSpeak.begin(myWifi.getWifiClient());
  timeClient.setTimeOffset(  myWifi.getCustomSettings().settings.UTC_OFFSET+myWifi.getCustomSettings().settings.DST );
  myWifi.getCustomSettings().print();
  
  timeClient.updateTime();
  
  delay(1000);
}

long stamp = 0;
long last_time_update = 2147483647; //MAX_LONG
long time_update_interval = 30*1000; //30 secs

void loop(void) {
  // Handle web server
  myWifi.handleClient();
  
  stamp = millis();
  if(stamp - last_time_update > time_update_interval || stamp < last_time_update ){ //read if the update interval has expired only
    last_time_update = stamp;

    Serial.println("Reading sensors:");
    myMoisture.measure();
    myDallas.measure();
    myThingSpeak.write(myDallas.getLastMeasured(),myMoisture.getLastMeasured());
  

  myDisplay.clearDisplay(); 
  myDisplay.write_moisture(myMoisture.getLastMeasured());
  myDisplay.write_temp(myDallas.getLastMeasured());
  myDisplay.write_time(timeClient.getFormattedTimeShort().c_str());
  myDisplay.showDisplay();

  }
  
  Serial.print(".");
  delay(1000);
}

