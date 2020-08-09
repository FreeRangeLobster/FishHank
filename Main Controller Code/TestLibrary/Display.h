/*
  IOCtrl.h - Library to connect to aux arduino board and set/reset outputs and get inputs.
    Created by JVivas, August , 2020.
  Released into the public domain.
*/
#ifndef Display_h
#define Display_h

#include "Arduino.h"

class Display
{
  public:
    Display(int pin);
    void DrawTable();         
    void ShowStatus(char sStatus[10]);         
    const char* ShowTemperature();
    const char* ShowOutputs();
    const char* ShowTime();
    const char* InitialDisplayShow();
    
  private:
    int _pin;
    String str = "Online,11111111,24.7,MON1030,SD_OK";
    String strb = "hello";
};

#endif
