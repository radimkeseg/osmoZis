/**The MIT License (MIT)

Copyright (c) 2017 by Radim Keseg

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

#include "TimeClient.h"

TimeClient::TimeClient(float utcOffset) {
  setTimeOffset(utcOffset);
}

void TimeClient::setTimeOffset(float utcOffset){
  myUtcOffset = utcOffset;
}

void TimeClient::updateTime() {
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect("www.google.com", httpPort)) {
    Serial.println("connection failed");
    return;
  }
  
  // This will send the request to the server
  client.print(String("GET / HTTP/1.1\r\n") +
               String("Host: www.google.com\r\n") + 
               String("Connection: close\r\n\r\n"));
  int repeatCounter = 0;
  while(!client.available() && repeatCounter < 10) {
    delay(1000); 
    Serial.println(".");
    repeatCounter++;
  }

  String line;

  uint32_t theTime = millis();
  int size = 0;
  client.setNoDelay(false);
  while(client.connected()) {
    if( ( millis() - theTime ) > 30*1000 || millis() < theTime ){Serial.print("INFINITE LOOP BREAK!"); break;}  //exit loop after one second.
    while((size = client.available()) > 0) {
      line = client.readStringUntil('\n');
      line.toUpperCase();
      // example: 
      // date: Thu, 19 Nov 2015 20:25:40 GMT
      if (line.startsWith("DATE: ")) {
        Serial.println(line.substring(23, 25) + ":" + line.substring(26, 28) + ":" +line.substring(29, 31));
        int parsedHours = line.substring(23, 25).toInt();
        int parsedMinutes = line.substring(26, 28).toInt();
        int parsedSeconds = line.substring(29, 31).toInt();
        Serial.println(String(parsedHours) + ":" + String(parsedMinutes) + ":" + String(parsedSeconds));

        localEpoc = (parsedHours * 60 * 60 + parsedMinutes * 60 + parsedSeconds);
        Serial.println(localEpoc);
        localMillisAtUpdate = millis();
      }
    }
  }

}

void TimeClient::checkForUpdate() {
  long stamp = millis();
  if(stamp - last_time_update > time_update_interval || stamp < last_time_update ){ //updates if the update interval has expired only
    last_time_update = stamp;
    updateTime();
  } 
}

String TimeClient::getHours() {
    if (localEpoc == 0) {
      return "--";
    }
    int hours = getHoursInt();
    if (hours < 10) {
      return "0" + String(hours);
    }
    return String(hours); // print the hour (86400 equals secs per day)

}
String TimeClient::getMinutes() {
    if (localEpoc == 0) {
      return "--";
    }
    int minutes = getMinutesInt();
    if (minutes < 10 ) {
      // In the first 10 minutes of each hour, we'll want a leading '0'
      return "0" + String(minutes);
    }
    return String(minutes);
}
String TimeClient::getSeconds() {
    if (localEpoc == 0) {
      return "--";
    }
    int seconds = getSecondsInt();
    if ( seconds < 10 ) {
      // In the first 10 seconds of each minute, we'll want a leading '0'
      return "0" + String(seconds);
    }
    return String(seconds);
}

String TimeClient::getFormattedTime() {
  checkForUpdate();
  return getHours() + ":" + getMinutes() + ":" + getSeconds();
}

long TimeClient::getCurrentEpoch() {
  return localEpoc + ((millis() - localMillisAtUpdate) / 1000);
}

long TimeClient::getCurrentEpochWithUtcOffset() {
  return round(getCurrentEpoch() + 3600 * myUtcOffset + 86400L) % 86400L;
}

int TimeClient::getHoursInt() {
  return ((getCurrentEpochWithUtcOffset()  % 86400L) / 3600) % 24;
}
int TimeClient::getMinutesInt() {
  return ((getCurrentEpochWithUtcOffset() % 3600) / 60);
}
int TimeClient::getSecondsInt() {
  return getCurrentEpochWithUtcOffset() % 60;
}
