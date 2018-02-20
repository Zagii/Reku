
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

   void CWentGUI::RysujBtn(uint8_t pwmNawiew,uint8_t pwmWywiew)
   {
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
            { 
              wentBtn[i]->zmienStan(BTN_STAN_AKTYWNY);
              //showBMP(bmpNorm[i],_x+bmpX[i],_y+bmpY[i]);
            }
          wentBtn[i]->Rysuj();
         }
        
   }
   void CWentGUI::RysujLuk(uint8_t pwmNawiew,uint8_t pwmWywiew)
   {
if(pwmNawiew==_pozNawiew &&pwmWywiew==_pozWywiew)//&& tryb==_tryb)
     {
       if(millis()-_ms>20000) // jesli dawno nie bylo odswiezenia to przerysuj
       {            //mimo wszystko
         _ms=millis();
         
       }
       else
       {
         return; //nie trzeba odswiezac
      }
     }
     uint16_t mp=_pozNawiew;
     if(_pozWywiew>mp)mp=_pozWywiew;
     uint16_t m=pwmNawiew;
     if(pwmWywiew>m)m=pwmWywiew;
     if(m<mp)
     {
      m=map(m, 0, 100, 0, 270);
      mp=map(mp, 0, 100, 0, 270);
      _lcd->setColor(0,0,0); 
     _lcd->drawArc(_x+_cx,_y+_cy,_r,m,mp,_gr);
     }
    _pozNawiew=pwmNawiew; _pozWywiew=pwmWywiew;// _tryb=tryb;
    
  
     /////////////////////////////////////////////
     /////////// rysowanie wskazowki zegara /////////////
    
     uint16_t maxRN= map(_pozNawiew, 0, 100, 0, 270);
     uint16_t maxRW= map(_pozWywiew, 0, 100, 0, 270);
   // maxRN=120;///tylko do debuga
   // maxRW=80;
 //   Serial.print("RN=");Serial.print(maxRN);
   // Serial.print("RW=");Serial.println(maxRW);
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
  //   Serial.print("tmp");Serial.print(tmp);
   // Serial.print("RN");Serial.println(maxRN);
  }else //jeśli nie to czerwony
  {
     _lcd->setColor(255,90,90); 
     _lcd->drawArc(_x+_cx,_y+_cy,_r,tmp,maxRW,_gr);
  //   Serial.print("tmp");Serial.print(tmp);
 //   Serial.print("RW");Serial.println(maxRW);
  }

  ///// koniec pomysłu :)

   }
   void CWentGUI::Rysuj(uint8_t pwmNawiew,uint8_t pwmWywiew, uint8_t tryb)
   {
       Serial.print("CWentGUI -");Serial.print(pwmNawiew); Serial.print(", ");Serial.print(_pozNawiew);Serial.print(", ");
       Serial.print(pwmWywiew);Serial.print(", "); Serial.print(_pozWywiew);Serial.print(", ");Serial.print(tryb);Serial.print(", ");Serial.println(_tryb);
	   _ms=millis()+22000;
		
		RysujLuk(pwmNawiew,pwmWywiew);
    RysujBtn(pwmNawiew,pwmWywiew);
	
	  
   
   }
   
   
   /***************************************
   *** zwraca wartosci ***********************
   ** -1 gdy klikniecie bylo nie istotne
   ** 0..100 nastaw ręczny PWM 
   ** >100 gdy kliknieto w jeden z przycikow na wiatraku program predefiniowany
   */
   int16_t CWentGUI::Touch(uint16_t x,uint16_t y)
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
		  double xx=(double)x-(double)_cx-(double)_x;
		  double yy=(double)y-(double)_cy-(double)_y;
     
		  double r2=xx*xx+yy*yy;
		  double rs=sqrt(r2);
     // Serial.print(xx);Serial.print("; ");Serial.print(yy);Serial.print("; r= ");Serial.print(rs);
		  if( rs>=_r && rs<=_r+_gr)
		  {
      double a=0;
     if(xx<0&&yy<0)a=360+asin(xx/rs)*(57296 / 1000); ///IV
     if(xx>0&&yy<0)a=asin(xx/rs)*(57296 / 1000); //I
     if(xx>0&&yy>0)a=180-asin(xx/rs)*(57296 / 1000);  // II
     if(xx<0&&yy>0)a=180-asin(xx/rs)*(57296 / 1000);; //III
     
			//Serial.print("; a=");Serial.println(a);
			int16_t ret= map(a, 0, 270, 0, 100);
			//Serial.print("; ret=");Serial.println(ret);
			ret=ret;
			return ret;
		  }    
	   }

     //kliknieto gdzies poza guzikami i kolkiem nic nie rob
     if(kliknietyBtn<0)return kliknietyBtn;

     
	      // zapalenie stanow odpowiednich butonow
	   for(int i=0;i<BMP_ILE;i++)
	   {
		   if(kliknietyBtn==i)
		   {
			   wentBtn[i]->zmienStan(BTN_STAN_AKTYWNY_WYBRANY);
			  
		   }else
		   {
			   wentBtn[i]->zmienStan(BTN_STAN_AKTYWNY);
		   }
	   }
	   return pwmPoz[kliknietyBtn];
   }
   void CWentGUI::zmienStan(uint8_t stan)
   {
	   
   }
   //#define ABS(x) (((x)<0)?(0-(x)):(x))

   
   
	uint8_t CWentGUI::inCircle( uint16_t x, uint16_t y,uint8_t R )
	{ //Serial.print("inCicle R=");

 if(x>_x+_cx-R&&x<_x+_cx+R&&y>_y+_cy-R&&y<_y+_cy+R)return 1;
 else
 return 0;
 //Serial.print(R);Serial.print(" ");
	  uint16_t dx = abs(x-_cx-_x);
  // Serial.println(dx);
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
//	  if(dx*dx + dy*dy <= R*R ) to zakomentowane bo chodzi tylko o wykluczenie nie istotnych, mnozenie i tak robie pozniej
		return 1;
	//  else 
		//return 0;
	}


