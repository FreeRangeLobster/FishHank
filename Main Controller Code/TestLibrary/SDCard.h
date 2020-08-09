/*
  IOCtrl.h - Library to connect to aux arduino board and set/reset outputs and get inputs.
  Created by JVivas, August , 2020.
  Released into the public domain.
*/
#ifndef SDCard_h
#define SDCard_h

#include "Arduino.h"

class SDCard
{
  public:
    SDCard(int pin);
    const char* FindEvent();      
    const char* GetStatus();         
    const char* GetConfiguration();
    const char* DumpEvents();
    
  private:
    int _pin;
    String str = "Online,11111111,24.7,MON1030,SD_OK";
    String strb = "hello";
};

#endif
