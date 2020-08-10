/*
  IOCtrl.h - Library to connect to aux arduino board and set/reset outputs and get inputs.
    Created by JVivas, August , 2020.
  Released into the public domain.
*/
#ifndef IOCtrl_h
#define IOCtrl_h


#include "Arduino.h"
//#include <HardwareSerial.h>
//HardwareSerial Serial_1(1);
//#define HardwareSerial Serial_1(1);
// Choose two free pins#

#define SERIAL1_RXPIN 2//12
#define SERIAL1_TXPIN 4//13

class IOCtrl
{
  public:
    
    IOCtrl(int pin);   
    const char* GetStatus();
    const char* GetInputs();
    const char* SetOutputTo(int OutputNumber, int State);
    const char* SetOutput(int OutputNumber);
    const char* ClearOutput(int OutputNumber); 
    const char* ClearEvent();
    const char* ClearOutputs();
    const char* CheckSerialAuxController();
    

  private:
    int _pin;

    //HardwareSerial(int uart_nr);
//    HardwareSerial Serial_1(1);
    String str = "Online,11111111,24.7,MON1030,SD_OK";
    String strb = "hello";
   
};

#endif
