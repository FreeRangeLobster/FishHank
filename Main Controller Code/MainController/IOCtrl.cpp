
#include "IOCtrl.h"
//HardwareSerial Serial_1(1);
//String readStringIOCtrl;


IOCtrl::IOCtrl(int pin)
{  
//  Serial_1.begin(9600, SERIAL_8N1, SERIAL1_RXPIN, SERIAL1_TXPIN,false);
  pinMode(pin, OUTPUT);
  _pin = pin;
}




const char* IOCtrl::GetStatus()
{
  //Serial_1.print("?ID");
  //String  mystring="OK,11110000,0000000";
  //const char* cnv=mystring.c_str();
  //return cnv;
  
  const char* test=CheckSerialAuxController();
  
 return test;
  
}


const char* IOCtrl::GetInputs()
{
const char* sInputs = "00100000";
return sInputs;
}

const char* IOCtrl::SetOutputTo(int OutputNumber,int State){
  String  Result= "SetOutputTo=OK;" + String(OutputNumber) + String(State);
  const char* cResult=Result.c_str();
  return cResult;
}

const char* IOCtrl::SetOutput(int OutputNumber){
   String  Result= "SetOutput=OK;" + String(OutputNumber);
   const char* cResult=Result.c_str();
   return cResult;
}

const char* IOCtrl::ClearOutput(int OutputNumber){
 String  Result= "ClearOutput=OK;" + String(OutputNumber);
 const char* cResult=Result.c_str();
 return cResult;
}

const char* IOCtrl::ClearEvent(){
   String  Result= "ClearEvent=OK;";
  const char* cResult=Result.c_str();
  return cResult;
  }


const char* IOCtrl::ClearOutputs(){
  String  Result= "ClearOutputs=OK;";
  const char* cResult=Result.c_str();
  return cResult;
}

/*
void checkIOCtrlSerial(){
    if (!Serial2.available()) return;
    while(Serial2.available()){
      readStringIOCtrl += char(Serial2.read());
    }
    Serial.println("Received data = " + readStringIOCtrl);
    //readStringIOCtrl = "";   
}*/



const char* IOCtrl::CheckSerialAuxController(){  

  String  mystring="OK,11110000,00001111";
  const char* cnv=mystring.c_str();
  return  cnv;
  /*String  sString="";
  const char* cResult=sString.c_str();
    if (!Serial_1.available()){
      return cResult;
    } 
      
    while (Serial_1.available()) {
      
      sString = Serial_1.readStringUntil('\n');// s1 is String type variable.
      //delay(10);
      //Serial.print("Received Data = ");
      //Serial.println(sString);//display same received Data back in serial monitor. 
      //nOption=sSerialUSB[0];
    }
    Serial.println(cResult);
    return cResult; */
 }
/* does not work

void IOCtrl::GetStatus2(char sStatus[50])
{

   //char *sStatus ;
// Define 
String str = "hello"; 
 
// Length (with one extra character for the null terminator)
int str_len = str.length() + 1; 
 
// Prepare the character array (the buffer) 
//char char_array[30];
 
// Copy it over 
str.toCharArray(sStatus, str_len);
//adds null character at the end
sStatus[str_len+1]=0;


} */ 

/*

void FlashMemory::ReadPage(int PageNumber, int *PageBuffer) {
    _enable->write(1);
    _enable->write(0);
  _spi->write(WB_READ_DATA);
  // Construct the 24-bit address from the 16-bit page
  // number and 0x00, since we will read 256 bytes (one
  // page).
 _spi->write((PageNumber >> 8) & 0xFF);
 _spi->write((PageNumber >> 0) & 0xFF);
 _spi->write(0);
  for (int i = 0; i < 256; ++i) {
    PageBuffer[i] =_spi->write(0);
  }
  _enable->write(1);
  NotBusy();
}

*/

    //void GetStatus();
    //void SetOutputTo(OutputNumber,State)();
    //void SetOutput(OutputNumber)();
    //void ClearOutput(OutputNumber)();
    //void GetInputs();
    //void ClearEvent();
    //void ClearOutputs();

