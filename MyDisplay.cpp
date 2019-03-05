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

#include "MyDisplay.h"

#include "Arduino.h" //not sure why but here it is needed

void MyDisplay::begin(){
  display->begin(SSD1306_SWITCHCAPVCC);
  display->clearDisplay();
}

void MyDisplay::write_intro(bool manual) {
  // text display tests
  display->setTextSize(2);
  display->setTextColor(WHITE);
  display->setCursor(4,16);
  //display->setTextColor(BLACK, WHITE); // 'inverted' text
  display->print("OsmoZis v2");
  display->setTextSize(1);
  display->setTextColor(WHITE);
  display->setCursor(20,36);
  display->print("by Radim Keseg");

  display->setTextColor(WHITE);
  display->setCursor(0,54);
  if(manual) display->print("manual wifi config ?");
  else display->print("auto wifi config 1min");
  
  showDisplay(); 
}

void MyDisplay::write_moisture(int x, int y, float moisture) {
  display->drawBitmap(x, y,  logo16_mois_bmp, 16, 16, 1);
  display->setTextColor(WHITE);
  display->setTextSize(2);
  
  display->setCursor(x+22,y+1);
  //display->print(":");
  display->setTextColor(WHITE);
  display->setTextSize(2);
  display->print(moisture);

  display->setTextColor(INVERSE);
  display->setCursor(x+12,y);
  display->setTextSize(1);
  display->println("%");
}

void MyDisplay::write_temp(int x, int y, float temperature) {
  display->drawBitmap(x, y,  logo16_temp_bmp, 16, 16, 1);
  display->setTextColor(WHITE);
  display->setTextSize(2);

  display->setCursor(x+22,y+1);
//  display->print(":");
  display->setTextColor(WHITE);
  display->setTextSize(2);
  display->print(temperature);

  display->setTextColor(INVERSE);
  display->setCursor(x+12,y);
  display->setTextSize(1);
  display->print("C");
}

//---------------------------------------
void MyDisplay::showDisplay(){
  display->display();
}

void MyDisplay::clearDisplay(){
  display->clearDisplay();
}

//---------------------------------------
void MyDisplay::write_time(int x, int y, const char* formatedTime){
  display->setTextSize(1);
  display->setTextColor(WHITE);
  display->setCursor(x,y);

  //right align the text
  uint16_t _x=0, _y=0, _w=0, _h=0;
  display->getTextBounds((char*)formatedTime,(int16_t)x,(int16_t)y,(int16_t*)&_x,(int16_t*)&_y,(uint16_t*)&_w,(uint16_t*)&_h);
  display->setCursor(x-_w-1,y); //x right
      
  display->print(formatedTime);
}

void MyDisplay::write_IP(int x, int y, const char* formatedIP){
  display->setTextSize(1);
  display->setTextColor(WHITE);
  display->setCursor(x,y);
  display->print(formatedIP);  
}

void MyDisplay::write_gauge(int x, int y, float value, bool showText, float min, float max){
  int lValue = value;
  if(value<min) value=min;
  if(value>max) value=max;
  value=map(value,min,max,0,100);

  clear_rect(x-2,y,34,16);
  display->drawBitmap(x, y, gauge_bmp, 32, 16, 1); 
  display->fillTriangle( x+16-1  , y+15,
                         -cos(value/100*3.14)*15+x+16, -sin(value/100*3.14)*15+y+16,
                         x+16+1, y+15, 
                         WHITE);
  for(int i=1; i<value; i+=5){
//    display->drawPixel( -cos(3.14*i/100)*8+x+16, -sin(3.14*i/100)*8+y+16,WHITE);
    display->drawPixel( -cos(3.14*i/100)*10+x+16, -sin(3.14*i/100)*10+y+16,WHITE);
    display->drawPixel( -cos(3.14*i/100)*12+x+16, -sin(3.14*i/100)*12+y+16,WHITE);
    display->drawPixel( -cos(3.14*i/100)*14+x+16, -sin(3.14*i/100)*14+y+16,WHITE);
/*
      display->drawLine( -cos(3.14*i/100)*8+x+16, -sin(3.14*i/100)*8+y+16,
                         -cos(3.14*i/100)*15+x+16, -sin(3.14*i/100)*15+y+16,
                             WHITE);      
*/
  }
  
  if(showText){
    display->setTextSize(1);
    display->setTextColor(WHITE);
    
    //center the text
    uint16_t _x=0, _y=0, _w=0, _h=0;
    display->getTextBounds((char*)String(lValue).c_str(),(int16_t)x,(int16_t)y,(int16_t*)&_x,(int16_t*)&_y,(uint16_t*)&_w,(uint16_t*)&_h);
    display->setCursor(x+(32-_w)/2,y+18); //x center
    
    display->print(lValue);
  }
  
}

void MyDisplay::write_progress(int x, int y, int width, int height, float value, bool showText, float min, float max){
  int lValue = value;
  if(value<min) value=min;
  if(value>max) value=max;
  value=map(value,min,max,0,100);
//  display->drawBitmap(x, y, gauge_bmp, 32, 16, 1); 
  if(height>2) display->drawRect( x, y, width, height, WHITE);
  display->fillRect( x, y, value/100*width, height, WHITE);
  
  if(showText){
    display->setTextSize(1);
    display->setTextColor(INVERSE);
    uint16_t _x=0, _y=0, _w=0, _h=0;
    display->getTextBounds((char*)String(lValue).c_str(),(int16_t)x,(int16_t)y,(int16_t*)&_x,(int16_t*)&_y,(uint16_t*)&_w,(uint16_t*)&_h);
    display->setCursor(x+(width-_w)/2,y+1); //center the value
    display->print(lValue);
  }
  
}

//--------------------------------------
void MyDisplay::clear_rect(int x, int y, unsigned int width, unsigned int height){
  display->fillRect( x, y, width, height, BLACK);
}


