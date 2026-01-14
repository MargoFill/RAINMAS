#include <Arduino.h>
#include "moisture_sensor_garden.h"
#include "temp_garden.h"
#include "waterlevel_sensor_tank.h"
#include "credentials.h"
#include "lorawanhandler.h"
#include "config.h"
#include "valve.h"
#include "p2psender.h"
#include "p2preceiver.h"
#include "tds.h"
#include "pH_sensor.h"

#ifndef USE_LEVEL
#include "waterlevel_sensor_tank.h"
#endif
#ifndef USE_TEMP
#include "temp_garden.h"
#endif
#ifndef USE_MOISTURE
#include "moisture_sensor_garden.h"
#endif
#ifndef USE_TDS
#include "tds.h"
#endif
#ifndef USE_PH
#include "pH_sensor.h"
#endif


unsigned delayEnvoi= 60000;//1 min, 300000ms-5min, 1800000ms-30min

void setup() {
  Serial.begin(115200);

  initValve();

  #ifndef USE_MOISTURE
  initMoistureSensor();
  #endif

  #ifndef USE_TEMP
  initTempSensor();
  #endif

  #ifndef USE_LEVEL
  initLevelSensor();
  #endif

  #ifndef USE_TDS
  initTDS();
  #endif

  #ifndef USE_PH
  initPH();
  #endif


  initLora();
}

void loop() {

  uint8_t sensorIDs[5];
  int values1[5];
  uint8_t count = 0;
  
  #ifndef USE_LEVEL
    sensorIDs[count]=SENSOR_LEVEL;
    float distance = readLevelSensor();
    Serial.print("Water Level: ");
    Serial.print(distance, 2);
    Serial.println(" cm");
    //values1[count] = (int)levelVal;
    values1[count] = static_cast<int>(distance*100);

    count++;
  #endif
  
  #ifndef USE_TEMP
    sensorIDs[count] = SENSOR_TEMP;
    float temp = readTempSensor();
    //values1[count] = (int)(tempVal * 100); // convert float °C to int (e.g., 23.45 -> 2345) for encoding and decoding
    Serial.print("Temperature: ");
    Serial.print(temp);
    Serial.println(" °C");
    values1[count] = static_cast<int>(temp*100);

    count++;
  #endif

  #ifndef USE_MOISTURE
    sensorIDs[count] = SENSOR_MOISTURE;
    int mois = readMoistureSensor();
    Serial.print("Moisture: ");
    Serial.println(mois);
    //values1[count] = moisVal;
    values1[count] = static_cast<int>(mois);

    count++;
  #endif

  #ifndef USE_TDS
    float tds = readTDS();
    if (millis() >= 60000UL) {
      sensorIDs[count] = SENSOR_TDS;
      //float tds = readTDS();
      Serial.print("TDS: ");
      Serial.println(tds);
      Serial.print("ppm");
      values1[count] = static_cast<int>(tds*100);

      count++;
    } else{
      Serial.println("TDS: TDS sensor hasn't gathered enough data yet");//TDS and PH codes work as an "array codes" so sensors need time to gather enough data for getting a final result of measurements
  #endif

  #ifndef USE_PH
    float pHValue = readPH();
    if (millis() >= 60000UL) {
      sensorIDs[count] = SENSOR_PH;
      Serial.print("pH: ");
      Serial.println(pHValue);
      values1[count] = static_cast<int>(pHValue*100);

      count++;
    } else{
      Serial.println("PH: PH sensor hasn't gathered enough data yet");
    }
  #endif

  Serial.println("--- Sending Data via LoRa ---");
  sendLoraMultiData(sensorIDs, values1, count);
  //int size = modem.receive(64); // wait for downlink
  if (modem.available()) {
    byte val = modem.read();  // read byte
    Serial.print("Received: ");
    Serial.println(val);

    if (val == 1) {
      setValveState(true); // open
    } else if (val == 0) {
      setValveState(false);  //close
    } else if (val == 2) {
      toggleValve(); // toggle
    }
  }


  delay(delayEnvoi);

}

