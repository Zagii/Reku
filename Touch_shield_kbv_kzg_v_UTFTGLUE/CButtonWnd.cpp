
#include "CButtonWnd.h"

CButtonWnd::CButtonWnd(UTFTGLUE* tft,  uint8_t _id, uint16_t x, uint16_t y, uint16_t w, uint16_t h,char *txt)
{
 _id=id;
  _x=x;
  _y=y;
  _w=w;
  _h=h;
  _x1=x+w;
  _y1=y+h;
  strcpy(_txt,txt);
  _stan=STAN_AKTYWNY;
  _tft=tft;

}
CButtonWnd(UTFTGLUE* tft,  uint8_t _id, uint16_t x, uint16_t y, uint16_t w, uint16_t h,uint16_t *bmpAktyw,uint16_t *bmpAktywWybr)
{
  _id=id;
  _x=x;
  _y=y;
  _w=w;
  _h=h;
  _x1=x+w;
  _y1=y+h;
  strcpy(_txt,"");
  _stan=STAN_AKTYWNY;
  _tft=tft;
  _bmpAktyw=bmpAktyw;
  _bmpAktywWybr=bmpAktywWybr;
}
void CButtonWnd:: Rysuj()//( MCUFRIEND_kbv* tft )
{
	if(_stan==STAN_UKRYTY)
	{
		return;
	}
	switch(_typ)
	{	  
		case TYP_TEXT:
			if(_stan==STAN_AKTYWNY)
			{
				_tft->setColor(0,0,255);
			}
			else  
			{
				_tft->setColor(200,200,255);
			}
			_tft->fillRoundRect(_x, _y, _x1, _y1);
			for (int i=1; i<6; i++)
			{
				_tft->setColor(i*15,i*15,255-i*15);
				_tft->drawRoundRect(_x+(2*i), _y+(2*i), _x1-(2*i), _y1-(2*i));
			}    
			 _tft->setCursor(_x+10, _y+10);
			if(_stan==0)
			{ 
				_tft->setTextColor(0xFFFFFF);}
			else  
			{
			   _tft->setTextColor(0x101010);
			}
			 _tft->print(_txt,_x+10,_y+10);
		return;
		case TYP_BMP:
			switch(_stan)
			{
			case STAN_AKTYWNY:
				_tft->drawBitmap(_x,_y,_w,_h,_bmpAktyw);
				break;
			case STAN_AKTYWNY_WYBRANY:
				_tft->drawBitmap(_x,_y,_w,_h,_bmpAktywWybr);
				break;
			case STAN_NIEAKTYWNY:
				_tft->setColor(200,200,255);
				_tft->fillRoundRect(_x, _y, _x1, _y1);
				break;
			}
			
		return;
	}  
}
int CButtonWnd::czyKlik(uint16_t x,uint16_t y)
{
	if(x>_x&&x<_x+_w&&y>_y&&y<_y+_h)
	{
		Serial.print("#");Serial.print(_id);
		Serial.print(" czyKlik "); Serial.print(x);
		Serial.print(", "); Serial.println(y);
		return 1;
	}
	else
	{
		return 0;
	}
}
void CButtonWnd::zmienStan(uint8_t stan)
{
  _stan=stan;
  Rysuj();
}

