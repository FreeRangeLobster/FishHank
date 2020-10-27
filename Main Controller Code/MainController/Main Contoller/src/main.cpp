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

//Stay alive LEDS
const int ledPin1 = 32;
const int ledPin2 = 33;

//Temperature Sensor
#define ONE_WIRE_BUS 15
OneWire oneWire(ONE_WIRE_BUS); // Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
DallasTemperature sensors(&oneWire); // Pass our oneWire reference to Dallas Temperature. 
DeviceAddress insideThermometer, outsideThermometer; // arrays to hold device addresses

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
 
int nCtrlOutputs [8] = { 0, 0, 0, 0, 0,0, 0, 0}; 
int nLastCtrlOutputs [8] = { 0, 0, 0, 0, 0,0, 0, 0}; 
bool bNewOutputUpdate=false;
bool bShowTrace =true;
String sCurrentTime;


//State machine states used in the main look or to handle the supervisory
enum StatesEnum { Start, Initialise, ReadSD, EnablePeripherals, UpdateStatus, updateTFT,  CheckSerialComms };
StatesEnum CurrentState; 
enum eStateEnum{eCalculateNextTimeCheck,eCheckSD,eUpdateOutputs,eUpdateDisplay,eIdle};
eStateEnum StateEnum;
//Time system is in idle state
const long interval = 5*1000;     //in seconds

//RTC
RTC_DS3231 rtc;
char daysOfTheWeek[7][12] = {"SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"};
char char_arrayTime[10];
DateTime now;
void RTCSetTimeAndDate();


#pragma region "Display constants"
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
  void DisplayCurrentTime();
  void DisplayUpdateTemperature();
#pragma endregion





//Prototypes 
void checkSerial_2();
void checkSerial_1();
void CheckSerialVer2();
void SerialCommandHandller();
void checkIOCtrlSetOutputTo(int OutputNumber, int State);
void checkIOCtrlSerial();
void RTCSetTimeAndDate(char *Currenttime);
void DumpSchedule();
void SetOutputTo();
void CheckEvent(String sTime);
void IOCtrlUpdateOutputs();
void DisplayUpdateOutputs();
void FormatCurrrentTime();
void ToggleLED1();
void ToggleLED2();
void printAddress(DeviceAddress deviceAddress);
void printTemperature(DeviceAddress deviceAddress);


void setup() {

  sSerialUSB="NOthing";
  Serial.begin(115200);
  Serial.println("-------Initialization Started-----");


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
  
  //RTC initialisation
  if (! rtc.begin()) {
  Serial.println("Couldn't find RTC");
  }
   else{
    Serial.println(F("RTC Started"));   
  }
  
  //Display initialisation
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

  // Temperature sensor 
  // Start up the library
  sensors.begin();  
  // locate devices on the bus
  Serial.print("Found ");
  Serial.print(sensors.getDeviceCount(), DEC);
  Serial.println(" devices.");
  // search for devices on the bus and assign based on an index.
  if (!sensors.getAddress(insideThermometer, 0)) Serial.println("Unable to find address for Device 0"); 
  if (!sensors.getAddress(outsideThermometer, 1)) Serial.println("Unable to find address for Device 1"); 
  //show the a ddresses we found on the bus
  Serial.print("Device 0 Address: ");
  printAddress(insideThermometer);
  Serial.println();
  sensors.requestTemperatures();
  sensors.requestTemperatures(); 
  Serial.println("DONE");
  
  // It responds almost immediately. Let's print out the data
  printTemperature(insideThermometer); // Use a simple function to print out the data
  
  //State machine parameters
  StateEnum=eCalculateNextTimeCheck;
  nPreviousMillis=millis();

  //Stay alive signals
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);

  digitalWrite(ledPin1, HIGH);
  digitalWrite(ledPin2, LOW);
  
  Serial.println("-");
  Serial.println("-------Initialization Finished-----");

}



void loop() {
  

 switch (StateEnum)
    {
        
        case eCalculateNextTimeCheck:
        {
          if (bShowTrace){Serial.println("State: eCalculateNextTimeCheck");}
           //Show time  
          now = rtc.now();                      
          Serial.println(String("->") + now.timestamp(DateTime::TIMESTAMP_FULL));
          StateEnum=eCheckSD;
          break;
        }

        case eCheckSD:{
          if (bShowTrace){Serial.println("State: eCheckSD");}                     
          FormatCurrrentTime();
          if (now.second()<10){
            if (bShowTrace){Serial.println("----> Check Events");}                     
            CheckEvent(sCurrentTime);          
          }            
            StateEnum=eUpdateOutputs;
          
          break;
        }

        case eUpdateOutputs:{
          if (bShowTrace){Serial.println("State: eUpdateOutputs");}            
          //IOCtrlUpdateOutputs();
          IOCtrlUpdateOutputs();
          DisplayUpdateOutputs();
          StateEnum=eUpdateDisplay;
          break;
        }


         case eUpdateDisplay:{          
          DisplayCurrentTime();
          DisplayUpdateTemperature();
          if (bShowTrace){Serial.println("State: eUpdateDisplay");}
          StateEnum=eIdle;
          break;
        }

         case eIdle:{
          //Serial.println("State: eIdle");
          nCurrentMillis=millis();
          if (nCurrentMillis - nPreviousMillis >= interval) {            
            nPreviousMillis = nCurrentMillis;
            StateEnum=eCalculateNextTimeCheck;
            ToggleLED1();               
            delay(500);
          }          
          ToggleLED2();  
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

#pragma region "Stay Alive Signals LEDs"

  void ToggleLED1(){
  if(digitalRead(ledPin1)==1){
      digitalWrite (ledPin1, LOW);
    }
    else
    {
      digitalWrite (ledPin1, HIGH);
    }
  }

  void ToggleLED2(){
if(digitalRead(ledPin2)==1){
    digitalWrite (ledPin2, LOW);
  }
  else
  {
    digitalWrite (ledPin2, HIGH);
  }
}

#pragma endregion

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
        CheckEvent("1038 MON");
        sSerialUSB="";
      break;
  
      case 'O':
        Serial.println("O Update Outputs");  
        SetOutputTo();   
        sSerialUSB="";
      break;
  
      case 'D':
        Serial.println("D Delete Memory");
        sSerialUSB="";
      break;
  
      case '0':
        Serial.println("0 Dumps Events");     
        DumpSchedule();
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
        Serial.println("3 Trace On");
        bShowTrace =true;
        sSerialUSB="";
      break;
  
      case '4':
        Serial.println("4 Update Output 4_11 Output 1 ON");    
        checkIOCtrlSetOutputTo(1,1);
        sSerialUSB="";
      break;
  
      case '5':
        Serial.println("5 Write Status on OLED 5_Hello");     
        sSerialUSB="";
      break;
  
      case '6':
        Serial.println("6 Set Time RTC 6_DDMMYYYY_HH:mm:ss"); 
        RTCSetTimeAndDate();    
        sSerialUSB="";
      break;
  
      case '7':
        Serial.println("7 Read Time"); 
        Serial.println(String("DateTime::TIMESTAMP_FULL:\t")+now.timestamp(DateTime::TIMESTAMP_FULL));    
        sSerialUSB="";
      break;
  
      case '8':
        Serial.println("8 Check Current Event");   
        CheckEvent("1044 MON");  
        sSerialUSB="";
      break;
             
      case '9':
        Serial.println("9 Trace Off"); 
        bShowTrace =false;    
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
        Serial.println("3 Trace ON");
        Serial.println("4 Update Output 4_11 Output 1 ON");
        Serial.println("5 Write Status on OLED 5_Hello"); 
        Serial.println("6 Set Time RTC 6_DDMMY_HHmmss"); 
        Serial.println("7 Read Time"); 
        Serial.println("8 Check Current Event");
        Serial.println("9 Trace OFF");              
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

void SetOutputTo(){
    int nOutput ;
    int nState;
    nOutput= int(sSerialUSB[2]-'0');
    nState= int(sSerialUSB[3]-'0'); 
    nCtrlOutputs[nOutput-1]=nState;     
    checkIOCtrlSetOutputTo(nOutput-1,nState);
}

void checkIOCtrlSetOutputTo(int OutputNumber, int State){
     String  sCommand= "!" + String(OutputNumber+1) + String(State);
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

void IOCtrlUpdateOutputs(){
  if (bNewOutputUpdate==true){
    int i=0;
    for(i=0;i<=7;i++){
      if (bShowTrace){Serial.printf("output register: %d %d %d %d %d %d %d %d \n",nCtrlOutputs[0],nCtrlOutputs[1],nCtrlOutputs[2],nCtrlOutputs[3],nCtrlOutputs[4],nCtrlOutputs[5],nCtrlOutputs[6],nCtrlOutputs[7]);}
      checkIOCtrlSetOutputTo(i,nCtrlOutputs[i]);       
      delay(100);     
   }
   bNewOutputUpdate=false;
  }
}

#pragma endregion

#pragma region "Display"
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

void DisplayUpdateOutputs(){

 //Column1  
    display.drawRect(DisplayColumnX*0,DisplayColumnY,DisplayColumnWidth,DisplayColumnHight, WHITE);    
    display.setCursor(5,DisplayColumnY+2);
    if (nCtrlOutputs [0]==1){
      display.printf("ON");  
    }
    else
    {
       display.printf("OFF");  
    }
     
    

      
   
    //Column2    
    display.drawRect(DisplayColumnX*1,DisplayColumnY,DisplayColumnWidth,DisplayColumnHight, WHITE);    
    display.setCursor((DisplayColumnX*1)+5,DisplayColumnY+2);
    //display.printf("--");    
    if (nCtrlOutputs [1]==1){
      display.printf("ON");  
    }
    else
    {
       display.printf("OFF");  
    }
    //display.display();     
    
    //Column3    
    display.drawRect(DisplayColumnX*2,DisplayColumnY,DisplayColumnWidth,DisplayColumnHight, WHITE);    
    display.setCursor((DisplayColumnX*2)+5,DisplayColumnY+2);
    if (nCtrlOutputs [2]==1){
      display.printf("ON");  
    }
    else
    {
       display.printf("OFF");  
    }
    //display.printf("--");
    //display.display(); 
    
    //Column4    
    display.drawRect(DisplayColumnX*3,DisplayColumnY,DisplayColumnWidth,DisplayColumnHight, WHITE);    
    display.setCursor((DisplayColumnX*3)+5,DisplayColumnY+2);
    if (nCtrlOutputs [3]==1){
      display.printf("ON");  
    }
    else
    {
       display.printf("OFF");  
    }
    //display.printf("--");
    //display.display();
    
    //second raw
    //Column1
    //display.drawRect(OutputsColumnCorner1*0,OffsetOutputsRaw2,OutputsColumnCorner2*2,12.5, WHITE);
    display.drawRect(DisplayColumnX*0,DisplayColumnY2,DisplayColumnWidth,DisplayColumnHight, WHITE);    
    display.setCursor((DisplayColumnX*0)+5,DisplayColumnY2+2);
    //display.printf("ON");    
    //display.display();    
    if (nCtrlOutputs [4]==1){
      display.printf("ON");  
    }
    else
    {
       display.printf("OFF");  
    }
    //Column2
    //display.drawRect(OutputsColumnCorner1*1,OffsetOutputsRaw2,OutputsColumnCorner2*2,12.5, WHITE);
    display.drawRect(DisplayColumnX*1,DisplayColumnY2,DisplayColumnWidth,DisplayColumnHight, WHITE);    
    display.setCursor((DisplayColumnX*1)+5,DisplayColumnY2+2);
    if (nCtrlOutputs [5]==1){
      display.printf("ON");  
    }
    else
    {
       display.printf("OFF");  
    }
    
    //display.printf("ON");    
    //display.display();
    //Column3
    //display.drawRect(OutputsColumnCorner1*2,OffsetOutputsRaw2,OutputsColumnCorner2*2,12.5, WHITE);
    display.drawRect(DisplayColumnX*2,DisplayColumnY2,DisplayColumnWidth,DisplayColumnHight, WHITE);    
    display.setCursor((DisplayColumnX*2)+5,DisplayColumnY2+2);
    if (nCtrlOutputs [6]==1){
      display.printf("ON");  
    }
    else
    {
       display.printf("OFF");  
    }
    
    //display.printf("ON");    
    //display.display();
    //Column4
    //display.drawRect(OutputsColumnCorner1*3,OffsetOutputsRaw2,OutputsColumnCorner2*2,12.5, WHITE);
    display.drawRect(DisplayColumnX*3,DisplayColumnY2,DisplayColumnWidth,DisplayColumnHight, WHITE);    
    display.setCursor((DisplayColumnX*3)+5,DisplayColumnY2+2);
    if (nCtrlOutputs [7]==1){
      display.printf("ON");  
    }
    else
    {
       display.printf("OFF");  
    }
    //display.printf("ON");    
    display.display();
  
}

void DisplayModifyStatus( char *status){
    display.fillRect(1,10,60,10, BLACK);    
    display.setCursor(15,10);
    display.printf(status);
    display.display();
}

void DisplayCurrentTime(){
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
    //Serial.println(sTime);
}

void DisplayUpdateTemperature(){
    sensors.requestTemperatures();

    float tempC = sensors.getTempC(insideThermometer);    
    char result[8]; // Buffer big enough for 7-character float
    dtostrf(tempC, 6, 2, result);

    
    
    display.fillRect(75,10,25,7, BLACK);    
    display.setCursor(75,10);
    display.printf(result);
    display.display();
}

#pragma endregion

#pragma region "Time"

void RTCSetTimeAndDate(){
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    //rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
    //Received signal  "6_ddMMyyyy HH:mm:ss"    
    //Received signal  "6_19102020 10:32:50"    

    int year ;
    int month;
    int day;
    int hour;
    int min; 
    int sec;

    day= int(sSerialUSB[2]-'0')*10 + int(sSerialUSB[3]-'0');
    Serial.printf(" Day: %d ",day);

    month=int(sSerialUSB[4]-'0')*10 + int(sSerialUSB[5]-'0');
    Serial.printf(" month: %d ",month);

    year = int(sSerialUSB[6]-'0')*1000 + int(sSerialUSB[7]-'0')*100 + int(sSerialUSB[8]-'0')*10 + int(sSerialUSB[9]-'0');
    Serial.printf(" year: %d ",year);
    
    //space [10] 
    hour=int(sSerialUSB[11]-'0')*10 + int(sSerialUSB[12]-'0');
    Serial.printf(" hour: %d ",hour); 

    min=int(sSerialUSB[14]-'0')*10 + int(sSerialUSB[15]-'0'); 
    Serial.printf(" min: %d ",min);

    sec=int(sSerialUSB[17]-'0')*10 + int(sSerialUSB[18]-'0');
    Serial.printf(" sec: %d ",sec);

    Serial.println(" -- ");
    rtc.adjust(DateTime(year, month, day, hour, min, sec));
  

}

void FormatCurrrentTime(){
  DateTime now = rtc.now();
  //Required format eg: 1034 MON

  
  String sTimeFull;
  String sTimeMod;
  String sMinutes;

  sTimeFull=now.timestamp(DateTime::TIMESTAMP_TIME);
  sTimeMod=sTimeFull.substring(0,2);// + sTimeFull.substring(3,4);
  sMinutes=sTimeFull.substring(3,5);
  
  sCurrentTime = String( sTimeMod + sMinutes + ' ' ) + daysOfTheWeek[now.dayOfTheWeek()] ;
  if (bShowTrace){Serial.print("Current time: ");}
  if (bShowTrace){Serial.println(sCurrentTime);}
}


#pragma endregion

#pragma region "Events"

void DumpSchedule(){

    // Open the file for reading:
  File myFile;
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
    Serial.println("error opening Schedule.txt");
  }
}

void CheckEvent(String sTime){

  String sBuffer="";
  String sSchTime="";
  String sSchTimeAll="";
  String sOutputNumber="";
  String sOutputState="";
  String sEnable="";
  int _nOutput;

  if (bShowTrace){Serial.println(sTime);}

  File myFile;
  // Open the file for reading:
  myFile = SD.open("Schedule.txt");
  
  if (myFile) {
    if (bShowTrace){Serial.println("Schedule.txt:"); }
    // read from the file until there's nothing else in it:
      while (myFile.available()) {
        sBuffer=myFile.readStringUntil('\n');
        //Check if the event is enabled
        sEnable = sBuffer.substring(16, 17);
        if (bShowTrace){Serial.println(sEnable);}
        
          if(sEnable=="E"){            
            //Get time of the schedule
            sSchTime = sBuffer.substring(4, 12);
            if (bShowTrace){Serial.print("Compare CurrentTime: ");}
            if (bShowTrace){Serial.print(sTime);}
            if (bShowTrace){Serial.print(" Vs Event: ");}
            if (bShowTrace){Serial.println(sSchTime);}
            //IN case the day is from MON to SUN
            if (sTime == sSchTime) {
                  Serial.print("Event Found: ");
                  //Get Output to be activated
                  sOutputNumber=sBuffer.substring(13, 14);
                  sOutputState=sBuffer.substring(14, 15);
                  if (bShowTrace){Serial.print(sOutputNumber);}
                  if (bShowTrace){Serial.println(sOutputState);}
                  _nOutput=sOutputNumber[0]-'0';
                  bNewOutputUpdate=true;
                  if (sOutputState=="1"){
                    nCtrlOutputs[_nOutput-1]=1;                    

                  }
                  else{
                    nCtrlOutputs[_nOutput-1]=0;                    
                  }                
            }
            
            //ALL In case it is an event that happend everyday
            sSchTimeAll=sBuffer.substring(9, 12);
            if (bShowTrace){Serial.print("Time All: ");}
            if (bShowTrace){Serial.println(sSchTimeAll);}
            if (sSchTimeAll=="ALL" ){
              if (bShowTrace){Serial.println("Time All: ");}
              if (bShowTrace){Serial.println(sSchTimeAll);}
            }
                
          }
      }

    // close the file:
    myFile.close();
    
  } 
  else {
  	// if the file didn't open, print an error:
    Serial.println("error opening Schedule.txt");
  }
}

#pragma endregion

#pragma region "Tempertaure sensor"
  void printAddress(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}

// function to print the temperature for a device
void printTemperature(DeviceAddress deviceAddress)
{
  float tempC = sensors.getTempC(deviceAddress);
  Serial.print("Temp C: ");
  Serial.print(tempC);
  Serial.print(" Temp F: ");
  Serial.print(DallasTemperature::toFahrenheit(tempC));
}
#pragma endregion
