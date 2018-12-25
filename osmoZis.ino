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

long stamp = 0;
long last_time_update = 2147483647; //MAX_LONG
long time_update_interval = 600*1000; //default 10 min

#define TRIGGER_PIN 0
#define AP_NAME "OsmoZis-v2"

void setup()   {                
  Serial.begin(9600);

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

 
  myWifi.setup(AP_NAME,60);
  myWifi.setDataHandler( stFncHandleData );

  myThingSpeak.begin(myWifi.getWifiClient());
  myThingSpeak.setup(myWifi.getCustomSettings().settings.TS_CHANNEL, myWifi.getCustomSettings().settings.TS_API_WRITE, myWifi.getCustomSettings().settings.TS_FIELD_TEMP, myWifi.getCustomSettings().settings.TS_FIELD_MOIS, myWifi.getCustomSettings().settings.TS_UPDATE_INTERVAL);
  
  timeClient.setTimeOffset(  myWifi.getCustomSettings().settings.UTC_OFFSET+myWifi.getCustomSettings().settings.DST );
  myWifi.getCustomSettings().print();
  
  timeClient.updateTime();


  time_update_interval = ((myWifi.getCustomSettings().settings.TS_UPDATE_INTERVAL<=0)?(600):(myWifi.getCustomSettings().settings.TS_UPDATE_INTERVAL))*1000; //default 10 min
  
  delay(1000);
}

void loop(void) {
  // Handle web server
  myWifi.handleClient();
  
  stamp = millis();
  if(stamp - last_time_update > time_update_interval || stamp < last_time_update ){ //read if the update interval has expired only
    last_time_update = stamp;

    Serial.println("Reading sensors:");
    myMoisture.measure();
    myDallas.measure();
    
    //Serial.print("TS: "); Serial.println(myWifi.getCustomSettings().settings.THINGSPEAK);
    if(myWifi.getCustomSettings().settings.THINGSPEAK){
      /*
      Serial.print("channel: "); Serial.println(myThingSpeak.channelNumber);
      Serial.print("write api key: "); Serial.println(myThingSpeak.writeAPIKey);
      Serial.print("field temp #"); Serial.println(myThingSpeak.field_temp);
      Serial.print("field mois #"); Serial.println(myThingSpeak.field_mois);
      Serial.print("update interval: "); Serial.print(myThingSpeak.update_interval); Serial.println("s");
      */
      myThingSpeak.write(myDallas.getLastMeasured(),myMoisture.getLastMeasured());
    }

    myDisplay.clearDisplay(); 
  
    myDisplay.write_IP(0,1, myWifi.getIP().c_str());
    myDisplay.write_time(127,1, timeClient.getFormattedTimeShort().c_str());
    
    myDisplay.write_moisture( 0,24,  myMoisture.getLastMeasured());
      myDisplay.write_gauge( 96,23, myMoisture.getLastMeasured(),false,0,30);
    myDisplay.write_temp(     0,44,  myDallas.getLastMeasured());
      myDisplay.write_gauge( 96,43, myDallas.getLastMeasured(),false,0,30);
  
    myDisplay.showDisplay();
  }

  myDisplay.clear_rect(0,0,127,9);
  myDisplay.write_IP(0,1, myWifi.getIP().c_str());
  myDisplay.write_time(127,1, timeClient.getFormattedTimeShort().c_str());
  myDisplay.write_progress(0,10,127,1,stamp,false,last_time_update,last_time_update+time_update_interval);
  myDisplay.showDisplay();
  
  //Serial.print(".");
  delay(1000);
}

