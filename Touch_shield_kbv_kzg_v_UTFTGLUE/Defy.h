#ifndef Defy_h
#define Defy_h
#include <arduino.h>

#define JSON_PWM_NAWIEW 'N'  // pwm wiatrak 1
#define JSON_PWM_WYWIEW 'W'   // pwm wiatrak 2
#define JSON_ROZMRAZANIE_WIATRAKI 'R' //
#define JSON_ROZMRAZANIE_GGWC 'G' //
#define JSON_KOMINEK 'K' //
#define JSON_AUTO 'A' //tryb automatycznego dobrania predkosci

#define KOMORA_CZERPNIA 0
#define KOMORA_WYRZUTNIA 1
#define KOMORA_NAWIEW 2
#define KOMORA_WYWIEW 3
#define KOMORA_ZEWN 4
#define KOMORA_SZT 4
#define WIATRAKI_SZT 2


#define DEBUG   //If you comment this line, the DPRINT & DPRINTLN lines are defined as blank.
#ifdef DEBUG    //Macros are usually in all capital letters.
  #define DPRINT(...)    Serial.print(__VA_ARGS__)     //DPRINT is a macro, debug print
  #define DPRINTLN(...)  Serial.println(__VA_ARGS__)   //DPRINTLN is a macro, debug print with new line
#else
  #define DPRINT(...)     //now defines a blank line
  #define DPRINTLN(...)   //now defines a blank line
#endif


bool isFloatChars(char * ctab);
bool isFloatString(String tString);
bool isIntChars(char * ctab) ;
 
#endif
