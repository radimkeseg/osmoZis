#pragma once

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

typedef String (*fncHandleData)();

class MyWifi{
private:
//WiFiManager
//Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager *wifiManager;
  ESP8266WebServer *server;
  ESP8266HTTPUpdateServer *httpUpdater;

  WiFiClient *wfclient;
  CustomSettings cs;


  void handle_root();
  void handle_store_settings();
  void configModeCallBack(WiFiManager *myWiFiManager);

  fncHandleData fDataHandler;
  void handle_data();

  void restart(unsigned int inSec = 3);

public:
  MyWifi(){
    wifiManager = new WiFiManager();
    server = new ESP8266WebServer(80);
    httpUpdater = new ESP8266HTTPUpdateServer();
  }
  ~MyWifi(){
    if(wfclient!=NULL) delete wfclient;

    delete wifiManager;
    delete server;
    delete httpUpdater;
  }

  String getIP();
  String getMAC();

  void setup(char* APname, int timeout_in_sec);
  void handleClient();
  WiFiClient& getWifiClient();
  CustomSettings& getCustomSettings();

  void setDataHandler(fncHandleData fDataHandler);
};
