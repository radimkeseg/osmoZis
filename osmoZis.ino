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

#include "MyOsmoZis.h"

static String stFncHandleData(){
  String res;
  res = "{\"temp\":\"";
  res += myDallas.getLastMeasured();
  res += "\",\"mois\":\"";
  res += myMoisture.getLastMeasured();
  res += "\"}";
  return res;
}

Interval measurementUpdate;
long update_interval = 600*1000; //default 10 min

#define TRIGGER_PIN 0
#define AP_NAME "OsmoZis-v2"

void setup()   {                
  Serial.begin(9600);

  pinMode(MEASURING, OUTPUT);
  
  setupOsmoZis();  

  myDisplay.begin();
  myDisplay.write_intro(true);

  pinMode(TRIGGER_PIN, INPUT); //when flash pressed get to wifi config portal
  for(int i=0; i<100; i++){
    if ( digitalRead(TRIGGER_PIN) == LOW ) {
      Serial.println("manual config portal triggered ");
      myWifi.forceManualConfig((String(AP_NAME)+"-OnDemandAP").c_str());
      Serial.println("connecting again ...)");
    }
    delay(50);
  }

  myDisplay.begin();
  myDallas.begin();

  myWifi.setup(AP_NAME,60); //1 min to configure the WIFI 
  myWifi.setDataHandler( stFncHandleData );

  myThingSpeak.begin(myWifi.getWifiClient());
  myThingSpeak.setup(myWifi.getCustomSettings().settings.TS_CHANNEL, myWifi.getCustomSettings().settings.TS_API_WRITE, myWifi.getCustomSettings().settings.TS_FIELD_TEMP, myWifi.getCustomSettings().settings.TS_FIELD_MOIS, myWifi.getCustomSettings().settings.TS_UPDATE_INTERVAL);
  
  timeClient.setTimeOffset(  myWifi.getCustomSettings().settings.UTC_OFFSET+myWifi.getCustomSettings().settings.DST );
  myWifi.getCustomSettings().print();
  
  timeClient.updateTime();


  update_interval = ((myWifi.getCustomSettings().settings.TS_UPDATE_INTERVAL<=0)?(600):(myWifi.getCustomSettings().settings.TS_UPDATE_INTERVAL)); //default 10 min
  
  delay(1000);
}

void loop(void) {
  // Handle web server
  myWifi.handleClient();
  
  if(measurementUpdate.expired()){ //read if the update interval has expired only
    measurementUpdate.set(update_interval*1000); // set new interval period 

    Serial.println("Reading sensors:");
    myMoisture.measure();
    myDallas.measure();
    
    myDisplay.clearDisplay(); 
  
    myDisplay.write_IP(0,1, myWifi.getIP().c_str());
    myDisplay.write_time(127,1, timeClient.getFormattedTimeShort().c_str());
    
    myDisplay.write_moisture( 0,24,  myMoisture.getLastMeasured());
      myDisplay.write_gauge( 96,23, myMoisture.getLastMeasured(),false,0,30);
    myDisplay.write_temp(     0,44,  myDallas.getLastMeasured());
      myDisplay.write_gauge( 96,43, myDallas.getLastMeasured(),false,0,30);
  
    myDisplay.showDisplay();

    if(myWifi.getCustomSettings().settings.THINGSPEAK){ //write to thingspeak
      if(myWifi.isConnected()){
        myThingSpeak.write(myDallas.getLastMeasured(),myMoisture.getLastMeasured());
      }
    }
  
  }

  myDisplay.clear_rect(0,0,127,9);
  myDisplay.write_IP(0,1, myWifi.getIP().c_str());
  myDisplay.write_time(127,1, timeClient.getFormattedTimeShort().c_str());
  myDisplay.write_progress(0,10,127,1,measurementUpdate.elapsed()/1000,false,0,measurementUpdate.getTimeout()/1000);
  //Serial.print("time elapsed: ");Serial.print(measurementUpdate.elapsed()/1000);Serial.print("time remains: ");Serial.println(measurementUpdate.remains()/1000);
  myDisplay.showDisplay();
  
  //Serial.print(".");
  delay(1000);
}

