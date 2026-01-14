#include <Arduino.h>
#define MOISTUREPIN A0

void initMoistureSensor() {
  //Serial.begin(9600);
}

int readMoistureSensor() {
  int mois=analogRead(MOISTUREPIN);
  //Serial.print("Moisture: ");
  //Serial.println(moisVal);
  return mois;

  //delay(1000);
}
