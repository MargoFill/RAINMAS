#include <Arduino.h>


#define PHSensorPin A2
#define PHVREF 5.0      // analog reference voltage(Volt) of the ADC
#define PHSCOUNT  30     // sum of sample point


int PHanalogBuffer[PHSCOUNT];    // store the analog value in the array, read from ADC
int PHanalogBufferTemp[PHSCOUNT];
int PHanalogBufferIndex = 0;
int PHcopyIndex = 0;
float voltage = 0;
float pHValue = 0;

bool PHbufferFull = false;


int PHgetMedianNum(int phbArray[], int phiFilterLen) 
{
      int phbTab[phiFilterLen];
      for (byte i = 0; i<phiFilterLen; i++)
	  phbTab[i] = phbArray[i];
      int i, j, phbTemp;
      for (j = 0; j < phiFilterLen - 1; j++) 
      {
	  for (i = 0; i < phiFilterLen - j - 1; i++) 
          {
	    if (phbTab[i] > phbTab[i + 1]) 
            {
		phbTemp = phbTab[i];
	        phbTab[i] = phbTab[i + 1];
		phbTab[i + 1] = phbTemp;
	     }
	  }
      }
      if ((phiFilterLen & 1) > 0)
	phbTemp = phbTab[(phiFilterLen - 1) / 2];
      else
	phbTemp = (phbTab[phiFilterLen / 2] + phbTab[phiFilterLen / 2 - 1]) / 2;
      return phbTemp;
}

void initPH()
{
    Serial.begin(115200);
    pinMode(PHSensorPin,INPUT);

    for (int i = 0; i < PHSCOUNT; i++) {
        PHanalogBuffer[i] = analogRead(PHSensorPin);
        delay(10);  // 
    }
}

float readPH(){
   static unsigned long PHanalogSampleTimepoint = millis();
   if(millis()-PHanalogSampleTimepoint > 30U)     //every 40 milliseconds,read the analog value from the ADC
   {
     PHanalogSampleTimepoint = millis();
     PHanalogBuffer[PHanalogBufferIndex] = analogRead(PHSensorPin);    //read the analog value and store into the buffer
     PHanalogBufferIndex++;
     if(PHanalogBufferIndex == PHSCOUNT){ 
      PHanalogBufferIndex = 0;
      PHbufferFull = true;
     }
   }   

   static unsigned long PHprintTimepoint = millis();
   if(millis()-PHprintTimepoint > 900U)
   {
      PHprintTimepoint = millis();
      for(PHcopyIndex=0;PHcopyIndex<PHSCOUNT;PHcopyIndex++)
        PHanalogBufferTemp[PHcopyIndex]= PHanalogBuffer[PHcopyIndex];

      voltage = PHgetMedianNum(PHanalogBufferTemp,PHSCOUNT) * (float)PHVREF / 1024.0;
      //pHValue = -1.9091*voltage+10.5609;
      pHValue = 8.33*voltage-8.485;//the formula depends on a calibration, you need to check ph sensor with different solutions before using it
   }
   return pHValue;
}
