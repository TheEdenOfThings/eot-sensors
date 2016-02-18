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
#include <OneWire.h>
#include <DallasTemperature.h>
//#define USE_SERIAL Serial

// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 1
#define TEMPERATURE_PRECISION 9 // Lower resolution

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

int numberOfDevices; // Number of temperature devices found

DeviceAddress tempDeviceAddress; // We'll use this variable to store a found device address

ESP8266WiFiMulti WiFiMulti;
PCF8591 a2d;
DHT11 sensor{2};
int Sequence = 0;
String live = "0";


void setup() {

    //USE_SERIAL.begin(115200);
   // USE_SERIAL.setDebugOutput(true);

    //USE_SERIAL.println();
    //USE_SERIAL.println();
    //USE_SERIAL.println();
    //Serial.begin(115200);
    Wire.begin(2, 0);
    a2d.InitialiseDevice();
      // Start up the library
    sensors.begin();
    
    // Grab a count of devices on the wire
    numberOfDevices = sensors.getDeviceCount();
    // Loop through each device, print out address
    for(int i=0;i<numberOfDevices; i++)
    {
      // Search the wire for address
      if(sensors.getAddress(tempDeviceAddress, i))
      {
    
      // set the resolution to TEMPERATURE_PRECISION bit (Each Dallas/Maxim device is capable of several different resolutions)
      sensors.setResolution(tempDeviceAddress, TEMPERATURE_PRECISION);
      
  
      
      }
    }


    //for(uint8_t t = 4; t > 0; t--) {
       // USE_SERIAL.printf("[SETUP] WAIT %d...\n", t);
       // USE_SERIAL.flush();
        //delay(1000);
    //}

    WiFiMulti.addAP("EdenOfThings", "edenofthings");

}

void dataSent (){

 /* if(a2d.UpdateReadings()) {
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

*/
    a2d.UpdateReadings();
    sensor.UpdateReadings();
    sensors.requestTemperatures();
  }

  



void SendLightData(){
        HTTPClient http;

       // USE_SERIAL.print("[HTTP] begin...\n");
        // configure traged server and url
        //http.begin("178.62.121.17", 80, "/api/station/1/temp", true, "7a 9c f4 db 40 d3 62 5a 6e 21 bc 5c cc 66 c8 3e a1 45 59 38"); //HTTPS
        http.begin("178.62.121.17", 80, "/api/station/1/light"); //HTTP

       // USE_SERIAL.print("[HTTP] POST...\n");

        //datasensors.requestTemperatures();
        // start connection and send HT(a2d.Data(3))TP header
        
        String payload = "light="+String(a2d.Data(1))+"&sequence="+String(Sequence)+"&live="+live;
        
        int httpCode = http.POST(payload);
        if(httpCode) {
            // HTTP header has been send and Server response header has been handled
           // USE_SERIAL.printf("[HTTP] POST... code: %d\n", httpCode,Sequence);

             
             

            // file found at server
            if(httpCode == 200) {
               // USE_SERIAL.println();
                
            }
        } else {
           // USE_SERIAL.print("[HTTP] POST... failed, no connection or no HTTP server\n");
        }
  
}

void SendTempData(){
        HTTPClient http;

        //USE_SERIAL.print("[HTTP] begin...\n");
        // configure traged server and url
        //http.begin("178.62.121.17", 80, "/api/station/1/temp", true, "7a 9c f4 db 40 d3 62 5a 6e 21 bc 5c cc 66 c8 3e a1 45 59 38"); //HTTPS
        http.begin("178.62.121.17", 80, "/api/station/1/temp"); //HTTP

       // USE_SERIAL.print("[HTTP] POST...\n");

        //datasensors.requestTemperatures();
        
        
        // start connection and send HTTP header
        String payload = "systemtemp="+String(a2d.Data(3))+"&sequence="+String(Sequence)+"&live="+live;
        
        int httpCode = http.POST(payload);
        if(httpCode) {
            // HTTP header has been send and Server response header has been handled
           // USE_SERIAL.printf("[HTTP] POST... code: %d\n", httpCode,Sequence);

             
             

            // file found at server
            if(httpCode == 200) {
                //USE_SERIAL.println();
                
            }
        } else {
            //USE_SERIAL.print("[HTTP] POST... failed, no connection or no HTTP server\n");
        }
  
}
void SendHumidityData(){
        HTTPClient http;

        //USE_SERIAL.print("[HTTP] begin...\n");
        // configure traged server and url
        //http.begin("178.62.121.17", 80, "/api/station/1/temp", true, "7a 9c f4 db 40 d3 62 5a 6e 21 bc 5c cc 66 c8 3e a1 45 59 38"); //HTTPS
        http.begin("178.62.121.17", 80, "/api/station/1/humidity"); //HTTP

        //USE_SERIAL.print("[HTTP] POST...\n");

        //data
        
        
        // start connection and send HTTP header
        String payload = "Humidity="+String(sensor.Humidity())+"&sequence="+String(Sequence)+"&live="+live;
        
        int httpCode = http.POST(payload);
        if(httpCode) {
            // HTTP header has been send and Server response header has been handled
           // USE_SERIAL.printf("[HTTP] POST... code: %d\n", httpCode,Sequence);

             
             

            // file found at server
            if(httpCode == 200) {
                //USE_SERIAL.println();
                
            }
        } else {
            //USE_SERIAL.print("[HTTP] POST... failed, no connection or no HTTP server\n");
        }
  
}

void SendWaterTempData(){
        HTTPClient http;

        //USE_SERIAL.print("[HTTP] begin...\n");
        // configure traged server and url
        //http.begin("178.62.121.17", 80, "/api/station/1/temp", true, "7a 9c f4 db 40 d3 62 5a 6e 21 bc 5c cc 66 c8 3e a1 45 59 38"); //HTTPS
        http.begin("178.62.121.17", 80, "/api/station/1/water"); //HTTP

        //USE_SERIAL.print("[HTTP] POST...\n");

        //data
         // Loop through each device, print out temperature data       
         // start connection and send HTTP header
        String payload = +"&sequence="+String(Sequence)+"live="+live;
        
      for(int i=0;i<numberOfDevices; i++)
       {
        // Search the wire for address
        if(sensors.getAddress(tempDeviceAddress, i))
       {
        // Output the device ID
       
        float tempC = sensors.getTempC(tempDeviceAddress);
        payload = payload + "&Temp_"+String(i)+"="+String(tempC); 
        // It responds almost immediately. Let's print out the data
        } 
      //else ghost device! Check your power requirements and cabling
      
      }
        

        
        int httpCode = http.POST(payload);
       
  
}
  void loop() {
      // wait for WiFi connection
      if((WiFiMulti.run() == WL_CONNECTED)) {
        SendWaterTempData();
        dataSent();
        SendLightData();
        SendTempData();
        SendHumidityData();
        Sequence++;
      }
  
      delay(1000);
  }

