#include "Morse.h"

Morse morse(13);
Morse IOCtrl(13);

void setup()
{
}

void loop()
{
  //morse.dot(); morse.dot(); morse.dot();
  //morse.dash(); morse.dash(); morse.dash();
  //morse.dot(); morse.dot(); morse.dot();
  delay(1000);
  IOCtrl.dot();
   delay(1000);
}
