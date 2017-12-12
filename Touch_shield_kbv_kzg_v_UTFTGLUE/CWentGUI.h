#ifndef CWentGUI_h
#define CWentGUI_h
#include <arduino.h>
#include <Adafruit_GFX.h>
#include <UTFTGLUE.h>    
#include <MCUFRIEND_kbv.h>
#include "CLcd.h"
//#define "CButtonWnd.h"

#define BMP_wiatrak 0
#define BMP_wakacje 1
#define BMP_normalny 2
#define BMP_turbo 3
#define BMP_kominek 4
#define BMP_ILE 6

#define CWentGUI_PWM_stop 0
#define CWentGUI_PWM_wakacje 10
#define CWentGUI_PWM_normalny 30
#define CWentGUI_PWM_impreza 50
#define CWentGUI_PWM_turbo 70
#define CWentGUI_PWM_max 100
#define CWentGUI_PWM_kominek 101
#define CWentGUI_PWM_rozmrazanie 102
#define CWentGUI_PWM_auto 254

static const char *bmpNorm[]= {"vB.bmp" ,"hB.bmp" ,"rB.bmp" ,"kB.bmp" ,"tB.bmp" ,"fB.bmp"	};
static const char *bmpAktyw[]={"vZ.bmp", "hZ.bmp", "rZ.bmp", "kZ.bmp", "tZ.bmp", "fZ.bmp"};
static const uint16_t bmpX[]= {		54		,	20	  ,		110	  ,		110	  ,		0	 ,	100		};
static const uint16_t bmpY[]= {		62		,	8	  ,		8	  ,		118	  ,		120	 ,	170		};
static const uint8_t pwmPoz[]={		CWentGUI_PWM_auto	,CWentGUI_PWM_wakacje,	CWentGUI_PWM_normalny ,
									CWentGUI_PWM_impreza,CWentGUI_PWM_turbo	 ,	CWentGUI_PWM_kominek}; 

#define CWentGUI_ukryj 0
#define CWentGUI_pokaz 1

class CButtonWnd;
class CLcd;

class CWentGUI
{
  uint16_t _x;
  uint16_t _y;
  uint16_t _x1;
  uint16_t _y1;
  uint16_t _w=140;
  uint16_t _h=159;
  uint16_t _cx=_w/2;	//srodek okregu od wskaznika
  uint16_t _cy=_h/2;
  uint8_t _r=30;		//promien okregu wskaznika
  uint8_t _gr=20;	//grubosc wskaznika
  uint8_t _stan=CWentGUI_pokaz; 
  uint8_t _pozNawiew=1;
  uint8_t _pozWywiew=1;
  unsigned long _ms=0;
  uint8_t _tryb=CWentGUI_PWM_stop;
 // uint8_t _klik=KLIK_PUSZCZONY;
CButtonWnd * wentBtn[BMP_ILE];
  CLcd *_lcd;
  public:
   CWentGUI(){};
   CWentGUI(CLcd *lcd,  uint16_t x, uint16_t y);
   void begin();
   void Rysuj(uint8_t pwmNawiew,uint8_t pwmWywiew,uint8_t tryb);
   int16_t Touch(uint16_t x,uint16_t y);
   void zmienStan(uint8_t stan);
   uint8_t inCircle( uint16_t x, uint16_t y,uint8_t R );
};


#endif
