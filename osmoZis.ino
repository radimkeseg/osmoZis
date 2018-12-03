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



//osmoZis generator 200Hz
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
  myDallas.begin();
  myDisplay.write_intro();

  myWifi.setup();

  myThingSpeak.begin(myWifi.getWifiClient());
  delay(1000);
}

void loop(void) {
  // Handle web server
  myWifi.handleClient();

  Serial.println("Reading sensors:");
  myMoisture.measure();
  myDallas.measure();
  myThingSpeak.write(myDallas.getLastMeasured(),myMoisture.getLastMeasured());

  myDisplay.clearDisplay(); 
  myDisplay.write_moisture(myMoisture.getLastMeasured());
  myDisplay.write_temp(myDallas.getLastMeasured());
  myDisplay.showDisplay();
  
  Serial.println("");
  delay(10000);
}

