#include "MyWifi.h"
#include "settings.h"

void MyWifi::handle_root()
{
  String content = FPSTR(PAGE_INDEX);      

  content.replace("{timeoffset}", String(cs.settings.UTC_OFFSET).c_str() );
  
  if (cs.settings.DST) content.replace("{dst}", "checked='checked'");
  else    content.replace("{dst}", "");
  
  server->send(200, "text/html", content);
} 


void MyWifi::handle_store_settings(){
  if(server->arg("_dst")==NULL && server->arg("_timeoffset")==NULL ){
    Serial.println("setting page refreshed only, no params");      
  }else{
    Serial.println("settings changed");  

    cs.settings.UTC_OFFSET = atof(server->arg("_timeoffset").c_str());
    cs.settings.DST = server->arg("_dst").length()>0;    
    
    cs.print();          
    cs.write();
  }
  server->send(200, "text/html", "OK");
  
  delay(500);
  ESP.reset(); 
} 

//---------------------------------------------------------

void MyWifi::setup(char* APname, int timeout_in_sec){
  // Uncomment for testing wifi manager
  //  wifiManager.resetSettings();
  //or use this for auto generated name ESP + ChipID
  wifiManager->autoConnect();
  //Manual Wifi
  //WiFi->begin(WIFI_SSID, WIFI_PWD);

  //sets timeout until configuration portal gets turned off
  //useful to make it all retry or go to sleep
  //in seconds
  wifiManager->setTimeout(timeout_in_sec);

  String hostname = String(APname) + String("-") + String(ESP.getChipId(), HEX);
  //fetches ssid and pass and tries to connect
  //if it does not connect it starts an access point with the specified name
  //here  "AutoConnectAP"
  //and goes into a blocking loop awaiting configuration
  if(!wifiManager->autoConnect( hostname.c_str() ) ){
    Serial.println("failed to connect and hit timeout");
    delay(3000);
    //reset and try again, or maybe put it to deep sleep
    ESP.reset();
    delay(5000);
  } 
  

  // OTA Setup
  WiFi.hostname(hostname);
  MDNS.begin( (hostname+"-webupdate").c_str() );
  httpUpdater->setup(server, "/firmware", update_username, update_password );

  cs.init();
  cs.read();   
  cs.print();

  //user setting handling
  server->on("/", std::bind(&MyWifi::handle_root, this));
  server->on("/offset", std::bind(&MyWifi::handle_store_settings,this)); 

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

CustomSettings& MyWifi::getCustomSettings(){
   return cs;
}

