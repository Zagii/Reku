#ifndef CKomora_h
#define CKomora_h
#include "arduino.h"


/*
 * uint8_t pin23[][8] = {
  {
0x28, 0xC0, 0x24, 0x23, 0x06, 0x00, 0x00, 0x4F  },
  {
0x28, 0x04, 0x1E, 0x22, 0x06, 0x00, 0x00, 0xBA  },
  {
0x28, 0x52, 0x96, 0x23, 0x06, 0x00, 0x00, 0x5C  },
  {
0x28, 0x67, 0xD4, 0x22, 0x06, 0x00, 0x00, 0xBF  },
};

 */

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
