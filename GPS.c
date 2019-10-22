#include "GPS.h"
#include "main.h"
#include "includes.h"
#include "string.h"
#include <math.h>
#include "funcs.h"

union // Voor het omzetten van GPS coordinaten van ints naar 3 bytes voor het verzenden in LoRa
{
	int coordInt;
	INT8U coordByte[3];
}dataSwap;

struct locatieGPS //Voor de mailbox
{
	int lat;
	int lon;
	float distance;
	float bearing;
}lGPS, PlGPS;
//////////////////////////////////////////////////////////////////////////////////////
//krijgt een string binnen, die deze gaat opdelen tussen de komma's
//ontvangt de string en de positie die gereturnd moet worden
//returnt de token op de n = tokenNr Positie
//Auth: Damian van Hoornaar
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
	float DestLatFloat = 52.0507582;
	float DestLonFloat = 5.1096538;

	while (TRUE) // For testing purposes
	{
		//float bearing = calcBearing(52.0507493, 5.1010854, 52.05, 5.1);

	}

	while (TRUE)
	{
		UARTGPS_gets(buffer, 0); // krijg data binnen van GPS

		// comment deze weg als de data van de GPS uitgelezen moet worden, wanneer de GPS zn locatie kan vinden dus
		strcpy(buffer, TESTRMCSTRING);

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
			lGPS.lat = DegreeLatInt;
			lGPS.lon = DegreeLongInt;
			lGPS.distance = calcDistance(IntToFloat(DegreeLatInt), IntToFloat(DegreeLongInt), DestLatFloat, DestLonFloat);
			lGPS.bearing = calcBearing(IntToFloat(DegreeLatInt), IntToFloat(DegreeLongInt), DestLatFloat, DestLonFloat);

			//hier de MboxPost met de struct waar de waardes in staan.
		}
		OSTimeDly(LOOP_DELAY);
	}
}


