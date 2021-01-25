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
#include <WiFi.h>
#include <AsyncMqttClient.h>

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
String sSystemStatus;
char nOption;

//String used in the communication with Arduino Aux Board
String readStringIOCtrl;
String sCurrentOutoutState;
unsigned long nCurrentMillis = millis();
unsigned long nPreviousMillis ;
 
int nCtrlOutputs [8] = { 0, 0, 0, 0, 0,0, 0, 0}; 
int nCurrentOutputs [9] = { 0, 0, 0, 0, 0,0, 0, 0,0}; 

int nLastCtrlOutputs [8] = { 0, 0, 0, 0, 0,0, 0, 0}; 
bool bNewOutputUpdate=false;
bool bShowTrace =false;
String sCurrentTime;
String sTraceString="";

#pragma region "Wifi MQTT"
//Wifi connections
//Home Address
#define WIFI_SSID "TALKTALK77D706"
#define WIFI_PASSWORD "DWTJF8G9"
//#define WIFI_SSID "ALPHR_SFLR_2G"
//#define WIFI_PASSWORD "AlphrWork24"
#define MQTT_HOST IPAddress(192, 168, 1, 11)
#define MQTT_PORT 1883
#define MQTT_PUB_TEMP "esp/bme680/temperature"
#define MQTT_SUS_BT1  "esp/bme680/button1"
#define MQTT_SUS_BT2  "esp/bme680/button2"
#define MQTT_SUS_BT3  "esp/bme680/button3"
#define MQTT_SUS_BT4  "esp/bme680/button4"
#define MQTT_SUS_BT5  "esp/bme680/button5"
#define MQTT_SUS_BT6  "esp/bme680/button6"
#define MQTT_SUS_BT7  "esp/bme680/button7"
#define MQTT_SUS_BT8  "esp/bme680/button8"
float temperature=25;
AsyncMqttClient mqttClient;
TimerHandle_t mqttReconnectTimer;
TimerHandle_t wifiReconnectTimer;
unsigned long previousMillis = 0;   // Stores last time temperature was published
const long interval = 10000;        // Interval at which to publish sensor readings




void connectToWifi() {
  Serial.println("Connecting to Wi-Fi...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

void connectToMqtt() {
  Serial.println("Connecting to MQTT...");
  mqttClient.connect();
}

void WiFiEvent(WiFiEvent_t event) {
  Serial.printf("[WiFi-event] event: %d\n", event);
  switch(event) {
    case SYSTEM_EVENT_STA_GOT_IP:
      Serial.println("WiFi connected");
      Serial.println("IP address: ");
      Serial.println(WiFi.localIP());
      connectToMqtt();
      break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
      Serial.println("WiFi lost connection");
      xTimerStop(mqttReconnectTimer, 0); // ensure we don't reconnect to MQTT while reconnecting to Wi-Fi
      xTimerStart(wifiReconnectTimer, 0);
      break;
  }
}

void onMqttConnect(bool sessionPresent) {
  Serial.println("Connected to MQTT.");
  Serial.print("Session present: ");
  Serial.println(sessionPresent);

  uint16_t packetIdSub1 = mqttClient.subscribe(MQTT_SUS_BT1, 2);
  uint16_t packetIdSub2 = mqttClient.subscribe(MQTT_SUS_BT2, 2);
  uint16_t packetIdSub3 = mqttClient.subscribe(MQTT_SUS_BT3, 2);
  uint16_t packetIdSub4 = mqttClient.subscribe(MQTT_SUS_BT4, 2);
  uint16_t packetIdSub5 = mqttClient.subscribe(MQTT_SUS_BT5, 2);
  uint16_t packetIdSub6 = mqttClient.subscribe(MQTT_SUS_BT6, 2);
  uint16_t packetIdSub7 = mqttClient.subscribe(MQTT_SUS_BT7, 2);
  uint16_t packetIdSub8 = mqttClient.subscribe(MQTT_SUS_BT8, 2);
  Serial.print("Subscribing at QoS 2, packetId: ");
  Serial.println(packetIdSub1);

}

void onMqttDisconnect(AsyncMqttClientDisconnectReason reason) {
  Serial.println("Disconnected from MQTT.");
  if (WiFi.isConnected()) {
    xTimerStart(mqttReconnectTimer, 0);
  }
}

void onMqttSubscribe(uint16_t packetId, uint8_t qos) {
  Serial.println("Subscribe acknowledged.");
  Serial.print("  packetId: ");
  Serial.println(packetId);
  Serial.print("  qos: ");
  Serial.println(qos);
}

void onMqttUnsubscribe(uint16_t packetId) {
  Serial.println("Unsubscribe acknowledged.");
  Serial.print("  packetId: ");
  Serial.println(packetId);
}

void onMqttPublish(uint16_t packetId) {
  
  //Serial.print("Publish acknowledged.");
  //Serial.print("  packetId: ");
  //Serial.println(packetId);
}

void onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) {
  Serial.println("Publish received.");
  Serial.print("  topic: ");
  Serial.println(topic);
  Serial.print("  qos: ");
  Serial.println(properties.qos);
  Serial.print("  dup: ");
  Serial.println(properties.dup);
  Serial.print("  retain: ");
  Serial.println(properties.retain);
  Serial.print("  len: ");
  Serial.println(len);
  Serial.print("  index: ");
  Serial.println(index);
  Serial.print("  total: ");
  Serial.println(total);

  if ((String(topic) == MQTT_SUS_BT1) || (String(topic) == MQTT_SUS_BT2) || (String(topic) == MQTT_SUS_BT3)||(String(topic) == MQTT_SUS_BT4)
    ||(String(topic) == MQTT_SUS_BT5) || (String(topic) == MQTT_SUS_BT6) || (String(topic) == MQTT_SUS_BT7)||(String(topic) == MQTT_SUS_BT8)) {
    Serial.println("Changing output");
    // if (digitalRead(LED)==HIGH){
    //   digitalWrite(LED,LOW);
    // }else
    // {
    //   digitalWrite(LED,HIGH);
    // }
  }
}




#pragma endregion

//State machine states used in the main look or to handle the supervisory
enum StatesEnum { Start, Initialise, ReadSD, EnablePeripherals, UpdateStatus, updateTFT,  CheckSerialComms };
StatesEnum CurrentState; 
enum eStateEnum{eCalculateNextTimeCheck,eShowStatus,ePublish,eCheckSD,ePrecheckOutputs,eUpdateOutputs,eUpdateDisplay,eIdle};
eStateEnum StateEnum;
eStateEnum LastStateEnum;

enum eSerialPort{eUSBPC,eBluetooth};

enum eIOCtrlCommsState{eStart,eSendCommand,eWaitForReply,eFormat,eCompare,eFinish,eTimeOut};

String sState ="";

//Time system is in idle state
const long interval2 = 5*1000;     //in seconds

//RTC
RTC_DS3231 rtc;
char daysOfTheWeek[7][12] = {"SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"};
char char_arrayTime[10];
DateTime now;
void RTCSetTimeAndDate();
float gTemperature=0;



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


void CheckSerialUSB();
void CheckSerialBlueT();
void SerialCommandHandller( eSerialPort  nSerial );
void checkIOCtrlSetOutputTo(int OutputNumber, int State);
void checkIOCtrlSerial();
void RTCSetTimeAndDate(char *Currenttime);
void DumpSchedule();
void DumpSchedule2(eSerialPort  nSerial );
void SetOutputTo();
void CheckEvent(String sTime);
void IOCtrlUpdateOutputs();
void DisplayUpdateOutputs();
void FormatCurrrentTime();
void ToggleLED1();
void ToggleLED2();
void printAddress(DeviceAddress deviceAddress);
void printTemperature(DeviceAddress deviceAddress);
void CheckConfigurationFile();
void IOCtrlCheckCurrentOutputState();



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
  Serial_1.print("Serial1 Working");
  Serial_1.print('\n');

  //Serial_2.print("Serial2 working");
  //Serial_2.print('\n');

  //Wifi MQTT
  mqttReconnectTimer = xTimerCreate("mqttTimer", pdMS_TO_TICKS(2000), pdFALSE, (void*)0, reinterpret_cast<TimerCallbackFunction_t>(connectToMqtt));
  wifiReconnectTimer = xTimerCreate("wifiTimer", pdMS_TO_TICKS(2000), pdFALSE, (void*)0, reinterpret_cast<TimerCallbackFunction_t>(connectToWifi));
  
  WiFi.onEvent(WiFiEvent);

  mqttClient.onConnect(onMqttConnect);
  mqttClient.onDisconnect(onMqttDisconnect);
  mqttClient.onSubscribe(onMqttSubscribe);
  mqttClient.onUnsubscribe(onMqttUnsubscribe);
  mqttClient.onMessage(onMqttMessage);
  mqttClient.onPublish(onMqttPublish);
  mqttClient.setServer(MQTT_HOST, MQTT_PORT);
  // If your broker requires authentication (username and password), set them below
  //mqttClient.setCredentials("REPlACE_WITH_YOUR_USER", "REPLACE_WITH_YOUR_PASSWORD");
  connectToWifi();


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

   if(SD.exists("Config.txt")) {
    Serial.println("Config file Present");
  }


  CheckConfigurationFile();
  DumpSchedule();


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

  //CheckConfigurationFile();
  
  Serial.println("-");
  Serial.println("-------Initialization Finished-----");

}


void loop() {
  
 switch (StateEnum){
        
  case eCalculateNextTimeCheck:{
    sState="eCalculateNextTimeCheck";     
    now = rtc.now();                      
    StateEnum=eShowStatus;
  }
  break;

  case eShowStatus:{
    sState="eCalculateNextTimeCheck";
    sSystemStatus= "->t:" +  now.timestamp(DateTime::TIMESTAMP_FULL)+ ">T[C]:" +  String(gTemperature) + ">O:"\
    + String(nCtrlOutputs[7]) + String(nCtrlOutputs[6]) + String(nCtrlOutputs[5]) + String(nCtrlOutputs[4])\
    + String(nCtrlOutputs[3]) + String(nCtrlOutputs[2]) + String(nCtrlOutputs[1]) + String(nCtrlOutputs[0]) +">-";
    Serial.println(sSystemStatus);
    Serial_1.println(sSystemStatus);
    StateEnum=ePublish;
  }
  break;

  case ePublish:{
    sState="ePublish";
    uint16_t packetIdPub1 = mqttClient.publish(MQTT_PUB_TEMP, 1, true, String(gTemperature).c_str());
    //Serial.printf("Publishing on topic %s at QoS 1, packetId: %i", MQTT_PUB_TEMP, packetIdPub1);
    //Serial.printf("Message: %.2f \n", gTemperature);
    StateEnum=ePrecheckOutputs;
  }
  break;

  case ePrecheckOutputs:{
    sState="ePrecheckOutputs";
    IOCtrlCheckCurrentOutputState();
    DisplayUpdateOutputs();
    StateEnum=eCheckSD;
  }
  break;

  case eCheckSD:{
     sState="eCheckSD";                             
    FormatCurrrentTime();
    if (now.second()<10){
      if (bShowTrace){Serial.println("----> Check Events");}                     
      CheckEvent(sCurrentTime);          
    }            
    StateEnum=eUpdateOutputs;
  }
  break;
  
  case eUpdateOutputs:{
    sState="eUpdateOutputs";          
    IOCtrlUpdateOutputs(); 
    StateEnum=eUpdateDisplay;
  }
  break;

  case eUpdateDisplay:{  
    sState="eUpdateDisplay";         
    DisplayCurrentTime();
    DisplayUpdateTemperature();
    StateEnum=eIdle;
  }
  break;

  case eIdle:{
    sState="eIdle";  
    nCurrentMillis=millis();
    if (nCurrentMillis - nPreviousMillis >= interval) {            
      nPreviousMillis = nCurrentMillis;
      StateEnum=eCalculateNextTimeCheck;
      ToggleLED1();               
      //delay(500);
    }  

    ToggleLED2();  
    CheckSerialUSB();
    if (sSerialUSB != ""){
      SerialCommandHandller(eUSBPC);
      StateEnum=eCalculateNextTimeCheck;
    } 
    
    CheckSerialBlueT();
    if (sSerialUSB != ""){
      SerialCommandHandller(eBluetooth);
      StateEnum=eCalculateNextTimeCheck;
    }
 
  }
  break;

  default:{
      // is likely to be an error
      Serial.println("Main State Machine Error");
      StateEnum=eCalculateNextTimeCheck;
  }
  }

  //Updates trace after state
if (StateEnum != LastStateEnum){
  LastStateEnum=StateEnum; 

  sTraceString= sTraceString + " LastState: " + sState + \
  "\n Internal Output Register: " + \
  String(nCtrlOutputs[7]) + String(nCtrlOutputs[6]) + String(nCtrlOutputs[5]) + String(nCtrlOutputs[4]) + \
  String(nCtrlOutputs[3]) + String(nCtrlOutputs[2]) + String(nCtrlOutputs[1]) + String(nCtrlOutputs[0]) + \
  "\n" ;

  if (bShowTrace){
    Serial.println(sTraceString + "\n");
    Serial_1.println(sTraceString + "\n"); 
  }    
  
  sTraceString="";

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
void CheckSerialUSB(){  
    if (!Serial.available()) return;
    
    if (Serial.available()) {   
      sSerialUSB = Serial.readStringUntil('\n');// s1 is String type variable.
      //delay(10);
      Serial.print("Received Data = ");
      Serial.println(sSerialUSB);//display same received Data back in serial monitor. 
      nOption=sSerialUSB[0];
    }
 }

void CheckSerialBlueT(){  
    if (!Serial_1.available()) return;
    
    if (Serial_1.available()) {   
      sSerialUSB = Serial_1.readStringUntil('\n');// s1 is String type variable.
      //delay(10);
      Serial_1.print("Received Data = ");
      Serial_1.println(sSerialUSB);//display same received Data back in serial monitor. 
      nOption=sSerialUSB[0];
    }
 }



void SerialCommandHandller( eSerialPort  nSerial ){    
  String sResponse;
  switch ( sSerialUSB[0]) {
      case 'K':
        sResponse="Initialise";
        sSerialUSB="";
      break;
      
      case 'I':
        sResponse=
        sSerialUSB="I Idle";
      break;
  
      case 'N':
        sResponse="I Idle";     
        sSerialUSB="";
      break;
  
      case 'M':
        sResponse="M Check Events";
        CheckEvent("1038 MON");
        sSerialUSB="";
      break;
  
      case 'O':
        sResponse="O Update Outputs";
        SetOutputTo(); 
        //Request information twice because there is a bug on the 
        //Aux arduino system. 
        IOCtrlCheckCurrentOutputState();
        IOCtrlCheckCurrentOutputState(); 
        sSerialUSB="";
      break;
  
      case 'D':
        sResponse="D Delete Memory";
        sSerialUSB="";
      break;
  
      case '0':
        sResponse="0 Dumps Events";   
        DumpSchedule2(nSerial);
        sSerialUSB="";
      break;
  
      case '1':
        sResponse="1 AddEvent  1_1030WED31"; 
        sSerialUSB="";
      break;
  
      case '2':
        sResponse="2 Disable Event  2_001";     
        sSerialUSB="";
      break;
  
      case '3':
        sResponse="3 Trace ON";
        bShowTrace = true;
        sSerialUSB="";
      break;
  
      case '4':
        sResponse="4 Update Output 4_11 Output 1 ON";    
        checkIOCtrlSetOutputTo(1,1);
        sSerialUSB="";
      break;
  
      case '5':
        sResponse="5 Write Status on OLED 5_Hello";     
        sSerialUSB="";
      break;
  
      case '6':
        sResponse="6 Set Time RTC 6_DDMMYYYY_HH:mm:ss"; 
        RTCSetTimeAndDate();    
        sSerialUSB="";
      break;
  
      case '7':
        sResponse="7 Read Time"; 
        Serial.println(String("DateTime::TIMESTAMP_FULL:\t")+now.timestamp(DateTime::TIMESTAMP_FULL));    
        sSerialUSB="";
      break;
  
      case '8':
        sResponse="8 Check Current Event";   
        CheckEvent("1044 MON");  
        sSerialUSB="";
      break;
             
      case '9':
        sResponse="9 Trace Off"; 
        bShowTrace = false;    
        sSerialUSB="";
      break;
  
      case '?':
        sResponse="? Help \n \
        K Initialise \n \
        I Idle \n \
        N Update Screen \n \
        M Check Events\n \
        O Update Outputs\n \
        D Delete Memory\n \
        0 Read Page Number 0_0000\n \
        1 AddEvent  1_1030WED31\n \
        2 Disable Event  2_001\n \
        3 Trace ON\n \
        4 Update Output 4_11 Output 1 ON\n \
        5 Write Status on OLED 5_Hello\n \
        6 Set Time RTC 6_DDMMY_HHmmss\n \
        7 Read Time\n \
        8 Check Current Event\n \
        9 Trace OFF";
        sSerialUSB="";
      break;
       
      
      default:
       //'In case nothing was received exits procedure'
        return ;
        break;
      }  
      if (nSerial==eUSBPC){
          Serial.println(sResponse);
      }
      else if(nSerial==eBluetooth){
          Serial_1.println(sResponse);
      }
      sSerialUSB="";
}

#pragma endregion

#pragma region "Aux IO Board Communication"

void SetOutputTo(){
    int nOutput ;
    int nState;
    nOutput= int(sSerialUSB[2]-'0');
    nState= int(sSerialUSB[3]-'0'); 
    //nCtrlOutputs[nOutput-1]=nState;     
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
    //Serial.println("Received data IO CTRL= " + readStringIOCtrl);
    //readStringIOCtrl = "";   
}

void IOCtrlUpdateOutputs(){
  if (bNewOutputUpdate==true){
    int i=0;
    for(i=0;i<=7;i++){
      if (bShowTrace){Serial.printf("IOCtrlUpdateOutputs: nCtrlOutputs: %d %d %d %d %d %d %d %d \n",nCtrlOutputs[7],nCtrlOutputs[6],nCtrlOutputs[5],nCtrlOutputs[4],nCtrlOutputs[3],nCtrlOutputs[2],nCtrlOutputs[1],nCtrlOutputs[0]);}
      checkIOCtrlSetOutputTo(i,nCtrlOutputs[i]);       
      delay(100);     
   }
   bNewOutputUpdate=false;
  }
}

void IOCtrlCheckCurrentOutputState(){

  boolean bFinish=false;
  int nState=eStart;
  double tTimeStart;
  double tCurrentTime;
  char cOutputValue;
  int nIndexOutput=0;
  //Get initial value to timeout

  tTimeStart=millis();

  do
  {
    //Timeout verification to avoid locking system
    tCurrentTime=millis();
    if ( tCurrentTime-tTimeStart  >= 2000) {  
      Serial.println("IOCtrlCheckCurrentOutputState - timeout Waiting, State: " + nState);          
      bFinish=true;
    }  

    switch (nState)
    {
    case eStart:
      sCurrentOutoutState="";
      nState=eSendCommand;
      break;

    case  eSendCommand:
      Serial_2.print("?O");
      Serial_2.print('\n'); 
      nState=eWaitForReply;
      break;

    case  eWaitForReply:
      if (Serial_2.available()){
        nState=eFormat;
      }
     
    break;

    case  eFormat:
      if (Serial_2.available()){
        cOutputValue=char(Serial_2.read());
        sCurrentOutoutState += cOutputValue;
        if (cOutputValue=='1'){
          nCurrentOutputs[nIndexOutput]=1;
          nIndexOutput=nIndexOutput+1;
        }
        else if (cOutputValue=='0')
        {
          nCurrentOutputs[nIndexOutput]=0;
          nIndexOutput=nIndexOutput+1;
        }
        else{
          delay(20);
        }    
        
      } 
      else
      {
        nState=eCompare;
      }     
    break;

    case  eCompare:
      nCtrlOutputs[0]=nCurrentOutputs[0];
      nCtrlOutputs[1]=nCurrentOutputs[1];
      nCtrlOutputs[2]=nCurrentOutputs[2];
      nCtrlOutputs[3]=nCurrentOutputs[3];
      nCtrlOutputs[4]=nCurrentOutputs[4];
      nCtrlOutputs[5]=nCurrentOutputs[5];
      nCtrlOutputs[6]=nCurrentOutputs[6];
      nCtrlOutputs[7]=nCurrentOutputs[7];
     nState=eFinish;
    break;

    case  eFinish:
     //nState=eSendCommand;
     
     if(bShowTrace){
          Serial.printf("output register: %d %d %d %d %d %d %d %d %d \n",nCurrentOutputs[0],nCurrentOutputs[1],nCurrentOutputs[2],nCurrentOutputs[3],nCurrentOutputs[4],nCurrentOutputs[5],nCurrentOutputs[6],nCurrentOutputs[7],nCurrentOutputs[8]);
        
        }
      bFinish=true;
    break;

    default:
      break;
    }
  } while ( bFinish==false );
     

}

void DumpStringToCurrentOutputs(String sStringReceived){

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
    display.fillRect(DisplayColumnX*0,DisplayColumnY,DisplayColumnWidth,DisplayColumnHight, BLACK);   
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
    display.fillRect(DisplayColumnX*1,DisplayColumnY,DisplayColumnWidth,DisplayColumnHight, BLACK); 
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
    
    //Column3    
    display.fillRect(DisplayColumnX*2,DisplayColumnY,DisplayColumnWidth,DisplayColumnHight, BLACK);
    display.drawRect(DisplayColumnX*2,DisplayColumnY,DisplayColumnWidth,DisplayColumnHight, WHITE);        
    display.setCursor((DisplayColumnX*2)+5,DisplayColumnY+2);
    if (nCtrlOutputs [2]==1){
      display.printf("ON");  
    }
    else
    {
       display.printf("OFF");  
    }
        
    //Column4  
    display.fillRect(DisplayColumnX*3,DisplayColumnY,DisplayColumnWidth,DisplayColumnHight, BLACK);      
    display.drawRect(DisplayColumnX*3,DisplayColumnY,DisplayColumnWidth,DisplayColumnHight, WHITE);    
    display.setCursor((DisplayColumnX*3)+5,DisplayColumnY+2);
    if (nCtrlOutputs [3]==1){
      display.printf("ON");  
    }
    else
    {
       display.printf("OFF");  
    }
    
    //second raw
    //Column1    
    display.fillRect(DisplayColumnX*0,DisplayColumnY2,DisplayColumnWidth,DisplayColumnHight, BLACK);
    display.drawRect(DisplayColumnX*0,DisplayColumnY2,DisplayColumnWidth,DisplayColumnHight, WHITE);    
    display.setCursor((DisplayColumnX*0)+5,DisplayColumnY2+2);    
    if (nCtrlOutputs [4]==1){
      display.printf("ON");  
    }
    else
    {
       display.printf("OFF");  
    }

    //Column2    
    display.fillRect(DisplayColumnX*1,DisplayColumnY2,DisplayColumnWidth,DisplayColumnHight, BLACK);   
    display.drawRect(DisplayColumnX*1,DisplayColumnY2,DisplayColumnWidth,DisplayColumnHight, WHITE);    
    display.setCursor((DisplayColumnX*1)+5,DisplayColumnY2+2);
    if (nCtrlOutputs [5]==1){
      display.printf("ON");  
    }
    else
    {
       display.printf("OFF");  
    }
        
    //Column3
    display.fillRect(DisplayColumnX*2,DisplayColumnY2,DisplayColumnWidth,DisplayColumnHight, BLACK); 
    display.drawRect(DisplayColumnX*2,DisplayColumnY2,DisplayColumnWidth,DisplayColumnHight, WHITE);    
    display.setCursor((DisplayColumnX*2)+5,DisplayColumnY2+2);
    if (nCtrlOutputs [6]==1){
      display.printf("ON");  
    }
    else
    {
       display.printf("OFF");  
    }
    
    //Column4
    display.fillRect(DisplayColumnX*3,DisplayColumnY2,DisplayColumnWidth,DisplayColumnHight, BLACK); 
    display.drawRect(DisplayColumnX*3,DisplayColumnY2,DisplayColumnWidth,DisplayColumnHight, WHITE);    
    display.setCursor((DisplayColumnX*3)+5,DisplayColumnY2+2);
    if (nCtrlOutputs [7]==1){
      display.printf("ON");  
    }
    else
    {
       display.printf("OFF");  
    }    

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
    gTemperature=   tempC;
    char result[8]; // Buffer big enough for 7-character float
    dtostrf(tempC, 6, 2, result);

    
    
    display.fillRect(75,10,30,7, BLACK);    
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

#pragma region "SD Events and Config"

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

void DumpSchedule2(eSerialPort  nSerial ){

    // Open the file for reading:
  File myFile;
  myFile = SD.open("Schedule.txt");
  if (myFile) {
    //Name of the schedule
    if (nSerial==eUSBPC ){
        Serial.println("Schedule.txt:");
    }
    else if (nSerial==eBluetooth){
        Serial_1.println("Schedule.txt:");
    }

    // read from the file until there's nothing else in it:
    while (myFile.available()) {
      //Writes on requested serial port 
      if (nSerial==eUSBPC ){
        Serial.write(myFile.read());
      }
      else if (nSerial==eBluetooth){
        Serial_1.write(myFile.read());
      }
    }

    // close the file:
    myFile.close();
  } 
  else {
  	// if the file didn't open, print an error:
    if (nSerial==eUSBPC ){
      Serial.println("error opening Schedule.txt");
    }
    else if (nSerial==eBluetooth){
      Serial_1.println("error opening Schedule.txt");
    }
  }
}

void CheckEvent(String sTime){

  String sBuffer="";
  String sSchTime="";
  String sSchTimeTrim="";
  String sTimeTrim="";

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
                  Serial.print("*Event Found");
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
              if (bShowTrace){Serial.println("-->Time All found: ");}
              if (bShowTrace){Serial.println(sSchTimeAll);}
              //Trim the sTime and sSchTime
              sSchTimeTrim=sSchTime.substring(0, 4);
              sTimeTrim=sTime.substring(0, 4);   
              if (bShowTrace){Serial.println("-->Time Trim ");}                         
              if (bShowTrace){Serial.print(sSchTimeTrim);}
              if (bShowTrace){Serial.print("VS");}                         
              if (bShowTrace){Serial.println(sTimeTrim);}
              //Check time and Outputs
              if (sTimeTrim == sSchTimeTrim) {
                  Serial.print("Event Found");
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

void CheckConfigurationFile(){
 File myFile2;
 String sEntry="";
 String sEnable="";
  myFile2 = SD.open("Config.txt");
  if (myFile2) {
    Serial.println("Config.txt");
    
    int i=0;
    // read from the file until there's nothing else in it:
    //Sample file:
    // close the file:
    //Configuration
    // Last Modified 17092020
    // 1 EnableTrace
    // 1 EnableBluetooth
    // 1 Clear Outputs


    while (myFile2.available()) {
    	
        sEntry=myFile2.readStringUntil('\n');
        //Check if the event is enabled
        sEnable = sEntry.substring(0, 1);

      //sEntry=myFile2.read();
      //Prints the file       
      Serial.println(sEntry);

      switch(i){
        case 0:
        //'Header'
        Serial.print("Header: ");
        Serial.println(sEntry);        
        break;

        case 1:
        //Last modified
        Serial.print("Last Modified: ");
        Serial.println(sEntry);
        break;

        case 2:
          //Enable trace
          Serial.print("Enable Trace: ");
          Serial.println(sEntry);
          Serial.println(sEnable);
          if (sEnable=="1") {
            bShowTrace =true;    
            Serial.println("Trace enabled in the SD card");
          }
          else {
            bShowTrace =false;    
            Serial.println("Trace disabled in the SD card");
          }

        break;

        case 3:
        //Enable Bluetooth
        Serial.print("Enable Bluetooth: ");
        Serial.println(sEntry);
        Serial.println(sEnable);
        break;

        case 4:
        //Clear outputs
        Serial.print("Clear Oututs: ");
        Serial.println(sEntry);
        Serial.println(sEnable);
        break;

        default:
        //nothing
        break;

      }          
      i=i+1;
    }






    myFile2.close();
  } else {
  	// if the file didn't open, print an error:
    Serial.println("error opening Config.txt");
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
