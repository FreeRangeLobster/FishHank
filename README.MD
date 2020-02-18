
# Description
This embedded system is designed to control control devices of a fish tank such as lights, filter pumps, air pumps, CO2 reactors amoung others. The system is capable of controllng 8 channels 220V AC at max 2A. 

The system is also equiped with the following systems: SD card, Real time clock, Bluetooth connection, flash memory and temperature sensor. This devices are available to be connected to a IOT terminal, which could be a Raspberry pi or a ESP32 or a ST Nucleo.

## PCB BOM
See parts schedule file ![Application](/Parts Schedule.ods)
Format: ![Alt Text](url)

## Production Files
Files submited to produce PCB

## PCB
Schematic and PCB files. Developed using Eagle

## Images
Schematic
![Application](/Images/Schematic.png)

PCB
![Application](/Images/PCB.png)
Format: ![Alt Text](url)

## Libraries
Libraries developed and downloaded for the pcb integration

## Export
This folder contains 1:1 export views of the PCB

## Datasheets
Datasheets of the components used in the PCB

## Arduino Code
Developed to test the SSRs and User Inputs







# To Do is To Be 
ItemNo|Description|status
1|Populate PCB and test|status

# Debugging and Testing
- Populate and test the PSU, test for shrt circuit and protections
- Populate and test One channel the SSR system
- Populate and test microcontroller
- populate and test bluetooth
- sd card reader
- realtime clock
- Temperature
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

special thanks to for the atmega library
https://github.com/njh/eagle-library
https://github.com/sparkfun/SparkFun-Eagle-Libraries
