#include "ads1274.h"
#include "spi.h"
#include "SysTick.h"

int valarray[4];
double vout[4];

void ADS1274_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
	EXTI_InitTypeDef  EXTI_InitStructure;
	
	/*spi2口初始化*/
	SPI2_Init();

	/*MCO时钟输出 PA8*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	/* IO口设置 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	RCC_MCOConfig(RCC_MCO_HSE);
	
	/* 打开GPIOA时钟      ADCLKDIV    ADSYNC*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	
	/* IO口设置 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	ADCLKDIV = 1;
	ADSYNC = 1;
	
	/* 打开GPIOB时钟     ADFORMAT[2:0]   ADMODE[1:0] */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	
	/* IO口设置 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	ADFORMAT0 = 1; ADFORMAT1 = 0; ADFORMAT2 = 0;
	ADMODE0 = 1; ADMODE1 = 1;

	/*打开数据接收中断 ADDRDY GPIOA2*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource2);

	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	EXTI_InitStructure.EXTI_Line = EXTI_Line2;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

}

void ADS1274_GetData(int* pData)
{
	SPI2_ReadByte(pData);
}

/*ADDRDY中断处理函数*/
void EXTI2_IRQHandler(void)
{
	//delay_us(1);
	if(EXTI_GetFlagStatus(EXTI_Line2) == SET){
		ADS1274_GetData(valarray);
	}
	EXTI_ClearITPendingBit(EXTI_Line2);
}
