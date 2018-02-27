#ifndef CKomora_h
#define CKomora_h
#include "arduino.h"
#include "Defy.h"

#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 23
#define TEMPERATURE_PRECISION 9

static OneWire oneWire(ONE_WIRE_BUS);  //statyczne?
  static DallasTemperature sensors(&oneWire); //statyczne?
  static bool czyBylTempBegin=false;

  static DeviceAddress termometrAddr[]={  { 0x28, 0x1D, 0x39, 0x31, 0x2, 0x0, 0x0, 0xF0 },
                                        { 0x28, 0x1D, 0x39, 0x31, 0x2, 0x0, 0x0, 0xF0 },
                                        { 0x28, 0x1D, 0x39, 0x31, 0x2, 0x0, 0x0, 0xF0 },
                                        { 0x28, 0x1D, 0x39, 0x31, 0x2, 0x0, 0x0, 0xF0 }};

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
	
	unsigned long _ms=0; //czas ms
	
	public:
	void begin(uint8_t komoraID);
	void loop();
	double dajTemp(){return _temp;}
	double dajCisnienie(){ return _cisnienie;}
	uint8_t dajWilgotnosc(){return _wilgotnosc;}
};

#endif
