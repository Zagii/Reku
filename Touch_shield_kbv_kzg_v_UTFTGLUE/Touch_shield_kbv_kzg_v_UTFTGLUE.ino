
#include <Arduino.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <TouchScreen.h>
#include <SPI.h>          // f.k. for Arduino-1.5.2
#include <SdFat.h>           // Use the SdFat library
#include <MCUFRIEND_kbv.h>
#include "CLcd.h"
CLcd lcd;

#define PIN_WIATRAK_CZERPNIA 46
#define PIN_TACHO_WIATRAK_CZERPNIA 19
#define PIN_WIATRAK_WYWIEW 45
#define PIN_TACHO_WIATRAK_WYWIEW 20

volatile int tachoWiatrakCzerpniaIle=0;
uint8_t tachoWiatrakCzerpniaIle_pop=0;
unsigned long czas=0;

void setup(void)
{
   Serial.begin(9600);
   lcd.begin();
   lcd.show_Serial();
   pinMode(PIN_WIATRAK_CZERPNIA, OUTPUT);
   
   attachInterrupt(digitalPinToInterrupt(PIN_TACHO_WIATRAK_CZERPNIA), tachoWiatrakCzerpnia, RISING );
   attachInterrupt(digitalPinToInterrupt(PIN_TACHO_WIATRAK_WYWIEW), tachoWiatrakWywiew, RISING );
}


void tachoWiatrakCzerpnia()
{
  tachoWiatrakCzerpniaIle++;  
}
volatile int tachoWiatrakWywiewIle=0;
uint8_t tachoWiatrakWywiewIle_pop=0;

void tachoWiatrakWywiew()
{
  tachoWiatrakWywiewIle++;  
}
void loop()
{
	if(millis()-czas>1000)
	{
		czas=millis();
		Serial.println(tachoWiatrakCzerpniaIle);
		tachoWiatrakCzerpniaIle_pop=tachoWiatrakCzerpniaIle;
		tachoWiatrakCzerpniaIle=0;
		tachoWiatrakCzerpniaIle_pop=tachoWiatrakCzerpniaIle;
		tachoWiatrakCzerpniaIle=0;
	}
	lcd.loop(tachoWiatrakCzerpniaIle_pop);

}



