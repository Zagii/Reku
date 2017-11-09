#ifndef CEkran_h
#define CEkran_h
#include "CButton.h"
#include "CWiatrak.h"
#include "CKomora.h"

#define ID_EKRAN_INFO 0
#define ID_EKRAN_DASHBOARD 1
#define ID_EKRAN_DEBUG 2
#define ID_EKRAN_PAINT 3

#define ILE_MENU_BTN 4

class CLcd;
class CButton;
class CWiatrak;
class CKomora;

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
	CEkran(CLcd *lcd,uint8_t _ekranID);
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
	uint8_t dajEkranID(){return ekranID;};
	void RysujZTlem(CWiatrak wiatraki[], CKomora komory[]);
	void RysujMenuGora(CWiatrak wiatraki[], CKomora komory[]);
	void RysujMenuDol();
	virtual void  Rysuj(CWiatrak wiatraki[], CKomora komory[])=0;
	virtual void  Touch(uint8_t x, uint8_t y)=0;
};

class CEkranInfo: public CEkran
{
	public:
	CEkranInfo(CLcd *lcd,uint8_t _ekranID):CEkran(lcd,_ekranID){};
	virtual void Rysuj(CWiatrak wiatraki[], CKomora komory[]);
	virtual void Touch(uint8_t x, uint8_t y);
};

#endif