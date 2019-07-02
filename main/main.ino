#include <SD.h>
#include <SPI.h>
#include "SequenceSet.cpp"

void setup() {
  Serial.begin(9600);
  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    return;
  }
  //Serial.println(sizeof(int));
  //Serial.println(sizeof(SequenceSet));
  //Serial.println(sizeof(Bloco));
  SequenceSet ss(30);
  for(int i = 2; i < 40; i++){
    ss.inserir(i);
  }
  ss.mergeSort();
}
void loop() {

}
