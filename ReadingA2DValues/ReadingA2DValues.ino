#include <PCF8591.h>
#include <Wire.h>

PCF8591 a2d;

void setup()
{
  Serial.begin(115200);
  Wire.begin(2, 0);
  a2d.InitialiseDevice();
}

void loop()
{
  if(a2d.UpdateReadings())
  {
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
  delay(5000);
}
