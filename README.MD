# Description
This embedded system is designed to control control devices used in a fish tank. Includes but is not limited to lights, filter pumps, air pumps, CO2 reactors, UV lights and others. The system is capable of controllng 8 channels 220V AC at max 2A. 

The system is also equiped with the following systems: SD card, Real time clock, Bluetooth connection, flash memory and temperature sensor. This devices are available to be connected to a IOT terminal, which could be a Raspberry pi or a ESP32 or a ST Nucleo. In this iteration it was selected an ESP32 WRoom 32.

## Weapons of choice
Embedded System ESP32: Visual Studio Code with platform IO
Aux IO Control syste : Visual Studio Code with platform IO
Supervisory system   : Visual Studio .net C# and WPF
PCB Development      : Eagle
Source Control and housekeeping: GitHub 
Quick prototyping    :Arduino IDE 1.6

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
CalculateFutureTimeCheck

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
|OLED|DrawTable |YES  |YES  |  |  |  |
|OLED|ShowStatus  |YES|YES  |  |  |  |
|OLED|ShowTemperature |  |  |  |  |  |
|OLED|ShowOutputs |  |  |  |  |  |
|OLED|ShowTime |YES|YES  |  |  |  |
|OLED|InitialDisplayShow |  |  |  |  |  |
|OLED|CalculateFutureTimeCheck |  |  |  |  |  |
|SD|FindEvent(CurrentTime) |  |  |  |  |  |
|SD|GetStatus|  |  |  |  |  |
|SD|GetConfiguration|  |  |  |  |  |
|SD|DumpEvents| YES |  |  |  |  |
|IO Ctrl|GetStatus| YES |  |  |  |  |
|IO Ctrl|SetOutputTo(OutputNumber,State)| YES |  |  |  |  |
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



#### Controller Pins
Top View 

 down conting from top - Right
GND    -> GND 
GPIO23 -> MOSI SD Card
GPIO22 -> I2C SCL Oled and RTC 
GPIO1  -> 
GPIO3  -> 
GPIO21 -> I2C SDA Oled and RTC
GND    -> 
GPIO19 -> MISO SD Card
GPIO18 -> CLK SD Card
GPIO5  -> Chip Select SD Card
GPIO17 -> Serial2 TX IO Controller 
GPIO16 -> Serial2 RX IO Controller
GPIO4  -> Serial2 RX Bluetooth
GPIO0  -> 
GPIO2  -> Serial2 TX Bluetooth
GPIO15 -> Temp 1 Wire
GPIO8  ->
GPIO7  ->
GPIO6  ->
3v3
USB Connection


down conting from top - Left
3v3    -> Level Shifter Power
EN     -> 
GPIO36 -> New Event IOCtrl
GPIO39 ->
GPIO34 ->
GPIO35 ->
GPIO32 ->
GPIO33 ->
GPIO25 ->
GPIO26 ->
GPIO27 -> 
GPIO14 ->
GPIO12 ->
GND    ->
GPIO13 ->
GPIO9  ->
GPIO10 ->
GPIO11 ->
VIN    -> 
USB Connection to PC
