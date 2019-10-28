/////////////////////////////////////////////////////////////////////////
//	File: button.h
//	Authors: projectgroep 2, EV3A
// 	Bevat de initialisatiecode voor het gebruik van de noodknop.
/////////////////////////////////////////////////////////////////////////

#include <includes.h>
#include "main.h"
#include "button.h"

/////////////////////////////////////////////////////////////////////////
//	Function: Noodknop_Init();
//	Doel: initialiseert de noodknop.
// 	args: geen
/////////////////////////////////////////////////////////////////////////
void Noodknop_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/*-------------------------- GPIO Configuration for PD6, de noodknop ----------------------------*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
}
