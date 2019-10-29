/////////////////////////////////////////////////////////////////////////
//	File: button.c
//	Authors: projectgroep 2, EV3A
// 	Bevat de initialisatiecode voor het gebruik van de noodknop en de knoptask.
/////////////////////////////////////////////////////////////////////////

#include <includes.h>
#include "main.h"
#include "button.h"

/////////////////////////////////////////////////////////////////////////
//	Function: Knoppen_Init();
//	Doel: initialiseert de noodknop.
// 	args: geen
/////////////////////////////////////////////////////////////////////////
void Knoppen_Init(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;

	/*-------------------------- GPIO Configuration for PD6, de noodknop ----------------------------*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	//GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_InitTypeDef GPIO_InitStructure2;

	/*-------------------------- GPIO Configuration for PD7, de tipknop ----------------------------*/
	GPIO_InitStructure2.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure2.GPIO_Mode = GPIO_Mode_IN;
	//GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure2.GPIO_PuPd = GPIO_PuPd_DOWN;
	//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure2);
}

/////////////////////////////////////////////////////////////////////////
//	Task: KnopTask();
//	Doel: polt op input van de knoppen en stuurt de ingedrukte knop via
//		  een flag naar gametask.
// 	args: geen
//////////////////////////////////////////////////////////////////////////
void KnopTask(void *pdata)
{
	UART_puts((char *)__func__);UART_puts(" is gestart.\n\r"); //Debugout die laat weten dat task is gestart.

	INT8U error; //Errorvariabele voor het os.

	while (TRUE)
	{
		INT8U ingedrukt = 0; //Variabele voor de ingedrukte knop. Wordt elke loop opnieuw aangemaakt en op 0 gesteld.

		if(GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_6) == (uint8_t)Bit_SET)
		{
			ingedrukt = NOODKNOP_ON; //Read value of noodknop.
			//Post de waarde van de ingedrukte toets naar de flag, zodat gametask hem op kan vangen:
			OSFlagPost(KnopFlag, ingedrukt, OS_FLAG_SET, &error);

			//Wacht tot je de go-ahead van gametask terugkrijgt om weer te gaan pollen:
			OSFlagPend(KnopFlag,
		                0xf000, //Check de eerste vier bits aan de linkerkant, dit stuurt de gametask naar deze flag.
		                OS_FLAG_WAIT_SET_ALL + OS_FLAG_CONSUME,
		                WAIT_FOREVER,
		                &error);

			#ifdef OUTPUT_DEBUG
			UART_puts("\r\nKnopTask: go-ahead received, continue polling\r\n");
			#endif
		}
		if(GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_7) == (uint8_t)Bit_SET)
		{
			ingedrukt = TIPKNOP_ON; //Read value of noodknop.
			//Post de waarde van de ingedrukte toets naar de flag, zodat gametask hem op kan vangen:
			OSFlagPost(KnopFlag, ingedrukt, OS_FLAG_SET, &error);

			//Wacht tot je de go-ahead van gametask terugkrijgt om weer te gaan pollen:
			OSFlagPend(KnopFlag,
		                0xf000, //Check de eerste vier bits aan de linkerkant, dit stuurt de gametask naar deze flag.
		                OS_FLAG_WAIT_SET_ALL + OS_FLAG_CONSUME,
			            WAIT_FOREVER,
		                &error);

			#ifdef OUTPUT_DEBUG
			UART_puts("\r\nKnopTask: go-ahead received, continue polling\r\n");
			#endif
		}

		OSTimeDly(LOOP_DELAY); //Give some time to other tasks.
	}
}
