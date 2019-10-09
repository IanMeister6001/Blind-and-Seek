#define DATATYPE "$GPRMC"
#define DECIMALS 8
#define COORDLENGTH 10

// prototypes
extern void GPSTask(void *pdata);
extern char * findToken(char buffer[], int tokenNr);
extern int RMCtoINT(char RMCval[]);
