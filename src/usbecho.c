// FreeRTOS USB_echo task

// Standard includes.
#include "stdio.h"

// Scheduler includes.
#include "FreeRTOS.h"
#include "task.h"

// System includes
#include "usbcdc.h"
#include "config.h"

void vUSBecho( void *pvParameters )
{
	int c;
	
	/* Just to prevent compiler warnings about the unused parameter. */
	( void ) pvParameters;

	// echo any character received (do USB stuff in interrupt)
	for( ;; )
	{
		c = VCOM_getchar();
		if (c != EOF) 
		{
			//VCOM_putchar(c);
			VCOM_puts("TESTE\r\n");
		}
	}
}
