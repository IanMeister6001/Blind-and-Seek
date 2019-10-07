#include "main.h"
#include "includes.h"
#include "string.h"
#include "GPS.h"

//////////////////////////////////////////////////////////////////////////////////////
//krijgt een string binnen, die deze gaat opdelen tussen de komma's
//ontvangt de string en de positie die gereturnd moet worden
//returnt de token op de n = tokenNr Positie
//////////////////////////////////////////////////////////////////////////////////////
char * findToken(char buffer[], int tokenNr)
{
	const char s[2] = ",";
	char *token;
	int i;

	token = strtok(buffer, s);

	for(i = 1; i < tokenNr; i++)
		token = strtok(NULL, s);

	return token;
}

void GPSTask(void *pdata)
{
	char buffer[100];					//om de string die van de GPS komt in te zetten
	char bufferCopy[100];				//tweede string, omdat buffer aangepast wordt door findToken...
	char checkDataType[] = DATATYPE;
	//char test[] = "$GPRMC,091248.00,A,5205.07493,N,00510.10854,E,0.043,,240919,,,A*71";
	UART_puts((char *)__func__); UART_puts("started\n\r");

	while (TRUE)
	{
		UARTGPS_gets(buffer, 0);

		//UARTGPS_puts("stront");

		//UART_puts(buffer);
		//strcpy(buffer, test);
		if (strstr(buffer, checkDataType) != NULL)
		{
			UART_puts(buffer);
			UART_puts("\r\n");

			strcpy(bufferCopy, buffer);
			char * foundLat = findToken(buffer, 4);
			char * foundLong = findToken(bufferCopy, 6);

			//char * foundCoords = strcat(foundLat, ", "); foundCoords = strcat(foundCoords, foundLong);
			//UART_puts(foundCoords); UART_puts("\r\n");
			UART_puts(foundLat);UART_puts(", ");UART_puts(foundLong);
			UART_puts("\r\n");
		}
		OSTimeDly(LOOP_DELAY);
	}
}


