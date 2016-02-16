#include <PCF8591.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <stdio.h>

//PCF8591 a2d{0, 2};
const char* ssid = "EdenOfThings";
const char* password = "edenofthings";

ESP8266WebServer server(80);

int A2dReading[4];

void handleRoot() {
  char buffer [16];
  sprintf(buffer, "%i", A2dReading[2]);
  server.send(200, "text/plain", buffer);

  
}

void handleNotFound(){
  
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);

}
void setup(void){

  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);

  server.on("/inline", [](){
    server.send(200, "text/plain", "this works as well");
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void){
  PCF8591 a2d{2, 0};
  server.handleClient();
  a2d.UpdateReadings();
  A2dReading[0] = a2d.Data(0);
  A2dReading[1] = a2d.Data(1);
  A2dReading[2] = a2d.Data(2);
  A2dReading[3] = a2d.Data(3);
  
}
  






  
