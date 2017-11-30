
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
     for(int i=0;i<BMP_ILE;i++)
    {
      wentBtn[i]=new CButtonWnd(_lcd,i,_x+bmpX[i],_y+bmpY[i],35,35,(char*)bmpNorm[i],(char*)bmpAktyw[i]);
      wentBtn[i]->zmienStan(BTN_STAN_AKTYWNY);
      
    }
	   Rysuj(0,0,CWentGUI_PWM_stop);
   }
   void CWentGUI::Rysuj(uint8_t pwmNawiew,uint8_t pwmWywiew, uint8_t tryb)
   {
	   if(pwmNawiew==_pozNawiew &&pwmWywiew==_pozWywiew&& tryb==_tryb)
	   {
		   if(millis()-_ms>20000) // jesli dawno nie bylo odswiezenia to przerysuj
		   {						//mimo wszystko
			   _ms=millis();
			   
		   }
		   else
		   {
			   return; //nie trzeba odswiezac
			}
	   }
		_pozNawiew=pwmNawiew; _pozWywiew=pwmWywiew; _tryb=tryb;
		
		
	
	   
	   /////////////////////////////////////////////
	   /////////// rysowanie wskazowki zegara /////////////
	  
	   uint16_t maxRN= map(_pozNawiew, 0, 100, 0, 270);
	   uint16_t maxRW= map(_pozWywiew, 0, 100, 0, 270);
   // maxRN=120;///tylko do debuga
   // maxRW=80;
    Serial.print("RN=");Serial.print(maxRN);
    Serial.print("RW=");Serial.println(maxRW);
	   ///ramka
      _lcd->setColor(70,70,70);
	  
	
    _lcd->drawArc(_x+_cx,_y+_cy,_r,0,270);
    _lcd->drawArc(_x+_cx,_y+_cy,_r+_gr,0,270);
     _lcd->drawLine(_x,     _y+_cy,_x+_cx,_y+_cy);//pozioma wskazowka
     _lcd->drawLine(_x+_cx,_y     ,_x+_cx,_y+_cy); //pionowa wskazowka
		  
		//wypełnienie

	//pomysl nowy zmienic na gruby łuk biały w zakresie od 0 do mniejszej wartosci wiatraka oraz od mniejszej do wiekszej w kolorze czerw/nieb

	uint16_t tmp=maxRN;
	if(maxRW<tmp)tmp=maxRW;
	_lcd->setColor(255,255,255);
	_lcd->drawArc(_x+_cx,_y+_cy,_r,0,tmp,_gr);

	if(maxRN>maxRW)  /// jesli nawiew wieje mocniej to rysuj niebiesko
	{
		_lcd->setColor(90,90,255);
		_lcd->drawArc(_x+_cx,_y+_cy,_r,tmp,maxRN,_gr);
     Serial.print("tmp");Serial.print(tmp);
    Serial.print("RN");Serial.println(maxRN);
	}else	//jeśli nie to czerwony
	{
		 _lcd->setColor(255,90,90); 
		 _lcd->drawArc(_x+_cx,_y+_cy,_r,tmp,maxRW,_gr);
     Serial.print("tmp");Serial.print(tmp);
    Serial.print("RW");Serial.println(maxRW);
	}

	///// koniec pomysłu :)

 
  // _lcd->setColor(0,0,255);
  // _lcd->drawRect(_x,_y,_x1,_y1);// ramka debigowa

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
         wentBtn[i]->zmienStan(BTN_STAN_AKTYWNY_WYBRANY);
    //    showBMP(bmpAktyw[i],_x+bmpX[i],_y+bmpY[i]);
        } 
    else 
{       wentBtn[i]->zmienStan(BTN_STAN_AKTYWNY);
  //showBMP(bmpNorm[i],_x+bmpX[i],_y+bmpY[i]);
  }
  wentBtn[i]->Rysuj();
     }
   
   }
   int CWentGUI::Touch(uint16_t x,uint16_t y)
   {
	   // sprawdzenie ktore z buttonow zostaly nacisniete
	   //bool bmpKlik[BMP_ILE];
	   int kliknietyBtn=-1;
	   for(int i=0;i<BMP_ILE;i++)
	   {
		   //bmpKlik[i]=false;
		   unsigned long bk=wentBtn[i]->czyKlik(x,y);
		   if(bk!=KLIK_PUSZCZONY)
		   {
			  // bmpKlik[i]=true;
			 
			  kliknietyBtn=i;
		   }
		   
	   }
	
	   // czy ktos nie kliknal w skale na kolku
	   if(//inCircle(x,y,_r)>0&&
	      inCircle(x,y,_r+_gr)>0)
	   {
		   Serial.print("Kolko ");
      
      double xx=x-_cx-_x;
      if(xx==0)xx=0.1;
      double yy=y-_cy-_y;
      if(yy==0)yy=0.1;
      double at=yy/xx;
      double dg=at*(57296 / 1000);
      Serial.print(xx);Serial.print("; ");Serial.print(yy);Serial.print("; ");Serial.print(at);Serial.print(";");
      double z=0;
      if(xx<0)
      {
        if(yy<0)
        {}
        else
        {}
      }else
      {
        if(yy<0)
        {}
        else
        {z+=90;}
      }
      Serial.println(dg);
	   }

     //kliknieto gdzies poza guzikami i kolkiem nic nie rob
     if(kliknietyBtn<0)return kliknietyBtn;

     
	      // zapalenie stanow odpowiednich butonow
	   for(int i=0;i<BMP_ILE;i++)
	   {
		   if(kliknietyBtn==i)
		   {
			   wentBtn[i]->zmienStan(BTN_STAN_AKTYWNY_WYBRANY);
			  _tryb=pwmPoz[i];
		   }else
		   {
			   wentBtn[i]->zmienStan(BTN_STAN_AKTYWNY);
		   }
	   }
   }
   void CWentGUI::zmienStan(uint8_t stan)
   {
	   
   }
   //#define ABS(x) (((x)<0)?(0-(x)):(x))

   
   
	uint8_t CWentGUI::inCircle( uint16_t x, uint16_t y,uint8_t R )
	{ //Serial.print("inCicle R=");
 //Serial.print(R);Serial.print(" ");
	  uint16_t dx = abs(x-_cx-_x);
   Serial.println(dx);
	  if (    dx >  R ) return 0;
	  uint16_t dy = abs(y-_cy-_y);
  // Serial.print(" ");Serial.print(dy);
	  if (    dy >  R ) return 0;
 
	  if ( dx+dy <= R ) return 1;
 //    Serial.print(" dx+dy=");
 //  Serial.print(dx+dy);
 //    Serial.print(" dx2*+dy2=");
 //  Serial.print(dx*dx + dy*dy);
//    Serial.print(" R2=");
 //  Serial.println(R*R);
	  if(dx*dx + dy*dy <= R*R )
		return 1;
	  else 
		return 0;
	}


