//////////////////////////////////////////////////////////////////////////////
// prog: mutex.c
// comm: demonstrates the usage of mutual exclusion semaphores or 'mutexes'
// auth: MSC
//////////////////////////////////////////////////////////////////////////////

#include <includes.h>
#include "main.h"


// action(), performs actions on shared resource SD
// taskname, lcd-string, leds-on
// for all tasks, the same action is performed
// the synchronization can be manipulated in 2 ways with keys:
// 1. the taskdelaytime can be altered: OSTimeDelay
// 2. the leddisplaytime: hardware-delay
void action(char *taskname, char *lcd, int leds);
void action(char *taskname, char *lcd, int leds)
{
    INT8U         error;
    OS_MUTEX_DATA osmd;

	// wait for os to give me the semaphore, if so... i rule until the post...
	OSMutexPend(MutexHandle, WAIT_FOREVER, &error);
	if (error)
    {
   		UART_puts("\n\r0:mutexpend error=");
 		UART_putint(error);
   		UART_puts("\n\r");
    }

	//#ifdef OUTPUT_DEBUG
	if (Uart_debug_out & MUTEX_DEBUG_OUT)
	{
		OSMutexQuery (MutexHandle, &osmd); // get prty of actual task using this handle
		UART_puts("\n\r"); UART_puts(lcd); UART_puts(taskname); UART_puts(" \ttask-prio="); UART_putint(osmd.OSOwnerPrio);
	}
    //#endif

    LCD_puts(lcd);       // output to lcd
   	LED_put(leds);       // set leds
   	DELAY_ms(LED_DELAY); // ugly timer, it graps the processor

	// finished handling shared resources, so release the mutex sem
	OSMutexPost(MutexHandle);

	// be sure to insert a delay into the loop, to give other tasks opportunity to rock 'n roll
   	OSTimeDly(LOOP_DELAY);
}


//////////////////////////////////////////////////////////////////////////////
// func: MutexTasks 1..4
// args: void *pdata, needed by os
// comm: to test semaphores; func waits for semaphore, then plays with leds,
//       then releases the sem
//////////////////////////////////////////////////////////////////////////////
void MutexTask1 (void *pdata)
{
	UART_puts((char *)__func__); UART_puts("started\n\r");

	while(TRUE)
	{
        action((char *)__func__, "1--- ", 1+2);
	}
}

void MutexTask2 (void *pdata)
{
	UART_puts((char *)__func__); UART_puts("started\n\r");

	while(TRUE)
	{
        action((char *)__func__, "-2-- ", 4+8);
	}
}

void MutexTask3 (void *pdata)
{
	UART_puts((char *)__func__); UART_puts("started\n\r");

	while(TRUE)
	{
        action((char *)__func__, "--3- ", 16+32);
	}
}

void MutexTask4 (void *pdata)
{
	UART_puts((char *)__func__); UART_puts("started\n\r");

	while(TRUE)
	{
        action((char *)__func__, "---4 ", 64+128);
	}
}
