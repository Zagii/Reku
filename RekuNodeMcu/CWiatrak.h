#ifndef CWiatrak_h
#define CWiatrak_h
#include "arduino.h"
#define TACHO_CZAS_SEK 3
#define TACHO_CZAS_MS (TACHO_CZAS_SEK*1000)

#define MAX_PWM 80

class CWiatrak
{
	uint8_t _pin_pwm;
	uint8_t _pin_tacho;
	volatile unsigned long _tachoLicznik=0;
	unsigned long _tachoOstPredkosc=0;	//obr/min
	unsigned long _ms=0; //millisy w loop
	uint8_t _zadanaPredkoscProcent=0;
public:
	CWiatrak(uint8_t pin_pwm,uint8_t pin_tacho)
	{
		_pin_pwm=pin_pwm;
		_pin_tacho=pin_tacho;
	}
	void begin();
  void obslugaTachoISR();
	void loop();
	void ustawPredkosc(uint8_t procent);
	uint8_t dajZadanaPredkoscProcent(){return _zadanaPredkoscProcent;}
	unsigned long dajOstPredkosc(){return _tachoOstPredkosc;}
  uint8_t dajISR() {return _pin_tacho;}
};

#endif
