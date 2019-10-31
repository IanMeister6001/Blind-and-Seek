#include "main.h"
#include <includes.h>
#include "GPS.h"
//#include "speelveld.h"
void SendZoekLocatie()
{
	VELD_GPS verstopper;
	verstopper = veld_ikzoek();  // locatie van de verstopper
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
		/*
    	//test waardes:
    	PlocatieGPS->functiecode = 2;
    	PlocatieGPS->lat = 5208050;
    	PlocatieGPS->lon = 517384;
		*/
    	BuildMessage(PlocatieGPS->functiecode, PlocatieGPS->lat, PlocatieGPS->lon);
    	for(i = 0;i < 60;i++)
    	{
    		if(PlocatieGPS->functiecode == NOODKNOP_ON)
    			break;
    		OSTimeDly(100);
    	}

    }
}
