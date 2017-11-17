#ifndef CButtonWnd_h
#define CButtonWnd_h
#include <arduino.h>
#include <Adafruit_GFX.h>
#include <UTFTGLUE.h>    
#include <MCUFRIEND_kbv.h>
#include "CLcd.h"


#define STAN_AKTYWNY 0
#define STAN_NIEAKTYWNY 1
#define STAN_AKTYWNY_WYBRANY 2
#define STAN_UKRYTY 3

#define TYP_UNDF 0
#define TYP_TEXT 1
#define TYP_BMP 2
#define TYP_BMP_str 3

#define KLIK_PUSZCZONY 0
#define KLIK_WCISKANY 1
#define KLIK_WCISNIETY 2
#define KLIK_PUSZCZANY 3

class CLcd;

class CButtonWnd
{
  uint8_t _id;
  uint16_t _x;
  uint16_t _y;
  uint16_t _x1;
  uint16_t _y1;
  uint16_t _w;
  uint16_t _h;
  uint8_t _stan; 
  uint16_t *_bmpAktyw;
  uint16_t *_bmpAktywWybr;
  char *_bmpAktywStr;
  char *_bmpAktywWybrStr;
  uint8_t _typ;
  char _txt[10];
  unsigned long _ms=0;
  uint8_t _klik=KLIK_PUSZCZONY;
  CLcd* _lcd;
  public:
    CButtonWnd(){_typ=TYP_UNDF;_id=0;};
    CButtonWnd(CLcd* lcd,  uint8_t id, uint16_t x, uint16_t y, uint16_t w, uint16_t h,char *txt);
	CButtonWnd(CLcd* lcd,  uint8_t id, uint16_t x, uint16_t y, uint16_t w, uint16_t h,uint16_t *bmpAktyw,uint16_t *bmpAktywWybr);
	CButtonWnd(CLcd* lcd,  uint8_t id, uint16_t x, uint16_t y, uint16_t w, uint16_t h,const char* bmpAktyw,const char *bmpAktywWybr);
    void Rysuj();
    unsigned long  czyKlik(uint16_t x,uint16_t y);
    void zmienStan(uint8_t stan);
};

#endif
