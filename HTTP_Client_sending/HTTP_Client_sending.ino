/**
 * BasicHTTPClient.ino
 *
 *  Created on: 24.05.2015
 *
 */

#include <Arduino.h>
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <PCF8591.h>
#include <ESP8266HTTPClient.h>

#define USE_SERIAL Serial

ESP8266WiFiMulti WiFiMulti;
PCF8591 a2d;

int Sequence = 0;


void setup() {

    USE_SERIAL.begin(115200);
   // USE_SERIAL.setDebugOutput(true);

    USE_SERIAL.println();
    USE_SERIAL.println();
    USE_SERIAL.println();
    Serial.begin(115200);
    Wire.begin(2, 0);
    a2d.InitialiseDevice();

    for(uint8_t t = 4; t > 0; t--) {
        USE_SERIAL.printf("[SETUP] WAIT %d...\n", t);
        USE_SERIAL.flush();
        delay(1000);
    }

    WiFiMulti.addAP("EdenOfThings", "edenofthings");

}

void dataSent (){

  if(a2d.UpdateReadings()) {
      Serial.println("");
      Serial.print("Light Sensor raw data: ");
      Serial.println(a2d.Data(1));
      Serial.println("");
      Serial.print("Twisty knob: ");
      Serial.println(a2d.Data(0));
      Serial.println("");
      Serial.print("Temperature: ");
      Serial.println(a2d.Data(3));
      Serial.println("");
      Serial.print("Random num 2: ");
      Serial.println(a2d.Data(2));
    }


    
  }

  

void loop() {
    // wait for WiFi connection
    if((WiFiMulti.run() == WL_CONNECTED)) {

        HTTPClient http;

        USE_SERIAL.print("[HTTP] begin...\n");
        // configure traged server and url
        //http.begin("178.62.121.17", 80, "/api/station/1/temp", true, "7a 9c f4 db 40 d3 62 5a 6e 21 bc 5c cc 66 c8 3e a1 45 59 38"); //HTTPS
        http.begin("178.62.121.17", 80, "/api/station/1/temp"); //HTTP

        USE_SERIAL.print("[HTTP] POST...\n");

        //data
        
        
        // start connection and send HTTP header
        dataSent();
        String payload = "temp="+String(a2d.Data(3))+"&sequence="+String(Sequence);
        
        int httpCode = http.POST(payload);
        if(httpCode) {
            // HTTP header has been send and Server response header has been handled
            USE_SERIAL.printf("[HTTP] POST... code: %d\n", httpCode,Sequence);

             
             

            // file found at server
            if(httpCode == 200) {
                USE_SERIAL.println();
                Sequence++;
            }
        } else {
            USE_SERIAL.print("[HTTP] POST... failed, no connection or no HTTP server\n");
        }
    }

    delay(10000);
}

