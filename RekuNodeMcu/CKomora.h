#ifndef CKomora_h
#define CKomora_h
#include "arduino.h"
#include "Defy.h"

#include <OneWire.h>
#include <DallasTemperature.h>

#define TEMPERATURE_PRECISION 12//9

static OneWire oneWire(PIN_ONEWIRE);  //statyczne?
  static DallasTemperature sensors(&oneWire); //statyczne?
  static bool czyBylTempBegin=false;

  static DeviceAddress termometrAddr[]={  { 0x28, 0x52, 0x96, 0x23, 0x06, 0x00, 0x00, 0x5C },
                                        { 0x28, 0x04, 0x1E, 0x22, 0x06, 0x00, 0x00, 0xBA },
                                        { 0x28, 0xC0, 0x24, 0x23, 0x06, 0x00, 0x00, 0x4F },
                                        { 0x28, 0x67, 0xD4, 0x22, 0x06, 0x00, 0x00, 0xC7 }};


/*
 * uint8_t pin23[][8] = {
  {
Device Address: 28C024230600004F Temp C: 27.50 Temp F: 81.50
Device Address: 28041E22060000BA Temp C: 26.50 Temp F: 79.70
Device Address: 285296230600005C Temp C: 25.50 Temp F: 77.90
Device Address: 28BEDA21060000C7 Temp C: 25.50 Temp F: 77.90


 */

class CKomora
{
  unsigned long _tempMillis;
  uint8_t id;
	double _temp=0;
	double _cisnienie=0;
	uint8_t _wilgotnosc=0;
	//uint8_t co2=0;

	void pomiarTemp();
	void pomiarCisnienia();
	void pomiarWilgotnosci();
	//void pomiarCO2();
	char addressStr[17];
	unsigned long _ms=0; //czas ms
	
	public:
void printAddress(DeviceAddress deviceAddress,char* buf);
char *getTempAddress(){return addressStr;}
	void begin(uint8_t komoraID);
	void loop();
	double dajTemp(){return _temp;}
	double dajCisnienie(){ return _cisnienie;}
	uint8_t dajWilgotnosc(){return _wilgotnosc;}
};

#endif
