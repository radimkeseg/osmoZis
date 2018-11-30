/*********************************************************************
OsmoZis by Radim Keseg
*********************************************************************/

#include "embHTML.h"
#include "settings.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h>

// Helps with connecting to internet
#include <WiFiManager.h>

//#include <ArduinoOTA.h>
#include <ESP8266mDNS.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>    

// HOSTNAME for OTA update
#define HOSTNAME "OSMOZA-RKG1-"

//WiFiManager
//Local intialization. Once its business is done, there is no need to keep it around
WiFiManager wifiManager;
ESP8266WebServer server(80);
ESP8266HTTPUpdateServer httpUpdater;  

#include "MyMoisture.h"
MyMoisture myMoisture;

#include "MyDallas.h"
MyDallas dallas;

#include "MyThingSpeak.h"
WiFiClient wfclient;
MyThingSpeak myThingSpeak;

#include "MyDisplay.h"
MyDisplay myDisplay;



/* webserver handlers */
void handle_root()
{
  String content = FPSTR(PAGE_INDEX);      
  server.send(200, "text/html", content);
} 

// Called if WiFi has not been configured yet
void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("Wifi Manager");
  Serial.println("Please connect to AP");
  Serial.println(myWiFiManager->getConfigPortalSSID());
  Serial.println("To setup Wifi Configuration");
}



volatile byte state = LOW;
volatile byte count = 0;
void ICACHE_RAM_ATTR blink() {
  if(count == 0) state = !state;
  if(count == 1) state = !state;
  if(count == 9) count = 0; //==9 if CHANGE or ==4 if RISING
  else count++;
  digitalWrite(D8, state);
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
  dallas.begin();
  myDisplay.write_intro();

/*wifi setup*/
  // Uncomment for testing wifi manager
  //  wifiManager.resetSettings();
  wifiManager.setAPCallback(configModeCallback);
  //or use this for auto generated name ESP + ChipID
  wifiManager.autoConnect();
  //Manual Wifi
  //WiFi.begin(WIFI_SSID, WIFI_PWD);

  // OTA Setup
  String hostname(HOSTNAME);
  hostname += String(ESP.getChipId(), HEX);
  WiFi.hostname(hostname);
  
  MDNS.begin( (hostname+"-webupdate").c_str() );
  httpUpdater.setup(&server, "/firmware", update_username, update_password );

  //user setting handling
  server.on("/", handle_root);

  server.begin(); 
  Serial.println("HTTP server started");

  MDNS.addService("http", "tcp", 80); 
/*end wifi setup*/

  myThingSpeak.begin(wfclient);
  delay(1000);
}

void loop(void) {
  // Handle web server
  server.handleClient();

  Serial.println("Reading sensors:");
  myMoisture.measure();
  dallas.measure();
  myThingSpeak.write(dallas.getLastMeasured(),myMoisture.getLastMeasured());

  myDisplay.clearDisplay(); 
  myDisplay.write_moisture(myMoisture.getLastMeasured());
  myDisplay.write_temp(dallas.getLastMeasured());
  myDisplay.showDisplay();
  
  Serial.println("");
  delay(10000);
}

