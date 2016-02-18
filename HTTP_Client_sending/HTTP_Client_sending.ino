/**
 * BasicHTTPClient.ino
 *
 *  Created on: 24.05.2015
 *
 */

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>

#define USE_SERIAL Serial

ESP8266WiFiMulti WiFiMulti;

int Sequence = 0;


void setup() {

    USE_SERIAL.begin(115200);
   // USE_SERIAL.setDebugOutput(true);

    USE_SERIAL.println();
    USE_SERIAL.println();
    USE_SERIAL.println();

    for(uint8_t t = 4; t > 0; t--) {
        USE_SERIAL.printf("[SETUP] WAIT %d...\n", t);
        USE_SERIAL.flush();
        delay(1000);
    }

    WiFiMulti.addAP("EdenOfThings", "edenofthings");

}

void loop() {
    // wait for WiFi connection
    if((WiFiMulti.run() == WL_CONNECTED)) {

        HTTPClient http;

        USE_SERIAL.print("[HTTP] begin...\n");
        // configure traged server and url
        //http.begin("178.62.121.17", 80, "/api/station/1/temp", true, "7a 9c f4 db 40 d3 62 5a 6e 21 bc 5c cc 66 c8 3e a1 45 59 38"); //HTTPS
        http.begin("178.62.121.17", 80, "/api/station/1/temp"); //HTTP

        USE_SERIAL.print("[HTTP] GET...\n");

        //data
        
        
        // start connection and send HTTP header
    String payload = "temp=25.5&sequence="+String(Sequence);
        
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

