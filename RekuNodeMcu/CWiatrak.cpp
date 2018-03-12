#include "Defy.h"
#include "CWiatrak.h"

void CWiatrak::begin()
{   
	pinMode(_pin_pwm, OUTPUT);
	analogWrite(_pin_pwm,0);
  analogWriteRange(100);
  pinMode(_pin_tacho, INPUT_PULLUP);
}

 void CWiatrak::obslugaTachoISR()
{
	_tachoLicznik++;
 //   DPRINT(__func__);DPRINT(" _pin_tacho ");DPRINT(_pin_tacho);DPRINT(" _tachoLicznik ");DPRINTLN(_tachoLicznik);
}

void CWiatrak::loop()
{
	if(millis()-_ms>=TACHO_CZAS_MS)
	{
		_ms=millis();
		_tachoOstPredkosc=_tachoLicznik/TACHO_CZAS_SEK*60;
		_tachoLicznik=0;
    DPRINT(__func__);DPRINT(" _pin_tacho ");DPRINT(_pin_tacho);DPRINT(" _tachoOstPredkosc ");DPRINTLN(_tachoOstPredkosc);
	}
}

void CWiatrak::ustawPredkosc(uint8_t procent)
{
  if(procent==_zadanaPredkoscProcent)return;
	//uint16_t pwm=map(procent, 0, 100, 0,1023);
	//if(pwm>MAX_PWM)pwm=MAX_PWM;
	if(procent>MAX_PWM)procent=MAX_PWM;
  
	analogWrite(_pin_pwm,procent);
	_zadanaPredkoscProcent=procent;
  DPRINT("CWiatrak - Zadana predkosc wiatrak ");DPRINT(_pin_pwm);DPRINT(" ");DPRINTLN(procent);
}
