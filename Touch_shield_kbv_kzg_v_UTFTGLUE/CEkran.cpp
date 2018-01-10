
#include "CEkran.h"

 char *plikiBMP_B[] = {"iB.bmp","dB.bmp","mB.bmp","iP.bmp"};
 char *plikiBMP_Z[] = {"iZ.bmp","dZ.bmp","mZ.bmp","iZ.bmp"};

CEkran::CEkran(CLcd *lcd,uint8_t ekranID,rozkazJson rozkazCallBack)
{
  Serial.print("start konstruktor CEkran " );
  Serial.println(ekranID);
	_lcd=lcd;
	_ekranID=ekranID;
	_rozkazCallBack=rozkazCallBack;
	// tworzy buttony menu dolnego
	for(int i=0;i<ILE_MENU_BTN;i++)
	{
		menuDolBtn[i]=new CButtonWnd(_lcd,i,i*45+4+i*25,190,45,45,(char*)plikiBMP_B[i],(char*)plikiBMP_Z[i]);
		
	}
  ZmienStanMenuDol(_ekranID);
  Serial.println("koniec konstruktora CEkran");
};
void CEkran::ZmienStanMenuDol(uint8_t naEkran)
{
  _ekranID=naEkran;
    for(int i=0;i<ILE_MENU_BTN;i++)
  {
    if(i==_ekranID)
      menuDolBtn[i]->zmienStan(BTN_STAN_AKTYWNY_WYBRANY);
    else
      menuDolBtn[i]->zmienStan(BTN_STAN_AKTYWNY);
  }
}
void CEkran::RysujZTlem(CWiatrak wiatraki[], CKomora komory[])
{
	_lcd->clrScr();
	Rysuj( wiatraki, komory);
}

void CEkran::RysujMenuGora(CWiatrak wiatraki[], CKomora komory[])
{
	//kreska oddzielajaca
	_lcd->setColor(255,0,0);
	_lcd->drawLine(0,25,320,25);
}

void CEkran::RysujMenuDol()
{
	//kreska oddzielajaca
	_lcd->setColor(255, 0, 0);
	_lcd->drawLine(0,185,320,185);
	for(int i=0;i<ILE_MENU_BTN;i++)
	{
		menuDolBtn[i]->Rysuj();
	}
}



//////////////////////////ekran info
void CEkranInfo::loop(CWiatrak Wiatraki[], CKomora Komory[])
{
  _wentGUI->RysujLuk(Wiatraki[0].dajZadanaPredkoscProcent(),Wiatraki[1].dajZadanaPredkoscProcent());
  }
 void CEkranInfo::Rysuj(CWiatrak wiatraki[], CKomora komory[])
{
	RysujMenuGora(wiatraki,komory);
	_wentGUI->Rysuj(wiatraki[0].dajZadanaPredkoscProcent(),wiatraki[1].dajZadanaPredkoscProcent(),CWentGUI_PWM_auto);
	RysujMenuDol();
	// rysuj domek i inne duperele
	_lcd->setColor(255,255,255);
	uint16_t x=10;uint16_t y=50;
	uint16_t w=100;uint16_t h=100;
	
	_lcd->kopnietyKwadrat(x,y+25+5,x+w/2,y+5,5,1,-1);
	_lcd->kopnietyKwadrat(x+w/2,y+5,x+w,y+25,5,0,1);
	_lcd->kopnietyKwadrat(x+w/2,y+5,x+w,y+25,5,0,1);
	_lcd->kopnietyKwadrat(x+w/2,y+5,x+w,y+25,5,0,1);
	
}


  bool CEkranInfo::Touch(uint16_t x, uint16_t y)
{
	// zmien wyswietlane GUI jesli w ramach tego samego ekranu
  int16_t retWent=_wentGUI->Touch(x,y);
  if(retWent>0)	//czyli ze kliknieto w wiatrakGUI i trzeba to obsłuzyć
  {
    //_lcd->przerysujEkran=true;
    
	  if(retWent>=0 &&retWent<=100)	//nastawy ogólne podane w % predkosci wiatrakow (4 biegi lub manual)
	  {
		  _rozkazCallBack(JSON_PWM_NAWIEW,retWent);  
		  _rozkazCallBack(JSON_PWM_WYWIEW,retWent);
    
	  }else
	  {
		  switch(retWent)
		  {
			  case CWentGUI_PWM_kominek:
				_rozkazCallBack(JSON_KOMINEK,1);
			  break;
			  case CWentGUI_PWM_rozmrazanie:
				_rozkazCallBack(JSON_ROZMRAZANIE_WIATRAKI,1);
			  break;
			  case CWentGUI_PWM_auto:
				_rozkazCallBack(JSON_AUTO,1);
			  break;
			  default:
				Serial.println("Nieznany tryb CEkranInfo::Touch");
			  break;
		  }
	  }
	return true;
  }
	// tworzy rozkaz do obslugi w klasie rodzica
  int but=-1;
	for(int i=0;i<ILE_MENU_BTN;i++)
  {
    unsigned long b=menuDolBtn[i]->czyKlik(x,y);
    if(b==KLIK_WCISKANY)
    {but=i;}
  }
  if(but>=0)
  {
   // ZmienStanMenuDol(but);
    _lcd->zmienEkran(but);
  }
	//czy trzeba przerysowac ekran?
	
return false;	
}
 void CEkranInfo::begin()
{
	_wentGUI= new CWentGUI(_lcd,   175, 26);
	_wentGUI->begin();
  Serial.println("koniec begin CEkranInfo");
}


////////////////////////// ekran dashboard
void CEkranDashboard::loop(CWiatrak Wiatraki[], CKomora Komory[])
{}
 void CEkranDashboard::Rysuj(CWiatrak wiatraki[], CKomora komory[])
{
	RysujMenuGora(wiatraki,komory);
	_lcd->setColor(255,255,255);
	_lcd->print("Tryb Dashboard",50, 100);
	RysujMenuDol();
	
	
}


  bool CEkranDashboard::Touch(uint16_t x, uint16_t y)
{
	
	
	//////////////////////////////////////// menu dół
	// tworzy rozkaz do obslugi w klasie rodzica
  int but=-1;
	for(int i=0;i<ILE_MENU_BTN;i++)
  {
    unsigned long b=menuDolBtn[i]->czyKlik(x,y);
    if(b==KLIK_WCISKANY) {but=i;}
  }
  if(but>=0)
  {
    ZmienStanMenuDol(but);
    _lcd->zmienEkran(but);
  }
	//czy trzeba przerysowac ekran?
	
return false;	
}
 void CEkranDashboard::begin()
{
  Serial.println("koniec begin CEkranDashboard");
}

////////////////////////// ekran CEkranDebug
 void CEkranDebug::loop(CWiatrak Wiatraki[], CKomora Komory[])
 {}
 void CEkranDebug::Rysuj(CWiatrak wiatraki[], CKomora komory[])
{
	//RysujMenuGora(wiatraki,komory);
	_lcd->setColor(255,255,255);
	_lcd->print("Tryb debug",100, 0);
	uint16_t wysWiersza=20;
	uint16_t szerKol=60;
	
	///// dane z zewnątrz
	_lcd->print("Na zewnatrz",0,0 );
	_lcd->print("T= ",0,1*wysWiersza );
	_lcd->print("H= ",0,2*wysWiersza );
	_lcd->print("P= ",0,3*wysWiersza );
//	_lcd->print("Powietrze ",0,4*wysWiersza );
	//dane za filtrem czerpni
	_lcd->print("Za filtrem",0,6*wysWiersza );
	_lcd->print("P= ",0,7*wysWiersza );
	//_lcd->print("Powietrze ",0,8*wysWiersza );
	//dane za nagrzewnica ggwc
	_lcd->print("Za nagrzewnica",szerKol,0 );
	_lcd->print("T= ",szerKol,1*wysWiersza );
	_lcd->print("H= ",szerKol,2*wysWiersza );
	_lcd->print("P= ",szerKol,3*wysWiersza );
	//_lcd->print("Powietrze ",szerKol,4*wysWiersza );
	// dane za wiatrakiem komora czerpnia
	_lcd->print("Za wiatrakiem czerpni",szerKol,6*wysWiersza );
	_lcd->print("T= ",szerKol,7*wysWiersza );
	_lcd->print("H= ",szerKol,8*wysWiersza );
	_lcd->print("P= ",szerKol,9*wysWiersza );
	//_lcd->print("Powietrze ",szerKol,10*wysWiersza );
	// dane za wymiennikiem komora nawiew
	_lcd->print("Za wymiennikiem - nawiew",szerKol,0 );
	_lcd->print("T= ",2*szerKol,1*wysWiersza );
	_lcd->print("H= ",2*szerKol,2*wysWiersza );
	_lcd->print("P= ",2*szerKol,3*wysWiersza );
	//_lcd->print("Powietrze ",2*szerKol,4*wysWiersza );
	// dane przed filtrem wywiew
	_lcd->print("Przed filtrem wywiew",2*szerKol,6*wysWiersza );
	_lcd->print("T= ",2*szerKol,7*wysWiersza );
	_lcd->print("H= ",2*szerKol,8*wysWiersza );
	_lcd->print("P= ",2* szerKol,9*wysWiersza );
	//_lcd->print("Powietrze ",2* szerKol,10*wysWiersza );
	//dane za filtrem wywiew
	_lcd->print("Za filtem - wywiew",3*szerKol,0 );
	_lcd->print("T= ",3*szerKol,1*wysWiersza );
	_lcd->print("H= ",3*szerKol,2*wysWiersza );
	_lcd->print("P= ",3*szerKol,3*wysWiersza );
	//_lcd->print("Powietrze ",3*szerKol,4*wysWiersza );
	//dane za wiatrakiem komora wywiew
	_lcd->print("Za wiatrakiem - wywiew",3*szerKol,6*wysWiersza );
	_lcd->print("T= ",3*szerKol,7*wysWiersza );
	_lcd->print("H= ",3*szerKol,8*wysWiersza );
	_lcd->print("P= ",3* szerKol,9*wysWiersza );
	//_lcd->print("Powietrze ",3* szerKol,10*wysWiersza );
	//dane za wymiennikiem komora wyrzutnia
	_lcd->print("Wyrzutnia",4*szerKol,0 );
	_lcd->print("T= ",4*szerKol,1*wysWiersza );
	_lcd->print("H= ",4*szerKol,2*wysWiersza );
	_lcd->print("P= ",4*szerKol,3*wysWiersza );
	
	//no i przyciski
	//wiatrak 1 (+/-), wiatrak 2(+/-), ggwc(+/- lub on off)
	// tryby auto, kominek, rozmrazanie
	//konfig delta kominek, delta rozmr wiatrakowe,
	//czy jest ggwc
	
	RysujMenuDol();
		
}


  bool CEkranDebug::Touch(uint16_t x, uint16_t y)
{
	
	
	//////////////////////////////////////// menu dół
	// tworzy rozkaz do obslugi w klasie rodzica
  int but=-1;
	for(int i=0;i<ILE_MENU_BTN;i++)
  {
    unsigned long b=menuDolBtn[i]->czyKlik(x,y);
    if(b==KLIK_WCISKANY) {but=i;}
  }
  if(but>=0)
  {
    ZmienStanMenuDol(but);
    _lcd->zmienEkran(but);
  }
	//czy trzeba przerysowac ekran?
	
return false;	
}
 void CEkranDebug::begin()
{
  Serial.println("koniec begin CEkranDebug");
}

