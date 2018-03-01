#include "Defy.h"
#include "CWiatrak.h"

void CWiatrak::begin()
{   

	pinMode(_pin_pwm, OUTPUT);
	analogWrite(_pin_pwm,0);

	
}

 void CWiatrak::obslugaTachoISR()
{
	_tachoLicznik++;
}

void CWiatrak::loop()
{
	if(millis()-_ms>=TACHO_CZAS_MS)
	{
		_ms=millis();
		_tachoOstPredkosc=_tachoLicznik/TACHO_CZAS_SEK*60;
		_tachoLicznik=0;
	}
}

void CWiatrak::ustawPredkosc(uint8_t procent)
{
	uint8_t pwm=map(procent, 0, 100, 0,255);
	if(pwm>MAX_PWM)pwm=MAX_PWM;
	analogWrite(_pin_pwm,pwm);
	_zadanaPredkoscProcent=procent;
  DPRINT("CWiatrak - Zadana predkosc wiatrak ");DPRINT(_pin_pwm);DPRINT(" ");DPRINTLN(procent);
}
