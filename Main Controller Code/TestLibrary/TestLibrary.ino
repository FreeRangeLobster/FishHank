#include "Morse.h"
#include "IOCtrl.h"

//Morse morse(13);
IOCtrl ioCtrl(13);
char *foo;
char *str ;
char *str2 ;


void setup()
{
  Serial.begin(9600);
}


char *myFunction()
{
 return "returnedText"; // this returns a pointer to the string, not the string itself
}

void loop()
{
  //morse.dot(); morse.dot(); morse.dot();
  //morse.dash(); morse.dash(); morse.dash();
  //morse.dot(); morse.dot(); morse.dot();
  delay(1000);
  ioCtrl.dot();
   delay(1000);
  
   foo=ioCtrl.GetStatus();
   Serial.println(foo);

 
}
