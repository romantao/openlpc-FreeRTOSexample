// FreeRTOS LedBlink task

// Standard includes.
#include "stdio.h"

// Scheduler includes.
#include "FreeRTOS.h"
#include "task.h"

// System includes
#include "config.h"
#include "LPC17xx.h"

void vLedBlink( void *pvParameters)
{
	//Execution Rate 1Hz
	portTickType xLastWakeTime;
	const portTickType xFrequency = FREQUENCY( 1 );
	
	char STATUS = 0; //Led Status

	// Just to prevent compiler warnings about the unused parameter.
	( void ) pvParameters;

	LPC_GPIO1->FIODIR |= (1<<29);

	xLastWakeTime = xTaskGetTickCount();

	for( ;; )
	{
		vTaskDelayUntil(&xLastWakeTime, xFrequency);
	
		if (STATUS)
		{
			LPC_GPIO1->FIOCLR = (1<<29);
			STATUS = ~STATUS;
		}		
		else
		{
			LPC_GPIO1->FIOSET = (1<<29);
			STATUS = ~STATUS;
		}
			
	}
}
