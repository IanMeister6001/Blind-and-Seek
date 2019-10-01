//////////////////////////////////////////////////////////////////////////////
// prog: flag.c
//       shows flag event handling
// note: for OSFlagCreate, see main.c!
// auth: MSC
//////////////////////////////////////////////////////////////////////////////

#include <includes.h>
#include "main.h"

 

//////////////////////////////////////////////////////////////////////////////
// func: FlagPostTask
// posts a flag according the key pressed
// args: void *pdata
//////////////////////////////////////////////////////////////////////////////
void FlagPostTask (void *pdata)
{
    unsigned int key;
    INT8U        error;

    UART_puts((char *)__func__); UART_puts("started, press a key on the ARM-board...\n\r");

    while(TRUE)
    {
       	OSTimeDly(3);             // don't block other tasks
        if ((key=KEYS_read())==0) // read keys...
			continue;

		switch(key)
		{
 		default:
			//BUZZER_put(1100);
			OSFlagPost(FlagHandle, key, OS_FLAG_SET, &error);
		    if (Uart_debug_out & FLAG_DEBUG_OUT)
		    {
				UART_puts("\n\rFlagPostTask posts key: "); UART_putint(key); UART_puts("\n\r");
		    }
		   	OSTimeDly(LOOP_DELAY); // ugly, solution, better is to wait for pend to be done
			break;
		}
	}
}


//////////////////////////////////////////////////////////////////////////////
// func: FlagPendTask
// args: void *pdata, needed by os
// comm: waits for a specific flag to be set and reacts
//////////////////////////////////////////////////////////////////////////////
void FlagPendTask(void *pdata)
{
	INT8U    error;
	OS_FLAGS value;  // OS_FLAGS == INT16U (os_cfg.h)

	UART_puts((char *)__func__);UART_puts("started\n\r");

	while (TRUE)
	{
		value = OSFlagPend(FlagHandle,
		                   0xffff, // 4 nibbles = 2 bytes
						   OS_FLAG_WAIT_SET_ANY + OS_FLAG_CONSUME,
						   WAIT_FOREVER,
						   &error);
		
  		LED_put((INT8U)value);   // set leds, force as byte, there are only 8 leds on board
  		LCD_putint(value);       // output to lcd

	    if (Uart_debug_out & FLAG_DEBUG_OUT)
	    {
	    	UART_puts("\n\rFlagPendTask reads key: "); UART_putint(value); UART_puts("\n\r");
	    }
	}
}
