#include "LedControl.h"
#include <ESP8266WiFi.h>
#include "TimeClient.h"
#include <WiFiUdp.h>

/*
   pin D8 is connected to the DataIn
   pin D7 is connected to the CLK
   pin D6 is connected to LOAD
   We have only a single MAX72XX.
 */
LedControl lc=LedControl(D8,D7,D6,1);

boolean tick = false;

long lastUpdate = millis();
long lastSecond = millis();

String hours, minutes, seconds;
int currentSecond, currentMinute, currentHour;

char ssid[] = "Earthshine";  //  your network SSID (name)
char pass[] = "DoAndroidsDreamOfElectricSheep2016";       // your network password

const float UTC_OFFSET = 0;
TimeClient timeClient(UTC_OFFSET);

void setup() {
        Serial.begin(115200);
        Serial.println();
        Serial.println();

        lc.shutdown(0,false);
        /* Set the brightness to a medium values */
        lc.setIntensity(0,8);
        /* and clear the display */
        lc.clearDisplay(0);
        // We start by connecting to a WiFi network
        Serial.print("Connecting to ");
        Serial.println(ssid);
        WiFi.begin(ssid, pass);
        WiFi.begin(ssid, pass);

        while (WiFi.status() != WL_CONNECTED) {
                delay(500);
                Serial.print(".");
        }
        Serial.println("");

        Serial.println("WiFi connected");
        Serial.println("IP address: ");
        Serial.println(WiFi.localIP());
        timeClient.updateTime();
        updateTime();
        lastUpdate = millis();
        lastSecond = millis();
}

void loop()
{
//  lc.setDigit(0,3,i,false);
        //
        if ((millis() - lastUpdate) > 1800000) updateTime();

        if ((millis() - lastSecond) > 1000)
        {
                tick = !tick;
                lc.clearDisplay(0);
                lastSecond = millis();
                currentSecond++;
                if (currentSecond > 59)
                { currentSecond = 0;
                  currentMinute++;
                  if (currentMinute > 59) {
                          currentMinute = 0;
                          currentHour++;
                          if (currentHour > 12) currentHour = 0;
                  }}
                String currentTime = "  " + String(currentHour) + ':' + String(currentMinute) + ':' + (currentSecond > 9 ? "" : "0") + String(currentSecond);
                Serial.println(currentTime);

                if (currentHour>12) lc.setDigit(0,0,int(currentHour/10),false);
                lc.setDigit(0,1,int(currentHour%10),(tick ? true : false));
                lc.setDigit(0,2,int(currentMinute/10),false);
                lc.setDigit(0,3,int(currentMinute%10),false);
        }
}

void updateTime() {
        hours = timeClient.getHours();
        minutes = timeClient.getMinutes();
        seconds = timeClient.getSeconds();
        currentHour = hours.toInt();
        if (currentHour>12) currentHour -= 12;
        currentMinute = minutes.toInt();
        currentSecond = seconds.toInt();
        lastUpdate = millis();
}
