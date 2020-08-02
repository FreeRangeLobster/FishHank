/*
  IOCtrl.h - Library to connect to aux arduino board and set/reset outputs and get inputs.
  Created by David A. Mellis, November 2, 2007.
  Released into the public domain.
*/
#ifndef IOCtrl_h
#define IOCtrl_h

#include "Arduino.h"

class IOCtrl
{
  public:
    IOCtrl(int pin);
    void dot();
    void dash();
    void dash2();
    
    const char* GetStatus();
    const char* GetInputs();
    const char* SetOutputTo(int OutputNumber, int State);
    const char* SetOutput(int OutputNumber);
    const char* ClearOutput(int OutputNumber); 
    const char* ClearEvent();
    const char* ClearOutputs();

  private:
    int _pin;
    String str = "Online,11111111,24.7,MON1030,SD_OK";
    String strb = "hello";
};

#endif
