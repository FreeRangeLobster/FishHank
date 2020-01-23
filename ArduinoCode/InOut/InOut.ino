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



// the setup function runs once when you press reset or power the board
void setup() {
   Serial.begin(9600);
   Serial.print("Remote IO Restarted");
 
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

// the loop function runs over and over again forever
void loop() {
  digitalWrite(Out1, HIGH);
  digitalWrite(Out2, HIGH);
  digitalWrite(Out3, HIGH);
  digitalWrite(Out4, HIGH);
  digitalWrite(Out5, HIGH);
  digitalWrite(Out6, HIGH);
  digitalWrite(Out7, HIGH);
  digitalWrite(Out8, HIGH);
  digitalWrite(NewDataAvailable, HIGH);
 
 
  delay(1000);                       // wait for a second
  digitalWrite(Out1, LOW);
  digitalWrite(Out2, LOW);
  digitalWrite(Out3, LOW);
  digitalWrite(Out4, LOW);
  digitalWrite(Out5, LOW);
  digitalWrite(Out6, LOW);
  digitalWrite(Out7, LOW);
  digitalWrite(Out8, LOW);
  digitalWrite(NewDataAvailable, LOW);
 
  delay(1000);                       // wait for a second

  // Check input status
  Inputs[0] = digitalRead (In1);
  Inputs[1] = digitalRead (In2);
  Inputs[2] = digitalRead (In3);
  Inputs[3] = digitalRead (In4);
  Inputs[4] = digitalRead (In5);
  Inputs[5] = digitalRead (In6);
  Inputs[6] = digitalRead (In7);
  Inputs[7] = digitalRead (In8);
 
  Serial.print(Inputs[0]);
  Serial.print(Inputs[1]);
  Serial.print(Inputs[2]);
  Serial.print(Inputs[3]);
  Serial.print(Inputs[4]);
  Serial.print(Inputs[5]);
  Serial.print(Inputs[6]);
  Serial.println(Inputs[7]);
 




//in case input is up flag up NewInput, update input vector
//check if manual/auto
//update outputs
//handle serial




  // if there's any serial available, read it:
  while (Serial.available() > 0) {

    // look for the next valid integer in the incoming serial stream:
    int red = Serial.parseInt();
    // do it again:
    int green = Serial.parseInt();
    // do it again:
    int blue = Serial.parseInt();

    // look for the newline. That's the end of your sentence:
    if (Serial.read() == '\n') {
      // constrain the values to 0 - 255 and invert
      // if you're using a common-cathode LED, just use "constrain(color, 0, 255);"
      red = 255 - constrain(red, 0, 255);
      green = 255 - constrain(green, 0, 255);
      blue = 255 - constrain(blue, 0, 255);

     

      // print the three numbers in one string as hexadecimal:
      Serial.print(red, HEX);
      Serial.print(green, HEX);
      Serial.println(blue, HEX);
    }
  }


}
