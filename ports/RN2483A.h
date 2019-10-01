
//#define LORADEBUG //debugtoggle voor loraoutput.


//Prototypes:
void RN2483A_init(void);
char* sendrecvlora(char *sendbuf);
char* recvlora(void);
void sendlora(char* sendbuf);
void resetlora(void);
