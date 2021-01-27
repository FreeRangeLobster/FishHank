# Description
This embedded system is designed to control control devices used in a fish tank. Includes but is not limited to lights, filter pumps, air pumps, CO2 reactors, UV lights and others. The system is capable of controllng 8 channels 220V AC at max 2A. 

The system is also equiped with the following systems: SD card, Real time clock, Bluetooth connection, flash memory and temperature sensor. This devices are available to be connected to a IOT terminal, which could be a Raspberry pi or a ESP32 or a ST Nucleo. In this iteration it was selected an ESP32 WRoom 32.

## Weapons of choice
Embedded System ESP32: Visual Studio Code with platform IO
Aux IO Control syste : Visual Studio Code with platform IO
Supervisory system   : Visual Studio .net C# and WPF
Supervisory system   : Mosquitto and NodeRed- Running on Raspberry pi(Experimetal)
PCB Development      : Eagle
Source Control and housekeeping: GitHub 
Fast prototyping    :Arduino IDE 1.6

## PCB BOM
See parts schedule file ![Application](/Parts Schedule.ods)
Format: ![Alt Text](url)

## Production Files
Files submited to produce PCB

## PCB
Schematic and PCB files. Developed using Eagle

## Supervisory and config App
Developed using c#

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
|Status|ID|Description|
| ------ | ------ | ------ |
|DONE|1|Populate and test the PSU, test for shrt circuit and protectionsn |
|DONE|2|Populate and test One channel the SSR system |DONE|
|DONE|3|Populate and test microcontroller |
|DONE|4|populate and test bluetooth DONE |
|DONE|5|sd card reader DONE |
|DONE|6|realtime clock DONE |
|DONE|7|Temperature DONE |
|DONE|8|Make APIs libraries to deal with peripherals |
|DONE|9|add ALL days event |
|DONE|10|Tidy up oled screen, clear display square before writing output |
|DONE|11|test bluetooth debugging messages|
|DONE|12|mod the arduino auc board to update and toggle output|
|NA|13|add Auto/Manual switch to the controller|
||14|Apply loads and test outputs|
|DONE|15|Rearrange outputs in arduino aux|
|DONE|16|Read IOAux output state|
|DONE|17|Test using Bluetooth|
|DONE|18|Test Application|
||19|Add the leds to the NodeRed|
||20|Top acrilic cover to test mains|
||21|Modify and update PCB layout and components-See notes in logbook|
|DONE|22|Tidy up code, files and merge to master|
|DONE|24|Double check and record GPIOs schedule ESP32-WROOM| 
||25||
||26||
||27||
||28||
||29||
||30||
||31||
||32||
||33||
||34||
||35||


# To Do is To Be Embedded system- Code implementation
| API | Function | Implemented | Tested Works | Doesnt work | Not Tested | Notes | 
| ------ | ------ | ------ | ------ | ------ | ------ | ----------------------------------- | 
|RTC  |GetTimeNumber 	| YES |YES|  |  |  |
|RTC  |GetTimeString 	| YES |YES|  |  |  |
|RTC  |SetTimeString 	| YES |YES|  |  |  |
|OLED |DrawTable     	|YES  |YES  |  |  |  |
|OLED |ShowStatus  		|YES|YES|  |  |  |
|OLED |ShowTemperature 	| YES |YES|  |  |  |
|OLED |ShowOutputs 		|YES  |YES|  |  |  |
|OLED |ShowTime 		|YES|YES |  |  |  |
|OLED |InitialDisplayShow | YES |YES |  |  |  |
|OLED |CalculateFutureTimeCheck | YES |YES  |  |  |  |
|SD   |FindEvent(CurrentTime) |YES|YES  |  |  |  |
|SD   |GetStatus		| YES |YES  |  |  |  |
|SD   |GetConfiguration	|  YES|YES  |  |  |  |
|SD   |DumpEvents		| YES 	| YES|  |  |  |
|IO Ctrl|GetStatus		| YES | YES |  |  |  |
|IO Ctrl|SetOutputTo(OutputNumber,State)| YES |YES  |  |  |  |
|IO Ctrl|ClearInputEvent| NA |  |  |  | NA as IOCtrl updates automatic   |
|IO Ctrl|ClearOutputs| NA|  |  |  | Covered in Set Output To |


# Commands
? Help
K Initialise
I Idle
N Update Screen
M Check Events
O Update Outputs
D Delete Memory
0 Read Page Number 0_0000
1 AddEvent  1_1030WED31
2 Disable Event  2_001
3 Trace ON
4 Update Output 4_11 Output 1 ON
5 Write Status on OLED 5_Hello
6 Set Time RTC 6_DDMMY_HHmmss
7 Read Time
8 Check Current Event
9 Trace OFF


# PCB Upgrades and Improvements
|Status|ID|Description|
| ------ | ------ | ------ |
||1|Remove JLCJLCJCL, manufacturer disregarded the notes |
||2|Git webpage is not legible, find a better place|
||3|Bluetooth pinout is back to front in the eagle pcb library|
||4|Transistor not BCE, evaluate to use another one- Wrong footprint|
||5|SSR Output changed to 150ohm for a 10K resistor, so far is working fine|
||6|Breakout connector pin 1 and 2 to be renamed. INT1-> 5V, INT2->3V|
||7|Footprint of SSR to be adjusted|
||8|Add level converters, BSS138|
||9|Add 2 additional LEDs to show stay alive signal|
||10|Change output numbers in PCB to make it more readable|
||11|Add ESP32-WRoom to the PCB|
||12||
||13||
  
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
 
git branch alphr | Creates a neew branch
git branch --list | List current branches
git checkout alphr | switch local head to printer
git branch | shows current branch
git push --set-upstream origin alphr| creates new branch and pushes to remote repository
git merge alphr| merges brench. first it is required to go to master by checkout master


special thanks to for the atmega library
https://github.com/njh/eagle-library
https://github.com/sparkfun/SparkFun-Eagle-Libraries



#### Controller Pins
Top View 

 Down conting from top - Right
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
