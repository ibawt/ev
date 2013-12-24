#include <stdio.h>
#include "evil.h"

static const char* lastError = "";


void ev_error(const char *fmt, ... )
{
    char buffer[512];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);

    fprintf(stderr, "%s\n", buffer);
}
void ev_log(const char *fmt, ... )
{
    char buffer[512*10];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);

    fprintf(stdout, "%s\n", buffer);
}
