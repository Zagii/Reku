
#include <Arduino.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <TouchScreen.h>
#include <SPI.h>          // f.k. for Arduino-1.5.2
#include <SdFat.h>           // Use the SdFat library
#include <MCUFRIEND_kbv.h>
#include "CLcd.h"
CLcd lcd;



void setup(void)
{
   Serial.begin(9600);
   lcd.begin();
   lcd.show_Serial();
   pinMode(46, OUTPUT);
   analogWrite(46,50);
   attachInterrupt(digitalPinToInterrupt(19), tacho19, RISING );
}

volatile int tacho19cnt=0;
int poptacho=0;
unsigned long ta=0;
void tacho19()
{
  tacho19cnt++;
  
}
void loop()
{
  if(millis()-ta>1000)
  {
ta=millis();
Serial.println(tacho19cnt);
poptacho=tacho19cnt;
tacho19cnt=0;
    }

lcd.loop(poptacho);

//delay(1000);
}



