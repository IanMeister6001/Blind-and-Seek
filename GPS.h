#include <includes.h>

#define DATATYPE "$GPRMC"
#define DECIMALS 8
#define COORDLENGTH 10
#define TESTRMCSTRING "$GPRMC,091248.00,A,5208.04093,N,00517.38854,E,0.043,,240919,,,A*71"
#define REQUIREDDISTANCE 2
#define BEARINGMARGIN 10

typedef struct locatieGPS//Voor de mailbox
{
	int lat;
	int lon;
	int distance;
	int bearing;
	INT8U functiecode;
}lGPS, *PlGPS;

// prototypes
extern void GPSTask(void *pdata);
extern char * findToken(char buffer[], int tokenNr);
