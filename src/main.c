// Standard includes.
#include "stdio.h"

// System includes
#include <math.h>
#include "arm_math.h" 

// Scheduler includes.
#include "FreeRTOS.h"
#include "task.h"
#include "config.h"

// Application includes. 
#include "usbcdc.h"

// Task priorities.
#define ledblink_PRIORITY				( tskIDLE_PRIORITY + 1 )
#define usbecho_PRIORITY				( tskIDLE_PRIORITY + 1 )

/* -------------- Task prototypes ---------------*/

// Task that handles the I'm Alive led control
extern void vLedBlink( void *pvParameters);

// Task that echoes a received char via serial interface .
extern void vUSBecho( void *pvParameters );

// Function Prototypes


/*-----------------------------------------------------------*/
int main( void )
{

    // Initialize system's peripherals
	USBinit();

    // Create the LED Blink task
    	xTaskCreate( vLedBlink, ( signed char * ) "LED", configMINIMAL_STACK_SIZE, ( void * ) NULL, ledblink_PRIORITY, NULL );
	
    // Create the Serial USB echo task.
	xTaskCreate( vUSBecho, ( signed char * ) "USB", configMINIMAL_STACK_SIZE, ( void * ) NULL, usbecho_PRIORITY, NULL );
    
    // Start the scheduler.
	vTaskStartScheduler();

    // Will only get here if there was insufficient memory to create the idle
    // task.  The idle task is created within vTaskStartScheduler()...
	for( ;; );
}

/*-----------------------------------------------------------*/

void vApplicationStackOverflowHook( xTaskHandle pxTask, signed char *pcTaskName )
{
	// This function will get called if a task overflows its stack.

	( void ) pxTask;
	( void ) pcTaskName;

	for( ;; )
	{

	}
}
/*-----------------------------------------------------------*/

