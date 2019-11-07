//////////////////////////////////////////////////////////////////////////////
// Programma: main.c
// Auteur: Projectgroep 2, EV3A
//////////////////////////////////////////////////////////////////////////////

#include <includes.h>

#include "taskcreate.h" // for stacksize
#include "main.h"       // defines, external declarations, enums, prototypes

char *version = "BLIND-AND-SEEK"; // name of this application, will be displayed on uart and lcd

OS_EVENT *GPSDataHandle; //Handle voor de mbox tussen GPS-task en gametask.
OS_EVENT *MessageHandle; //Handle voor de mbox tussen gametask en TTN-task.
OS_FLAG_GRP *KnopFlag;   //Handle voor de flag tussen knoptask en gametask.

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
// Functie: MakeHandles
// Doel: Maakt alle handles aan voor de OS-mechanismen.
// Auteur: Projectgroep 2, EV3A
//////////////////////////////////////////////////////////////////////////////
void MakeHandles()
{
	INT8U error;
	GPSDataHandle = OSMboxCreate(NULL); //Maak de handle voor de Mbox tussen gpstask en gametask aan.
	MessageHandle = OSMboxCreate(NULL); //Maak de handle voor de mbox tussen gametask en ttn-task aan.

	KnopFlag = OSFlagCreate(0x0000, &error);

	//Hier errorhandling voor de knopflag toevoegen.

}	
   

//////////////////////////////////////////////////////////////////////////////
// Functie: InitProgram
// Doel: start het programma zoals gewenst op door alle board outputs
//		 te initialiseren, OS-info te printen, etc.
// Auteur: Projectgroep 2, EV3A
//////////////////////////////////////////////////////////////////////////////
void InitProgram(void)
{
   	LCD_puts(version);
	UART_puts(version); UART_puts("\n\r");

	OSinfo();  //Output OS info to UART.

	MakeHandles();  // create all handles
	CreateTasks();    // start all threads/tasks

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
	BUZZER_init();
    MP3_init();
    I2C_Initialize();
    Compass_Initialize();
    Knoppen_Init();
    //LCD_init();
    //playFirst();
    //Comment deze weg als de RN2483A niet is aangesloten.
    RN2483A_init(); //Initialize RN2483.
    DELAY_ms(4000);
    SendZoekLocatie();
    DELAY_ms(4000);
}

int main (void)
{

	SystemInit();	// Set SystemCLK

	InitBoard(); //Initialiseer alle bord-outputs

    OSInit(); //Initialiseer OS.

    InitProgram(); //Initialiseer alles van het programma wat het os gebruikt.

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	GPIOD -> MODER |= ( 1 << 24 );
	GPIOD -> MODER |= ( 1 << 26 );
	GPIOD -> MODER |= ( 1 << 28 );
	GPIOD -> MODER |= ( 1 << 30 );

    OSStart();
	// Nothing comes beyond this point
    while(TRUE);
}

