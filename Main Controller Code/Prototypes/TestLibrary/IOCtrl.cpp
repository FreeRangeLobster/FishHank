/*
  Morse.cpp - Library for flashing Morse code.
  Created by David A. Mellis, November 2, 2007.
  Released into the public domain.
*/

#include "Arduino.h"
#include "IOCtrl.h"

IOCtrl::IOCtrl(int pin)
{
  pinMode(pin, OUTPUT);
  _pin = pin;
}

void IOCtrl::dot()
{
  digitalWrite(_pin, HIGH);
  delay(250);
  digitalWrite(_pin, LOW);
  delay(250);  
}

void IOCtrl::dash()
{
  digitalWrite(_pin, HIGH);
  delay(1000);
  digitalWrite(_pin, LOW);
  delay(250);
}

void IOCtrl::dash2()
{
  digitalWrite(_pin, HIGH);
  delay(1000);
  digitalWrite(_pin, LOW);
  delay(250);
}


const char* IOCtrl::GetStatus()
{
String  mystring="OK,11110000,0000000";
const char* cnv=mystring.c_str();
return cnv;
}


const char* IOCtrl::GetInputs()
{
const char* ssid = "GetInputs=OK;11001100";
return ssid;
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

