# define START       0x7E
# define VERSION     0xFF
# define LENGTH      0x06
# define END   		 0xEF
# define ACK 		 0x00

#define LEFTMP3 2
#define RIGHTMP3 1
#define FORWARDMP3 5
#define BACKWARDMP3 4
#define DORAMP3 3

void SendCommand(char,char,char);
void setVolume(char);
void playFirst();
void playNext();
void playTrack(int);
void resetModule();
void MP3_init();
