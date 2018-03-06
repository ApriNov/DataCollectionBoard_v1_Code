#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "usart.h" 
#include "spi.h"
#include "dac8830.h"
#include "ads1274.h"

int main()
{
	SysTick_Init(72);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //中断优先级分组 分2组
	USART1_Init(115200);
	DAC8830_Init();
	ADS1274_Init();
	
	//DAC_SetValue(0x0106);
	DAC_SetVout(17);
	
	while(1){
		if(bUartNewData == TRUE){
			bUartNewData = FALSE;
			dataFrame_OK(&uart, RxBuf);			
		}
	}
}
