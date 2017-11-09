
#include "CEkran.h"

CEkran::CEkran(CLcd *lcd)
{
	_lcd=lcd;
	// tworzy buttony menu dolnego
	for(int i=0;i<ILE_MENU_BTN;i++)
	{
		menuDolBtn[i]=new CButtonWnd(this,i,i*45+4+i*25,190,45,45,plikiBMP_B,plikiBMP_Z);
	}

};

void CEkran::RysujZTlem()
{
	
}

void CEkran::RysujMenuGora()
{
	
}

void CEkran::RysujMenuDol()
{
	
}