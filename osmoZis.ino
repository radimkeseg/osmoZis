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

void setup()   {                
  Serial.begin(9600);

  setupOsmoZis();  

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
long time_update_interval = THINGSPEAK_UPDATE_INTERVAL*1000; //10 min

void loop(void) {
  // Handle web server
  myWifi.handleClient();
  
  stamp = millis();
  if(stamp - last_time_update > time_update_interval || stamp < last_time_update ){ //read if the update interval has expired only
    last_time_update = stamp;

    Serial.println("Reading sensors:");
    myMoisture.measure();
    myDallas.measure();
//    myThingSpeak.write(myDallas.getLastMeasured(),myMoisture.getLastMeasured()); //uncomment once tested

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
  
  Serial.print(".");
  delay(500);
}

