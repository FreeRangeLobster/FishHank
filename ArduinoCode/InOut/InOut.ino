//Inputs A5,A4,A3,A2,A1,A0,D13,D12
//Outputs D2,D3,D4,D5,D6,B0,B1,B2
//New Data B4
//Tx/RxD0,D1


//http://www.hobbytronics.co.uk/arduino-atmega328-pinout
//Outputs
#define Out1  2 //D2
#define Out2  3 //D3
#define Out3  4 //D4
#define Out4  5 //D5
#define Out5  6 //D6
#define Out6  7 //D6
#define Out7  8 //B0
#define Out8  9 //B1


//New Data Available
#define NewDataAvailable  10 //B2

//Inputs
#define In1 A5
#define In2 A4
#define In3 A3
#define In4 A2
#define In5 A1
#define In6 A0
#define In7 13 //B5
#define In8 12 //B4

int Inputs[8];
int Outputs[8];
String readString;
int nOutputNumber;

// the setup function runs once when you press reset or power the board
void setup() {
   Serial.begin(9600);
   Serial.print("Remote IO Restarted");
   String sMsg;
   
   
 
  // initialize digital pin LED_BUILTIN as an output.
  //pinMode(LED_BUILTIN, OUTPUT);
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
 
  digitalWrite(In1, HIGH);
  digitalWrite(In2, HIGH);
  digitalWrite(In3, HIGH);
  digitalWrite(In4, HIGH);
  digitalWrite(In5, HIGH);
  digitalWrite(In6, HIGH);
  digitalWrite(In7, HIGH);
  digitalWrite(In8, HIGH);

 
}

void executeCommand(String cmd)
{
    int nOutputNumber,i;
    String sOutputNumber;
    if (cmd == "?ID") Serial.println("FishHank I/O Microcontroller Ver 0.1");
    else if(cmd == "HELP"){
      Serial.println("?I = Returns the state of all inputs");
      Serial.println("?O = Returns the state of all outputs");
      Serial.println("!A0 = Resets all outputs");
      Serial.println("!11 = Sets Output 1 to high, there are 1-8 outputs");
      Serial.println("!10 = Resets Output 1 to Low, there are 1-8 outputs");
    }
    
    
    else if (cmd == "?I"){
      Serial.print(Inputs[0]);
      Serial.print(Inputs[1]);
      Serial.print(Inputs[2]);
      Serial.print(Inputs[3]);
      Serial.print(Inputs[4]);
      Serial.print(Inputs[5]);
      Serial.print(Inputs[6]);
      Serial.println(Inputs[7]);
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
      Serial.println(Outputs[7]);
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
      Serial.println(cmd);
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
      }
      
    else Serial.println("Bad command");
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
  
void checkSerial()
{
    if (!Serial.available()) return;
    
    char c = Serial.read();
    if (c == '\n') {
      Serial.println(readString);
      executeCommand(readString);
      readString = "";    
    }
    else readString += c;
}

void ConvertInputsToString (String cmd){
 
}

// the loop function runs over and over again forever
void loop() {
  checkSerial();
  /*digitalWrite(Out1, HIGH);
  digitalWrite(Out2, HIGH);
  digitalWrite(Out3, HIGH);
  digitalWrite(Out4, HIGH);
  digitalWrite(Out5, HIGH);
  digitalWrite(Out6, HIGH);
  digitalWrite(Out7, HIGH);
  digitalWrite(Out8, HIGH);
  digitalWrite(NewDataAvailable, HIGH);
 
 
  delay(1);                       // wait for a second
  /*digitalWrite(Out1, LOW);
  digitalWrite(Out2, LOW);
  digitalWrite(Out3, LOW);
  digitalWrite(Out4, LOW);
  digitalWrite(Out5, LOW);
  digitalWrite(Out6, LOW);
  digitalWrite(Out7, LOW);
  digitalWrite(Out8, LOW);
  digitalWrite(NewDataAvailable, LOW);*/
 
  delay(1);                       // wait for a second

  // Check input status
  Inputs[0] = digitalRead (In1);
  Inputs[1] = digitalRead (In2);
  Inputs[2] = digitalRead (In3);
  Inputs[3] = digitalRead (In4);
  Inputs[4] = digitalRead (In5);
  Inputs[5] = digitalRead (In6);
  Inputs[6] = digitalRead (In7);
  Inputs[7] = digitalRead (In8);

  Outputs[0] = digitalRead (Out1);
  Outputs[1] = digitalRead (Out2);
  Outputs[2] = digitalRead (Out3);
  Outputs[3] = digitalRead (Out4);
  Outputs[4] = digitalRead (Out5);
  Outputs[5] = digitalRead (Out6);
  Outputs[6] = digitalRead (Out7);
  Outputs[7] = digitalRead (Out8);
  

 /*
  Serial.print(Inputs[0]);
  Serial.print(Inputs[1]);
  Serial.print(Inputs[2]);
  Serial.print(Inputs[3]);
  Serial.print(Inputs[4]);
  Serial.print(Inputs[5]);
  Serial.print(Inputs[6]);
  Serial.println(Inputs[7]);
 */

  checkSerial();


//in case input is up flag up NewInput, update input vector
//check if manual/auto
//update outputs
//handle serial


}

/*
  // substring(index) looks for the substring from the index position to the end:
  if (stringOne.substring(19) == "html") {
    Serial.println("It's an html file");
  }
  // you can also look for a substring in the middle of a string:
  if (stringOne.substring(14, 18) == "text") {
    Serial.println("It's a text-based file");
  } */
