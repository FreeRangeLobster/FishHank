# Description
This embedded system is designed to control control devices of a fish tank such as lights, filter pumps, air pumps, CO2 reactors amoung others. The system is capable of controllng 8 channels 220V AC at max 2A. 

The system is also equiped with the following systems: SD card, Real time clock, Bluetooth connection, flash memory and temperature sensor. This devices are available to be connected to a IOT terminal, which could be a Raspberry pi or a ESP32 or a ST Nucleo.

Developing using platform IO

Arduino IDE 1.8.5

## PCB BOM
See parts schedule file ![Application](/Parts Schedule.ods)
Format: ![Alt Text](url)

## Production Files
Files submited to produce PCB

## PCB
Schematic and PCB files. Developed using Eagle

## Supervisory and config App


## Images
Schematic
![Application](/Images/Schematic.png)

PCB
![Application](/Images/PCB.png)
Format: ![Alt Text](url)

## Libraries
Libraries developed and downloaded for the pcb integration

RTC
GetTimeNumber
GetTimeString
SetTimeString

OLED
DrawTable
ShowStatus
ShowTemperature
ShowOutputs
ShowTime
InitialDisplayShow

SD
FindEvent(CurrentTime)
GetStatus
GetConfiguration
DumpEvents

INPUTOUTPUT CONTROLLER 
GetStatus
SetOutputTo(OutputNumber,State)
SetOutput(OutputNumber)
ClearOutput(OutputNumber)
GetInputs
ClearEvent
ClearOutputs

## Export
This folder contains 1:1 export views of the PCB

## Datasheets
Datasheets of the components used in the PCB

## Arduino Code
Developed to test the SSRs and User Inputs

# To Do is To Be 
|Item|Description|status|
| ------ | ------ | ------ |
|1|Populate and test the PSU, test for shrt circuit and protectionsn |DONE|
|2|Populate and test One channel the SSR system |DONE|
|3|Populate and test microcontroller |DONE|
|4|populate and test bluetooth DONE |DONE|
|5|sd card reader DONE |DONE|
|6|realtime clock DONE |DONE|
|7|Temperature DONE |DONE|
|8|Make APIs libraries to deal with peripherals |DONE|
|9|Develop Supervisory |InProgress|


# To Do is To Be Embedded system
| API | Function | Implemented | Tested Works | Doesnt work | Not Tested | Notes | 
| ------ | ------ | ------ | ------ | ------ | ------ | ----------------------------------- | 
|RTC|GetTimeNumber |  |  |  |  |  |
|RTC|GetTimeString |  |  |  |  |  |
|RTC|SetTimeString |  |  |  |  |  |
|OLED|DrawTable |  |  |  |  |  |
|OLED|ShowStatus  |  |  |  |  |  |
|OLED|ShowTemperature |  |  |  |  |  |
|OLED|ShowOutputs |  |  |  |  |  |
|OLED|ShowTime |  |  |  |  |  |
|OLED|InitialDisplayShow |  |  |  |  |  |
|SD|FindEvent(CurrentTime) |  |  |  |  |  |
|SD|GetStatus|  |  |  |  |  |
|SD|GetConfiguration|  |  |  |  |  |
|SD|DumpEvents|  |  |  |  |  |
|IO Ctrl|GetStatus|  |  |  |  |  |
|IO Ctrl|SetOutputTo(OutputNumber,State)|  |  |  |  |  |
|IO Ctrl|SetOutput(OutputNumber)|  |  |  |  |  |
|IO Ctrl|ClearOutput(OutputNumber)|  |  |  |  |  |
|IO Ctrl|ClearEvent|  |  |  |  |  |
|IO Ctrl|ClearOutputs|  |  |  |  |  |


# Debugging and Testing

-
---

### Cheatsheet
#### Git

Command | Description
--- | --- 
git fetch | gets the remote status of the repository
git status | display the local current status of the repository
git pull | updates changes in the local repository
git add . | updates (adds, removes) files and infor from local to remote repository
git commit -m "" | Commits the changes and allows for a message to be updated
git push origin master| Pushes changes to remote repository
git checkout --| Reverts the files to the last commited version
git reset --hard | UNDO local file changes and KEEP your last commit

special thanks to for the atmega library
https://github.com/njh/eagle-library
https://github.com/sparkfun/SparkFun-Eagle-Libraries




