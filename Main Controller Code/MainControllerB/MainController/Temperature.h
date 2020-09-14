/*
  IOCtrl.h - Library to connect to aux arduino board and set/reset outputs and get inputs.
    Created by JVivas, August , 2020.
  Released into the public domain.
*/
#ifndef Temperature_h
#define Temperature_h

#include "Arduino.h"

class Temperature
{
  public:
    Temperature(int pin);
    const char* GetTemperature();
    const char* SetSerial();


  private:
    int _pin;
    String str = "Online,11111111,24.7,MON1030,SD_OK";
    String strb = "hello";
};

#endif
