
#include <Arduino.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <TouchScreen.h>
#include <SPI.h>          // f.k. for Arduino-1.5.2
#include <SdFat.h>           // Use the SdFat library
#include <MCUFRIEND_kbv.h>
#include "CLcd.h"
#include "CWiatrak.h"
#include "CKomora.h"

#define WIATRAK_IN 0
#define WIATRAK_OUT 1
#define PIN_WIATRAK_CZERPNIA 46
#define PIN_TACHO_WIATRAK_CZERPNIA 19
#define PIN_WIATRAK_WYWIEW 45
#define PIN_TACHO_WIATRAK_WYWIEW 20

#define KOMORA_CZERPNIA 0
#define KOMORA_WYRZUTNIA 1
#define KOMORA_NAWIEW 2
#define KOMORA_WYWIEW 3
#define KOMORA_ZEWN 4
#define KOMORA_SZT 4
#define WIATRAKI_SZT 2

CLcd lcd;
//CWiatrak wiatrakIN(PIN_WIATRAK_CZERPNIA,PIN_TACHO_WIATRAK_CZERPNIA);
//CWiatrak wiatrakOUT(PIN_WIATRAK_WYWIEW,PIN_TACHO_WIATRAK_WYWIEW);
CKomora komory[KOMORA_SZT];
CWiatrak wiatraki[WIATRAKI_SZT]=
{
	CWiatrak(PIN_WIATRAK_CZERPNIA,PIN_TACHO_WIATRAK_CZERPNIA),
	CWiatrak(PIN_WIATRAK_WYWIEW,PIN_TACHO_WIATRAK_WYWIEW)
};



//volatile int tachoWiatrakCzerpniaIle=0;
//uint8_t tachoWiatrakCzerpniaIle_pop=0;
//unsigned long czas=0;

void setup(void)
{
   Serial.begin(9600);
   lcd.begin();
   lcd.show_Serial();
   
   for(uint8_t i=0;i<KOMORA_SZT;i++)
   {
	   komory[i]=CKomora();
	   komory[i].begin();
   }
   wiatraki[WIATRAK_IN].begin();
   wiatraki[WIATRAK_OUT].begin();
   
   
   
//   pinMode(PIN_WIATRAK_CZERPNIA, OUTPUT);
   
 //  attachInterrupt(digitalPinToInterrupt(PIN_TACHO_WIATRAK_CZERPNIA), tachoWiatrakCzerpnia, RISING );
 //  attachInterrupt(digitalPinToInterrupt(PIN_TACHO_WIATRAK_WYWIEW), tachoWiatrakWywiew, RISING );
}

/*
void tachoWiatrakCzerpnia()
{
  tachoWiatrakCzerpniaIle++;  
}
volatile int tachoWiatrakWywiewIle=0;
uint8_t tachoWiatrakWywiewIle_pop=0;

void tachoWiatrakWywiew()
{
  tachoWiatrakWywiewIle++;  
}*/
void loop()
{/*
	if(millis()-czas>1000)
	{
		czas=millis();
		Serial.println(tachoWiatrakCzerpniaIle);
		tachoWiatrakCzerpniaIle_pop=tachoWiatrakCzerpniaIle;
		tachoWiatrakCzerpniaIle=0;
		tachoWiatrakCzerpniaIle_pop=tachoWiatrakCzerpniaIle;
		tachoWiatrakCzerpniaIle=0;
	}*/
	for(uint8_t i=0;i<KOMORA_SZT;i++)
    {
		komory[i].loop();
    }
	wiatraki[WIATRAK_IN].loop();
    wiatraki[WIATRAK_OUT].loop();
	if(lcd.loop( wiatraki, komory)!=0)
	{
		//parsowanie rozkazu
		/*
		*
		ustaw pwm wiatraka na wartosc
		ustaw tryb kominka
		ustaw tryb auto
		ustaw rozmrozenie
		*/
	}
	

}



