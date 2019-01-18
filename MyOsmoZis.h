/*
//osmoZis generator 200Hz
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
  analogWrite(D8, 100);
}

