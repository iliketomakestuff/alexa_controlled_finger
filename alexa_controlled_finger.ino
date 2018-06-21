/**********************************************************************************
* This example code is to support the project at https://www.iliketomakestuff.com/make-an-alexa-controlled-finger/
* project by Bob Clagett @ I Like To Make Stuff
* http://www.iliketomakestuff.com/

*This example code is built from the library and example code below and is provided WITH NO SUPPORT

* Home Automation with Alexa and NodeMCU
* WeMos smart devices emulation using FAUXMOESP Library
*
* Code based on the great open source lib & example code at: 
* http://tinkerman.cat/emulate-wemo-device-esp8266/
* which is based off of the Python example code by: 
* https://github.com/makermusings/fauxmo
* 
* Also, thanks to Sid for Sid's E Classroom
* https://www.youtube.com/c/SidsEClassroom
* 
* fauxmoESP is a library for ESP8266-based devices that emulates a Belkin WeMo device 
* and thus allows you to control them using this protocol, in particular from 
* Alexa-powered devices like the Amazon Echo or the Dot.
* 
*  To activate a device or a gropu of devices, you should use voice commands, like: 
*  "Computer (or Alexa), turn on Light1" or "..., turn off Light1"
*  "Computer (or Alexa), turn on Living Room" or "..., turn off Living Room"
*  "Computer (or Alexa), turn on All Devices" or "..., turn off All Devices"
*
*  Based on code developed by Marcelo Rovai on 19Aug17
*  Visit his blog: https://MJRoBot.org 
  *********************************************************************************/
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "fauxmoESP.h"
#include <Servo.h>

/* Network credentials */
#define WIFI_SSID "$$$$$$"
#define WIFI_PASS "$$$$$$"

#define SERIAL_BAUDRATE 115200  //this is important to set in the Arduino IDE
bool resetMe=false;
int beginVal=0;

/* Belkin WeMo emulation */
fauxmoESP fauxmo;
Servo myservo;  // create servo object to control a servo

void setup() 
{
   Serial.begin(SERIAL_BAUDRATE);
   //setup and wifi connection
   wifiSetup();
    pinMode(D4, OUTPUT);
    myservo.attach(2);
    beginVal = myservo.read();
   // Device Names for Simulated Wemo switches
   fauxmo.addDevice("finger");
   fauxmo.onMessage(callback); 
}

void loop() 
{
  if(resetMe){
    delay(1000);
    Serial.println("reset the servo");
    myservo.write(0);
    resetMe=false;
  }
  fauxmo.handle();
}

/* ---------------------------------------------------------------------------
 Device Callback
 ----------------------------------------------------------------------------*/
void callback(uint8_t device_id, const char * device_name, bool state) 
{
  Serial.print("Device "); Serial.print(device_name); 
  Serial.print(" state: ");
  if (state) 
  {
    Serial.println("ON");
  } 
  else 
  {
    Serial.println("OFF");
  }
  
  //Switching action on detection of device name, useful for adding
  //multiple "devices" to a single ESP unit.
  
  if ( (strcmp(device_name, "finger") == 0) ) 
  {
    if (state) 
    {
      int newVal = beginVal+15;
      myservo.write(newVal);
      resetMe=true;
      Serial.println("ok");  
    } else{
      myservo.write(beginVal);
    }
  
  }
  
}

void moveServo(int val){
  Serial.println("moveServo");
    //if you need to scale from a potentiometer to use it with the servo (value between 0 and 180)
    //val = map(val, 0, 1023, 0, 180);     
    myservo.write(val);
}

/* -----------------------------------------------------------------------------
 Wifi Setup
 -----------------------------------------------------------------------------*/
void wifiSetup() 
{
   // Set WIFI module to STA mode
   WiFi.mode(WIFI_STA);

   // Connect
   Serial.println ();
   Serial.printf("[WIFI] Connecting to %s ", WIFI_SSID);
   Serial.println();
   WiFi.begin(WIFI_SSID, WIFI_PASS);

   // Wait
   while (WiFi.status() != WL_CONNECTED) 
   {
      Serial.print(".");
      delay(100);
   }
   Serial.print(" ==> CONNECTED!" );
   Serial.println();

   // Connected!
   Serial.printf("[WIFI] STATION Mode, SSID: %s, IP address: %s\n", WiFi.SSID().c_str(), WiFi.localIP().toString().c_str());
   Serial.println();
}
