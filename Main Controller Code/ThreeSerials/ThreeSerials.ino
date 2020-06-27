HardwareSerial Serial1(1);
HardwareSerial Serial2(2);

void setup(){
  Serial.begin(115200);
  Serial1.begin(115200);
  Serial2.begin(115200);
}

void loop(){
  Serial.println("This Message is sent through USB Serial");
  Serial1.println("This Message is sent through UART1 Serial");
  Serial2.println("This Message is sent through UART2 Serial");
  delay(1000);  
}

