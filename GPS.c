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

/////////////////////////////////////////////////////////////////////////////////////////////////////////
//krijgt coordinaten als string binnen en zet ze om naar een int waarde
//neemt 5 decimalen mee
int RMCtoINT(char RMCval[])
{
	float Floatval = atof(RMCval);
	Floatval *= 1000;

	return (int)Floatval;
}


void GPSTask(void *pdata)
{
	char buffer[100];					//om de string die van de GPS komt in te zetten
	char bufferCopy[100];				//tweede string, omdat buffer aangepast wordt door findToken...

	char test[] = "$GPRMC,091248.00,A,5205.07493,N,00510.10854,E,0.043,,240919,,,A*71";

	while (TRUE)
	{
		UARTGPS_gets(buffer, 0); // krijg data binnen van GPS

		// comment deze weg als de data van de GPS uitgelezen moet worden, wanneer de GPS zn locatie kan vinden dus
		strcpy(buffer, test);

		if (strstr(buffer, DATATYPE) != NULL)
		{
			UART_puts(buffer);
			UART_puts("\r\n");

			strcpy(bufferCopy, buffer);  // maak een kopie, omdat buffer aangepast wordt in de eerste findToken
			char * foundLat = findToken(buffer, 4);			// latitude op 4de positie
			char * foundLong = findToken(bufferCopy, 6);	// longitude op 6de positie

			int DegreeLatInt = RMCtoINT(foundLat);
			int DegreeLongInt = RMCtoINT(foundLong);


			if (foundLong != NULL)
			{
				UART_puts("Found coordinates: ");
				UART_putint(DegreeLatInt); UART_puts(", "); UART_putint(DegreeLongInt); UART_puts("\r\n");
			}
		}
		OSTimeDly(LOOP_DELAY);
	}
}


