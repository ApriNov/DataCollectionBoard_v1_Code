#ifndef DAC8830_H
#define DAC8830_H

#include "system.h"
#include "spi.h"

#define CS1 PAout(4)

void DAC8830_Init(void);
void DAC_SetValue(u16 val);
void DAC_SetVout(int mv);

#endif
