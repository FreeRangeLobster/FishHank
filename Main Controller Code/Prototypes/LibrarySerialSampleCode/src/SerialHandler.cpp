#include <SerialHandler.h>

void InitialiseSerial(){
    Serial.begin(115200); 
}

void SendHelloWorld(){
    Serial.println("Hello World");
}

void SendCruel(){
  Serial.println("Cruel Summer");
}

void ReadSerialComs(){
    if (Serial.available() > 0){
        byte incomingByte;
        incomingByte = Serial.read();
        Serial.print("I received: ");
        Serial.println(incomingByte, DEC);
    } 
}