#ifndef CButtonWnd_h
#define CButtonWnd_h
#include <arduino.h>
#include <Adafruit_GFX.h>
#include <UTFTGLUE.h>    
#include <MCUFRIEND_kbv.h>

class CButtonWnd
{
  uint16_t _x;
  uint16_t _y;
  uint16_t _x1;
  uint16_t _y1;
  uint16_t _w;
  uint16_t _h;
uint8_t _stan; //0-aktywny 1- nieaktywny
 // String _txt;
 char _txt[10];
 UTFTGLUE* _tft;
  public:
    CButtonWnd(){};
    CButtonWnd(UTFTGLUE* tft,uint16_t x, uint16_t y, uint16_t w, uint16_t h,char *txt);
    void Rysuj();
    int czyKlik(uint16_t x,uint16_t y);
    void zmienStan(uint8_t stan);
};

#endif
