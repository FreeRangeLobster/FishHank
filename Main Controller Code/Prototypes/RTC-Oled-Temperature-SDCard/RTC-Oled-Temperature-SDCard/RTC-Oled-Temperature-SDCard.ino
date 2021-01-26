/*********
  Complete project details at https://randomnerdtutorials.com
  
  This is an example for our Monochrome OLEDs based on SSD1306 drivers. Pick one up today in the adafruit shop! ------> http://www.adafruit.com/category/63_98
  This example is for a 128x32 pixel display using I2C to communicate 3 pins are required to interface (two I2C and one reset).
  Adafruit invests time and resources providing this open source code, please support Adafruit and open-source hardware by purchasing products from Adafruit!
  Written by Limor Fried/Ladyada for Adafruit Industries, with contributions from the open source community. BSD license, check license.txt for more information All text above, and the splash screen below must be included in any redistribution. 
  JV integrated OLED, RTC and Temperature 
*********/


//RTC
#include <RTClib.h>
RTC_DS3231 rtc;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
String sHour;
String sMinute;
String sSecond;
String stringOne;

//Temperature sensor
#include <OneWire.h>
#include <DallasTemperature.h>
// Data wire is connected to GPIO15
#define ONE_WIRE_BUS 15
// Setup a oneWire instance to communicate with a OneWire device
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);
DeviceAddress sensor1 = { 0x28, 0xFF, 0x12, 0xA2, 0x61, 0x15, 0x2, 0xB8 };

//Display
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define NUMFLAKES     10 // Number of snowflakes in the animation example

#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16
static const unsigned char PROGMEM logo_bmp[] =
{ B00000000, B11000000,
  B00000001, B11000000,
  B00000001, B11000000,
  B00000011, B11100000,
  B11110011, B11100000,
  B11111110, B11111000,
  B01111110, B11111111,
  B00110011, B10011111,
  B00011111, B11111100,
  B00001101, B01110000,
  B00011011, B10100000,
  B00111111, B11100000,
  B00111111, B11110000,
  B01111100, B11110000,
  B01110000, B01110000,
  B00000000, B00110000 };

  // Libraries for SD card
#include "FS.h"
#include "SD.h"
#define SD_CS 5
// Save reading number on RTC memory
RTC_DATA_ATTR int readingID = 0;
String dataMessage;
// Variables to save date and time
String formattedDate;
String dayStamp;
String timeStamp;



void setup() {
//Start serial 
  Serial.begin(115200);
  delay(3000); // wait for console opening
  
//RTC
    if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
  if (rtc.lostPower()) {
    Serial.println("RTC lost power, lets set the time!");
    // following line sets the RTC to the date &amp; time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date &amp; time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }

//Temperature Sensor
  sensors.begin();

//Display
 // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.setTextColor(WHITE);
  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(1000);

  // Clear the buffer
  display.clearDisplay();

  delay(1000);

//SD Card
// Initialize SD card
  SD.begin(SD_CS);  
  if(!SD.begin(SD_CS)) {
    Serial.println("Card Mount Failed");
    return;
  }
  uint8_t cardType = SD.cardType();
  if(cardType == CARD_NONE) {
    Serial.println("No SD card attached");
    return;
  }
  Serial.println("Initializing SD card...");
  if (!SD.begin(SD_CS)) {
    Serial.println("ERROR - SD card initialization failed!");
    return;    // init failed
  }

  // If the data.txt file doesn't exist
  // Create a file on the SD card and write the data labels
  File file = SD.open("/data.txt");
  if(!file) {
    Serial.println("File doens't exist");
    Serial.println("Creating file...");
    writeFile(SD, "/data.txt", "Reading ID, Date, Hour, Temperature \r\n");
  }
  else {
    Serial.println("File already exists");  
  }
  file.close();

}




  
  

 

void loop() {

    DateTime now = rtc.now();

    //Time        
    //Hour
    sHour=now.hour();
    //Minute
    sMinute=now.minute();
    //Second
    sSecond=now.second();    
    stringOne =  String(sHour + ":" + sMinute + ":" + sSecond);
    Serial.println(stringOne);
 
    display.clearDisplay();
    display.setTextSize(2);                         // set these parameters according to your need..
    display.setCursor(0, 0);    
    display.println(stringOne);    
    delay(10);

    //Temperature
    sensors.requestTemperatures(); // Send the command to get temperatures  
    display.setTextSize(2);                         // set these parameters according to your need..
    display.setCursor(0, 20);    
    stringOne = String(sensors.getTempC(sensor1));
    stringOne =stringOne + " C";
    display.println(stringOne);                 
    
    display.display();
    delay(1000);

  
}



void testdrawrect(void) {
  display.clearDisplay();

  for(int16_t i=0; i<display.height()/2; i+=2) {
    display.drawRect(i, i, display.width()-2*i, display.height()-2*i, WHITE);
    display.display(); // Update screen with each newly-drawn rectangle
    delay(1);
  }

  delay(2000);
}



void testdrawchar(void) {
  display.clearDisplay();

  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
  display.cp437(true);         // Use full 256 char 'Code Page 437' font

  // Not all the characters will fit on the display. This is normal.
  // Library will draw what it can and the rest will be clipped.
  for(int16_t i=0; i<256; i++) {
    if(i == '\n') display.write(' ');
    else          display.write(i);
  }

  display.display();
  delay(2000);
}

// Write to the SD card (DON'T MODIFY THIS FUNCTION)
void writeFile(fs::FS &fs, const char * path, const char * message) {
  Serial.printf("Writing file: %s\n", path);

  File file = fs.open(path, FILE_WRITE);
  if(!file) {
    Serial.println("Failed to open file for writing");
    return;
  }
  if(file.print(message)) {
    Serial.println("File written");
  } else {
    Serial.println("Write failed");
  }
  file.close();
}

// Append data to the SD card (DON'T MODIFY THIS FUNCTION)
void appendFile(fs::FS &fs, const char * path, const char * message) {
  Serial.printf("Appending to file: %s\n", path);

  File file = fs.open(path, FILE_APPEND);
  if(!file) {
    Serial.println("Failed to open file for appending");
    return;
  }
  if(file.print(message)) {
    Serial.println("Message appended");
  } else {
    Serial.println("Append failed");
  }
  file.close();
}
