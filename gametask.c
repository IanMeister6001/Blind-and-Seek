/////////////////////////////////////////////////////////////////////////
//	File: gametask.c
//	Authors: projectgroep 2, EV3A
// 	Bevat de belangrijkste taak van het programma: de gametask die het verloop van het spel regelt.
/////////////////////////////////////////////////////////////////////////

#include <includes.h>
#include "main.h"

/////////////////////////////////////////////////////////////////////////
//	Task: GameTask
//	Doel: Regelt het verloop van het spel.
// 	args: geen
//////////////////////////////////////////////////////////////////////////
void GameTask(void *pdata)
{
	INT8U error;
	OS_FLAGS gedrukt;

	UART_puts((char *)__func__);UART_puts(" is gestart.\n\r"); //Debugout die laat weten dat task is gestart.

	while (TRUE)
	{
		//Ontvang ingedrukte knop van de knoptask.
		gedrukt = OSFlagPend(KnopFlag,
				                   0x0fff,
								   OS_FLAG_WAIT_SET_ANY + OS_FLAG_CONSUME,
								   WAIT_FOREVER,
								   &error);

		//Print even welke knop we binnen hebben gekregen:
		UART_puts("\r\ngametask: "); UART_putint((int)gedrukt);

		//Doe deze flagpost terug naar knoptask om aan te geven dat je de ingedrukte toets
		//heb afgehandeld en dat knoptask weer mag gaan pollen:
		OSFlagPost(KnopFlag, 0xf000, OS_FLAG_SET, &error);

		OSTimeDly(LOOP_DELAY);
	}

}
