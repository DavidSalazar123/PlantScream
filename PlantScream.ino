#include "DFRobotDFPlayerMini.h"
#include "SoftwareSerial.h"

#define ACTIVATED 0
#define DEACTIVATED 1
const short DRY = 508; // value for Dry
const short WET = 250; //Value for Wet *Needs to be changed
static const uint8_t RX = 3; //Speaker input RX
static const uint8_t TX = 2; //Speaker input TX

short percentage; //Percentage of how wet it is
bool isPlaying; //Is music playing or not
bool isWatered; //Is the plant being watered

const byte PIN = A0; //Moisture Stick
const byte SWITCH = 4; //Input for SWITCH
byte SWITCHstate = 1; //Sets switch to off (0 = on ; 1 = off

SoftwareSerial softwareSerial(RX,TX);
DFRobotDFPlayerMini player; //Constructor for DFPlayerMini

void setup() 
{
  Serial.begin(9600);
  softwareSerial.begin(9600);

  //Test to see if Speaker works
  if(player.begin(softwareSerial))
  {
    Serial.println("It is working");
  }
  else
  {
    Serial.println("It is not working");
  }
  
  pinMode(PIN, INPUT); //Moisture Stick
  
  pinMode(SWITCH, INPUT); //Take SWITCH as input
  digitalWrite(SWITCH, HIGH); //Set the SWITCH to high

  player.volume(27); //Volume for player (0-30)
}

void loop() 
{
  int sensorInput = analogRead(PIN);
  SWITCHstate = digitalRead(SWITCH);

/*
 * Shows how "wet" a plant is in percentage
 */
  percentage = map(sensorInput, WET, DRY, 100, 0); //Map converts numbers to a percentage
  //Serial.println(percentage);
  delay(500);

  if(SWITCHstate == ACTIVATED && isPlaying == false)
  {
    Serial.println("SWITCH is on");
    
    if(percentage <= 10 && isPlaying == false)
    {
      Serial.println("We in this < 10");
      isPlaying = true;
      isWatered = false;
      player.loop(1);
    }
    
  }
  else if(SWITCHstate == ACTIVATED && isWatered == false)
  {
    if (percentage > 10 && percentage < 90)
    {
      Serial.println("We in this > 10 && < 90");
      isWatered = true;
      player.loop(2);
    }
  }
  else if(SWITCHstate == ACTIVATED && isPlaying == true)
  {
    if(percentage >= 90)
    {
      Serial.println("We in this > 90");
      isPlaying = false;
      player.play(3);
      delay(4000);
      player.stop();
    }
  }
  else if(SWITCHstate == DEACTIVATED && isPlaying == true)
  {
      player.stop();  
      isPlaying = false;
  }

}
