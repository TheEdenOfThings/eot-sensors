/**
 * BasicHTTPClient.ino
 *
 *  Created on: 24.05.2015
 *
 */
 #include <DHT11.h>
#include <Arduino.h>
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <PCF8591.h>
#include <ESP8266HTTPClient.h>
#define USE_SERIAL Serial


ESP8266WiFiMulti WiFiMulti;
PCF8591 a2d;
DHT11 sensor{2};
int Sequence = 0;
String live = "0";


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
     
    }

     if(sensor.UpdateReadings() == DHT11::UpdateResult::OK)
  {
    Serial.println("Humidity: ");
    Serial.print(sensor.Humidity());
    Serial.println("");
  }


    
  }

  



void SendLightData(){
        HTTPClient http;

        USE_SERIAL.print("[HTTP] begin...\n");
        // configure traged server and url
        //http.begin("178.62.121.17", 80, "/api/station/1/temp", true, "7a 9c f4 db 40 d3 62 5a 6e 21 bc 5c cc 66 c8 3e a1 45 59 38"); //HTTPS
        http.begin("178.62.121.17", 80, "/api/station/1/light"); //HTTP

        USE_SERIAL.print("[HTTP] POST...\n");

        //data
        // start connection and send HT(a2d.Data(3))TP header
        
        String payload = "light="+String(a2d.Data(1))+"&sequence="+String(Sequence)+"&live="+live;
        
        int httpCode = http.POST(payload);
        if(httpCode) {
            // HTTP header has been send and Server response header has been handled
            USE_SERIAL.printf("[HTTP] POST... code: %d\n", httpCode,Sequence);

             
             

            // file found at server
            if(httpCode == 200) {
                USE_SERIAL.println();
                
            }
        } else {
            USE_SERIAL.print("[HTTP] POST... failed, no connection or no HTTP server\n");
        }
  
}

void SendTempData(){
        HTTPClient http;

        USE_SERIAL.print("[HTTP] begin...\n");
        // configure traged server and url
        //http.begin("178.62.121.17", 80, "/api/station/1/temp", true, "7a 9c f4 db 40 d3 62 5a 6e 21 bc 5c cc 66 c8 3e a1 45 59 38"); //HTTPS
        http.begin("178.62.121.17", 80, "/api/station/1/temp"); //HTTP

        USE_SERIAL.print("[HTTP] POST...\n");

        //data
        
        
        // start connection and send HTTP header
        String payload = "temp="+String(a2d.Data(3))+"&sequence="+String(Sequence)+"&live="+live;
        
        int httpCode = http.POST(payload);
        if(httpCode) {
            // HTTP header has been send and Server response header has been handled
            USE_SERIAL.printf("[HTTP] POST... code: %d\n", httpCode,Sequence);

             
             

            // file found at server
            if(httpCode == 200) {
                USE_SERIAL.println();
                
            }
        } else {
            USE_SERIAL.print("[HTTP] POST... failed, no connection or no HTTP server\n");
        }
  
}
void SendHumidityData(){
        HTTPClient http;

        USE_SERIAL.print("[HTTP] begin...\n");
        // configure traged server and url
        //http.begin("178.62.121.17", 80, "/api/station/1/temp", true, "7a 9c f4 db 40 d3 62 5a 6e 21 bc 5c cc 66 c8 3e a1 45 59 38"); //HTTPS
        http.begin("178.62.121.17", 80, "/api/station/1/humidity"); //HTTP

        USE_SERIAL.print("[HTTP] POST...\n");

        //data
        
        
        // start connection and send HTTP header
        String payload = "Humidity="+String(sensor.Humidity())+"&sequence="+String(Sequence)+"&live="+live;
        
        int httpCode = http.POST(payload);
        if(httpCode) {
            // HTTP header has been send and Server response header has been handled
            USE_SERIAL.printf("[HTTP] POST... code: %d\n", httpCode,Sequence);

             
             

            // file found at server
            if(httpCode == 200) {
                USE_SERIAL.println();
                
            }
        } else {
            USE_SERIAL.print("[HTTP] POST... failed, no connection or no HTTP server\n");
        }
  
}
  void loop() {
      // wait for WiFi connection
      if((WiFiMulti.run() == WL_CONNECTED)) {
        dataSent();
        SendLightData();
        SendTempData();
        SendHumidityData();
        Sequence++;
      }
  
      delay(60000);
  }

