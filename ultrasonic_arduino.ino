#include <Adafruit_VS1053.h>
#include <SPI.h>
#include <SD.h>
#include <EEPROM.h>
#include <stdint.h>
#include "Maxbotix.h"
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

static const int RXPin = 2, TXPin = 3;
// DREQ should be an Int pin, see http://arduino.cc/en/Reference/attachInterrupt
#define DREQ 3       // VS1053 Data request, ideally an Interrupt pin. Not used
// These are common pins between breakout and shield
#define CARDCS 4     // Card chip select pin
// These are the pins used for the music maker shield
#define SHIELD_RESET  -1      // VS1053 reset pin (unused!)
#define SHIELD_CS     8      // VS1053 chip select pin (output)
#define SHIELD_DCS    6      // VS1053 Data/command select pin (output)

static const uint32_t GPSBaud = 4800;
byte offsetValue;
long inches, cms, actual;
char altstring[10], offsetstring[10]; // create string arrays
/** Declare functions **/
void serialOut();
char inchStr[4];

Adafruit_VS1053_FilePlayer musicPlayer = Adafruit_VS1053_FilePlayer(-1, 8, 6, 3, 4);

//SS for LCD
SoftwareSerial LCD(7,8); // pin 7 = TX, pin 6 = RX (unused)
//Set up Ultrasonic.
Maxbotix rangeSensorAD(A0, Maxbotix::AN, Maxbotix::HRLV);
// The TinyGPS++ object
TinyGPSPlus gps;
// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

void setup(){
  LCD.begin(9600);
  Serial.begin(9600);
  ss.begin(GPSBaud);
  SD.begin(CARDCS);    // initialise the SD card
  musicPlayer.setVolume(20,20);
  delay(500);

// If DREQ is on an interrupt pin (on uno, #2 or #3) we can do background
  // audio playing
  musicPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT);  // DREQ int
  
  // Set Sensor Offset. Get initial height off the ground in inches and write to permanent memory.
  cms = rangeSensorAD.getRange();
  inches = rangeSensorAD.toInches(cms);
  EEPROM.write(1, inches);
  // Now grab the value we just wrote and use it as the offset in height calcs
  offsetValue = EEPROM.read(1);
  LCD.print(254); // move cursor to beginning of first line
  LCD.print(128);
  LCD.print("         "); // clear display + legends
  LCD.print("         ");
  LCD.print("ACT INS "); // clear display + legends
  LCD.print("OFFSET  ");

}

//After setup, this function just loops over and over.
void loop() {
  while (ss.available() > 0)
    if (gps.encode(ss.read()))
      displayInfo();
  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
    while(true);
  }
  /** Main ultrasonic ping function and then convert to actual distance **/
  cms = rangeSensorAD.getRange();
  inches = rangeSensorAD.toInches(cms);
  actual = inches - offsetValue;
  

 if((actual <= 246) && (actual > 234)) {
    musicPlayer.playFullFile("2feet.wav");
    }
  else if((actual <= 126) && (actual > 114)) {
  musicPlayer.playFullFile("2feet.wav");
    }
  else if((actual <= 86) && (actual > 54)) {
   musicPlayer.playFullFile("2feet.wav");
    }
  else if((actual <= 42) && (actual > 30)) {
   musicPlayer.playFullFile("2feet.wav");
    }
  else if((actual <= 30) && (actual > 18)) {
   musicPlayer.playFullFile("2feet.wav");
    }
  else if((actual <= 18) && (actual > 6)) {
 musicPlayer.playFullFile("2feet.wav");
    }
  //sendtoLCD();
  serialOut();
  delay(1000);
} //loop

