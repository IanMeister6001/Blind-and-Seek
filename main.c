//////////////////////////////////////////////////////////////////////////////
// Programma: main.c
// Auteur: Projectgroep 2, EV3A
//////////////////////////////////////////////////////////////////////////////

#include "RN2483A.h"
#include <includes.h>

#include "taskcreate.h" // for stacksize
#include "main.h"       // defines, external declarations, enums, prototypes

// stuff for the queue: 1. handle, 2. my data area, 3. q-array for OS
/*OS_EVENT           *QueueHandle;        // name of handel on 1 instance of queue-mechanism
Q                   data_queue[QSIZE];  // data queue, in this case array of Q-structs
void*               os_queue[QSIZE];    // queue with void-pointers for OS, holds addresses of data-q-members
*/

// application name
char *version = "BLIND-AND-SEEK"; // name of this application, will be displayed on uart and lcd


//////////////////////////////////////////////////////////////////////////////
// Functie: OSinfo
// Doel: print een opstartbericht naar de debug-UART.
// Auteur: Projectgroep 2, EV3A
//////////////////////////////////////////////////////////////////////////////
void OSinfo(void)
{
	char *functionality =
"Blind-and-seek software, EV3A, Projectgroep 2, 07-10-2019\r\n" \
"Dit is de software die het eindproduct aanstuurt.\r\n\r\n";

    UART_puts(functionality);
	UART_puts("\t[uC/OS-II versie ");    UART_putint(OSVersion());
    UART_puts(" voor de CORTEX M4 op "); UART_putint(SystemCoreClock/1000000);
    UART_puts(" MHz]\n\r");
}

//////////////////////////////////////////////////////////////////////////////
// Functie: MaakHandles
// Doel: Maakt alle handles aan voor de OS-mechanismen.
// Auteur: Projectgroep 2, EV3A
//////////////////////////////////////////////////////////////////////////////
void MakeHandles()
{
	//INT8U error;

}	
   

//////////////////////////////////////////////////////////////////////////////
// Functie: InitProgram
// Doel: start het programma zoals gewenst op door alle board outputs
//		 te initialiseren, OS-info te printen, etc.
// Auteur: Projectgroep 2, EV3A
//////////////////////////////////////////////////////////////////////////////
void InitProgram(void)
{

	InitBoard(); //Initialize all board outputs.

   	LCD_puts(version);
	UART_puts(version); UART_puts("\n\r");

	OSinfo();  //Output OS info to UART.

	MakeHandles();  // create all handles
	//CreateTasks();    // start all threads/tasks

	OS_CPU_SysTickInit(840000);	// 84000 = 1kHz AHB divider staat op 2!
}

//////////////////////////////////////////////////////////////////////////////
// Functie: InitBoard
// Doel: Draait de initfuncties voor alle board-outputs.
// Auteur: Projectgroep 2, EV3A
//////////////////////////////////////////////////////////////////////////////
void InitBoard(void)
{
	DELAY_init();
	UART_init();
	UART3_init();
	UARTGPS_init();
	UARTBT_init();
	LCD_init();
	LED_init();
	BUZZER_init();
    KEYS_init();
    //Comment deze weg als de RN2483A niet is aangesloten.
    //RN2483A_init(); //Initialize RN2483.
}

int main (void)
{
	SystemInit();	// Set SystemCLK
	// initialize all board-outputs

    InitProgram(); //Initialiseer het programma.

    OSInit(); //Initialiseer OS.

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	GPIOD -> MODER |= ( 1 << 24 );
	GPIOD -> MODER |= ( 1 << 26 );
	GPIOD -> MODER |= ( 1 << 28 );
	GPIOD -> MODER |= ( 1 << 30 );

	//test LORA
	LCD_put("functie start");

	//testmeuk:
	sendlora("hallo\r\n");
	UART_puts(recvlora());

	UART_puts(sendrecvlora("halloootjes\r\n"));

	resetlora();

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

