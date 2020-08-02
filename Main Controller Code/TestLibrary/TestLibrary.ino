//#include "Morse.h"
#include "IOCtrl.h"

//Morse morse(13);
IOCtrl ioCtrl(13);
const char *foo;
const char* fii;
char *str ;
char *str2 ;
String readStringB;
String sSerialUSB;
char nOption;

void setup()
{
  Serial.begin(115200);
}

void CheckSerialVer3();


void loop()
{ 
   ioCtrl.dot();
   fii=ioCtrl.GetInputs();
   Serial.println(fii);
   delay(1000);
    
   foo=ioCtrl.GetStatus();
   Serial.println(foo);
   delay(1000);

   CheckSerialVer3();
    delay(1000);

   foo=ioCtrl.SetOutputTo(2,1);
   Serial.println(foo);

    delay(1000);
    foo=ioCtrl.SetOutput(2);
    Serial.println(foo);
    foo=ioCtrl.ClearOutput(2); 
    Serial.println(foo);
    foo=ioCtrl.ClearEvent();
    Serial.println(foo);
    foo=ioCtrl.ClearOutputs();
    Serial.println(foo);
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

