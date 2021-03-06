#include <DHT11.h>

DHT11 sensor{2};

void setup()
{
  Serial.begin(115200);
}

void loop()
{
  if(sensor.UpdateReadings() == DHT11::UpdateResult::OK)
  {
    Serial.println("Humidity: ");
    Serial.print(sensor.Humidity());
    Serial.println("");
  }
  delay(5000);
}
