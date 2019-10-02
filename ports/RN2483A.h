
//#define LORADEBUG //debugtoggle voor loraoutput.
#define APPKEY "D0A6153C8967FC0508157FE0016DDE2A"
#define APPEUI "70B3D57ED0022C2A"

//Prototypes:
void RN2483A_init(void);
char* sendrecvlora(char *sendbuf);
char* recvlora(void);
void sendlora(char* sendbuf);
void resetlora(void);
void RN2483A_connectLORA();
