#ifndef CLcd_h
#define CLcd_h



#include <arduino.h>
#include <Adafruit_GFX.h>
#include <SPI.h>          // f.k. for Arduino-1.5.2
#include <SdFat.h>           // Use the SdFat library
#include <TouchScreen.h>
#include <MCUFRIEND_kbv.h>
#include <UTFTGLUE.h>              // class methods are in here

typedef char*( * rozkazJson )( uint8_t,uint16_t);

#include "CButtonWnd.h"
#include "CWiatrak.h"
#include "CKomora.h"
#include "CWentGUI.h"
#include "CEkran.h"
// Declare which fonts we will be using
#if !defined(SmallFont)
extern uint8_t SmallFont[];    //.kbv GLUE defines as GFXFont ref
#endif




#define SD_CS 10
#if defined(__SAM3X8E__)
#undef __FlashStringHelper::F(string_literal)
#define F(string_literal) string_literal
#endif


#define MINPRESSURE 20
#define MAXPRESSURE 1000
// Assign human-readable names to some common 16-bit color values:
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

#define TFT_H 240
#define TFT_W 320

#define SWAP(a, b) {uint16_t tmp = a; a = b; b = tmp;}


#define EKRAN_INFO 0
#define EKRAN_DASHBOARD 1
#define EKRAN_DEBUG 2
#define ekran_test 3
#define ekran_debug 4
#define ekran_prosty 5
#define ILE_EKRANOW 3
 
#define BMPIMAGEOFFSET 54

#define PALETTEDEPTH   8
#define BUFFPIXEL 20

class CWentGUI;
class CEkran;
class CEkranInfo;
 
class CLcd:public UTFTGLUE
{
  public:
 
  uint8_t XM = A2;  // must be an analog pin, use "An" notation!
  uint8_t YM = 7;   // can be a digital pin
  uint8_t XP = 6;   // can be a digital pin
 
    
  uint16_t TS_LEFT =917;
  uint16_t TS_RT  = 149;
  uint16_t TS_TOP = 150;
  uint16_t TS_BOT = 958;
  char *name = "x";
    
  // For better pressure precision, we need to know the resistance
  // between X+ and X- Use any multimeter to read it
  // For the one we're using, its 300 ohms across the X plate
  TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
  TSPoint tp;
bool przerysujEkran=true;
 uint8_t ekran=0;
   uint8_t aw=0;
  // most mcufriend shields use these pins and Portrait mode:
  uint8_t YP = A1;  // must be an analog pin, use "An" notation!

uint16_t xpos=0, ypos=0;  //screen coordinates ostatni wykryty touch
  int16_t BOXSIZE;
  int16_t PENRADIUS = 3;
  uint16_t identifier, oldcolor, currentcolor;
  uint8_t Orientation = 3;    //PORTRAIT  4- ok
 uint8_t SwapXY =1;
 char* btn="test";

CEkranInfo* ekranInfo;

CEkran *ekrany[ILE_EKRANOW]; 
uint16_t read16(File& f);
uint32_t read32(File& f);
rozkazJson _rozkazCallBack;
  public:
  CLcd(rozkazJson rozkazCallBack) : UTFTGLUE(0x0154,A2,A1,A3,A4,A0){_rozkazCallBack=rozkazCallBack;};
  void begin();
  void initGUI();

  void kopnietyKwadrat(uint16_t x, uint16_t y, uint16_t x2, uint16_t y2, uint16_t g,int przesunX, int przesunY);
 
  void stary_loop();
  uint8_t loop( CWiatrak Wiatraki[], CKomora Komory[],rozkazJson rozkazCallBack);//return czy jest rozkaz od uzytkownika lcd?
  void show_Serial(void);

  void RysujMenuDol();
 
  int touch();
  void zmienEkran(uint8_t e);
  uint8_t showBMP(char *nm, int x, int y);
  void drawMidpointCircle(uint16_t cx,uint16_t cy,uint16_t radius,uint16_t startAngle, uint16_t endangle);
  void drawCirclePoints( uint16_t centerX, uint16_t centerY, uint16_t x, uint16_t y, uint16_t startAngle, uint16_t endAngle);
};

#endif
