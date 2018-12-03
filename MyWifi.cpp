#include "MyWifi.h"
#include "settings.h"

void MyWifi::handle_root()
{
  String content = FPSTR(PAGE_INDEX);      
  server->send(200, "text/html", content);
} 

//---------------------------------------------------------

void MyWifi::setup(){
  // Uncomment for testing wifi manager
  //  wifiManager.resetSettings();
  //or use this for auto generated name ESP + ChipID
  wifiManager->autoConnect();
  //Manual Wifi
  //WiFi->begin(WIFI_SSID, WIFI_PWD);

  // OTA Setup
  String hostname(HOSTNAME);
  hostname += String(ESP.getChipId(), HEX);
  WiFi.hostname(hostname);
  
  MDNS.begin( (hostname+"-webupdate").c_str() );
  httpUpdater->setup(server, "/firmware", update_username, update_password );

  //user setting handling
  server->on("/", std::bind(&MyWifi::handle_root, this));

  server->begin(); 
  Serial.println("HTTP server started");

  MDNS.addService("http", "tcp", 80); 
}

void MyWifi::handleClient(){
  // Handle web server
  server->handleClient(); 
}

WiFiClient& MyWifi::getWifiClient(){
  if(wfclient==NULL) wfclient = new WiFiClient();
  return *wfclient;
}

