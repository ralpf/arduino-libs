#pragma once

#define PRINTER_ESP23


#ifdef PRINTER_ESP23
    #define PRINTER_BUFFER_SZ 512
    #define PRINTER_BAUD      921600
#else
    #define PRINTER_BUFFER_SZ 256
    #define PRINTER_BAUD      9600
#endif



void SPrint(const char *format, ...);
void SPrint(char ch);
