/**
 * potrzebne biblioteki: 
 * MCUFRIEND_kbv.h ->https://github.com/prenticedavid/MCUFRIEND_kbv 
 * touchscreen.h -> https://github.com/adafruit/Touch-Screen-Library
 * Adafruit_GFX.h -> def arduino
 * SdFat.h -> def arduino https://github.com/greiman/SdFat trzeba tez zmienic flage na soft spi w bibliotece konfiguracji
 * 
 * 
 */


#include <Arduino.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <TouchScreen.h>
#include <SPI.h>          // f.k. for Arduino-1.5.2
#include <SdFat.h>           // Use the SdFat library
#include <MCUFRIEND_kbv.h>
#include <ArduinoJson.h>



#define JSON_DL_ROZKAZU 150
char rozkazStr[JSON_DL_ROZKAZU];

#include "Defy.h"
#include "CLcd.h"
#include "CWiatrak.h"
#include "CKomora.h"
//#include "CWentGUI.h"

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


char* zrobJson(uint8_t paramName, uint16_t paramValue);

CLcd lcd(zrobJson);
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
  
   
   for(uint8_t i=0;i<KOMORA_SZT;i++)
   {
	   komory[i]=CKomora();
	   komory[i].begin();
   }
   attachInterrupt(digitalPinToInterrupt( wiatraki[WIATRAK_IN].dajISR()), isrIN, RISING );
   attachInterrupt(digitalPinToInterrupt( wiatraki[WIATRAK_OUT].dajISR()), isrOUT, RISING );
   wiatraki[WIATRAK_IN].begin();
   wiatraki[WIATRAK_OUT].begin();
   Serial.println("koniec setup glowny");
}
void isrIN()
{
  wiatraki[WIATRAK_IN].obslugaTachoISR();
}
void isrOUT()
{
  wiatraki[WIATRAK_OUT].obslugaTachoISR();
}


char* zrobJson2(uint8_t paramName, uint16_t paramValue) 
{
	StaticJsonBuffer<JSON_DL_ROZKAZU> jsonBuffer;
	JsonArray& rT = jsonBuffer.parseArray(rozkazStr);
	JsonArray& r = jsonBuffer.createArray();
	r.add(paramName);
	r.add(paramValue);
    rT.add(r);
	rT.printTo((char*)rozkazStr, rT.measureLength() + 1);
  

  return rozkazStr;
}

#define MAX_ROZKAZOW 10
uint8_t ile_w_kolejce=0;
uint16_t kolejkaRozkazow[MAX_ROZKAZOW][2];
void dodajDoKolejki(uint16_t paramName,uint16_t paramValue)
{
  Serial.print("Dodaj do kolejki");
  Serial.print(paramName);Serial.print(", ");Serial.println(paramValue);
	if(ile_w_kolejce>=MAX_ROZKAZOW) return;
	kolejkaRozkazow[ile_w_kolejce][0]=paramName;
	kolejkaRozkazow[ile_w_kolejce][1]=paramValue;
	ile_w_kolejce++;
}


char* zrobJson(uint8_t paramName, uint16_t paramValue) 
{
	dodajDoKolejki(paramName,paramValue);
	return 0;
}

const byte numChars = 32;
char receivedChars[numChars];

boolean newData = false;
void recvWithStartEndMarkers() {
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    char rc;

    while (Serial.available() > 0 && newData == false) {
        rc = Serial.read();

        if (recvInProgress == true) {
            if (rc != endMarker) {
                receivedChars[ndx] = rc;
                ndx++;
                if (ndx >= numChars) {
                    ndx = numChars - 1;
                }
            }
            else {
                receivedChars[ndx] = '\0'; // terminate the string
                recvInProgress = false;
                ndx = 0;
                newData = true;
            }
        }

        else if (rc == startMarker) {
            recvInProgress = true;
        }
    }
}

void showNewData() {
    if (newData == true) {
        Serial.print("This just in ... ");
        Serial.println(receivedChars);
        newData = false;
    }
}


void realizujRozkaz(uint16_t paramName,uint16_t paramValue) 
{
	switch(paramName)
	{
		case JSON_PWM_NAWIEW:
			wiatraki[WIATRAK_IN].ustawPredkosc(paramValue);
		break;
		case JSON_PWM_WYWIEW:
			wiatraki[WIATRAK_OUT].ustawPredkosc(paramValue);
		break;
		case JSON_ROZMRAZANIE_WIATRAKI: 
			wiatraki[WIATRAK_IN].ustawPredkosc(15);
			wiatraki[WIATRAK_OUT].ustawPredkosc(40);
		break;
		case JSON_ROZMRAZANIE_GGWC:
		break;
		case JSON_KOMINEK:
			wiatraki[WIATRAK_IN].ustawPredkosc(50);
			wiatraki[WIATRAK_OUT].ustawPredkosc(15);
		break;
		case JSON_AUTO:
		break;		
	}
	
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
		//parsowanie rozkazu zapisanego juz w rozkazStr
		/*
		*
		ustaw pwm wiatraka na wartosc
		ustaw tryb kominka
		ustaw tryb auto
		ustaw rozmrozenie
		*/
	}
	/// odczytaj rozkaz z Seriala
	//recvWithStartEndMarkers();
    //showNewData();
	/// przetwarzanie rozkazu
	
	if(ile_w_kolejce>0)
	{
		for(int i=0;i<ile_w_kolejce;i++)
		{
      Serial.print("Jest w kolejce "); Serial.print(i); Serial.print("/");Serial.print(ile_w_kolejce);Serial.print(" ");
    Serial.print(kolejkaRozkazow[i][0]);Serial.print(", ");Serial.println(kolejkaRozkazow[i][1]);
			realizujRozkaz(kolejkaRozkazow[i][0],kolejkaRozkazow[i][1]);
		}
		ile_w_kolejce=0;
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

