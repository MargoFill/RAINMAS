#ifndef LORAWANHANDLER_H
#define LORAWANHANDLER_H

#include <Arduino.h>
#include <MKRWAN.h>
#include "credentials.h"
#include "sensorsid.h"


extern LoRaModem modem;

void initLora();


void sendLoraMultiData (uint8_t sensorIDs[], int values1[], uint8_t sensorCount);

#endif