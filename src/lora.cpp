#include "lorawanhandler.h"


LoRaModem modem;

void initLora(){
    Serial.println("Initializing LoRa...");
    if (!modem.begin(EU868))
    {

        Serial.println("Failed to start LoRa modem!");
        while (1)
        {
        
        };
        
    }

    Serial.println("Joining LoRaWAN network...");
    Serial.print("Using AppEUI : ");
    Serial.println(cr.appEUI);
    Serial.print("Using AppKey : ");
    Serial.println(cr.appKey);

    modem.joinOTAA(cr.appEUI, cr.appKey);
    
}

//encoding data and sending it to the server
 void sendLoraMultiData (uint8_t sensorIDs[], int values1[], uint8_t sensorCount){
    //Serial.println("Sending data..."); 

    uint8_t payload[20];  
    uint8_t index = 0;

    for (uint8_t i =0; i<sensorCount; i++){
        payload[index++] = sensorIDs[i];

        switch (sensorIDs[i])
        {
            case 0x05:{ //Ultrasonic water level
                uint16_t distance = static_cast<uint16_t>(values1[i]);
            
                payload[index++] = (distance >> 8) & 0xFF;
                payload[index++] = distance & 0xFF;
                break;
            }

            case 0x06:{ // Temperature

                uint16_t temp = static_cast<uint16_t>(values1[i]);

                payload[index++] = (temp >> 8) & 0xFF;
                payload[index++] = temp & 0xFF;
                break;
            }

            case 0x07:{ // Moisture
                uint16_t mois = static_cast<uint16_t>(values1[i]);

                payload[index++] = (mois >> 8) & 0xFF;
                payload[index++] = mois & 0xFF;
                break;
            }
            case 0x08:{ //TDS
                uint16_t tds = static_cast<uint16_t>(values1[i]);

                payload[index++] = (tds >> 8) & 0xFF;
                payload[index++] = tds & 0xFF;
                break;

            }
            case 0x09:{ // pH
                uint16_t pHValue = static_cast<uint16_t>(values1[i]);

                payload[index++] = (pHValue >> 8) & 0xFF;
                payload[index++] = pHValue & 0xFF;
                break;
            }
        } 
    }

    modem.beginPacket();
    modem.write(payload, index);
    int success = modem.endPacket(true);

    Serial.println(success ? "Sent!" : "Didn't send");
    
 }