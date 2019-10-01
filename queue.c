//////////////////////////////////////////////////////////////////////////////
// prog: queue.c
//       shows queue handling: OSQPost, OSQPend/Accept, OSQFlush
// note: for OSQueueCreate, see main.c!
// auth: MSC
//////////////////////////////////////////////////////////////////////////////

#include <string.h>
#include <stdlib.h> // atoi
#include <ctype.h>  // toupper

#include "includes.h"
#include "taskcreate.h" // for task function prototypes
#include "main.h"


void outQ_member(PQ, char *);
void outQ_member(PQ pq, char *func)
{
	UART_puts("\n\r==============================\n\r");
	UART_puts(func);       UART_puts("\n\r");
	UART_puts("q.nr:   "); UART_putint(pq->nr); UART_puts("\n\r");
	UART_puts("q.text: "); UART_puts(pq->text); UART_puts("\n\r");
	UART_puts("==============================");
}



// QueuePostTask:
// 1. fills every sec the next struct out of array
// 2. posts address of that struct to os-queue
// 3. increments to next array-member
void QueuePostTask (void *pdata)
{
	PQ            pq; // pointer to Q structure
	int           i     = 0;
	int           round = 1;

	UART_puts((char *)__func__); UART_puts("started\n\r");

    while(TRUE) 
    {
       	OSTimeDly(400);

    	// time has past, fill next data structure member and post its address to os-queue
    	pq = &data_queue[i]; // data_queue is declared in main.c
    	pq->nr = i;
    	// fill text with some data
    	sprintf(pq->text, "qid=%d::%d", round, i);

	    if (Uart_debug_out & QUEUE_DEBUG_OUT)
	    	outQ_member(pq, (char *)__func__); // display data to uart

		OSQPost(QueueHandle, pq);

    	if (i == QSIZE-1) // queue is full
    	{
    		i = 0;   // start from scratch
    		round++; // next round coming up...
    	    if (Uart_debug_out & QUEUE_DEBUG_OUT)
    	    	UART_puts("\n\rQueuePostTask: done, waiting a bit...\n");
           	OSTimeDly(800); // ugly solution, synchronization needed...
    	}
    	else i++;
	}
}


// QueuePendTask:
// 1. waits until queue is full
// 2. scans thru queue, accepting every member one by one
// 3. outputs data of every q-member
// 4. cleans up queue
void QueuePendTask(void *pdata)
{
	INT8U         error;
	PQ            pq; // pointer to Q structure
	OS_Q_DATA     qd; // queue data structure, to get actual num of members from OS

    UART_puts((char *)__func__); UART_puts("started\n\r");

    while(TRUE)
	{
		OSTimeDly(LOOP_DELAY);

		OSQQuery(QueueHandle, &qd); // get number of members
		if (qd.OSNMsgs < QSIZE) // wait until q is full
			continue;

	    // q is full, so we use accept in stead of pend (why wait if not needed)
		while ((pq = (PQ)OSQAccept(QueueHandle, &error)) != NULL) // as long as address is returned by OS...
    	    if (Uart_debug_out & QUEUE_DEBUG_OUT)
    	    	outQ_member(pq, (char *)__func__); // display data to uart
	}
}
