#include "main.h"
#include <includes.h>
#include "GPS.h"
#include "speelveld.h"

void LORATask(void *pdata)
{
	UART_puts("\n\r"); UART_puts((char *)__func__); UART_puts("started");
	INT8U   error;
	int i;
	int   londist;
	int      latdist;
	INT8U    Message[3];
	PlGPS    PlocatieGPS;
    while(TRUE)
    {
    	for(i = 0;i < 60;i++)
    	{
    		PlocatieGPS = OSMboxPend(MessageHandle, WAIT_FOREVER, &error);
    		if(PlocatieGPS->functiecode == NOODKNOPINGEDRUKT)
    			break;
    		OSTimeDly(100);
    	}
    	PlocatieGPS->functiecode = 6;
    	PlocatieGPS->lat = 5208050;
    	PlocatieGPS->lon = 517384;
    	latdist = PlocatieGPS->lat - veld_midden.lat*100000;
    	londist = PlocatieGPS->lon - veld_midden.lon*100000;
    	if(latdist < 0)
    		latdist = 256 + latdist;
    	if(londist < 0)
    		londist = 256 + londist;

    	Message[0] = PlocatieGPS->functiecode;
    	Message[1] = latdist;
    	Message[2] = londist;
    	sendmactxlora(Message);
    }
}
