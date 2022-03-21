#include "Ex/Systems/Logger.h"

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#define FORMAT_SIZE 1024
#define MESSAGE_SIZE 1024

char format[FORMAT_SIZE] = {};
char message[MESSAGE_SIZE] = {};

void ExLogInternal(const char* function, const char* file, i32 line, char* fmt, ...)
{
    // Build format string
    {
        memset(format, 0, FORMAT_SIZE);
        snprintf(format, FORMAT_SIZE, "%s(%d) - %s - %s", file, line, function, fmt);
    }

    // Build message string
    {
        memset(message, 0, MESSAGE_SIZE);

        va_list args;
        va_start(args, fmt);
        vsnprintf(message, MESSAGE_SIZE, format, args);
        va_end(args);
    }

    // TODO: print message to console and/or to file
    {
        puts(message);
    }
}
