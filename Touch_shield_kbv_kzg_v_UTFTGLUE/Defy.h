#ifndef Defy_h
#define Defy_h

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

void dPrintf(const char *format, ...);

#endif
