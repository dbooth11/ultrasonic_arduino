#include <Adafruit_VS1053.h>
#include <SPI.h>
#include <SD.h>
#include <EEPROM.h>
#include <stdint.h>
#include "Maxbotix.h"
#include <SoftwareSerial.h>


int upState = 0;
int downState = 0;
int volume = 20;
byte offsetValue;
long inches, cms, actual;
/** Declare functions **/
void serialOut();
char inchStr[4];
int sensor_status = 0; //to to test ready state
//Set up Ultrasonic.
Maxbotix rangeSensorAD(A0, Maxbotix::AN, Maxbotix::HRLV);
// Pins for Sound card
#define SHIELD_RESET  -1      // VS1053 reset pin (unused!)
#define SHIELD_CS     7      // VS1053 chip select pin (output)
#define SHIELD_DCS    6      // VS1053 Data/command select pin (output)
#define CARDCS 4    
#define DREQ 3       // VS1053 Data request, ideally an Interrupt pin

Adafruit_VS1053_FilePlayer musicPlayer = 
  Adafruit_VS1053_FilePlayer(SHIELD_RESET, SHIELD_CS, SHIELD_DCS, DREQ, CARDCS);

void setup(){
  Serial.begin(19200);
  SD.begin(CARDCS);    // initialise the SD card
  musicPlayer.setVolume(volume,volume);
  delay(500);
  
  pinMode(A3, INPUT_PULLUP); // sets analog pin for volume up 
  pinMode(A2, INPUT_PULLUP); // sets analog pin for volume down 
// initialise the music player
  if (! musicPlayer.begin()) { // initialise the music player
     Serial.println(F("Couldn't find sound board, do you have the right pins defined?"));
     while (1);
  }
  Serial.println(F("Sound board found"));
  
  sensor_status = sensor_status + SD.begin(CARDCS);    // initialise the SD card
  // Set volume for left, right channels. lower numbers == louder volume!
  musicPlayer.setVolume(20,20);
  
  // Set Sensor Offset. Get initial height off the ground in inches and write to permanent memory.
  cms = rangeSensorAD.getRange();
  inches = rangeSensorAD.toInches(cms);
  if(cms > 10){
    sensor_status = ++sensor_status;
    }
  //Sensor reads 195 when pointed into the open air. Check existing offset. If less than 80 inches, write a new offset.
  if(inches < 80){
    EEPROM.write(1, inches);
  } 
  // Now grab the value we just wrote and use it as the offset in height calcs
  offsetValue = EEPROM.read(1);
  //Ready to go. Let them know it's working.
  if(sensor_status = 2){
   musicPlayer.playFullFile("ready.mp3");
   Serial.print("ready");
   Serial.print(sensor_status);
  }
}


void loop() {
  upState = digitalRead(A3);
  downState = digitalRead(A2);
  
  adjustVolume();
  /** Main ultrasonic ping function and then convert to actual distance **/
  cms = rangeSensorAD.getRange();
  inches = rangeSensorAD.toInches(cms);
  actual = inches - offsetValue;
  
Serial.println(upState);
Serial.println(downState);
 if((actual <= 246) && (actual > 234)) {
    musicPlayer.playFullFile("20feet.wav");
    }
  else if((actual <= 126) && (actual > 114)) {
  musicPlayer.playFullFile("10feet.wav");
    }
  else if((actual <= 66) && (actual > 54)) {
   musicPlayer.playFullFile("5feet.wav");
    }
  else if((actual <= 42) && (actual > 30)) {
   musicPlayer.playFullFile("3feet.wav");
    }
  else if((actual <= 30) && (actual > 18)) {
   musicPlayer.playFullFile("2feet.wav");
    }
  else if((actual <= 18) && (actual > 6)) {
   musicPlayer.playFullFile("hold.wav");
    }
  serialOut();
  delay(500);
} //loop

void adjustVolume(){
  //volume down
  if(downState == 0){
     volume = volume + 4;
     musicPlayer.setVolume(volume,volume);
     Serial.print("Volume Down");
     Serial.println(volume);
    }
    //Volume up
    if(upState == 0){
       volume = volume - 4;
       musicPlayer.setVolume(volume,volume);
       Serial.print ("Volume Up");
       Serial.println(volume);
    }
  
  }
  
   // Write out data to serial monitor
void serialOut() {
    Serial.print("Raw height: ");
    Serial.print(inches);
    Serial.print("   ");
  
    Serial.print("Offset Value: ");
    Serial.print(offsetValue);
    Serial.print("   ");
//Take inches value and subtract the offset to get height over ground.
    Serial.print("Calc height: ");
    Serial.print(inches - offsetValue);
    Serial.println();
}
