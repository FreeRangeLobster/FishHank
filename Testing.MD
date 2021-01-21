# Testing Specification

## RTC
PASS Get time using USB port 
PASS Set Time using USB port
PASS Get time using Bluetooth port 
PASS Set Time using Bluetooth port

## Display
PASS Change temperature and check display
PASS Change Date using usb port check display
PASS Change Date using bluetooth port check display
PASS Change Output State using usb port, check all 8 outputs
PASS Change Output State using bluetooth, check all 8 outputs
PASS Change output state using inputs from Arduino, check all 8 outputs


## Temperature
Not Implemented Read Temperature using usb port
Not Implemented Read Temperature using bluetooth port
PASS Read Temperature using Display

## Bluetooth
PASS Connect and and provide same menu functionality(Parameters speed:9600, Parity none, 1 stop bit)

## Inputs
PASS Toggle every single output on/off
Check the display reflects the state of the output

## Outputs
PASS Turn ON/OFF all outputs using USB port
PASS Turn ON/OFF all outputs using Bluetooth Port
PASS Turn ON/OFF all outputs using Inputs

## SD Card
PASS Dump SD card schedule contents using USB Port
PASS Dump SD Card schedule contents using Bluetooth port
Check Events manually using USB Port
Check Events manually using Bluetooth Port

## System Running
Run system for a full day as follows:

Setup schedule to turn output x on/off for 1h every minute. 
Connect load and check is working. Repeat the same procedure to all outputs. Using supervisory system log all events

Setup schedule to turn on a load for 12hours, check components are not hot and the output is working according to schedule
Using supervisory system log all events



    
    

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
