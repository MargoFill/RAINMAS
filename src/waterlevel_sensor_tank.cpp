#include <Arduino.h>
#define SIGNAL_PIN 2

void initLevelSensor() {
  pinMode(SIGNAL_PIN, OUTPUT);
  digitalWrite(SIGNAL_PIN, LOW);
  delay(100);
}

float readLevelSensor() {
  long duration; 
  float distance;
  
  pinMode(SIGNAL_PIN, OUTPUT);
  digitalWrite(SIGNAL_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(SIGNAL_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(SIGNAL_PIN, LOW);

  pinMode(SIGNAL_PIN, INPUT);

  duration=pulseIn(SIGNAL_PIN, HIGH, 30000);

  distance=30.00-((float)duration*0.034/2);// formula for counting water level in a box; 
  //if the box differs from mine, you should change "30.00" to the height of your box
  //distance=(float)duration*0.034/2;

  //Serial.print("Distance: ");
  //Serial.print(distance);
  //Serial.print(" cm");

  return distance;

}