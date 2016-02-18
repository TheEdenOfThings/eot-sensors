
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
    
    WiFiMulti.addAP("EdenOfThings", "edenofthings");

}

void dataSent (){

    a2d.UpdateReadings();
    sensor.UpdateReadings();
    sensors.requestTemperatures();
  }

void SendLightData(){
        HTTPClient http;

        http.begin("178.62.121.17", 80, "/api/station/1/light"); //HTTP
        String payload = "light="+String(a2d.Data(1))+"&sequence="+String(Sequence)+"&live="+live;
        int httpCode = http.POST(payload);
    
}

void SendTempData(){
        HTTPClient http;

        http.begin("178.62.121.17", 80, "/api/station/1/temp"); 
        String payload = "systemtemp="+String(a2d.Data(3))+"&sequence="+String(Sequence)+"&live="+live;
        int httpCode = http.POST(payload);
  
}
void SendHumidityData(){
        HTTPClient http;

        http.begin("178.62.121.17", 80, "/api/station/1/humidity");
        String payload = "Humidity="+String(sensor.Humidity())+"&sequence="+String(Sequence)+"&live="+live;
        int httpCode = http.POST(payload);
  
}

void SendWaterTempData(){
        HTTPClient http;
        
        http.begin("178.62.121.17", 80, "/api/station/1/water"); 
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

