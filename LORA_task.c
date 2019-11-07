//////////////////////////////////////////////////////////////////////////////////////
// Dit bestand bevat een hoop functies die de verbinding tussen de gametask en het TTN maken
//////////////////////////////////////////////////////////////////////////////////////

#include "main.h"
#include <includes.h>

//////////////////////////////////////////////////////////////////////////////////////
//Haalt nieuwe locatie op uit speelveld.c en verstuurt deze via LoRa naar TTN
//////////////////////////////////////////////////////////////////////////////////////
void SendZoekLocatie()
{
	do
	{
		verstopper = veld_ikzoek();  // locatie van de verstopper
	} while(veld_binnen(&verstopper) == 0);

		BuildMessage(42,verstopper.lat*100000,verstopper.lon*100000);
}

//////////////////////////////////////////////////////////////////////////////////////
// Bouwt een bericht aan de hand van de gekregen parameters en verstuurd deze via LoRa naar TTN
//////////////////////////////////////////////////////////////////////////////////////
void BuildMessage(INT8U functiecode,int lat,int lon)
{
	//Zet de lat en lon om naar offsets vanaf het middenpunt van het veld
	int latdist = lat - veld_midden.lat*100000;
	int londist = lon - veld_midden.lon*100000;

	//Negatieve getallen omzetten naar 2s complement
	if(latdist < 0)
		latdist = 256 + latdist;
	if(londist < 0)
		londist = 256 + londist;

	//Het bericht met de juiste functiecode maken
	INT8U    Message[3] = {functiecode, latdist, londist};

	//Het bericht versturen via de LoRa Functies
	sendmactxlora(Message);
}

//////////////////////////////////////////////////////////////////////////////////////
//De LoRa Task haalt de GPS locatie op uit de mailbox van gametask
//en verstuurt deze eens in de 30 seconden naar het TTN
//Tijdens de 30 seconden delay wordt er opgelet of de noodknop ingedrukt wordt,
//als dit het geval is wordt er een noodbericht verstuurd
//////////////////////////////////////////////////////////////////////////////////////
void LORATask(void *pdata)
{
	INT8U   error;
	int i;
	PlGPS    PlocatieGPS;

	UART_puts((char *)__func__); UART_puts(" is gestart.\r\n");

	while(TRUE)
    {
		PlocatieGPS = OSMboxPend(MessageHandle, WAIT_FOREVER, &error);
		UART_putint((int)error);

		UART_puts("\r\nLORA HEEFT BINNEN\r\n");

    	BuildMessage(PlocatieGPS->functiecode, PlocatieGPS->lat, PlocatieGPS->lon);
    	for(i = 0;i < 30;i++)
    	{
    		PlocatieGPS = OSMboxPend(MessageHandle, WAIT_FOREVER, &error);
    		if(PlocatieGPS->functiecode == NOODKNOP_ON)
    			break;
    		OSTimeDly(20);
    	}

    }
}
