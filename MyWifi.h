/**The MIT License (MIT)

Copyright (c) 2019 by Radim Keseg

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

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

  void restart(unsigned int inSec = 3);
  void forceManualConfig(const char* APname);

  String getIP();
  String getMAC();
  bool isConnected();

  void setup(const char* APname, int timeout_in_sec);
  void handleClient();
  WiFiClient& getWifiClient();
  CustomSettings& getCustomSettings();

  void setDataHandler(fncHandleData fDataHandler);
};
