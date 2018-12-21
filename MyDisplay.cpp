#include "MyDisplay.h"

#include "Arduino.h" //not sure why but here it is needed

void MyDisplay::begin(){
  display->begin(SSD1306_SWITCHCAPVCC);
  display->clearDisplay();
}

void MyDisplay::write_intro() {
  // text display tests
  display->setTextSize(2);
  display->setTextColor(WHITE);
  display->setCursor(0,16);
  display->setTextColor(BLACK, WHITE); // 'inverted' text
  display->print("osmoZis");
  display->setTextSize(1);
  display->setTextColor(WHITE);
  display->setCursor(0,36);
  display->print("by Radim Keseg");

  display->setTextColor(WHITE);
  display->setCursor(0,46);
  display->print("wifi config ...");
  
  showDisplay();
}

void MyDisplay::write_moisture(float moisture) {
  display->drawBitmap(6, 24,  logo16_mois_bmp, 16, 16, 1);
  display->setTextSize(2);
  display->setTextColor(WHITE);
  display->setCursor(26,26);
  //display->setTextColor(BLACK, WHITE); // 'inverted' text
  display->print(":");
  display->setTextSize(2);
  display->setTextColor(WHITE);
  display->print(moisture);
  display->println("%");
}

void MyDisplay::write_temp(float temperature) {
  display->drawBitmap(6, 40,  logo16_temp_bmp, 16, 16, 1);
  display->setTextSize(2);
  display->setTextColor(WHITE);
  display->setCursor(26,42);
  //display->setTextColor(BLACK, WHITE); // 'inverted' text
  display->print(":");
  display->setTextSize(2);
  display->setTextColor(WHITE);
  display->print(temperature);
  display->print("C");
}

void MyDisplay::write_time(const char* formatedTime){
  display->setTextSize(1);
  display->setTextColor(WHITE);
  display->setCursor(76,8);
  display->print(formatedTime);
}

void MyDisplay::showDisplay(){
  display->display();
}

void MyDisplay::clearDisplay(){
  display->clearDisplay();
}


