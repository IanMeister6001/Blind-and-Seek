
//#define LORADEBUG //debugtoggle voor loraoutput.

#define APPKEY "D0A6153C8967FC0508157FE0016DDE2A"
#define APPEUI "70B3D57ED0022C2A"
#define DEVEUI "0004A30B00209450"
#define MSGSIZE 7
//Prototypes:
void RN2483A_init(void);
char* sendrecvlora(char *sendbuf);
char* recvlora(void);
void sendlora(char* sendbuf);
void resetlora(void);
void RN2483A_connectLORA();
void sendmactxlora(unsigned char*);
