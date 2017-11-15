
#include "CEkran.h"



CEkran::CEkran(CLcd *lcd,uint8_t ekranID,rozkazJson rozkazCallBack)
{
	_lcd=lcd;
	_ekranID=ekranID;
	_rozkazCallBack=rozkazCallBack;
	// tworzy buttony menu dolnego
	for(int i=0;i<ILE_MENU_BTN;i++)
	{
		menuDolBtn[i]=new CButtonWnd(this,i,i*45+4+i*25,190,45,45,plikiBMP_B,plikiBMP_Z);
		if(i==_ekranID)
			menuDolBtn[i]->zmienStan(STAN_AKTYWNY_WYBRANY);
		else
			menuDolBtn[i]->zmienStan(STAN_AKTYWNY);
	}
  
};

void CEkran::RysujZTlem(CWiatrak wiatraki[], CKomora komory[])
{
	_lcd->clrScr();
	Rysuj( wiatraki, komory);
}

void CEkran::RysujMenuGora(CWiatrak wiatraki[], CKomora komory[])
{
	//kreska oddzielajaca
	_lcd->setColor(0x000000);
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
virtual void CEkranInfo::Rysuj(CWiatrak wiatraki[], CKomora komory[])
{
	RysujMenuGora(wiatraki,komory);
	_wentGUI->Rysuj(wiatraki,komory);
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


virtual bool CEkranInfo::Touch(uint8_t x, uint8_t y)
{
	// zmien wyswietlane GUI jesli w ramach tego samego ekranu
	// tworzy rozkaz do obslugi w klasie rodzica
	
	//czy trzeba przerysowac ekran?
	
return false;	
}
virtual void CEkranInfo::begin()
{
	_wentGUI= new CWentGUI(this,   190, 40);
	_wentGUI->begin(this);
}