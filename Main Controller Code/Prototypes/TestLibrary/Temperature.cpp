/*
  Morse.cpp - Library for flashing Morse code.
  Created by David A. Mellis, November 2, 2007.
  Released into the public domain.
*/

#include "Arduino.h"
#include "Temperature.h"

Temperature::Temperature(int pin)
{
  pinMode(pin, OUTPUT);
  _pin = pin;
}




const char* Temperature::GetTemperature()
{
String  mystring="OK,11110000,0000000";
const char* cnv=mystring.c_str();
return cnv;
}


const char* Temperature::SetSerial()
{
const char* ssid = "GetInputs=OK;11001100";
return ssid;
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

