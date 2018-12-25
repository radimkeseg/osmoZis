#include "MyWifi.h"
#include "settings.h"

void MyWifi::handle_root()
{
  String content = FPSTR(PAGE_INDEX);      

  content.replace("{timeoffset}", String(cs.settings.UTC_OFFSET).c_str() );
  if (cs.settings.DST) content.replace("{dst}", "checked='checked'");
  else    content.replace("{dst}", "");

  if (cs.settings.THINGSPEAK) content.replace("{ts}", "checked='checked'");
  else    content.replace("{ts}", "");
  content.replace("{ts_channel}", String(cs.settings.TS_CHANNEL).c_str() );
  content.replace("{ts_write_api_key}", String(cs.settings.TS_API_WRITE).c_str() );
  content.replace("{ts_field_temp}", String(cs.settings.TS_FIELD_TEMP).c_str() );
  content.replace("{ts_field_mois}", String(cs.settings.TS_FIELD_MOIS).c_str() );
  content.replace("{ts_update_interval}", String(cs.settings.TS_UPDATE_INTERVAL).c_str() );
  
  server->send(200, "text/html", content);
} 


void MyWifi::handle_store_settings(){
  if(server->arg("_dst")==NULL && server->arg("_timeoffset")==NULL ){
    Serial.println("setting page refreshed only, no params");      
  }else{
    Serial.println("settings changed");  

    cs.settings.UTC_OFFSET = atof(server->arg("_timeoffset").c_str());
    cs.settings.DST = server->arg("_dst").length()>0;    

    cs.settings.THINGSPEAK = server->arg("_ts").length()>0;
        
    cs.settings.TS_CHANNEL = atol(server->arg("_ts_channel").c_str());
    strncpy(cs.settings.TS_API_WRITE, server->arg("_ts_write_api_key").c_str(), 17); 
    cs.settings.TS_FIELD_TEMP = atoi(server->arg("_ts_field_temp").c_str());
    cs.settings.TS_FIELD_MOIS = atoi(server->arg("_ts_field_mois").c_str());
    cs.settings.TS_UPDATE_INTERVAL = atoi(server->arg("_ts_update_interval").c_str());

    cs.print();          
    cs.write();
  }
  server->send(200, "text/html", "OK - restart");
  
  restart(1);
} 

void MyWifi::handle_data(){
  if(fDataHandler!=NULL){
      server->send(200, "application/json", fDataHandler());
  }else{
      server->send(200, "application/json", "{}");
  }
}

void MyWifi::setDataHandler(fncHandleData fDataHandler){
  this->fDataHandler = fDataHandler;
}

void MyWifi::restart(unsigned int inSec){
    delay(inSec*1000);
    //reset and try again, or maybe put it to deep sleep
    Serial.print("restart in "); Serial.print(inSec); Serial.println(" sec");    
    digitalWrite(0,HIGH);  //from some reason this has to be set before reset|restart https://github.com/esp8266/Arduino/issues/1017
    ESP.reset();
    delay(5000);  
}

void MyWifi::forceManualConfig(const char* APname){
      if (!wifiManager->startConfigPortal((String(APname)+"-OnDemandAP").c_str())) {
        Serial.println("failed to connect and hit timeout");
        restart(3);
      }  
}


//---------------------------------------------------------
String MyWifi::getIP(){
  char ipAddr[16]; //aaa.bbb.ccc.ddd
  IPAddress ip = WiFi.localIP();
  sprintf(ipAddr, "%d\.%d\.%d\.%d", ip[0], ip[1], ip[2], ip[3]);
  return String(ipAddr);
/*
  String s="";
  IPAddress ip = WiFi.localIP();
  for (int i=0; i<4; i++)
    s += i  ? "." + String(ip[i]) : String(ip[i]);
  return s;
*/
}
String MyWifi::getMAC(){
  return WiFi.macAddress();
}

//---------------------------------------------------------
void MyWifi::setup(const char* APname, int timeout_in_sec){
  // Uncomment for testing wifi manager
  //  wifiManager.resetSettings();
  //or use this for auto generated name ESP + ChipID

  //Manual Wifi
  //WiFi->begin(WIFI_SSID, WIFI_PWD);

  //sets timeout until configuration portal gets turned off
  //useful to make it all retry or go to sleep
  //in seconds
  wifiManager->setTimeout((timeout_in_sec<0)?180:timeout_in_sec);

  String hostname = ((APname==NULL)?String(HOSTNAME):String(APname)) + String("-") + String(ESP.getChipId(), HEX);
  //fetches ssid and pass and tries to connect
  //if it does not connect it starts an access point with the specified name
  //here  "AutoConnectAP"
  //and goes into a blocking loop awaiting configuration
  Serial.println("AP: "+hostname);
  if(!wifiManager->autoConnect( hostname.c_str() ) ){
    Serial.println("failed to connect and hit timeout");
    
    Serial.println("switching to no wifi mode");
    //restart(); //no wifi mode - uncomment the restart() if endess retry is needed
  }else{     
      // OTA Setup
      WiFi.hostname(hostname);
      MDNS.begin( (hostname+"-webupdate").c_str() );
      httpUpdater->setup(server, "/firmware", update_username, update_password );
    
    
      //user setting handling
      server->on("/", std::bind(&MyWifi::handle_root, this));
      server->on("/offset", std::bind(&MyWifi::handle_store_settings,this)); 
      server->on("/data",  std::bind(&MyWifi::handle_data,this)); 
    
      server->begin(); 
      Serial.println("HTTP server started");
    
      MDNS.addService("http", "tcp", 80); 
  }
  
  Serial.println("reading settings ...");
  cs.init();
  cs.read();   
  cs.print();
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

