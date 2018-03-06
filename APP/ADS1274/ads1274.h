#ifndef ADS1274_H
#define ADS1274_H

#include "system.h"

extern int valarray[4];
extern double vout[4];

#define ADFORMAT0 PBout(5)  //format[2:0] 000 spi->dynamic
#define ADFORMAT1 PBout(6)  //            001 spi->fixed
#define ADFORMAT2 PBout(7)  

#define ADMODE0   PBout(8)  // 高分辨率模式 mode0 = 1  mode1 = 0
#define ADMODE1   PBout(9)  //                

#define ADCLKDIV  PAout(0) //时钟分频， High-resolution=>保持高电平
#define ADSYNC	  PAout(1) //同步引脚，无需同步=>保持高电平

void ADS1274_Init(void);
void ADS1274_GetData(int* pData);
#endif
