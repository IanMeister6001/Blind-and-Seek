#include "includes.h"

uint32_t I2C_Timeout;

void I2C_Initialize()
{
	I2C_InitTypeDef I2C_InitStruct;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C3, ENABLE);

	GPIO_InitTypeDef GPIO_InitDef;
	GPIO_InitDef.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitDef.GPIO_OType = GPIO_OType_OD;
	GPIO_InitDef.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitDef.GPIO_Speed = GPIO_Speed_100MHz;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_I2C3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_I2C3);

	//SCL
	GPIO_InitDef.GPIO_Pin = GPIO_Pin_8;
	GPIO_Init(GPIOA, &GPIO_InitDef);

	//SDA
	GPIO_InitDef.GPIO_Pin = GPIO_Pin_9;
	GPIO_Init(GPIOC, &GPIO_InitDef);

	I2C_InitStruct.I2C_ClockSpeed = 100000;
	I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStruct.I2C_OwnAddress1 = MYADDRESS;
	I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;

	//Init I2C
	I2C_Init(I2C3, &I2C_InitStruct);

	//Enable I2C
	I2C_Cmd(I2C3, ENABLE);

}

uint8_t I2C_Read(I2C_TypeDef* I2Cx, uint8_t address, uint8_t reg) {
	uint8_t received_data;
	I2C_SendStart(I2Cx, address, I2C_Direction_Transmitter, 0);
	I2C_WriteData(I2Cx, reg);
	I2C_SendStop(I2Cx);
	I2C_SendStart(I2Cx, address, I2C_Direction_Receiver, 0);
	received_data = I2C_ReadNack(I2Cx);
	return received_data;
}

void I2C_Write(I2C_TypeDef* I2Cx, uint8_t address, uint8_t reg, uint8_t data)
{
	I2C_SendStart(I2Cx, address, I2C_Direction_Transmitter, 0);
	I2C_WriteData(I2Cx, reg);
	I2C_WriteData(I2Cx, data);
	I2C_SendStop(I2Cx);
}

void I2C_WriteMulti(I2C_TypeDef* I2Cx, uint8_t address, uint8_t reg, uint8_t* data, uint16_t count) {
	uint8_t i;
	I2C_SendStart(I2Cx, address, I2C_Direction_Transmitter, 0);
	I2C_WriteData(I2Cx, reg);
	for (i = 0; i < count; i++) {
		I2C_WriteData(I2Cx, data[i]);
	}
	I2C_SendStop(I2Cx);
}

void I2C_ReadMulti(I2C_TypeDef* I2Cx, uint8_t address, uint8_t reg, uint8_t* data, uint16_t count) {
	uint8_t i;
	I2C_SendStart(I2Cx, address, I2C_Direction_Transmitter, 1);
	I2C_WriteData(I2Cx, reg);
	I2C_SendStop(I2Cx);
	I2C_SendStart(I2Cx, address, I2C_Direction_Receiver, 1);
	for (i = 0; i < count; i++) {
		if (i == (count - 1))
			data[i] = I2C_ReadNack(I2Cx);
		else
			data[i] = I2C_ReadAck(I2Cx);
	}
}

int16_t I2C_SendStart(I2C_TypeDef* I2Cx, uint8_t address, uint8_t direction, uint8_t ack)
{
	I2C_GenerateSTART(I2Cx, ENABLE);

	I2C_Timeout = I2C_TIMEOUT;
	while (!I2C_GetFlagStatus(I2Cx, I2C_FLAG_SB) && I2C_Timeout) {
		if (--I2C_Timeout == 0x00) {
			return 1;
		}
	}

	if (ack) {
		I2C_AcknowledgeConfig(I2Cx, ENABLE);
	}

	I2C_Send7bitAddress(I2Cx, address, direction);

	if (direction == I2C_Direction_Transmitter) {
		I2C_Timeout = I2C_TIMEOUT;
		while (!I2C_GetFlagStatus(I2Cx, I2C_FLAG_ADDR) && I2C_Timeout) {
			if (--I2C_Timeout == 0x00) {
				return 2;
			}
		}
	} else if (direction == I2C_Direction_Receiver) {
		I2C_Timeout = I2C_TIMEOUT;
		while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED) && I2C_Timeout) {
			if (--I2C_Timeout == 0x00) {
				return 3;
			}
		}
	}
	I2Cx->SR2;

	return 0;
}

void I2C_WriteData(I2C_TypeDef* I2Cx, uint8_t data)
{
	I2C_Timeout = I2C_TIMEOUT;
	while (!I2C_GetFlagStatus(I2Cx, I2C_FLAG_TXE) && I2C_Timeout) {
		I2C_Timeout--;
	}
	I2C_SendData(I2Cx, data);
}

uint8_t I2C_SendStop(I2C_TypeDef* I2Cx)
{
	I2C_Timeout = I2C_TIMEOUT;
	while (((!I2C_GetFlagStatus(I2Cx, I2C_FLAG_TXE)) || (!I2C_GetFlagStatus(I2Cx, I2C_FLAG_BTF))) && I2C_Timeout) {
		if (--I2C_Timeout == 0x00) {
			return 1;
		}
	}

	I2C_GenerateSTOP(I2Cx, ENABLE);

	return 0;
}

uint8_t I2C_ReadAck(I2C_TypeDef* I2Cx) {
	uint8_t data;
	I2C_AcknowledgeConfig(I2Cx, ENABLE);

	I2C_Timeout = I2C_TIMEOUT;
	while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED) && I2C_Timeout) {
		I2C_Timeout--;
	}

	data = I2C_ReceiveData(I2Cx);
	return data;
}

uint8_t I2C_ReadNack(I2C_TypeDef* I2Cx) {
	uint8_t data;

	I2C_AcknowledgeConfig(I2Cx, DISABLE);

	I2C_GenerateSTOP(I2Cx, ENABLE);

	I2C_Timeout = I2C_TIMEOUT;
	while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED) && I2C_Timeout) {
		I2C_Timeout--;
	}

	data = I2C_ReceiveData(I2Cx);
	return data;
}

