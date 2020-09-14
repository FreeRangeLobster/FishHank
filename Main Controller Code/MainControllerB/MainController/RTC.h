/*
  IOCtrl.h - Library to connect to aux arduino board and set/reset outputs and get inputs.
  C  Created by JVivas, August , 2020.
  Released into the public domain.
*/
#ifndef RTC_h
#define RTC_h

#include "Arduino.h"

class RTC
{
  public:
    RTC(int pin);
    const char* GetTimeNumber();         
    const char* GetTimeString();         
    const char* SetTimeString();
    const char* ConvertTimeToString();
    const char* CalculateFutureCheckTime();
    
    
  private:
    int _pin;
    String str = "Online,11111111,24.7,MON1030,SD_OK";
    String strb = "hello";
};

#endif
