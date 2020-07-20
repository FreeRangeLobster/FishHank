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

enum StatesEnum {Start , Initialise, ReadSD, EnablePeripherals, UpdateStatus, updateTFT,  CheckSerialComms,HandleSerials,Idle };
StatesEnum CurrentState;
StatesEnum LastState;

void setup() {
  sSerialUSB="NOthing";
  Serial.begin(115200);
  //Serial1.begin();
  Serial_1.begin(9600, SERIAL_8N1, SERIAL1_RXPIN, SERIAL1_TXPIN,false);
  Serial_2.begin(9600);  // pin 16=RX, pin 17=TX

  Serial.println("Restarted");
  Serial.println("");
  Serial.println("");
  Serial_1.print("Serial1 Working");
  Serial_1.print('\n');

  Serial_2.print("Serial2 working");
  Serial_2.print('\n');

  CurrentState=Start;
}


void checkSerial_2();
void checkSerial_1();
void checkSerial1();
void SerialCommandHandller();
void CheckSerialVer2();
void CheckSerialVer3();


void loop() {


  switch (CurrentState){

    case Start: 
      CurrentState=Initialise;
    break;
    
    case Initialise: 
      CurrentState=ReadSD;    
    break;
            
    case ReadSD:
       CurrentState=HandleSerials;     
    break;
    
    case HandleSerials: 
        //CheckSerialVer2();
        CheckSerialVer3();
        SerialCommandHandller();
        CurrentState=Idle;
    break;

    case Idle:
      CurrentState=UpdateStatus;      
    break;

    case UpdateStatus:
       CurrentState=ReadSD;     
    break;

   }

  
//  checkSerial_2();
//  checkSerial_1();

  //Serial_1.print("Serial1 Working");
  //Serial_1.print('\n');
//delay(200);
  //Serial_2.print("Serial2 working");
  //Serial_2.print('\n');
 
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

      case 'S':
        Serial.println("S Get Status");     
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
        Serial.println("? Help");
        Serial.println("K Initialise");  
        Serial.println("I Idle");
        Serial.println("N Update Screen");   
        Serial.println("M Check Events");
        Serial.println("S Get Status");
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
