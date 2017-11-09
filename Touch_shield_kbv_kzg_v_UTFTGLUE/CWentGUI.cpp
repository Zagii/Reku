
#include "CWentGUI.h"

  CWentGUI::CWentGUI(CLcd *lcd,  uint16_t x, uint16_t y)
   {
	  _x=x;_y=y;_x1=x+_w;_y1=y+_h; 
    _lcd=lcd;
   }
void   CWentGUI::begin() //inicjalne rysowanie z przykryciem tła
   {
   
	   _lcd->setColor(0,0,0);
	   _lcd->fillRect(_x, _y, _x1, _y1);
	   Rysuj(0,0,CWentGUI_PWM_stop);
   }
   void CWentGUI::Rysuj(uint8_t pwmNawiew,uint8_t pwmWywiew, uint8_t tryb)
   {
	   if(pwmNawiew==_pozNawiew &&pwmWywiew==_pozWywiew&& tryb==_tryb)
	   {
		   if(millis()-_ms>2000) // jesli dawno nie bylo odswiezenia to przerysuj
		   {						//mimo wszystko
			   _ms=millis();
			   
		   }
		   else
		   {
			   return; //nie trzeba odswiezac
			}
	   }
		_pozNawiew=pwmNawiew; _pozWywiew=pwmWywiew; _tryb=tryb;
		
		
		/////////// wybor bitmap do malowania /////////////
	   bool bmpStatus[BMP_ILE];
	   for(int i=0;i<BMP_ILE;i++)
	   {
		   if(_pozNawiew==pwmPoz[i])
			    bmpStatus[i]=true;
		   else
				bmpStatus[i]=false;
	   }
	   if(_pozNawiew>_pozWywiew) 
	   {
		   bmpStatus[BMP_kominek]=true;
	   }
	   
	   if(_tryb==CWentGUI_PWM_auto)
	   {
		   bmpStatus[BMP_wiatrak]=true;
	   }
	   
	   for(int i=0;i<BMP_ILE;i++)
	   {
		  if( bmpStatus[i])
		   {
		//    showBMP(bmpAktyw[i],_x+bmpX[i],_y+bmpY[i]);
		    } 
		else 
{		   
  //showBMP(bmpNorm[i],_x+bmpX[i],_y+bmpY[i]);
  }
	   }
	   
	   /////////////////////////////////////////////
	   /////////// rysowanie wskazowki zegara /////////////
	  
	   uint16_t maxRN= map(_pozNawiew, 0, 100, 0, 270);
	   uint16_t maxRW= map(_pozWywiew, 0, 100, 0, 270);
	   ///ramka
	   _lcd->setColor(255,255,255);
	   _lcd->drawMidpointCircle(_cx,_cy,_r,0, 270); 
	   _lcd->drawMidpointCircle(_cx,_cy,_r+_gr,0, 270); 
		  
		//wypełnienie
		for(uint8_t i=0;i<_gr;i++)
		{
			_lcd->drawMidpointCircle(_cx,_cy,i,0, maxRN); 
		}
		//niby maksymalny zakres
		_tft->setColor(20,20,20); 
		for(uint8_t i=0;i<_gr;i++)
		{		
			_lcd->drawMidpointCircle(_cx,_cy,i,270, 300); 
		}			
		//koncowka drugiego wentyla
		_tft->setColor(255,0,0); 
		for(uint8_t i=0;i<_gr;i++)
		{
			_lcd->drawMidpointCircle(_cx,_cy,_r,maxRW-1, maxRW+1); 
		}
   }
   uint16_t CWentGUI::czyKlik(uint16_t x,uint16_t y)
   {
	   bool bmpKlik[BMP_ILE];
	   for(int i=0;i<BMP_ILE;i++)
	   {
		   if(x>bmpX[i]&&x<bmpX[i]+40&&y>bmpX[y]&&y<bmpX[y]+40)bmpKlik[i]=true;
	   }
	   if(inCircle(x,y,_r)>0&&inCircle(x,y,_r+_gr)>0)
	   {
		   
	   }
   }
   void CWentGUI::zmienStan(uint8_t stan)
   {
	   
   }
   #define ABS(x) (((x)<0)?(0-(x)):(x))
   
	uint8_t CWentGUI::inCircle( uint16_t x, uint16_t y,uint8_t R )
	{ 
	  uint16_t dx = ABS(x-_cx);
	  if (    dx >  R ) return 0;
	  uint16_t dy = ABS(y-_cy);
	  if (    dy >  R ) return 0;
	  if ( dx+dy <= R ) return 1;
	  if(dx*dx + dy*dy <= R*R )
		return 1;
	  else 
		return 0;
	}


