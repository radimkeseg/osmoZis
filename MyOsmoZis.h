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

/*
//in case of instability of the ocilator, this part can be used, althougt no issue observed, keeping the piece just in case

//osmoZis generator 200Hz - software dividing from 1kHz to 200hz
volatile byte state = LOW;
volatile byte count = 0;

void ICACHE_RAM_ATTR divide() {
  if(count == 0) state = !state;
  if(count == 1) state = !state;
  if(count == 9) count = 0; //==9 if CHANGE or ==4 if RISING
  else count++;
  digitalWrite(D8, state);
}


// the trick to generate impulses using PWM - 1kHz
void setupOsmoZis(){  
  pinMode(D6, OUTPUT);
  pinMode(D7, INPUT);
  pinMode(D8, OUTPUT);
  analogWrite(D6, 128);
  attachInterrupt(digitalPinToInterrupt(D7), divide, CHANGE);//CHANGE or RISING
}
*/

void setupOsmoZis(){
  pinMode(D8, OUTPUT);
  analogWriteFreq(200);
  analogWrite(D8, 10);
}

