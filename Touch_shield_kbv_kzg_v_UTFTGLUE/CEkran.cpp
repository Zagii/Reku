
#include "CEkran.h"

CEkran::CEkran(CLcd *lcd,uint8_t ekranID)
{
	_lcd=lcd;
	_ekranID=ekranID;
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
	
}

virtual void CEkranInfo::Touch(uint8_t x, uint8_t y)
{
	
}