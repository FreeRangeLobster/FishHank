#include <Arduino.h>
#include <SerialHandler.h>

void setup() {
  // put your setup code here, to run once:
    InitialiseSerial();
}

void loop() {
  // put your main code here, to run repeatedly:
  SendHelloWorld();
  delay(1000);
  SendCruel();
  delay(1000);
  ReadSerialComs();
  delay(1000);
}