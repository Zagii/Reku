#ifndef CWentGUI_h
#define CWentGUI_h
#include <arduino.h>
#include <Adafruit_GFX.h>
#include <UTFTGLUE.h>    
#include <MCUFRIEND_kbv.h>
#include "CLcd.h"

#define BMP_wiatrak 0
#define BMP_wakacje 1
#define BMP_normalny 2
#define BMP_turbo 3
#define BMP_kominek 4
#define BMP_ILE 5

#define CWentGUI_PWM_stop 0
#define CWentGUI_PWM_wakacje 10
#define CWentGUI_PWM_normalny 30
#define CWentGUI_PWM_impreza 50
#define CWentGUI_PWM_turbo 70
#define CWentGUI_PWM_max 100
#define CWentGUI_PWM_auto 254

static const char *bmpNorm[]= {"wiatr.bmp" ,"wak.bmp" ,"norm.bmp" ,"impr.bmp" ,"tur.bmp" ,"kom.bmp"	};
static const char *bmpAktyw[]={"wiatrA.bmp","wakA.bmp","normA.bmp","imprA.bmp","turA.bmp","komA.bmp"};
static const uint16_t bmpX[]= {		50		,	20	  ,		70	  ,		70	  ,		20	 ,	80		};
static const uint16_t bmpY[]= {		50		,	20	  ,		40	  ,		70	  ,		90	 ,	100		};
static const uint8_t pwmPoz[]={		254		,	10	  ,		30	  ,		50	  ,		70	 ,	254		}; //=254 nie dotyczy

#define CWentGUI_ukryj 0
#define CWentGUI_pokaz 1

class CLcd;

class CWentGUI
{
  uint16_t _x;
  uint16_t _y;
  uint16_t _x1;
  uint16_t _y1;
  uint16_t _w=150;
  uint16_t _h=150;
  uint16_t _cx=70;	//srodek okregu od wskaznika
  uint16_t _cy=70;
  uint8_t _r=50;		//promien okregu wskaznika
  uint8_t _gr=10;	//grubosc wskaznika
  uint8_t _stan=CWentGUI_pokaz; 
  uint8_t _pozNawiew=1;
  uint8_t _pozWywiew=1;
  unsigned long _ms=0;
  uint8_t _tryb=CWentGUI_PWM_stop;
 // uint8_t _klik=KLIK_PUSZCZONY;

  CLcd *_lcd;
  public:
   CWentGUI(){};
   CWentGUI(CLcd *lcd,  uint16_t x, uint16_t y);
   void begin();
   void Rysuj(uint8_t pwmNawiew,uint8_t pwmWywiew,uint8_t tryb);
   uint16_t  czyKlik(uint16_t x,uint16_t y);
   void zmienStan(uint8_t stan);
   uint8_t inCircle( uint16_t x, uint16_t y,uint8_t R );
};


#endif
