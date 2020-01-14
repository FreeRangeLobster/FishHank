
const byte In1=36;
const byte In2=37;
const byte In3=38;
const byte In4=39;
const byte In5=40;
const byte In6=41;
const byte In7=11;
const byte In8=9;

const byte Out1=32;
const byte Out2=26;
const byte Out3=12;
const byte Out4=30;
const byte Out5=29;
const byte Out6=27;
const byte Out7=31;
const byte Out8=18;

const byte NewInfo=20;

// the setup function runs once when you press reset or power the board
void setup() {
   Serial.begin(9600);
  
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  //outputs
  pinMode(Out1, OUTPUT);
  pinMode(Out2, OUTPUT);
  pinMode(Out3, OUTPUT);
  pinMode(Out4, OUTPUT);
  pinMode(Out5, OUTPUT);
  pinMode(Out6, OUTPUT);
  pinMode(Out7, OUTPUT);
  pinMode(Out8, OUTPUT);
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  pinMode(A4, OUTPUT);
  pinMode(A5, OUTPUT);
  pinMode(NewInfo, OUTPUT);
  
  //inputs
  pinMode(In1, INPUT);
  pinMode(In2, INPUT);
  pinMode(In3, INPUT);
  pinMode(In4, INPUT);
  pinMode(In5, INPUT);
  pinMode(In6, INPUT);
  pinMode(In7, INPUT);
  pinMode(In8, INPUT);


  
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(32, HIGH);
  digitalWrite(26, HIGH);
  digitalWrite(12, HIGH);
  digitalWrite(30, HIGH);
  digitalWrite(29, HIGH);
  digitalWrite(27, HIGH);
  digitalWrite(31, HIGH);
  digitalWrite(18, HIGH);
  digitalWrite(18, HIGH);
  
  delay(1000);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
   digitalWrite(32, LOW);
  digitalWrite(26, LOW);
  digitalWrite(12, LOW);
  digitalWrite(30, LOW);
  digitalWrite(29, LOW);
  digitalWrite(27, LOW);
  digitalWrite(31, LOW);
  digitalWrite(18, LOW);
  
  delay(1000);                       // wait for a second




// Check input status
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

