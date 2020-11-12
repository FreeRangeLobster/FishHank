#include <Arduino.h>


/*
 * Developed by JVivas to be used in the FishHank project 
 * github.com/freerangelobster
 * 15/4/2020
 * juanpadillavivas@gmail.com
 * Schecdule of outputs/inputs
 * Inputs A5,A4,A3,A2,A1,A0,D13,D12
 * Outputs D2,D3,D4,D5,D6,B0,B1,B2
 * New Data B4
 * Tx/RxD0,D1
 * Pinout ATMEga328P//http://www.hobbytronics.co.uk/arduino-atmega328-pinout
 * USB to Serial Adapter for Arduino 3.3V / 5V Switchable adaptor
 * Connection using the programmer:
 * Programmer side   --- PCB Side
 * DTR - RTS
 * RXT - UTS
 * TXD - URX
 * 5V  - +5V(Optional)
 * CTS - No connected
 * GND - GND
 * Serial Comms 115200
*/


//Outputs
#define Out1  5 //D5
#define Out2  4 //D4
#define Out3  3 //D3
#define Out4  2 //D2
#define Out5  9 //B1
#define Out6  8 //B0
#define Out7  7 //D6
#define Out8  6 //D7



//New Data Available
#define NewDataAvailable  10 //B2


#define In1 12 //A5      
#define In2 13 //A4      
#define In3 A0 //A3      
#define In4 A1 //A2       
#define In5 A2 //A1      
#define In6 A3 //A0      
#define In7 A4 //13      
#define In8 A5 //12      

int Inputs[8];
//debouncing values
int CurrentInputValue[8];
int LastInputValue[8];
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers

int Outputs[8];
int nDataAvailable;
int i;
String readString;
int nOutputNumber;


void UpdateOutputs();  

// the setup function runs once when you press reset or power the board
void setup() {
   Serial.begin(115200);
   Serial.print("Remote IO Restarted");
   Serial.print('\n');
   
   String sMsg;
    //outputs
    pinMode(Out1, OUTPUT); //D2
    pinMode(Out2, OUTPUT);
    pinMode(Out3, OUTPUT);
    pinMode(Out4, OUTPUT);
    pinMode(Out5, OUTPUT);
    pinMode(Out6, OUTPUT);
    pinMode(Out7, OUTPUT);
    pinMode(Out8, OUTPUT);
    pinMode(NewDataAvailable, OUTPUT); 
    //inputs
    pinMode(In1, INPUT);
    pinMode(In2, INPUT);
    pinMode(In3, INPUT);
    pinMode(In4, INPUT);
    pinMode(In5, INPUT);
    pinMode(In6, INPUT);
    pinMode(In7, INPUT);
    pinMode(In8, INPUT);
   //Setting the pull up resistors
    digitalWrite(In1, HIGH);
    digitalWrite(In2, HIGH);
    digitalWrite(In3, HIGH);
    digitalWrite(In4, HIGH);
    digitalWrite(In5, HIGH);
    digitalWrite(In6, HIGH);
    digitalWrite(In7, HIGH);
    digitalWrite(In8, HIGH);
}

void ClearLatchInputs(){
    for(i=0;i<8;i++){
          Inputs[i]=0;
    }
    digitalWrite(NewDataAvailable, LOW);
}

void executeCommand(String cmd)
{
    int nOutputNumber,i;
    String sOutputNumber;
    if (cmd == "?ID") Serial.println("FishHank I/O Microcontroller Ver 0.1");
    
    else if(cmd == "HELP"){
      Serial.println("?I = Returns the state of all inputs");
      Serial.println("?O = Returns the state of all outputs + New Data Available");
      Serial.println("!A0 = Resets all outputs");
      Serial.println("!11 = Sets Output 1 to high, there are 1-8 outputs");
      Serial.println("!10 = Resets Output 1 to Low, there are 1-8 outputs");
      Serial.println("ACK = Acknowledges the new data available and clears the pin flag");
    }
    
    
    else if (cmd == "?I"){
        Serial.print(Inputs[0]);
        Serial.print(Inputs[1]);
        Serial.print(Inputs[2]);
        Serial.print(Inputs[3]);
        Serial.print(Inputs[4]);
        Serial.print(Inputs[5]);
        Serial.print(Inputs[6]);
        Serial.print(Inputs[7]);
        Serial.print('\n');
        }

     
     else if (cmd == "?O"){
        //Serial.println(Outputs[0] + Outputs[1] + Outputs[2] + Outputs [3] + Outputs[4] + Outputs[5] + Outputs[6] + Outputs[7]);
        Serial.print(Outputs[0]);
        Serial.print(Outputs[1]);
        Serial.print(Outputs[2]);
        Serial.print(Outputs[3]);
        Serial.print(Outputs[4]);
        Serial.print(Outputs[5]);
        Serial.print(Outputs[6]);
        Serial.print(Outputs[7]);
        Serial.print(nDataAvailable);
        Serial.print('\n');
        }

     
     else if (cmd == "!A0"){
        //All to 0
        for(i=0;i<8;i++){
          Outputs[i]=0;
          }
        UpdateOutputs();  
        }

      
     else if (cmd.substring(0, 1) == "!"){
        //Captures in case there is a set of an output
        //Serial.println(cmd);
        //Serial.print("Setting Value to output:");
        sOutputNumber=cmd.substring(1, 2);
        nOutputNumber=sOutputNumber.toInt()-1;
        //Serial.println(sOutputNumber);
        if (cmd.substring(2, 3)=="1"){
          //Serial.println("Output High");
          Outputs[nOutputNumber]=1;
          }
        else{
          //Serial.println("Output Low");
          Outputs[nOutputNumber]=0;    
          }
        UpdateOutputs();
        Serial.print("OK");
        Serial.print('\n');     
        }


      else if (cmd == "ACK"){
          ClearLatchInputs();
          Serial.println("OK");   
         }

     
      else {
        Serial.print("NOK");
        Serial.print('\n');
      }
      
}

void UpdateOutputs(){
    if (Outputs[0]==1)digitalWrite(Out1, HIGH); else digitalWrite(Out1, LOW);
    if (Outputs[1]==1)digitalWrite(Out2, HIGH); else digitalWrite(Out2, LOW);
    if (Outputs[2]==1)digitalWrite(Out3, HIGH); else digitalWrite(Out3, LOW);
    if (Outputs[3]==1)digitalWrite(Out4, HIGH); else digitalWrite(Out4, LOW);
    if (Outputs[4]==1)digitalWrite(Out5, HIGH); else digitalWrite(Out5, LOW);
    if (Outputs[5]==1)digitalWrite(Out6, HIGH); else digitalWrite(Out6, LOW);
    if (Outputs[6]==1)digitalWrite(Out7, HIGH); else digitalWrite(Out7, LOW);
    if (Outputs[7]==1)digitalWrite(Out8, HIGH); else digitalWrite(Out8, LOW);
    }
  
void checkSerial(){
    if (!Serial.available()) return;
    char c = Serial.read();
    if ((c == '\n')||(c == '\r')) {
      Serial.print(readString);
      Serial.print('\n');
      executeCommand(readString);
      readString = "";    
    }
    else readString += c;
}

void CheckInputPressedAndLatch(){

    if (digitalRead (In1)==HIGH) {
      Inputs[0]=1;
      digitalWrite(NewDataAvailable, HIGH);
      Outputs[0]=!Outputs[0];
      UpdateOutputs();
      delay(500);
      do
      {    
        delay(10);
      } while ( digitalRead (In1)==HIGH );
    }

    if (digitalRead (In2)==HIGH) {
      Inputs[1]=1;
      digitalWrite(NewDataAvailable, HIGH);
      Outputs[1]=!Outputs[1];
      UpdateOutputs();
      delay(500);
      do
      {    
        //Waiting for user to release the button    
        delay(10);
      } while ( digitalRead (In2)==HIGH );
    }

    if (digitalRead (In3)==HIGH) {
      Inputs[2]=1;
      digitalWrite(NewDataAvailable, HIGH);
      Outputs[2]=!Outputs[2];
      UpdateOutputs();
      delay(500);
      do
      {    
        //Waiting for user to release the button    
         delay(10);
      } while ( digitalRead (In3)==HIGH );
    }

    if (digitalRead (In4)==HIGH) {
      Inputs[3]=1;
      digitalWrite(NewDataAvailable, HIGH);
      Outputs[3]=!Outputs[3];
      UpdateOutputs();
      delay(500);
      do
      {    
        delay(10);
      } while ( digitalRead (In4)==HIGH );
    }
    
    if (digitalRead (In5)==HIGH) {
      Inputs[4]=1;
      digitalWrite(NewDataAvailable, HIGH);
      Outputs[4]=!Outputs[4];
      UpdateOutputs();
      delay(500);
      do
      {    
        //Waiting for user to release the button    
        delay(10);;
      } while ( digitalRead (In5)==HIGH );
    }
    
    if (digitalRead (In6)==HIGH) {
      Inputs[5]=1;
      digitalWrite(NewDataAvailable, HIGH);
      Outputs[5]=!Outputs[5];
      UpdateOutputs();
      delay(500);
      do
      {    
        //Waiting for user to release the button    
        delay(10);
      } while ( digitalRead (In6)==HIGH );
    }
    
    if (digitalRead (In7)==HIGH) {
      Inputs[6]=1;
      digitalWrite(NewDataAvailable, HIGH);
      Outputs[6]=!Outputs[6];
      UpdateOutputs();
      delay(500);
      do
      {    
        //Waiting for user to release the button    
        delay(10);
      } while ( digitalRead (In7)==HIGH );
    }
    
    if (digitalRead (In8)==HIGH) {
      Inputs[7]=1;
      digitalWrite(NewDataAvailable, HIGH);
      Outputs[7]=!Outputs[7];
      UpdateOutputs();
      delay(500);
      do
      {    
        //Waiting for user to release the button    
        delay(10);
      } while ( digitalRead (In8)==HIGH );
    }
}

void ReadOutputStatus(){
    Outputs[0] = digitalRead (Out1);
    Outputs[1] = digitalRead (Out2);
    Outputs[2] = digitalRead (Out3);
    Outputs[3] = digitalRead (Out4);
    Outputs[4] = digitalRead (Out5);
    Outputs[5] = digitalRead (Out6);
    Outputs[6] = digitalRead (Out7);
    Outputs[7] = digitalRead (Out8);
    nDataAvailable = digitalRead (NewDataAvailable);
}


// the loop function runs over and over again forever
void loop() {
  checkSerial();
  CheckInputPressedAndLatch();
  ReadOutputStatus();
  delay(1);                       
}
