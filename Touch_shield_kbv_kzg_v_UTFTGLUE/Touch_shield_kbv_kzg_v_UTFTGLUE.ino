/***TODO
 * mqttpublish parametrów
 * godzina
 * pasek gora
 * sprzatanie po restarcie
 */

/**
 * potrzebne biblioteki: 
 * MCUFRIEND_kbv.h ->https://github.com/prenticedavid/MCUFRIEND_kbv 
 * touchscreen.h -> https://github.com/adafruit/Touch-Screen-Library
 * Adafruit_GFX.h -> def arduino
 * SdFat.h -> def arduino https://github.com/greiman/SdFat trzeba tez zmienic flage na soft spi w bibliotece konfiguracji
 * OneWire
 * DallasTemerature
 * 
 */
#include <Arduino.h>

#include <Adafruit_GFX.h>    // Core graphics library
#include <TouchScreen.h>
#include <SPI.h>          // f.k. for Arduino-1.5.2
#include <SdFat.h>           // Use the SdFat library
#include <MCUFRIEND_kbv.h>
#include <ArduinoJson.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <EasyTransfer.h>



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
#define MAX_MSG_LENGHT 20

struct RS_DATA_STRUCTURE
{
  char msg[MAX_MSG_LENGHT];
  char topic[MAX_TOPIC_LENGHT];
  char typ;
};

//give a name to the group of data
//create two objects
RS_DATA_STRUCTURE txdata,rxdata;
EasyTransfer ETin,ETout;

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

/*******************************
 * *********  sendRS ***********
 ******************************/

void sendRS(char * typ, char* topic, char* msg, char* inne="")
{
  //dPrintf("%s:%d: %s -> wysyłam typ=%c, topic=%s, msg=%s\n", __FILE__, __LINE__, __func__,typ,topic,msg);

  DPRINT(__func__);DPRINT(" typ=");DPRINT(typ);DPRINT(", topic=");DPRINT(topic);DPRINT(", msg=");DPRINT(msg);DPRINT(", inne=");DPRINTLN(inne);
 
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

/******************************
 * *********  Setup ***********
 ******************************/

void setup(void)
{
   Serial.begin(115200);
   Serial1.begin(115200);
   ETin.begin(details(rxdata), &Serial1);
   ETout.begin(details(txdata), &Serial1);

   sendRS(RS_DEBUG_INFO,debugTopic,__func__,"restart");
   
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
   sendRS(RS_DEBUG_INFO,debugTopic,__func__,"koniec");
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
  /*
	StaticJsonBuffer<JSON_DL_ROZKAZU> jsonBuffer;
	JsonArray& rT = jsonBuffer.parseArray(rozkazStr);
	JsonArray& r = jsonBuffer.createArray();
	r.add(paramName);
	r.add(paramValue);
    rT.add(r);
	rT.printTo((char*)rozkazStr, rT.measureLength() + 1);
  

  return rozkazStr;*/
  return "";
}

#define MAX_ROZKAZOW 10
uint8_t ile_w_kolejce=0;
uint16_t kolejkaRozkazow[MAX_ROZKAZOW][2];
void dodajDoKolejki(uint16_t paramName,uint16_t paramValue)
{
  DPRINT("Dodaj do kolejki"); DPRINT(paramName);DPRINT(", ");DPRINTLN(paramValue);
  
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
   
    DPRINT(__func__);DPRINT(" typ=");DPRINT(rxdata.typ);DPRINT(", topic=");DPRINT(rxdata.topic);DPRINT(", msg=");DPRINTLN(rxdata.msg);
   
    switch(rxdata.typ)
    {
      case RS_CONN_INFO:   // wifi / mqtt status
    
           break;
      case RS_RECEIVE_MQTT:  // msg from mqtt serwer
           if(strstr(rxdata.topic,"WiatrakN")>0)
           {
              if(isIntChars(rxdata.msg))
              {
                dodajDoKolejki(JSON_PWM_NAWIEW,atoi(rxdata.msg));
                
              }else
              {
                DPRINT("ERR msg WiatrakN nie int, linia:");DPRINTLN(__LINE__);
              }
           }else
           if(strstr(rxdata.topic,"WiatrakW")>0)
           {
               if(isIntChars(rxdata.msg))
              {
                dodajDoKolejki(JSON_PWM_WYWIEW,atoi(rxdata.msg));
                
              }else
              {
                DPRINT("ERR msg WiatrakW nie int, linia:");DPRINTLN(__LINE__);
              }
           }
           break;
      case RS_PUBLISH_MQTT:  // msg to send, nie pojawi sie
           break;
      case RS_SETUP_INFO:  //
           break;
      case RS_DEBUG_INFO:  //debug info
          
           break;
      }
     
}

/******************************
 * ********* loop *************
 ******************************/
 
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
      DPRINT("Jest w kolejce ");  DPRINT(i);  DPRINT("/"); DPRINT(ile_w_kolejce); DPRINT(" ");
      DPRINT(kolejkaRozkazow[i][0]); DPRINT(", "); DPRINTLN(kolejkaRozkazow[i][1]);
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

