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
#include <EasyTransfer.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#if !defined(BigFont)
extern uint8_t BigFont[];    //.kbv GLUE defines as GFXFont ref
#endif

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

#define MAX_TOPIC_LENGHT 50
#define MAX_MSG_LENGHT 50

struct SERIAL_DATA_STRUCTURE
{
  char msg[MAX_MSG_LENGHT];
  char topic[MAX_TOPIC_LENGHT];
  char typ;
};

//give a name to the group of data
SERIAL_DATA_STRUCTURE rxdata;
SERIAL_DATA_STRUCTURE txdata;
//create two objects
EasyTransfer ETin, ETout; 

char* zrobJson(uint8_t paramName, uint16_t paramValue);

CLcd lcd(zrobJson);
CKomora komory[KOMORA_SZT];
CWiatrak wiatraki[WIATRAKI_SZT]=
{
	CWiatrak(PIN_WIATRAK_CZERPNIA,PIN_TACHO_WIATRAK_CZERPNIA),
	CWiatrak(PIN_WIATRAK_WYWIEW,PIN_TACHO_WIATRAK_WYWIEW)
};

#define RS_CONN_INFO 'c'  // wifi / mqtt status
#define RS_RECEIVE_MQTT 'r' // msg from mqtt serwer
#define RS_PUBLISH_MQTT 'p' // msg to send
#define RS_SETUP_INFO 's' //
#define RS_DEBUG_INFO 'd' //debug info

const char* publishTopic="Reku";
const char* debugTopic="DebugTopic/Reku/Mega2560";

void sendRS(char * typ, char* topic, char* msg, char* inne="")
{
  dPrintf("%s:%d: %s -> wysyłam typ=%c, topic=%s, msg=%s\n", __FILE__, __LINE__, __PRETTY_FUNCTION__,typ,topic,msg);
  txdata.typ=typ;
  strcpy(txdata.topic,topic);
  strcpy(txdata.msg,msg);
  if(strlen(inne)>0)
  { 
    strcat(txdata.msg,"_");
    strcat(txdata.msg,inne);
  }
  ETout.sendData();
  
}

void setup(void)
{
   Serial.begin(115200);
   Serial1.begin(115200);
   ETin.begin(details(rxdata), &Serial1);
   ETout.begin(details(txdata), &Serial1);

   sendRS(RS_DEBUG_INFO,debugTopic,__PRETTY_FUNCTION__,"restart");
   
   lcd.begin();
   for(uint8_t i=0;i<KOMORA_SZT;i++)
   {
	   komory[i]=CKomora();
	   komory[i].begin(i);
   }
   attachInterrupt(digitalPinToInterrupt( wiatraki[WIATRAK_IN].dajISR()), isrIN, RISING );
   attachInterrupt(digitalPinToInterrupt( wiatraki[WIATRAK_OUT].dajISR()), isrOUT, RISING );
   wiatraki[WIATRAK_IN].begin();
   wiatraki[WIATRAK_OUT].begin();
    sendRS(RS_DEBUG_INFO,debugTopic,__PRETTY_FUNCTION__,"koniec");
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
void readRS()
{
    if(!ETin.receiveData()) return;
    dPrintf("readRS> typ=%c, topic=%s, msg=%s\n",rxdata.typ,rxdata.topic,rxdata.msg);
    switch(rxdata.typ)
    {
      case RS_CONN_INFO:   // wifi / mqtt status
    
           break;
      case RS_RECEIVE_MQTT:  // msg from mqtt serwer
    
           break;
      case RS_PUBLISH_MQTT:  // msg to send, nie pojawi sie
           break;
      case RS_SETUP_INFO:  //
           break;
      case RS_DEBUG_INFO:  //debug info
          
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
   readRS();
	
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

