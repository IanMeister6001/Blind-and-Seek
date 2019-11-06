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
	PlGPS GPSData;
	float heading = 0;
	float headingDifference = 0;
	INT8U mp3 = 0;

	UART_puts((char *)__func__);UART_puts(" is gestart.\n\r"); //Debugout die laat weten dat task is gestart.

	while (TRUE)
	{
		//Haal de GPS data op uit de mailbox
		GPSData = OSMboxPend(GPSDataHandle, WAIT_FOREVER, &error);
		//PlocatieGPS = OSMboxPend(MessageHandle, WAIT_FOREVER, &error);

		//Kijk of de distance naar het doel minder is dan de benodigde distance
		if(GPSData->distance < REQUIREDDISTANCE)
		{
			UART_puts("We zijn dichtbij! \r\n");
			//Stuur de nieuwe target locatie over de LoRa
			SendZoekLocatie();

			//Speel de Dora Victory Song :)
			playTrack(DORAMP3);

			//Zorg er voor dat de while loop weer vanaf het begin begint
			//Hierdoor wordt de code onder deze if niet uitgevoerd
			continue;
		}

		//Haal de heading op van het kompas
		heading = Compass_GetHeading();

		//Bereken het verschil tussen de heading die de speler heeft en de heading die de speler moet hebben.
		headingDifference = heading - GPSData->bearing;
        if(headingDifference < 0){
        	UART_puts("negatief: ");
        	int bulshit = headingDifference * -1;
		UART_putint(bulshit);
        }
        if(headingDifference > 0){
        	UART_puts("positief: ");
		UART_putint(headingDifference);
        }
		UART_puts("\r\n");

		//Het verschil in heading is negatief.
		// Dit betekent dat de speler naar rechts moet
		if(headingDifference < -30 && headingDifference > -180)
		{
			UART_puts("We gaan naar rechts! \r\n");
			mp3 = RIGHTMP3;
		}
		//Het verschil in heading ligt binnen de marges.
		//Dit betekent dat de speler naar voren moet
		else if(headingDifference > -30 && headingDifference < BEARINGMARGIN)
		{
			UART_puts("We gaan vooruit! \r\n");
			mp3 = FORWARDMP3;
		}
		//Het verschil in heading is positief.
		//Dit betekent dat de speler naar links moet
		else if(headingDifference > BEARINGMARGIN && headingDifference < 180 )
		{
			UART_puts("We gaan naar links! \r\n");
			mp3 = LEFTMP3;
		}
		//Ontvang ingedrukte knop van de knoptask.
		gedrukt = OSFlagPend(KnopFlag,
				                   0x0fff,
								   OS_FLAG_WAIT_SET_ANY + OS_FLAG_CONSUME,
								   2,
								   &error);

		if(error == OS_ERR_NONE)
		{
			//Kijk of de tip knop ingedrukt is
			if(gedrukt == TIPKNOP_ON)
			{
				UART_puts("We spelen af!! \r\n");
				GPSData->functiecode = 2;
				playTrack(mp3);
			}
			else if(gedrukt == NOODKNOP_ON)
			{
				UART_puts("NOOD! \r\n");
				//GPSData->functiecode = 6;
			}

			//Doe deze flagpost terug naar knoptask om aan te geven dat je de ingedrukte toets
			//heb afgehandeld en dat knoptask weer mag gaan pollen:
			OSFlagPost(KnopFlag, 0xf000, OS_FLAG_SET, &error);
		}
		GPSData->functiecode = 2;
		OSMboxPost(MessageHandle, GPSData);

		OSTimeDly(LOOP_DELAY);
	}
}

