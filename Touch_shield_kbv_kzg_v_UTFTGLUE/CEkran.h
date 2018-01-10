#ifndef CEkran_h
#define CEkran_h
#include <ArduinoJson.h>
#include "CButtonWnd.h"
#include "CWiatrak.h"
#include "CKomora.h"
#include "CWentGUI.h"
#include "Defy.h"

#define ID_EKRAN_INFO 0
#define ID_EKRAN_DASHBOARD 1
#define ID_EKRAN_DEBUG 2
#define ID_EKRAN_PAINT 3

#define ILE_MENU_BTN 4

class CLcd;
class CButtonWnd;
class CWiatrak;
class CKomora;
class CWentGUI;



class CEkran
{
  protected:
	///elementy menu gora
	/// elementy menu dol
	CButtonWnd* menuDolBtn[ILE_MENU_BTN];
	///////////////////////

	uint8_t _ekranID;
	
	rozkazJson _rozkazCallBack;
	public:
  CLcd *_lcd;
	CEkran(CLcd *lcd,uint8_t _ekranID,rozkazJson rozkazCallBack);
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
	uint8_t dajEkranID(){return _ekranID;};
	void RysujZTlem(CWiatrak wiatraki[], CKomora komory[]);
	void RysujMenuGora(CWiatrak wiatraki[], CKomora komory[]);
	void RysujMenuDol();
  void ZmienStanMenuDol(uint8_t naEkran);
	//JsonObject& dajRozkaz{}{return jsonRozkaz;};
	void ZrobRozkaz(uint8_t pwmNawiew, uint8_t pwmWywiew,uint8_t tryb);
	virtual void  Rysuj(CWiatrak wiatraki[], CKomora komory[])=0;
	virtual bool  Touch(uint16_t x, uint16_t y)=0;	//zwraca true jesli powstal rozkaz do obslugi dla petli glownej
	virtual void begin()=0;
 virtual void loop(CWiatrak Wiatraki[], CKomora Komory[])=0;
};

class CEkranInfo: public CEkran
{
	CWentGUI *_wentGUI;
	public:
	CEkranInfo(CLcd *lcd,uint8_t _ekranID,rozkazJson rozkazCallBack):CEkran(lcd,_ekranID,rozkazCallBack){};
	virtual void Rysuj(CWiatrak wiatraki[], CKomora komory[]);
	virtual bool Touch(uint16_t x, uint16_t y);
    virtual void begin();
    virtual void loop(CWiatrak Wiatraki[], CKomora Komory[]);
};

class CEkranDashboard: public CEkran
{

	public:
	CEkranDashboard(CLcd *lcd,uint8_t _ekranID,rozkazJson rozkazCallBack):CEkran(lcd,_ekranID,rozkazCallBack){};
	virtual void Rysuj(CWiatrak wiatraki[], CKomora komory[]);
	virtual bool Touch(uint16_t x, uint16_t y);
    virtual void begin();
    virtual void loop(CWiatrak Wiatraki[], CKomora Komory[]);
};

class CEkranDebug: public CEkran
{

	public:
	CEkranDebug(CLcd *lcd,uint8_t _ekranID,rozkazJson rozkazCallBack):CEkran(lcd,_ekranID,rozkazCallBack){};
	virtual void Rysuj(CWiatrak wiatraki[], CKomora komory[]);
	virtual bool Touch(uint16_t x, uint16_t y);
    virtual void begin();
    virtual void loop(CWiatrak Wiatraki[], CKomora Komory[]);
};

#endif
