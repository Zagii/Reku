#ifndef CKomora_h
#define CKomora_h
#include "arduino.h"

class CKomora
{
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
	void begin();
	void loop();
	double dajTemp(){return _temp;}
	double dajCisnienie(){ return _cisnienie;}
	uint8_t dajWilgotnosc(){return _wilgotnosc;}
};

#endif
