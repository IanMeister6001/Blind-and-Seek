/////////////////////////////////////////////////////////////////////////
//	Custom RN2483A functions
//	Authors: Lex Bleyenberg, Ian Baak
// 	Date: 18-09-2019
/////////////////////////////////////////////////////////////////////////

#include <includes.h>
#include "RN2483A.h"

#define BUFLEN 30
//app id ildlbbhp_6000_rn2483a_stm32
//Initialiseert de RN2483A, zodat hij altijd goed werkt bij opstarten.
void RN2483A_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	 char buf[BUFLEN]; //Temporary buffer for debug output from module.

	/*-------------------------- GPIO Configuration for PB13, the reset pin ----------------------------*/
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_Init(GPIOB, &GPIO_InitStructure);

	  //Sweep PB13 high, low and high, so that the RN2483A resets when the system resets.
	  GPIO_SetBits(GPIOB, GPIO_Pin_13);
	  GPIO_ResetBits(GPIOB, GPIO_Pin_13);
	  DELAY_ms(500);
	  GPIO_SetBits(GPIOB, GPIO_Pin_13);

	  //Print startup message
	  UART3_gets(buf, 0);

	  //Hier wat code toevoegen die de software verder laat gaan als de RN2483A niet is gedetecteerd.

	  UART_puts("\r\nRN2483 Init: ontvangen: "); UART_puts(buf);
	//join things network
	 RN2483A_connectLORA();
}
void RN2483A_connectLORA()
{
	int i = 0;
	#ifdef LORADEBUG
		UART_puts(sendrecvlora("sys get hweui\r\n"));
//Hweui is nodig voor het connecten van het things network
	#endif
	do
	{
		UART_puts("\r\n");
		resetlora();
		//restlora zodat de module verbind op channel 1
		DELAY_ms(500);
		//set de appkey, appeui zodat deze overeenkomen met deze van het thingnetwork
		UART_puts(sendrecvlora("mac set appkey " APPKEY "\r\n"));
		UART_puts(sendrecvlora("mac set appeui " APPEUI "\r\n"));
		UART_puts(sendrecvlora("mac set deveui " DEVEUI "\r\n"));
		#ifdef LORADEBUG
			UART_puts("\r\n");
			UART_puts(sendrecvlora("mac get appeui\r\n"));
		#endif
		//save de settings
		UART_puts(sendrecvlora("mac save\r\n"));
		DELAY_ms(100);
		UART_puts(sendrecvlora("mac join otaa\r\n"));
		//UART_puts(recvlora());
		i = strcmp(recvlora(),"accepted");
        #ifdef LORADEBUG
		if(i != 0) UART_puts("\r\n DENIED r\n");
        #endif

	}
	while(i != 0);
}

void sendmactxlora(unsigned char* data)
{
	int i;
	char buf[2];
	//buffer waarin de hexadecimale getallen als character worden opgeslagen
	UART3_puts("mac tx uncnf 1 ");
	//commandline voor unconfirmed message
	for(i = 0; data[i] != NULL;i++)
	{
		//hexadecimale getallen moeten omgezet worden naar characters
		//sprintf zorgt ervoor dat dit gebeurt wanneer een decimaal echter maar 1 hexadecimaal getal nodig heeft
		//wordt alleen dat getal gedisplayed en niet de 0 daarvoor
		(data[i] < 16)
				? (sprintf(buf, "0%x", data[i]))
				: (sprintf(buf, "%x", data[i]));
		UART3_puts(buf);
	}
	UART3_puts("\r\n");
	UART_puts(recvlora());
	LCD_puts(recvlora());
}
//Function that sends a string to the RN2483A and returns the pointer to the response string.
char* sendrecvlora(char *sendbuf)
{
	//Static, because the array needs to be remembered after function ends.
	static char recvbuf[BUFLEN * 10]; //Reserve 300 bytes for the receive string.
	for(INT16U i = 0; i < (BUFLEN*10 - 1); i++); //Wipe recvbuf before you reuse it.

	UART3_puts(sendbuf); //Send the string to uart3, where rn2483 lives.
	UART3_gets(recvbuf, 0); //Receive the returnstring.

	#ifdef LORADEBUG
		UART_puts("sendrecv: ontvangen: "); UART_puts(recvbuf); UART_puts("\r\n");//Post received string to serial console.
		LCD_clear();
		LCD_put(recvbuf);
	#endif

	return recvbuf;
}

//Function that only listens to the loramodule for a message and returns the pointer to the received string.
char* recvlora(void)
{
	//Static, because the array needs to be remembered after function ends.
		static char recvbuf[BUFLEN * 10]; //Reserve 100 bytes for the receive string.
		for(INT16U i = 0; i < (BUFLEN*10 - 1); i++); //Wipe recvbuf before you reuse it.

		UART3_gets(recvbuf, 0); //Receive string from loramodule.

	#ifdef LORADEBUG
	UART_puts("recvlora: ontvangen: "); UART_puts(recvbuf); UART_puts("\r\n");//Post received string to serial console.
    LCD_clear();
	LCD_put(recvbuf);
	#endif

		return recvbuf;
}
//Function that sends a string to the lora module. Kinda unnecesary, but more legible than calling UART3.
void sendlora(char* sendbuf)
{
	UART3_puts(sendbuf);
}

//Function that resets the loramodule in case of malfunctioning
void resetlora(void)
{
	sendlora("sys reset\r\n");

    #ifdef LORADEBUG //Print if reset went well.
	UART_puts("resetlora: ");
	recvlora(); //recvlora receives response and prints it automatically.
	#endif
}
