

#define DATATYPE "$GPRMC"
#define DECIMALS 8
#define COORDLENGTH 10
#define TESTRMCSTRING "$GPRMC,091248.00,A,5205.07493,N,00510.10854,E,0.043,,240919,,,A*71"

// prototypes
extern void GPSTask(void *pdata);
extern char * findToken(char buffer[], int tokenNr);

