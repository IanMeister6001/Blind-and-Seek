//////////////////////////////////////////////////////////////////////////////
// prog: interrupt.c
// comm: demonstrates interrupt handling
// auth: MSC
//////////////////////////////////////////////////////////////////////////////

#include <includes.h>
#include "main.h"

#include <stm32f4xx_tim.h>
#include <stm32f4xx_rcc.h>
#include <misc.h>

void InitializeTimer()
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    TIM_TimeBaseInitTypeDef timerInitStructure;
    // every 1 second an interrupt on timer2
    timerInitStructure.TIM_Prescaler = 42000 - 1;  // 168 MHz / 2 = 84 MHz / 42 kz = 2 KHz
    timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up; // countup timer mode
    timerInitStructure.TIM_Period = 2000 - 1;      // 2 KHz / 2000 = 1 sec
    timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    timerInitStructure.TIM_RepetitionCounter = 0;

    TIM_TimeBaseInit(TIM2, &timerInitStructure);

    TIM_Cmd(TIM2, ENABLE);
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
}

void EnableTimerInterrupt()
{
    NVIC_InitTypeDef nvicStructure;

    nvicStructure.NVIC_IRQChannel = TIM2_IRQn;
    nvicStructure.NVIC_IRQChannelPreemptionPriority = 0;
    nvicStructure.NVIC_IRQChannelSubPriority = 1;
    nvicStructure.NVIC_IRQChannelCmd = ENABLE;

    NVIC_Init(&nvicStructure);
}

extern void TIM2_IRQHandler()
{
	INT8U    			 error;
    static unsigned char counter = 0;

    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

		OSFlagPost(FlagIntHandle, counter, OS_FLAG_SET, &error);
	    if (Uart_debug_out & INTERRUPT_DEBUG_OUT)
	    {
	    	UART_puts("\n\rTIM2_IRQ interrupt posted: "); UART_putint(counter); UART_puts("\n\r");
	    }
	    counter++;
    }
}


//////////////////////////////////////////////////////////////////////////////
// func: InterruptTask
// args: void *pdata, needed by os
// comm: waits for an interrupt and reacts
//////////////////////////////////////////////////////////////////////////////
void InterruptTask(void *pdata)
{
	INT8U    error;
  	OS_FLAGS value;  // OS_FLAGS == INT16U (os_cfg.h)

	UART_puts((char *)__func__);UART_puts("started\n\r");

    InitializeTimer();
	EnableTimerInterrupt();

	while (TRUE)
	{
   		value = OSFlagPend(FlagIntHandle,
		                   0xffff, // 4 nibbles - 2 bytes
						   OS_FLAG_WAIT_SET_ANY + OS_FLAG_CONSUME,
						   WAIT_FOREVER,
						   &error);
		
  		//LCD_putint(value);       // output to lcd
  		//LED_put((INT8U)value);   // set leds, force as byte, there are only 8 leds on board

	    if (Uart_debug_out & INTERRUPT_DEBUG_OUT)
	    {
	    	UART_puts("\n\rInterruptTask received: "); UART_putint(value); UART_puts("\n\r");
	    }
	}
}

