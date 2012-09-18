
#include "FreeRTOS.h"
#include "queue.h"


// Functions for virtual serial interface

int  VCOM_getchar( void );
int8_t  VCOM_putchar( char );
int8_t  VCOM_puts( char * string);
void USBinit( void );




