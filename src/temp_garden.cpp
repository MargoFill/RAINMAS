#include <DHT.h>
#define DHTPIN 2
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

void initTempSensor() {
  dht.begin();
}

float readTempSensor(){
  float temp = dht.readTemperature();

  if (isnan(temp)) {
    Serial.println("Failed to read from DHT sensor!");
    return -999.0;
  }


//Serial.print("Temperature: ");
//Serial.print(temperature);
//Serial.println(" °C");
return temp;


}