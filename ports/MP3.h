# define START       0x7E
# define VERSION     0xFF
# define LENGTH      0x06
# define END   		 0xEF
# define ACK 		 0x00

void SendCommand(char,char,char);
void setVolume(char);
void playFirst();
void playNext();
void playTrack(int);
void resetModule();
void MP3_init();
