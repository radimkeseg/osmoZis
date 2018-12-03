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


class MyWifi{
private:
//WiFiManager
//Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager *wifiManager;
  ESP8266WebServer *server;
  ESP8266HTTPUpdateServer *httpUpdater;

  WiFiClient *wfclient;

  void handle_root();
  void configModeCallBack(WiFiManager *myWiFiManager);

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

  void setup();
  void handleClient();
  WiFiClient& getWifiClient();
};
