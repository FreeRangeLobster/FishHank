

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  //outputs
  pinMode(32, OUTPUT);
  pinMode(26, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(30, OUTPUT);
  pinMode(29, OUTPUT);
  pinMode(27, OUTPUT);
  pinMode(31, OUTPUT);
  pinMode(18, OUTPUT);
  //inputs
  pinMode(36, INPUT);
  pinMode(37, INPUT);
  pinMode(38, INPUT);
  pinMode(39, INPUT);
  pinMode(40, INPUT);
  pinMode(41, INPUT);
  pinMode(11, INPUT);
  pinMode(9, INPUT);


  
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
}
