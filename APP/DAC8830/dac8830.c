#include "dac8830.h"

void DAC8830_Init(void)
{
	SPI1_Init();
}

void DAC_SetValue(u16 val)
{
	CS1 = 0;
	SPI1_WriteByte(val);
	CS1 = 1;
}


void DAC_SetVout(int mv)
{
	u16 val = (double)mv/2500.0*65536;
	DAC_SetValue(val);
}
