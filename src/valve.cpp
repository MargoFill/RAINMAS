#include <Arduino.h>
#include <MKRWAN.h>

#define VALVE_PIN 4


void initValve() {
  Serial.begin(115200);
  //while (!Serial);

  //if (!modem.begin(EU868)) {
   // Serial.println("Failed to start module");
   // while (1);
  //}

  // Connecting to LoRa network
  //modem.joinOTAA("a8610a3539298008", "6c54a728-1b0c-425f-a1c4-cc798292c98d", "b388de6914bfdc1f6f473deb8f60930c");

  // Connect valve pin
  pinMode(VALVE_PIN, OUTPUT); 
  digitalWrite(VALVE_PIN, LOW); 
}

void setValveState(bool open) {
  digitalWrite(VALVE_PIN, open ? HIGH : LOW); 
}


void toggleValve() {
  digitalWrite(VALVE_PIN, !digitalRead(VALVE_PIN));
}
