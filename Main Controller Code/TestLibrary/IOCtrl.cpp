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


char* IOCtrl::GetStatus()
{

   //char *sStatus ;
// Define 
String str = "this is my string"; 
 
// Length (with one extra character for the null terminator)
int str_len = str.length() + 1; 
 
// Prepare the character array (the buffer) 
char char_array[20];
 
// Copy it over 
str.toCharArray(char_array, str_len);
//adds null character at the end
char_array[str_len+1]=0;

return char_array;
}



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

