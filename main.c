//////////////////////////////////////////////////////////////////////////////
// prog: main.c:: output: US_METHODS
// comm: demonstrates the basic usage of uc/OSII-methods, such as
//       taskcreation, mailboxing, flagging, queueing, semaphores (mutexes)
// auth: MSC
//////////////////////////////////////////////////////////////////////////////

#include "RN2483A.h"
#include <includes.h>

#include "taskcreate.h" // for stacksize
#include "main.h"       // defines, external declarations, enums, prototypes


int Uart_debug_out = 0xffff; // debug output to uart for all tasks
//int Uart_debug_out = INTERRUPT_DEBUG_OUT; // start alleen moet interrupt output


// GLOBAL DATA, in main.h defined 'extern'
// handles, used for q, mailboxes and flags
// defined as a pointer; the os will allocate a specific OS_EVENT on it
OS_STK              InitTaskStk[STACK_SIZE]; // stack of inittask
OS_EVENT           *MutexHandle;   // name of handle on 1 instance of mutex-mechanism
OS_EVENT           *MboxHandle;    // name of handle on 1 instance of mailbox-mechanism
OS_EVENT           *StudentstuffHandle;
OS_FLAG_GRP        *FlagHandle;    // name of handle on 1 instance of flag-mechanism
OS_FLAG_GRP        *FlagIntHandle; // name of handle on 1 instance of flag-mechanism


// stuff for the queue: 1. handle, 2. my data area, 3. q-array for OS
OS_EVENT           *QueueHandle;        // name of handel on 1 instance of queue-mechanism
Q                   data_queue[QSIZE];  // data queue, in this case array of Q-structs
void*               os_queue[QSIZE];    // queue with void-pointers for OS, holds addresses of data-q-members

// application name
char *version = "UC_METHODS"; // name of this application, will be displayed on uart and lcd


// put some output to uart
void DisplayOSData(void)
{
	char *functionality =
"Dit is de hackerversie van Blind and Seek.\r\n";

    UART_puts(functionality);
	UART_puts("\t[uC/OS-II versie ");    UART_putint(OSVersion());
    UART_puts(" voor de CORTEX M4 op "); UART_putint(SystemCoreClock/1000000);
    UART_puts(" MHz]\n\r");
}

// create all handles
// if a new handle is needed, create it here
// note: first, define the handle globally above, then also in main.h as external
// author MScager
void CreateHandles()
{
	INT8U error;

	MutexHandle = OSMutexCreate(MUTEX_PRTY, &error); // for mutex.c
	if (error)
    {
   		UART_puts("\n\rmutexcreate error=");
   		UART_putint(error);
   		UART_puts("\n\r");
    }
	MboxHandle    = OSMboxCreate(NULL); // for mailbox.c
	StudentstuffHandle    = OSMboxCreate(NULL);
	FlagHandle    = OSFlagCreate(0x00, &error);
	if (error)
    {
   		UART_puts("\n\rflagcreate error=");
   		UART_putint(error);
   		UART_puts("\n\r");
    }
	FlagIntHandle = OSFlagCreate(0x00, &error);
	if (error)
    {
   		UART_puts("\n\rflagintcreate error=");
   		UART_putint(error);
   		UART_puts("\n\r");
    }
	QueueHandle = OSQCreate(&os_queue[0], QSIZE);
}	
   

//////////////////////////////////////////////////////////////////////////////
// func: InitTask
// args: void *pdata, needed by os
// comm: This is the initialisation-task. It's the first task to run, and 
//       it will peform some tasks that need to be done after uC/OS-II 
//       has been started. Most important task here is starting the timer.
// note: this task deletes itself when finished to free memory
//////////////////////////////////////////////////////////////////////////////
void InitTask(void *pdata)
{
	__asm("CPSID   I"); // disable interrupts

   	LCD_puts(version);
	UART_puts(version); UART_puts("\n\r");

	DisplayOSData();  // output to uart of some data

	CreateHandles();  // create all handles
	CreateTasks();    // start all threads/tasks

	OS_CPU_SysTickInit(840000);	// 84000 = 1kHz AHB divider staat op 2!

	__asm( "CPSIE   I"); // enable interrupts

   	OSTaskDel(OS_PRIO_SELF); // This task is no longer needed; delete it
}

int main (void)
{
	SystemInit();	// Set SystemCLK
	// initialize all board-outputs
	DELAY_init();
	UART_init();
	UART3_init();
	LCD_init();
	LED_init();
	BUZZER_init();
    KEYS_init();
    RN2483A_init(); //Initialize RN2483.

    OSInit();

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	GPIOD -> MODER |= ( 1 << 24 );
	GPIOD -> MODER |= ( 1 << 26 );
	GPIOD -> MODER |= ( 1 << 28 );
	GPIOD -> MODER |= ( 1 << 30 );

    OSTaskCreate(InitTask, NULL, &InitTaskStk[STACK_SIZE-1], INITTASK_PRTY);
    //char buf[30];   // output buffer
	UART_puts("\n\r"); UART_puts((char *)__func__); UART_puts("started");
	//test LORA
	LCD_put("functie start");
    while(TRUE)
    {
    	/*UART3_puts("mac status\r\n");
    	UART3_gets(buf, 0);

    	UART_puts(buf);
    	DELAY_ms(1000);*/

    	//char *point = sendrecv("mac poep\r\n");
 /*   	UART_puts(sendrecvlora("mac save\r\n")); //Zo mag je de functie eigenlijk niet aanroepen: je moet de pointer die teruggegeven wordt onthouden en daarop free() draaien.
    	UART_puts("\r\n");

    	sendlora("sys poep\r\n");
    	UART_puts(recvlora());
    	UART_puts("\r\n");

    	//DELAY_ms(100);
    	resetlora();
    	DELAY_ms(1000);*/
    }
    //test lora start

    OSStart();
	// Nothing comes beyond this point
    while(TRUE);
}

