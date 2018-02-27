
#include "Defy.h"

void dPrintf(const char *format, ...)
{
static char line[80];
va_list args;
va_start(args, format);
int len = vsnprintf(line, sizeof(line), format, args);
va_end(args);
for (char *p = &line[0]; *p; p++) {
    if (*p == '\n') {
        Serial.write('\r');
    }
    Serial.write(*p);
}
if (len >= sizeof(line))
    Serial.write('$');
}
