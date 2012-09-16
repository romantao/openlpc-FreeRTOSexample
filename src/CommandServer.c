/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"

/* FreeRTOS+CLI includes. */
#include "FreeRTOS_CLI.h"

/* Dimensions the buffer into which input characters are placed. */
#define cmdMAX_INPUT_SIZE	60

/* Dimensions the buffer into which string outputs can be placed. */
#define cmdMAX_OUTPUT_SIZE	1024

/* Dimensions the buffer passed to the recvfrom() call. */
#define cmdSOCKET_INPUT_BUFFER_SIZE 60


/*-----------------------------------------------------------*/

/* 
 * Task that provides the input and output for the FreeRTOS+CLI command
 * interpreter.  In this case a UDP port is used.  See the URL in the comments
 * within main.c for the location of the online documentation.
 */

static void prvUARTCommandConsoleTask( void *pvParameters )
{
int8_t cRxedChar, cInputIndex = 0, *pcOutputString;
static int8_t cInputString[ cmdMAX_INPUT_SIZE ];
portBASE_TYPE xReturned;
//Peripheral_Descriptor_t xConsoleUART;

	( void ) pvParameters;

	/* Obtain the address of the output buffer.  Note there is no mutual
	exclusion on this buffer as it is assumed only one command console
	interface will be used at any one time. */
	pcOutputString = FreeRTOS_CLIGetOutputBuffer();

	/* Open the UART port used for console input.  The second parameter
	(ulFlags) is not used in this case.  The default board rate is set by the
	boardDEFAULT_UART_BAUD parameter.  The baud rate can be changed using a
	FreeRTOS_ioctl() call with the ioctlSET_SPEED command. */
	xConsoleUART = FreeRTOS_open( boardCOMMAND_CONSOLE_UART, ( uint32_t ) cmdPARAMTER_NOT_USED );
	configASSERT( xConsoleUART );

	/* Change the Tx usage model from straight polled mode to use zero copy
	buffers with interrupts.  In this mode, the UART will transmit characters
	directly from the buffer passed to the FreeRTOS_write()	function. */
	xReturned = FreeRTOS_ioctl( xConsoleUART, ioctlUSE_ZERO_COPY_TX, cmdPARAMTER_NOT_USED );
	configASSERT( xReturned );

	/* Change the Rx usage model from straight polled mode to use a character
	queue.  Character queue reception is appropriate in this case as characters
	can only be received as quickly as they can be typed, and need to be parsed
	character by character. */
	xReturned = FreeRTOS_ioctl( xConsoleUART, ioctlUSE_CHARACTER_QUEUE_RX, ( void * ) cmdMAX_INPUT_SIZE );
	configASSERT( xReturned );

	/* By default, the UART interrupt priority will have been set to the lowest
	possible.  It must be kept at or below configMAX_LIBRARY_INTERRUPT_PRIORITY,
	but	can be raised above its default priority using a FreeRTOS_ioctl() call
	with the ioctlSET_INTERRUPT_PRIORITY command. */
	xReturned = FreeRTOS_ioctl( xConsoleUART, ioctlSET_INTERRUPT_PRIORITY, ( void * ) ( configMIN_LIBRARY_INTERRUPT_PRIORITY - 1 ) );
	configASSERT( xReturned );

	/* Send the welcome message. */
	if( FreeRTOS_ioctl( xConsoleUART, ioctlOBTAIN_WRITE_MUTEX, cmd50ms ) == pdPASS )
	{
		FreeRTOS_write( xConsoleUART, pcWelcomeMessage, strlen( ( char * ) pcWelcomeMessage ) );
	}

	for( ;; )
	{
		/* Only interested in reading one character at a time. */
		cRxedChar = VCOM_getchar(void)

		if( cRxedChar == '\n' )
		{
			/* The input command string is complete.  Ensure the previous
			UART transmission has finished before sending any more data.
			This task will be held in the Blocked state while the Tx completes,
			if it has not already done so, so no CPU time will be wasted by
			polling. */
			/*if( FreeRTOS_ioctl( xConsoleUART, ioctlOBTAIN_WRITE_MUTEX, cmd50ms ) == pdPASS )
			{
				// Start to transmit a line separator, just to make the output
				//easier to read.
				FreeRTOS_write( xConsoleUART, pcNewLine, strlen( ( char * ) pcNewLine ) );
			}
			*/
			VCOM_PUTS("\r\n>");
			/* Pass the received command to the command interpreter.  The
			command interpreter is called repeatedly until it returns
			pdFALSE as it might generate more than one string. */
			do
			{
				/* Once again, just check to ensure the UART has completed
				sending whatever it was sending last.  This task will be held
				in the Blocked state while the Tx completes, if it has not
				already done so, so no CPU time	is wasted polling. */
				//xReturned = FreeRTOS_ioctl( xConsoleUART, ioctlOBTAIN_WRITE_MUTEX, cmd50ms );
				
				if( xReturned == pdPASS )
				{
					/* Get the string to write to the UART from the command
					interpreter. */
					xReturned = FreeRTOS_CLIProcessCommand( cInputString, pcOutputString, configCOMMAND_INT_MAX_OUTPUT_SIZE );

					/* Write the generated string to the UART. */
					//FreeRTOS_write( xConsoleUART, pcOutputString, strlen( ( char * ) pcOutputString ) );
					VCOM_puts(pcOutputString);
				}

			} while( xReturned != pdFALSE );

			/* All the strings generated by the input command have been sent.
			Clear the input	string ready to receive the next command. */
			cInputIndex = 0;
			memset( cInputString, 0x00, cmdMAX_INPUT_SIZE );

			/* Ensure the last string to be transmitted has completed. */
			if( FreeRTOS_ioctl( xConsoleUART, ioctlOBTAIN_WRITE_MUTEX, cmd50ms ) == pdPASS )
			{
				/* Start to transmit a line separator, just to make the output
				easier to read. */
				FreeRTOS_write( xConsoleUART, "\r\n>", strlen( "\r\n>" ) );
			}
		}
		else
		{
			if( cRxedChar == '\r' )
			{
				/* Ignore the character. */
			}
			else if( cRxedChar == '\b' )
			{
				/* Backspace was pressed.  Erase the last character in the
				string - if any. */
				if( cInputIndex > 0 )
				{
					cInputIndex--;
					cInputString[ cInputIndex ] = '\0';
				}
			}
			else
			{
				/* A character was entered.  Add it to the string
				entered so far.  When a \n is entered the complete
				string will be passed to the command interpreter. */
				if( cInputIndex < cmdMAX_INPUT_SIZE )
				{
					cInputString[ cInputIndex ] = cRxedChar;
					cInputIndex++;
				}
			}
		}
	}
}
/*-----------------------------------------------------------*/



