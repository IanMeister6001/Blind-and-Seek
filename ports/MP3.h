# define START       0x7E
# define VERSION     0xFF
# define LENGTH      0x06
# define END    0xEF
# define ACK 0x00

void SendComand(char,char,char);
void SetVolume(int);
void playFirst();
void resetModule();
void MP3_init();
