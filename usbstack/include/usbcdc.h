
#include "FreeRTOS.h"
#include "queue.h"


// Functions for virtual serial interface

int  VCOM_getchar( void );
int  VCOM_putchar( char );
int  VCOM_puts( char * string);
void USBinit( void );




