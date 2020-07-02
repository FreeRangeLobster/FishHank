#include <HardwareSerial.h>
//HardwareSerial Serial_1(1);
String readString;
String readStringB;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  // set the data rate for the HardwareSerial port
  Serial2.begin(115200);
//  Serial_1.begin(115200);
  Serial.println("Restarted");
  Serial2.print("?I");
  Serial2.print('\n');
//  Serial_1.print("Serial1Reading");
//  Serial_1.print('\n');


  
  //Serial.println("HELP");
}

void checkSerial2(){
    if (!Serial2.available()) return;
    char c = Serial2.read();
    if (c == '\n') {
      Serial.println(readString);
      //executeCommand(readString);
      readString = "";    
    }
    else readString += c;
}

/*
void checkSerial_1(){
    if (!Serial_1.available()) return;
    char c = Serial_1.read();
    if (c == '\n') {
      Serial.println(readString);
      //executeCommand(readString);
      readString = "";    
    }
    else readString += c;
//} */

void checkSerial1(){
    if (!Serial.available()) return;
    char c = Serial.read();
    if (c == '\n') {
      Serial2.print(readStringB);
      Serial2.print('\n');
      //executeCommand(readStringB);
      readStringB = "";    
    }
    else readStringB += c;
}

void loop() {
  checkSerial2();
  checkSerial1(); 
  //checkSerial_1(); 
}


