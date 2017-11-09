
#include "CButtonWnd.h"

CButtonWnd::CButtonWnd(UTFTGLUE* tft,  uint8_t id, uint16_t x, uint16_t y, uint16_t w, uint16_t h,char *txt)
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
  _typ=TYP_TEXT;
}
CButtonWnd::CButtonWnd(UTFTGLUE* tft,  uint8_t id, uint16_t x, uint16_t y, uint16_t w, uint16_t h,uint16_t *bmpAktyw,uint16_t *bmpAktywWybr)
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
  _typ=TYP_BMP;
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
/**
* Zwracany jest stan klikniecia 0-4 lub jesli jest wciskany i trzymany to czas wcisniecia w ms
*
*/

 unsigned long  CButtonWnd::czyKlik(uint16_t x,uint16_t y)
{
	unsigned long ret=0;
	bool klik_wew=false;
	if(x>_x&&x<_x+_w&&y>_y&&y<_y+_h)klik_wew=true;
	
	switch(_klik)
	{
		case KLIK_PUSZCZONY: //wykrycie dotknięcia butona
			if(klik_wew)
			{
				_ms=millis();
				_klik=KLIK_WCISKANY;
			}
			
		break;
		case KLIK_WCISKANY: // wcisniecie butona
			if(klik_wew) //jesli nadal jest trzymany
			{
				if(millis()-_ms>30) //po tym czasie uznaj ze guzik jest trzymany
				{
					_klik=KLIK_WCISNIETY;
				}
			}else
			{
				_klik=KLIK_PUSZCZANY;
			}
		break;
		case KLIK_WCISNIETY: //wcisniecie i trzymanie butona
			if(klik_wew) //jesli nadal jest trzymany
			{
				ret=millis()-_ms;								
			}else
			{
				_klik=KLIK_PUSZCZANY;
			}
		break;
		case KLIK_PUSZCZANY:
			_klik=KLIK_PUSZCZONY;
		break;
	}
	
		Serial.print("#");Serial.print(_id);
		Serial.print(" czyKlik "); Serial.print(x);
		Serial.print(", "); Serial.println(y);
	
		if(ret>0)return ret;
		else return _klik;
	
	
}
void CButtonWnd::zmienStan(uint8_t stan)
{
  _stan=stan;
  Rysuj();
}

