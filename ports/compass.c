#include "includes.h"
#include  <math.h>

void Compass_WriteRegister(uint8_t reg, uint8_t data)
{
	I2C_Write(I2CINTERFACE, HMC5883L_ADDRESS, reg, data);
}
uint8_t Compass_ReadRegister(uint8_t reg)
{
	return I2C_Read(I2CINTERFACE, HMC5883L_ADDRESS, reg);
}

void Compass_SetOffset(int xo, int yo)
{
	xOffset = xo;
	yOffset = yo;
}

void Compass_SetRange(hmc5883l_range_t range)
{
	switch(range)
	{
		case HMC5883L_RANGE_0_88GA:
			MGPerDigit = 0.073f;
			break;

		case HMC5883L_RANGE_1_3GA:
			MGPerDigit = 0.92f;
			break;

		case HMC5883L_RANGE_1_9GA:
			MGPerDigit = 1.22f;
			break;

		case HMC5883L_RANGE_2_5GA:
			MGPerDigit = 1.52f;
			break;

		case HMC5883L_RANGE_4GA:
			MGPerDigit = 2.27f;
			break;

		case HMC5883L_RANGE_4_7GA:
			MGPerDigit = 2.56f;
			break;

		case HMC5883L_RANGE_5_6GA:
			MGPerDigit = 3.03f;
			break;

		case HMC5883L_RANGE_8_1GA:
			MGPerDigit = 4.35f;
			break;

		default:
			break;
	}

	Compass_WriteRegister(HMC5883L_REG_CONFIG_B, range << 5);
}

hmc5883l_range_t Compass_GetRange(void)
{
    return (hmc5883l_range_t)((Compass_ReadRegister(HMC5883L_REG_CONFIG_B) >> 5));
}

void Compass_SetMeasurementMode(hmc5883l_mode_t mode)
{
	uint8_t value;

	value = Compass_ReadRegister(HMC5883L_REG_MODE);
	value &= 0b11111100;
	value |= mode;

	Compass_WriteRegister(HMC5883L_REG_MODE, value);
}

hmc5883l_mode_t Compass_GetMeasurementMode(void)
{
	uint8_t value;

	value = Compass_ReadRegister(HMC5883L_REG_MODE);
	value &= 0b00000011;

	return (hmc5883l_mode_t)value;
}

void Compass_SetDataRate(hmc5883l_dataRate_t dataRate)
{
	uint8_t value;

	value = Compass_ReadRegister(HMC5883L_REG_CONFIG_A);
	value &= 0b11100011;
	value |= (dataRate << 2);

	Compass_WriteRegister(HMC5883L_REG_CONFIG_A, value);
}

hmc5883l_dataRate_t Compass_GetDataRate(void)
{
	uint8_t value;

	value = Compass_ReadRegister(HMC5883L_REG_CONFIG_A);
	value &= 0b00011100;
	value >>= 2;

	return (hmc5883l_dataRate_t)value;
}

void Compass_SetSamples(hmc5883l_samples_t samples)
{
	uint8_t value;

	value = Compass_ReadRegister(HMC5883L_REG_CONFIG_A);
	value &= 0b10011111;
	value |= (samples << 5);

	Compass_WriteRegister(HMC5883L_REG_CONFIG_A, value);
}

hmc5883l_samples_t Compass_GetSamples(void)
{
	uint8_t value;

	value = Compass_ReadRegister(HMC5883L_REG_CONFIG_A);
	value &= 0b01100000;
	value >>= 5;

	return (hmc5883l_samples_t)value;
}

void Compass_Initialize(void)
{
	//Set range
	Compass_SetRange(HMC5883L_RANGE_1_3GA);

	//Set measurement mode
	Compass_SetMeasurementMode(HMC5883L_CONTINOUS);

	//Set data rate
	Compass_SetDataRate(HMC5883L_DATARATE_30HZ);

	//Set number of samples averaged
	Compass_SetSamples(HMC5883L_SAMPLES_8);

	//Set the calibration offsets
	Compass_SetOffset(-51, -134);
}

float Compass_GetHeading(void)
{

	uint8_t data[6];
	I2C_ReadMulti(I2CINTERFACE, HMC5883L_ADDRESS, HMC5883L_REG_OUT_X_M, &data, 6);

	INT16S xheading = ((data[0] << 8) + data[1]) - -202;
	INT16S yheading = ((data[4] << 8) + data[5]) - -284;

	float HeadingX = xheading * MGPerDigit;
	float HeadingY = yheading * MGPerDigit;

	float heading = atan2f(HeadingY, HeadingX);

	//Compensation for the declination angle of 1 degree and 28 minutes
	float declinationAngle = (1.0 + (28.0 / 60.0)) / (180 / M_PI);
	heading += declinationAngle;

	if (heading < 0)
		heading += 2 * M_PI;

	if (heading > 2 * M_PI)
		heading -= 2 * M_PI;

	heading *= (180/M_PI);

	return heading;
}

