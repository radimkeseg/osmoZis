/*********************************************************************
This is an example for our Monochrome OLEDs based on SSD1306 drivers

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/category/63_98

This example is for a 128x64 size display using SPI to communicate
4 or 5 pins are required to interface

Adafruit invests time and resources providing this open source code, 
please support Adafruit and open-source hardware by purchasing 
products from Adafruit!

Written by Limor Fried/Ladyada  for Adafruit Industries.  
BSD license, check license.txt for more information
All text above, and the splash screen must be included in any redistribution
*********************************************************************/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Adafruit_SSD1306.h"

#include <OneWire.h> 
#include <DallasTemperature.h>
#define TEMP_BUS D5 
OneWire oneWire(TEMP_BUS); 
DallasTemperature sensors(&oneWire);

#include "embHTML.h"
#include "settings.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <Interval.h>
#include <ThingSpeak.h>

// Helps with connecting to internet
#include <WiFiManager.h>

//#include <ArduinoOTA.h>
#include <ESP8266mDNS.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>    

// If using software SPI (the default case):
#define OLED_MOSI  D3
#define OLED_CLK   D4
#define OLED_DC    D1
#define OLED_CS    D0
#define OLED_RESET D2
Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

// HOSTNAME for OTA update
#define HOSTNAME "OSMOZA-RKG1-"

//WiFiManager
//Local intialization. Once its business is done, there is no need to keep it around
WiFiManager wifiManager;
ESP8266WebServer server(80);
ESP8266HTTPUpdateServer httpUpdater;  

WiFiClient wfclient;

Interval thingSpeakUpdate;

// ThingSpeak
unsigned long myChannelNumber = 211292;
const char * myWriteAPIKey = "CHF36K1C8B6YQUV4";

#define LOGO16_GLCD_HEIGHT 16 
#define LOGO16_GLCD_WIDTH  16 
static const unsigned char PROGMEM logo16_mois_bmp[] =
{ B00000000, B00000000,
  B00000001, B10000000,
  B00000010, B01000000,
  B00000010, B01000000,
  B00000100, B00100000,
  B00001000, B00010000,
  B00010000, B00001000,
  B00100100, B00000100,
  B00101000, B00000100,
  B00100000, B00000100,
  B00100000, B00000100,
  B00010000, B00001000,
  B00001000, B00010000,
  B00000100, B00100000,
  B00000011, B11000000,
  B00000000, B00000000 };

static const unsigned char PROGMEM logo16_temp_bmp[] =
{ B00000000, B00000000,
  B00000001, B10000000,
  B00000011, B11000000,
  B00000010, B01000000,
  B00000011, B01000000,
  B00000010, B01000000,
  B00000011, B01000000,
  B00000010, B01000000,
  B00000011, B01000000,
  B00000010, B01000000,
  B00000011, B01000000,
  B00000100, B00100000,
  B00001010, B00010000,
  B00001000, B00010000,
  B00000111, B11100000,
  B00000000, B00000000 };

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

#define MEASURING D5 // Wet Indicator at Digital pin D0
#define SENSE_PIN  A0 // sensor input at Analog pin A0

volatile float value_moisture;
volatile float value_temp;

void write_ThingsSpeak(){
  if (isnan(value_moisture) || isnan(value_temp))
  {
    Serial.println("Failed to read from sensors!");
    return;
  }

  Serial.println("ThingsSpeak write");
  Serial.println("=================");
  Serial.println("-----------------");
  Serial.print("Moisture: ");
  Serial.print(value_moisture);
  Serial.println("%");
  Serial.print("Temperature: ");
  Serial.print(value_temp);
  Serial.println("C");
  Serial.println("-----------------");

  //send data to thingSpeak
  ThingSpeak.setField(1,value_temp);
  ThingSpeak.setField(2,value_moisture);
  ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);        
}

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

void write_intro(void) {
  // text display tests
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,10);
  display.setTextColor(BLACK, WHITE); // 'inverted' text
  display.print("osmoZis");
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,30);
  display.print("by Radim Keseg");

  display.setTextColor(WHITE);
  display.setCursor(0,40);
  display.print("wifi config ...");
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

  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC);
  // init done
  
  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  display.display();

  // Clear the buffer.
  display.clearDisplay();

  // miniature bitmap display
  //display.drawBitmap(30, 16,  logo16_glcd_bmp, 16, 16, 1);
  display.display();

  pinMode(MEASURING, OUTPUT);

  pinMode(D6, OUTPUT);
  pinMode(D7, INPUT);
  pinMode(D8, OUTPUT);
  analogWrite(D6, 128);
  attachInterrupt(digitalPinToInterrupt(D7), blink, CHANGE);//CHANGE or RISING

  sensors.begin();

  write_intro();
  display.display();

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

  ThingSpeak.begin(wfclient);
    
  delay(1000);
}

double read_moisture(){
   Serial.print("Moisture: ");
   digitalWrite(MEASURING, LOW);
   value_moisture= analogRead(SENSE_PIN);
   value_moisture= value_moisture/10;
   value_moisture= map(value_moisture,33,96,100,0);
   if(value_moisture>100) value_moisture = 100;
   if(value_moisture<0) value_moisture = 0;
   Serial.print(value_moisture);Serial.println("%");
   delay(100);
   digitalWrite(MEASURING, HIGH);

   return value_moisture;
}

void write_moisture(void) {
  // text display tests
  display.drawBitmap(6, 14,  logo16_mois_bmp, 16, 16, 1);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(26,16);
  //display.setTextColor(BLACK, WHITE); // 'inverted' text
  display.print(":");
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.print((int)value_moisture);
  display.println("%");
}

void write_temp(void) {
  // text display tests
  display.drawBitmap(6, 30,  logo16_temp_bmp, 16, 16, 1);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(26,32);
  //display.setTextColor(BLACK, WHITE); // 'inverted' text
  display.print(":");
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.print(sensors.getTempCByIndex(0));
  display.print("C");
}

double  read_temp(){
 sensors.requestTemperatures(); // Send the command to get temperature readings 
 Serial.print("Temperature: "); 
 value_temp=sensors.getTempCByIndex(0); // Why "byIndex"?
 Serial.print(value_temp);   
 Serial.println("C");   

 return value_temp;
}


void loop(void) {
  // Handle web server
  server.handleClient();

  Serial.println("Reading sensors:");
  read_moisture();
  read_temp();

  if (thingSpeakUpdate.expired()) {
    thingSpeakUpdate.set(THINGSPEAK_UPDATE_INTERVAL*1000); // set new interval period
    write_ThingsSpeak();
  }

  display.clearDisplay(); 
  write_moisture();
  write_temp();
  display.display();
  Serial.println("");

  delay(10000);
}

