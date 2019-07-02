#include <SD.h>
#include <SPI.h>
#include "SequenceSet.cpp"

void setup() {
  Serial.begin(9600);
  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    return;
  }
  SequenceSet ss(30);
}

void loop() {
  // put your main code here, to run repeatedly:

}
