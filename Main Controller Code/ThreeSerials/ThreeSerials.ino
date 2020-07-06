//https://esp32.com/viewtopic.php?t=328  
/* ESP32 3 UARTs */

HardwareSerial Serial_1(1);
HardwareSerial Serial_2(2);

// Choose two free pins
#define SERIAL1_RXPIN 2//12
#define SERIAL1_TXPIN 4//13

String readString;
String readStringB;

void setup() {
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

void CheckSerialVer2(){
  
 if (!Serial.available()) return;

 if (Serial.available()) {
    String s1 = Serial.readStringUntil('\n');// s1 is String type variable.
    Serial.print("Received Data = ");
    Serial.println(s1);//display same received Data back in serial monitor.
   // delay(500);
  }
 
 }


void loop() {
// Do something wonderful
//  checkSerial_2();
//  checkSerial_1();

  Serial_1.print("Serial1 Working");
  Serial_1.print('\n');
delay(200);
  Serial_2.print("Serial2 working");
  Serial_2.print('\n');

  CheckSerialVer2();

  delay(200);
}


