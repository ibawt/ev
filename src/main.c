#include <stdio.h>
#include "evil.h"

void ev_error(const char *fmt, ... )
{
    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
    fputs("\n", stderr);
}

void ev_logger(ev_log_level level, const char *fmt, ... )
{
    va_list args;
    va_start(args, fmt);
    vfprintf(stdout, fmt, args);
    va_end(args);
}
