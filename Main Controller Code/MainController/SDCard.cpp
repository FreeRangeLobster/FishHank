/*

*/

#include "Arduino.h"
#include "SDCard.h"

SDCard::SDCard(int pin)
{
  pinMode(pin, OUTPUT);
  _pin = pin;
}


    const char* SDCard::FindEvent(){}         
    const char* SDCard::GetStatus(){}         
    const char* SDCard::GetConfiguration(){}
    const char* SDCard::DumpEvents(){}



/*
const char* RTC::GetStatus()
{
String  mystring="OK,11110000,0000000";
const char* cnv=mystring.c_str();
return cnv;
}


const char* RTC::GetInputs()
{
const char* ssid = "GetInputs=OK;11001100";
return ssid;
}

const char* RTC::SetOutputTo(int OutputNumber,int State){
  String  Result= "SetOutputTo=OK;" + String(OutputNumber) + String(State);
  const char* cResult=Result.c_str();
  return cResult;
}

const char* RTC::SetOutput(int OutputNumber){
   String  Result= "SetOutput=OK;" + String(OutputNumber);
   const char* cResult=Result.c_str();
   return cResult;
}

const char* RTC::ClearOutput(int OutputNumber){
 String  Result= "ClearOutput=OK;" + String(OutputNumber);
 const char* cResult=Result.c_str();
 return cResult;
}

const char* RTC::ClearEvent(){
   String  Result= "ClearEvent=OK;";
  const char* cResult=Result.c_str();
  return cResult;
  }


const char* RTC::ClearOutputs(){
  String  Result= "ClearOutputs=OK;";
  const char* cResult=Result.c_str();
  return cResult;
}


*/
