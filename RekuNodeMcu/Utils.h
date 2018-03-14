#ifndef Utils_h
#define Utils_h
#include "Defy.h"

class Utils
{
	public:
	Utils(){};

static	bool isNumber(char * tmp);
static	bool isFloatChars(char * ctab);
static bool isFloatString(String tString);
static	bool isIntChars(char * ctab);
static char * TimeToString(unsigned long t);

};




#endif
