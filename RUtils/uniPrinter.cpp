#include "uniPrinter.h"

#include <asyncWebsocket.h>
#include <Arduino.h>


static char printerBuffer[PRINTER_BUFFER_SZ];
static uint8_t wasInited;

//......................................................

void init_serial()
{
    Serial.begin(PRINTER_BAUD);
    Serial.println();
    Serial.println();
    Serial.print("Inited Serial with boud: ");
    Serial.println(PRINTER_BAUD);
    wasInited = true;
}

//......................................................

void SPrint(const char *format, ...)
{
    if (!wasInited) init_serial();
    va_list args;
    va_start(args, format);
    vsnprintf(printerBuffer, PRINTER_BUFFER_SZ, format, args);
    Serial.println(printerBuffer);
    websocket_log(printerBuffer);
    va_end(args);
}


void SPrint(char ch)
{
    if (!wasInited) init_serial();
    Serial.print(ch);
}