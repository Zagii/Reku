#ifndef CLcd_h
#define CLcd_h



#include <arduino.h>
#include <Adafruit_GFX.h>
#include <SPI.h>          // f.k. for Arduino-1.5.2
#include <SdFat.h>           // Use the SdFat library
#include <TouchScreen.h>
#include <MCUFRIEND_kbv.h>
#include <UTFTGLUE.h>              // class methods are in here
#include "CButtonWnd.h"
#include "CWiatrak.h"
#include "CKomora.h"

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

#define ekran_test 0
#define ekran_debug 1
#define ekran_prosty 2
#define EKRAN_INFO 3
#define EKRAN_DASHBOARD 4
#define EKRAN_DEBUG 5

 
#define BMPIMAGEOFFSET 54

#define PALETTEDEPTH   8
#define BUFFPIXEL 20
 
class CLcd
{
  public:
 // MCUFRIEND_kbv tft;       // hard-wired for UNO shields anyway.

  uint8_t XM = A2;  // must be an analog pin, use "An" notation!
  uint8_t YM = 7;   // can be a digital pin
  uint8_t XP = 6;   // can be a digital pin
 
    
  uint16_t TS_LEFT =917;
  uint16_t TS_RT  = 149;
  uint16_t TS_TOP = 150;
  uint16_t TS_BOT = 958;
  char *name = "Unknown controller";
    
  // For better pressure precision, we need to know the resistance
  // between X+ and X- Use any multimeter to read it
  // For the one we're using, its 300 ohms across the X plate
  TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
  TSPoint tp;

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
CButtonWnd b1;
CButtonWnd b2;
CButtonWnd bplus;
CButtonWnd bminus;
CButtonWnd bInfo;
CButtonWnd bDashboard;
CButtonWnd bDebug;
uint16_t read16(File& f);
uint32_t read32(File& f);
uint8_t showBMP(char *nm, int x, int y);
  public:
  CLcd(){ };
  void begin();
  void initGUI();
  void show_tft();
  void loopDyn();
  void loopStat();
  void stary_loop();
  uint8_t loop( CWiatrak Wiatraki[], CKomora Komory[]);//return czy jest rozkaz od uzytkownika lcd?
  void show_Serial(void);

  void RysujMenuDol();
 
  int touch();
  void zmienEkran(uint8_t e);

};

#endif
