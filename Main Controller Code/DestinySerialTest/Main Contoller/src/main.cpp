#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "RTClib.h"
#include <SPI.h>
#include <mySD.h>
#include <OneWire.h>
#include <DallasTemperature.h>








String readString;
String readStringB;
String sSerialUSB;
char nOption;

//String used in the communication with Arduino Aux Board
String readStringIOCtrl;
unsigned long nCurrentMillis = millis();
unsigned long nPreviousMillis ;


enum StatesEnum { Start, Initialise, ReadSD, EnablePeripherals, UpdateStatus, updateTFT,  CheckSerialComms };
StatesEnum CurrentState; 

enum eStateEnum{eCalculateNextTimeCheck,eCheckSD,eUpdateOutputs,eUpdateDisplay,eIdle};
eStateEnum StateEnum;


//Time system is idle
const long interval = 5*1000;     //in seconds

















void setup() {
  sSerialUSB="NOthing";
  Serial.begin(115200);
  //Serial1.begin();
  
  Serial.println("Restarted");
  Serial.println("");
  Serial.println("");
  //Serial_1.print("Serial1 Working");
  //Serial_1.print('\n');

  //Serial_2.print("Serial2 working");
  //Serial_2.print('\n');
  

  
  Serial.println("Couldn't find RTC");
  Serial.println(F("SSD1306 allocation failed"));   
  Serial.println(" SD initialization failed!");
  Serial.println("Configuration file Present");
  Serial.println("Schedule.txt:");
    
  Serial.println("initialization done.");



  StateEnum=eCalculateNextTimeCheck;
  nPreviousMillis=millis();

}

void checkSerial_2();
void checkSerial_1();
void CheckSerialVer2();
void SerialCommandHandller();
//Serial Communication Handling
void checkIOCtrlSetOutputTo(int OutputNumber, int State);
void checkIOCtrlSerial();

void loop() {
  
 switch (StateEnum)
    {
        
        case eCalculateNextTimeCheck:
        {
            Serial.println("State: eCalculateNextTimeCheck");

             //Show time  
            
          
            Serial.println("1030Mon");

            StateEnum=eCheckSD;
            break;
        }

        case eCheckSD:{
           Serial.println("State: eCheckSD");
           StateEnum=eUpdateOutputs;
            break;
        }

         case eUpdateOutputs:{
            Serial.println("State: eUpdateOutputs");
           StateEnum=eUpdateDisplay;
            break;
        }


         case eUpdateDisplay:{          
          Serial.println("State: eUpdateDisplay");
          StateEnum=eIdle;
          break;
        }

         case eIdle:{
          //Serial.println("State: eIdle");
          nCurrentMillis=millis();
          if (nCurrentMillis - nPreviousMillis >= interval) {            
            nPreviousMillis = nCurrentMillis;
            StateEnum=eCalculateNextTimeCheck;
            delay(500);
          }

          CheckSerialVer2();
          SerialCommandHandller();
         break;

        default:
        {
            // is likely to be an error
             Serial.println("error opening test.txt");
            StateEnum=eCalculateNextTimeCheck;
        }
    };



}
}

#pragma region "Serial Communication Handling"
void CheckSerialVer2(){  
    if (!Serial.available()) return;
    
    if (Serial.available()) {   
      sSerialUSB = Serial.readStringUntil('\n');// s1 is String type variable.
      //delay(10);
      Serial.print("Received Data = ");
      Serial.println(sSerialUSB);//display same received Data back in serial monitor. 
      nOption=sSerialUSB[0];
    }
 
 }


void SerialCommandHandller(){    

  switch ( sSerialUSB[0]) {
      case 'K':
        Serial.println("OK,K Initialise");
        sSerialUSB="";
      break;
      
      case 'I':
        Serial.println("OK,I Idle");
        sSerialUSB="";
      break;
  
      case 'N':
        Serial.println("OK,N Update Screen");     
        sSerialUSB="";
      break;
  
      case 'M':
        Serial.println("OK,M Check Events");
        sSerialUSB="";
      break;
  
      case 'O':
        Serial.println("OK,O Update Outputs");     
        sSerialUSB="";
      break;
  
      case 'D':
        Serial.println("OK,D Delete Memory");
        sSerialUSB="";
      break;
  
      case '0':
        Serial.println("OK,0 Read Page Number 0_0000");     
        sSerialUSB="";
      break;
  
      case '1':
        Serial.println("OK,1 AddEvent  1_1030WED31");     
        sSerialUSB="";
      break;
  
      case '2':
        Serial.println("OK,2 Disable Event  2_001");     
        sSerialUSB="";
      break;
  
      case '3':
        Serial.println("OK,3 Spare");
        sSerialUSB="";
      break;
  
      case '4':
        Serial.println("OK,4 Update Output 4_11 Output 1 ON");     
        sSerialUSB="";
      break;
  
      case '5':
        Serial.println("OK,5 Write Status on OLED 5_Hello");     
        sSerialUSB="";
      break;
  
      case '6':
        Serial.println("OK,6 Set Time RTC 6_DDMMY_HHmmss"); 
        
        sSerialUSB="";
      break;
  
      case '7':
        Serial.println("OK,7 Read Time");     
        sSerialUSB="";
      break;
  
      case '8':
        Serial.println("Ok,8 Check Current Event");     
        sSerialUSB="";
      break;
             
      case '9':
        Serial.println("OK,9 Spare");     
        sSerialUSB="";
      break;
  
      case '?':
        Serial.println("OK,? Help"); 
        Serial.println("? Help");
        Serial.println("K Initialise");  
        Serial.println("I Idle");
        Serial.println("N Update Screen");   
        Serial.println("M Check Events");
        Serial.println("O Update Outputs");
        Serial.println("D Delete Memory");     
        Serial.println("0 Read Page Number 0_0000");
        Serial.println("1 AddEvent  1_1030WED31");
        Serial.println("2 Disable Event  2_001");             
        Serial.println("3 Spare");
        Serial.println("4 Update Output 4_11 Output 1 ON");
        Serial.println("5 Write Status on OLED 5_Hello"); 
        Serial.println("6 Set Time RTC 6_DDMMY_HHmmss"); 
        Serial.println("7 Read Time"); 
        Serial.println("8 Check Current Event");
        Serial.println("9 Spare");              
        sSerialUSB="";
      break;
       
      
      default:
       //Serial.println("! Not supported");   
       
        //sSerialUSB="";
        break;
      }  
}



