
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
CKomora komory[KOMORA_SZT];
CWiatrak wiatraki[WIATRAKI_SZT]=
{
	CWiatrak(PIN_WIATRAK_CZERPNIA,PIN_TACHO_WIATRAK_CZERPNIA),
	CWiatrak(PIN_WIATRAK_WYWIEW,PIN_TACHO_WIATRAK_WYWIEW)
};

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
   attachInterrupt(digitalPinToInterrupt( wiatraki[WIATRAK_IN].dajISR()), isrIN, RISING );
   attachInterrupt(digitalPinToInterrupt( wiatraki[WIATRAK_OUT].dajISR()), isrOUT, RISING );
   wiatraki[WIATRAK_IN].begin();
   wiatraki[WIATRAK_OUT].begin();
}
void isrIN()
{
  wiatraki[WIATRAK_IN].obslugaTachoISR();
}
void isrOUT()
{
  wiatraki[WIATRAK_OUT].obslugaTachoISR();
}

void loop()
{
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

/*
uint8_t conv2d(const char* p)
{
uint8_t v = 0;
if (‘0’ <= *p && *p <= ‘9’) v = *p – ‘0’;
return 10 * v + *++p – ‘0’;
}


hh = conv2d(__TIME__);
mm = conv2d(__TIME__ + 3);
ss = conv2d(__TIME__ + 6);
*/

