#include "includes.h"
//////////////////////////////////////////////////////////////////////////////
// func: MP3_init
// args: void
// comm: Stuurt initialisatie parameters
// note:
//
//////////////////////////////////////////////////////////////////////////////
void MP3_init()
{
	  resetModule();
	  SendCommand(0x3F, 0, 0);
	  DELAY_ms(1000);
	  setVolume(15);
}
//////////////////////////////////////////////////////////////////////////////
// func: SendCommand
// args: CMD:commandnummer par1:data byte 1 par 2: data byte 2
// comm: creeert een bericht met checksum en verstuurt deze naar de module
// note:
//
//////////////////////////////////////////////////////////////////////////////
void SendCommand(char CMD, char Par1, char Par2)
{
int i;
 //bereken de checksum van 2 bytes
 INT16U checksum = -(VERSION + LENGTH + CMD + ACK + Par1 + Par2);

 //zet het bericht in elkaar
 char Command_line[10] = { START, VERSION, LENGTH, CMD, ACK, Par1, Par2, checksum >> 8, checksum & 0xFF, END};

 //verstuur het command
 for (i=0; i < 10; i++)
 {
  //UART3_putint(Command_line[i]);
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET); // Wait for Empty
    USART_SendData(USART1, Command_line[i]);
 }
}
//////////////////////////////////////////////////////////////////////////////
// func: setVolume
// args: volume = volumeniveau van 1 tot 30
// comm: past het volume aan
// note:
//
//////////////////////////////////////////////////////////////////////////////
void setVolume(char volume)
{
  SendCommand(0x06, 0, volume); // Set the volume (0x00 tot 0x30)
  DELAY_ms(100);
}
//////////////////////////////////////////////////////////////////////////////
// func: playFirst
// args: void
// comm: speel de tracks in een loop af beginnent met de eerste track
// note:
//
//////////////////////////////////////////////////////////////////////////////
void playFirst()
{
  SendCommand(0x11,0,1);
  DELAY_ms(100);
}
//////////////////////////////////////////////////////////////////////////////
// func: playNext
// args: void
// comm: speel volgende track
// note:
//
//////////////////////////////////////////////////////////////////////////////
void playNext()
{
  SendCommand(0x01,0,0);
  DELAY_ms(100);
}
//////////////////////////////////////////////////////////////////////////////
// func: resetModule
// args:
// comm: reset de module
// note:
//
//////////////////////////////////////////////////////////////////////////////
void resetModule()
{
  SendCommand(0x0C, 0, 0);
  DELAY_ms(500);
}
//////////////////////////////////////////////////////////////////////////////
// func: playTrack
// args: track:tracknummer
// comm: speelt specifieke track af
// note:
//
//////////////////////////////////////////////////////////////////////////////
void playTrack(int track)
{
  SendCommand(0x03, 0, track);
  DELAY_ms(100);
}


