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
  private:
    int _pin;
};

#endif
