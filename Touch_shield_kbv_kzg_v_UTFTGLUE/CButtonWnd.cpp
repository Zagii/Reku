
#include "CButtonWnd.h"

CButtonWnd::CButtonWnd(UTFTGLUE* tft,uint16_t x, uint16_t y, uint16_t w, uint16_t h,char *txt)
{
  _x=x;
  _y=y;
  _w=w;
  _h=h;
  _x1=x+w;
  _y1=y+h;
  strcpy(_txt,txt);
  _stan=0;
  _tft=tft;
  //_txt=txt;
}

void CButtonWnd:: Rysuj()//( MCUFRIEND_kbv* tft )
{
  if(_stan==0)
  { _tft->setColor(0,0,255);}
   else  {_tft->setColor(200,200,255);}
   _tft->fillRoundRect(_x, _y, _x1, _y1);
      for (int i=1; i<6; i++)
  {
    _tft->setColor(i*15,i*15,255-i*15);
     _tft->drawRoundRect(_x+(2*i), _y+(2*i), _x1-(2*i), _y1-(2*i));
    }
    
  
  
   //  tft->drawRect(_x, _y, _w, _h, 0xFFFF);
   //  tft->fillRect(_x, _y, _w, _h, 0x001F);
     _tft->setCursor(_x+10, _y+10);
    // tft->setTextSize(1);
    if(_stan==0)
  { _tft->setTextColor(0xFFFFFF);}
   else  {_tft->setTextColor(0x101010);}
    
  // tft->settextcursor((char*)st, x, y);
 
     _tft->print(_txt,_x+10,_y+10);
   
}
int CButtonWnd::czyKlik(uint16_t x,uint16_t y)
{
  if(x>_x&&x<_x+_w&&y>_y&&y<_y+_h)
  {
    Serial.print("czyKlik ");
  Serial.print(x);
  Serial.print(", ");
  Serial.println(y);
    return 1;}
  else
  {return 0;}
}
void CButtonWnd::zmienStan(uint8_t stan)
{
  _stan=stan;
  Rysuj();
}

