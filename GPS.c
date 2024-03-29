#include "main.h"
#include "includes.h"
#include "string.h"
#include <math.h>
#include "GPSmath.h"
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
		token = strtok(NULL, s);	//NULL omdat strtok() de vorige ingevoerde string neemt
	return token;
}

void GPSTask(void *pdata)
{
	char buffer[100];					//om de string die van de GPS komt in te zetten
	char bufferCopy[100];				//tweede string, omdat buffer aangepast wordt door findToken...

	INT8U error = 0; //Errorvariabele voor het zenden via mbox.

	//Floats voor destinations, momenteel zijn deze random ingevuld. Huidig doel is net voor de deur van Padualaan 99
	lGPS lGPS;

	int DestLat;
	int DestLon;

	char * foundLat;
	char * foundLong;
	int DegreeLatInt;
	int DegreeLongInt;

	UART_puts((char *)__func__);UART_puts(" is gestart.\n\r"); //Debugout die laat weten dat task is gestart.

	while (TRUE)
	{
		UARTGPS_gets(buffer, 0); // krijg data binnen van GPS, comment weg als je wilt testen zon gps-module.
		DestLat = verstopper.lat * 100000;
		DestLon = verstopper.lon * 100000;
		// comment deze weg als de data van de GPS uitgelezen moet worden, wanneer de GPS zn locatie kan vinden dus
		//strcpy(buffer, TESTRMCSTRING);

		if (strstr(buffer, DATATYPE) != NULL)
		{
			strcpy(bufferCopy, buffer);  // maak een kopie, omda t buffer aangepast wordt in de eerste findToken
			foundLat = findToken(buffer, 4);			// latitude op 4de positie
			foundLong = findToken(bufferCopy, 6);	// longitude op 6de positie

			DegreeLatInt = RMCtoINT(foundLat);
			DegreeLongInt = RMCtoINT(foundLong);

			if (foundLong != NULL)
			{
				UART_puts("Found coordinates: ");
				UART_putint(DegreeLatInt); UART_puts(", "); UART_putint(DegreeLongInt); UART_puts("\r\n");
			}

			lGPS.lat = DegreeLatInt; lGPS.lon = DegreeLongInt;								//zet de gegevens in de struct
			lGPS.distance = calcDistance(DegreeLatInt, DegreeLongInt, DestLat, DestLon);
			lGPS.bearing = calcBearing(DegreeLatInt, DegreeLongInt, DestLat, DestLon);
			lGPS.functiecode = 2;

			UART_puts("afstand: "); UART_putint(lGPS.distance); UART_puts(", en bearing: "); UART_putint(lGPS.bearing); UART_puts("\r\n"); UART_puts("\r\n");
			UART_puts("kompas: ");UART_putint(Compass_GetHeading()); UART_puts("\r\n");

			if(lGPS.distance != 0)	//soms geeft de functie een afstand van 0 door, dus deze if filtert die eruit.
			{
				error = OSMboxPost(GPSDataHandle, &lGPS); //Stuur coordinaten op de mbox en cast de pointer naar de struct als het type structpointer.
			}

			UART_putint((int)error);
		}
		OSTimeDly(LOOP_DELAY);
	}
}
