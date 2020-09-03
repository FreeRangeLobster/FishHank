
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "RTClib.h"


#include <HardwareSerial.h>
#include "IOCtrl.h"


RTC_DS3231 rtc;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
 
#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels
#define OLED_RESET    -1  // Reset pin # (or -1 if sharing reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


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
void ShowCurrentTime();


void setup()
{

  Serial.begin(115200);
  Serial2.begin(115200);

  Serial2.print("?I");
  Serial2.print('\n'); 
  checkIOCtrlSerial();

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

  rtc.adjust(DateTime(__DATE__, __TIME__));
 
 display.display();
 delay(2);
 display.clearDisplay();
 
 
  display.clearDisplay();
  display.setTextColor(WHITE);
  //display.startscrollright(0x00, 0x0F);
  display.setTextSize(2);
  display.setCursor(0,5);
  display.print("  Clock ");
  display.display();
  delay(3000);
  
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

  ShowCurrentTime();
  
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


