#ifndef CEkran_h
#define CEkran_h
#include "CButton.h"

#define ID_EKRAN_INFO 0
#define ID_EKRAN_DASHBOARD 1
#define ID_EKRAN_DEBUG 2
#define ID_EKRAN_PAINT 3

#define ILE_MENU_BTN 4

class CLcd;
class CButton;

const char *plikiBMP_B[] = {"iB.bmp","dB.bmp","mB.bmp","iP.bmp"};
const char *plikiBMP_Z[] = {"iZ.bmp","dZ.bmp","mZ.bmp","iZ.bmp"};

class CEkran
{
	///elementy menu gora
	/// elementy menu dol
	CButton* menuDolBtn[ILE_MENU_BTN];
	///////////////////////
	CLcd *_lcd;
	uint8_t ekranID;
	public:
	CEkran(CLcd *lcd);
	~CEkran()
	{
		for(int i=0;i<ILE_MENU_BTN;i++)
		{
			if(menuDolBtn[i]!=NULL)
			{
				delete menuDolBtn[i];
				menuDolBtn[i]=NULL;
			}
		}
	};
	
	void RysujZTlem();
	void RysujMenuGora();
	void RysujMenuDol();
	void virtual Rysuj()=0;
}


#endif