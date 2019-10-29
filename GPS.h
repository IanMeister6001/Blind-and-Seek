#include <includes.h>

#define DATATYPE "$GPRMC"
#define DECIMALS 8
#define COORDLENGTH 10
#define TESTRMCSTRING "$GPRMC,091248.00,A,5205.07493,N,00510.10854,E,0.043,,240919,,,A*71"

typedef struct //Voor de mailbox
{
	int lat;
	int lon;
	INT8U functiecode;
	int distance;
	int bearing;
}lGPS,*PlGPS;

// prototypes
extern void GPSTask(void *pdata);
extern char * findToken(char buffer[], int tokenNr);
