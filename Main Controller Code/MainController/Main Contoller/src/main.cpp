#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "RTClib.h"
#include <SPI.h>
#include <mySD.h>



//https://esp32.com/viewtopic.php?t=328  
/* ESP32 3 UARTs */

HardwareSerial Serial_1(1);
HardwareSerial Serial_2(2);

// Choose two free pins
#define SERIAL1_RXPIN 2//12
#define SERIAL1_TXPIN 4//13

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


//Delay implementation
// constants won't change :
const long interval = 5*1000;     //in seconds

//RTC
RTC_DS3231 rtc;
char daysOfTheWeek[7][12] = {"SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"};
char char_arrayTime[10];
DateTime now;

//Display
#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels
#define OLED_RESET    -1  // Reset pin # (or -1 if sharing reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
#define OffsetOutputsRaw1 20
#define OffsetOutputsRaw2 40
#define OutputsColumnCorner1 128/4
#define OutputsColumnWidth 128/8
#define OutputsColumnCorner2  128/4

#define DisplayColumnX  128/4
#define DisplayColumnY  40
#define DisplayColumnY2  DisplayColumnY+12 //32
#define DisplayColumnWidth 128/4
#define DisplayColumnHight 12.5
char cStatus[10];
void OLEDDrawTable();
void DisplayModifyStatus( char *status);
void DisplayModifyTime( char *status);
void ShowCurrentTime();
void TimeFormatForDisplay();


void setup() {
  sSerialUSB="NOthing";
  Serial.begin(115200);
  //Serial1.begin();
  Serial_1.begin(9600, SERIAL_8N1, SERIAL1_RXPIN, SERIAL1_TXPIN,false);
  Serial_2.begin(115200);  // pin 16=RX, pin 17=TX

  Serial.println("Restarted");
  Serial.println("");
  Serial.println("");
  //Serial_1.print("Serial1 Working");
  //Serial_1.print('\n');

  //Serial_2.print("Serial2 working");
  //Serial_2.print('\n');
  

  if (! rtc.begin()) {
  Serial.println("Couldn't find RTC");
  }
   else{
    Serial.println(F("RTC Started"));   
  }

   if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) 
  { 
    Serial.println(F("SSD1306 allocation failed"));   
  }
  else{
    Serial.println(F("Display started"));   
  }


  OLEDDrawTable();
    //Show status
  strcpy(cStatus, "Auto" );
  DisplayModifyStatus(cStatus);

  //Show time  
  now = rtc.now(); 

  //SD Card
  File myFile;
  const int chipSelect = 5;
  pinMode(SS, OUTPUT);

  if (!SD.begin(chipSelect, 23, 19, 18)) {
    Serial.println(" SD initialization failed!");
    return;
  }
  else
  {
     Serial.println("SD Card Initialised");
  }
  
  if(SD.exists("Schedule.txt")) {
    Serial.println("Schedule file Present");
  }

   if(SD.exists("Configuration.txt")) {
    Serial.println("Configuration file Present");
  }

  // Open the file for reading:
  myFile = SD.open("Schedule.txt");
  if (myFile) {
    Serial.println("Schedule.txt:");
    
    // read from the file until there's nothing else in it:
    while (myFile.available()) {
    	Serial.write(myFile.read());
    }
    // close the file:
    myFile.close();
  } else {
  	// if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }


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
  // Do something wonderful
  //  checkSerial_2();
  //  checkSerial_1();

  //Serial_1.print("Serial1 Working");
  //Serial_1.print('\n');
  //delay(200);
  //Serial_2.print("Serial2 working");
  //Serial_2.print('\n');

  //CheckSerialVer2();
  //SerialCommandHandller();

  /*
  readStringIOCtrl = ""; 
  checkIOCtrlSetOutputTo(1,1);
  checkIOCtrlSerial();  
  delay(1000); 

  readStringIOCtrl = ""; 
  checkIOCtrlSetOutputTo(1,0);  
  checkIOCtrlSerial(); 
  delay(1000);  */


 switch (StateEnum)
    {
        
        case eCalculateNextTimeCheck:
        {
            Serial.println("State: eCalculateNextTimeCheck");

             //Show time  
            now = rtc.now(); 
            Serial.println(String("DateTime::TIMESTAMP_FULL:\t")+now.timestamp(DateTime::TIMESTAMP_FULL));
           

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
          DisplayShowCurrentTime();
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
void checkSerial_1(){
    if (!Serial_1.available()) return;
    char c = Serial_1.read();
    if (c == '\n') {
      Serial.println(readString);
      //executeCommand(readString);
      readString = "";    
    }
    else readString += c;
} 
void checkSerial_2(){
    if (!Serial_2.available()) return;
    char c = Serial_2.read();
    if (c == '\n') {
      Serial.println(readString);
      //executeCommand(readString);
      readString = "";    
    }
    else readString += c;
}
void SerialCommandHandller(){    

  switch ( sSerialUSB[0]) {
      case 'K':
        Serial.println("Initialise");
        sSerialUSB="";
      break;
      
      case 'I':
        Serial.println("I Idle");
        sSerialUSB="";
      break;
  
      case 'N':
        Serial.println("N Update Screen");     
        sSerialUSB="";
      break;
  
      case 'M':
        Serial.println("M Check Events");
        sSerialUSB="";
      break;
  
      case 'O':
        Serial.println("O Update Outputs");     
        sSerialUSB="";
      break;
  
      case 'D':
        Serial.println("D Delete Memory");
        sSerialUSB="";
      break;
  
      case '0':
        Serial.println("0 Read Page Number 0_0000");     
        sSerialUSB="";
      break;
  
      case '1':
        Serial.println("1 AddEvent  1_1030WED31");     
        sSerialUSB="";
      break;
  
      case '2':
        Serial.println("2 Disable Event  2_001");     
        sSerialUSB="";
      break;
  
      case '3':
        Serial.println("3 Spare");
        sSerialUSB="";
      break;
  
      case '4':
        Serial.println("4 Update Output 4_11 Output 1 ON");     
        sSerialUSB="";
      break;
  
      case '5':
        Serial.println("5 Write Status on OLED 5_Hello");     
        sSerialUSB="";
      break;
  
      case '6':
        Serial.println("6 Set Time RTC 6_DDMMY_HHmmss");     
        sSerialUSB="";
      break;
  
      case '7':
        Serial.println("7 Read Time");     
        sSerialUSB="";
      break;
  
      case '8':
        Serial.println("8 Check Current Event");     
        sSerialUSB="";
      break;
             
      case '9':
        Serial.println("9 Spare");     
        sSerialUSB="";
      break;
  
      case '?':
        Serial.println("Help");
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

#pragma endregion

#pragma region "Aux IO Board Communication"
void checkIOCtrlSetOutputTo(int OutputNumber, int State){
     String  sCommand= "!" + String(OutputNumber) + String(State);
     Serial_2.print(sCommand);
     Serial_2.print('\n');   
}

void checkIOCtrlSerial(){
    if (!Serial_2.available()) return;
    while(Serial_2.available()){
      readStringIOCtrl += char(Serial_2.read());
    }
    Serial.println("Received data = " + readStringIOCtrl);
    //readStringIOCtrl = "";   
}
#pragma endregion

#pragma region Display
void ShowCurrentTime(){
    DateTime now = rtc.now();     
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(75,0);
    display.println(now.second(), DEC);
     
    display.setTextSize(2);
    display.setCursor(25,0);
    display.println(":");
     
    display.setTextSize(2);
    display.setCursor(65,0);
    display.println(":");
     
    display.setTextSize(2);
    display.setCursor(40,0);
    display.println(now.minute(), DEC);
     
    display.setTextSize(2);
    display.setCursor(0,0);
    display.println(now.hour(), DEC);
     
    display.setTextSize(2);
    display.setCursor(0,20);
    display.println(now.day(), DEC);
     
    display.setTextSize(2);
    display.setCursor(25,20);
    display.println("-");
     
    display.setTextSize(2);
    display.setCursor(40,20);
    display.println(now.month(), DEC);
     
    display.setTextSize(2);
    display.setCursor(55,20);
    display.println("-");
     
    display.setTextSize(2);
    display.setCursor(70,20);
    display.println(now.year(), DEC);
     
    display.setTextSize(2);
    display.setCursor(0,40);
    display.print(daysOfTheWeek[now.dayOfTheWeek()]);
     
    display.display(); 

}

void OLEDDrawTable(){
    //first Raw
    display.clearDisplay();    
    display.setTextSize(1.5);
    display.setTextColor(WHITE);
    
    //Column1  
    display.drawRect(DisplayColumnX*0,DisplayColumnY,DisplayColumnWidth,DisplayColumnHight, WHITE);    
    display.setCursor(5,DisplayColumnY+2);
    display.printf("--");    
    display.display();     
    //Column2    
    display.drawRect(DisplayColumnX*1,DisplayColumnY,DisplayColumnWidth,DisplayColumnHight, WHITE);    
    display.setCursor((DisplayColumnX*1)+5,DisplayColumnY+2);
    display.printf("--");    
    display.display();     
    //Column3    
    display.drawRect(DisplayColumnX*2,DisplayColumnY,DisplayColumnWidth,DisplayColumnHight, WHITE);    
    display.setCursor((DisplayColumnX*2)+5,DisplayColumnY+2);
    display.printf("--");
    display.display(); 
    //Column4    
    display.drawRect(DisplayColumnX*3,DisplayColumnY,DisplayColumnWidth,DisplayColumnHight, WHITE);    
    display.setCursor((DisplayColumnX*3)+5,DisplayColumnY+2);
    display.printf("--");
    display.display();
    
    //second raw
    //Column1
    //display.drawRect(OutputsColumnCorner1*0,OffsetOutputsRaw2,OutputsColumnCorner2*2,12.5, WHITE);
    display.drawRect(DisplayColumnX*0,DisplayColumnY2,DisplayColumnWidth,DisplayColumnHight, WHITE);    
    display.setCursor((DisplayColumnX*0)+5,DisplayColumnY2+2);
    display.printf("ON");    
    display.display();    
    //Column2
    //display.drawRect(OutputsColumnCorner1*1,OffsetOutputsRaw2,OutputsColumnCorner2*2,12.5, WHITE);
    display.drawRect(DisplayColumnX*1,DisplayColumnY2,DisplayColumnWidth,DisplayColumnHight, WHITE);    
    display.setCursor((DisplayColumnX*1)+5,DisplayColumnY2+2);
    display.printf("ON");    
    display.display();
    //Column3
    //display.drawRect(OutputsColumnCorner1*2,OffsetOutputsRaw2,OutputsColumnCorner2*2,12.5, WHITE);
    display.drawRect(DisplayColumnX*2,DisplayColumnY2,DisplayColumnWidth,DisplayColumnHight, WHITE);    
    display.setCursor((DisplayColumnX*2)+5,DisplayColumnY2+2);
    display.printf("ON");    
    display.display();
    //Column4
    //display.drawRect(OutputsColumnCorner1*3,OffsetOutputsRaw2,OutputsColumnCorner2*2,12.5, WHITE);
    display.drawRect(DisplayColumnX*3,DisplayColumnY2,DisplayColumnWidth,DisplayColumnHight, WHITE);    
    display.setCursor((DisplayColumnX*3)+5,DisplayColumnY2+2);
    display.printf("ON");    
    display.display();
     


    
    
    display.setCursor(3,10);
    display.printf("Booting...");
    display.setCursor(3,1);
    display.printf("DAY");
    display.setCursor(60,1);        
    display.printf("00");
    display.setCursor(80,1);
    display.printf(":");
    display.setCursor(90,1);
    display.printf("00");
    display.display(); 
}

void DisplayModifyStatus( char *status){
    display.fillRect(1,10,60,10, BLACK);    
    display.setCursor(15,10);
    display.printf(status);
    display.display();
}

void DisplayShowCurrentTime(){
    now = rtc.now(); 
    char Buff[15];
    String sTime;
    String sCurrentTime;

    sCurrentTime=now.timestamp(DateTime::TIMESTAMP_TIME);      
    sTime= daysOfTheWeek[now.dayOfTheWeek()]+ String("  " + now.timestamp(DateTime::TIMESTAMP_HANK));
    sTime.toCharArray(Buff,15);
    //string.toCharArray(buf, len)
    display.fillRect(1,1,128,7, BLACK);
    display.setCursor(4,1);
    display.printf(Buff);
    display.display();
    Serial.println(sTime);
}


#pragma endregion

#pragma region  Time






#pragma endregion
