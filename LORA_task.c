#include "main.h"
#include <includes.h>
//#include "speelveld.h"
void SendZoekLocatie()
{
	do
	{
		verstopper = veld_ikzoek();  // locatie van de verstopper
	} while(veld_binnen(&verstopper) == 0);

		BuildMessage(42,verstopper.lat*100000,verstopper.lon*100000);
}

void BuildMessage(INT8U functiecode,int lat,int lon)
{
	int latdist = lat - veld_midden.lat*100000;
	int londist = lon - veld_midden.lon*100000;
	if(latdist < 0)
		latdist = 256 + latdist;
	if(londist < 0)
		londist = 256 + londist;

	INT8U    Message[3] = {functiecode, latdist, londist};
	sendmactxlora(Message);
}


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

    	//test waardes:
		/*
    	PlocatieGPS->functiecode = 2;
    	PlocatieGPS->lat = 5208040;
    	PlocatieGPS->lon = 517388;
		UART_puts("lat: \r\n");
    	UART_putint(PlocatieGPS->lat);
    	UART_puts("lon: \r\n");
        UART_putint(PlocatieGPS->lon);
        */
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
