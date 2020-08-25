#include <HardwareSerial.h>
#include "IOCtrl.h"
//#include "Display.h"
//#include "RTC.h"
//#include "SDCard.h"
//#include "Temperature.h"




String readStringB;
String sSerialUSB;
char nOption;
String readStringIOCtrl;
//HardwareSerial Serial2(2);

void CheckSerialVer3();
void checkIOCtrlSerial();
void checkIOCtrlSetOutputTo(int OutputNumber, int State);


void setup()
{

  Serial.begin(115200);
  Serial2.begin(115200);

  Serial2.print("?I");
  Serial2.print('\n'); 
  checkIOCtrlSerial();
}





void loop()
{ 

  //clear variables
  readStringIOCtrl = ""; 
  checkIOCtrlSetOutputTo(1,1);
  checkIOCtrlSerial();  
  delay(1000);  

  readStringIOCtrl = ""; 
  checkIOCtrlSetOutputTo(1,0);  
  checkIOCtrlSerial(); 
  delay(1000);  

  readStringIOCtrl = ""; 
  checkIOCtrlInputs();
  checkIOCtrlSerial(); 
  delay(1000);  
  
}


void CheckSerialVer3(){  
    if (!Serial.available()) return;      
    while (Serial.available()) {
      sSerialUSB = Serial.readStringUntil('\n');// s1 is String type variable.
      //delay(10);
      Serial.print("Received Data = ");
      Serial.println(sSerialUSB);//display same received Data back in serial monitor. 
      nOption=sSerialUSB[0];
    }
 }


void checkIOCtrlInputs(){
    Serial2.print("?I");
    Serial2.print('\n');     
}

void checkIOCtrlOutputs(){
    if (!Serial2.available()) return;
    while(Serial2.available()){
      readStringIOCtrl += char(Serial2.read());
    }
    Serial.println("Received data = " + readStringIOCtrl);
    //readStringIOCtrl = "";   
}



void checkIOCtrlSetOutputTo(int OutputNumber, int State){
     String  sCommand= "!" + String(OutputNumber) + String(State);
     Serial2.print(sCommand);
     Serial2.print('\n');   
}


void checkIOCtrlSerial(){
    if (!Serial2.available()) return;
    while(Serial2.available()){
      readStringIOCtrl += char(Serial2.read());
    }
    Serial.println("Received data = " + readStringIOCtrl);
    //readStringIOCtrl = "";   
}



