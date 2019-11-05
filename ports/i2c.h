/*
 * At the moment, the active interface is I2C3. This has the following pinout:
 * - SCL on A8
 * - SDA on C9
 *
 * This is the I2C interface that has been exposed by the blue ARM development board
 */

#define MYADDRESS 0x01
#define ACTIVEINTERFACE I2C3
#define I2C_TIMEOUT	200000

void I2C_Initialize();
void I2CSendData(uint8_t data);
uint8_t I2C_Read(I2C_TypeDef* I2Cx, uint8_t address, uint8_t reg);
void I2C_Write(I2C_TypeDef* I2Cx, uint8_t address, uint8_t reg, uint8_t data);
void I2C_ReadMulti(I2C_TypeDef* I2Cx, uint8_t address, uint8_t reg, uint8_t* data, uint16_t count);
int16_t I2C_SendStart(I2C_TypeDef* I2Cx, uint8_t address, uint8_t direction, uint8_t ack);
void I2C_WriteData(I2C_TypeDef* I2Cx, uint8_t data);
uint8_t I2C_SendStop(I2C_TypeDef* I2Cx);
uint8_t I2C_ReadAck(I2C_TypeDef* I2Cx);
uint8_t I2C_ReadNack(I2C_TypeDef* I2Cx);
