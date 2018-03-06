#include "usart.h"	
#include "stdlib.h"
#include "dac8830.h"

extern double vout[4];
extern int valarray[4];

//u8 i = 0;
char RxBuf[256] = {0}; //0xff
//u8 RxPtr = 0; //接收指针
//u8 GetPtr = 0; //解析指针

uartCtrl uart;
BOOL bUartNewData = FALSE;

int fputc(int ch,FILE *p)  //函数默认的，在使用printf函数时自动调用
{
	USART_SendData(USART1,(u8)ch);	
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
	return ch;
}

/*******************************************************************************
* 函 数 名         : USART1_Init
* 函数功能		   : USART1初始化函数
* 输    入         : bound:波特率
* 输    出         : 无
*******************************************************************************/ 
void USART1_Init(u32 bound)
{
   //GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	 //打开时钟
 
	
	/*  配置GPIO的模式和IO口 */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;//TX			   //串口输出PA9
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;	    //复用推挽输出
	GPIO_Init(GPIOA,&GPIO_InitStructure);  /* 初始化串口输入IO */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;//RX			 //串口输入PA10
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;		  //模拟输入
	GPIO_Init(GPIOA,&GPIO_InitStructure); /* 初始化GPIO */
	

   //USART1 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART1, &USART_InitStructure); //初始化串口1
	
	USART_Cmd(USART1, ENABLE);  //使能串口1 
	
	USART_ClearFlag(USART1, USART_FLAG_TC);
		
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启相关中断

	//Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、	
}

/*******************************************************************************
* 函 数 名         : USART1_IRQHandler
* 函数功能		   : USART1中断函数
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/ 
void USART1_IRQHandler(void)                	//串口1中断服务程序
{
//	u8 r;
//	int mv;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断
	{
		RxBuf[uart.iRxPtr] =USART_ReceiveData(USART1);//(USART1->DR);	//读取接收到的数据
		uart.iRxPtr = (uart.iRxPtr + 1)&0xFF; //环形缓冲处理
		bUartNewData = TRUE;
	} 
} 	


void dataFrame_OK(uartCtrl* pUart, char* pBuf)
{
	while(pUart->iGetPtr != pUart->iRxPtr){
		if(pUart->iOrder == 0)
		{
			if(pBuf[pUart->iGetPtr] == 'v'){
				pUart->iOrder = VOUT;
				pUart->iEndPtr = (pUart->iGetPtr+1)&0xFF;
			}
			else if(pBuf[pUart->iGetPtr] == 'n'){
				pUart->iOrder = VALOUT;
				pUart->iEndPtr = (pUart->iGetPtr+1)&0xFF;
			}
			else if(pBuf[pUart->iGetPtr] == 'd'){
				pUart->iOrder = DACSET;
				pUart->iEndPtr = (pUart->iGetPtr+4)&0xFF;
			}
		}
		else{
			if(pUart->iGetPtr == pUart->iEndPtr){
				switch(pUart->iOrder){
					case VOUT:
						switch(pBuf[pUart->iGetPtr]){
							case '1':
								printf("%f\n", vout[0]);
								break;
							case '2':
								printf("%f\n", vout[1]);
								break;
							case '3':
								printf("%f\n", vout[2]);
								break;
							case '4':
								printf("%f\n", vout[3]);
								break;
							case '5':
								printf("Vout1: %f, Vout2: %f, Vout3: %f, Vout4: %f\n", vout[0],vout[1],vout[2],vout[3]);
								break;
							default: break;
						}
						break;
					case VALOUT:
						switch(pBuf[pUart->iGetPtr]){
							case '1':
								printf("%x\n", valarray[0]);
								break;
							case '2':
								printf("%x\n", valarray[1]);
								break;
							case '3':
								printf("%x\n", valarray[2]);
								break;
							case '4':
								printf("%x\n", valarray[3]);
								break;
							case '5':
								printf("Val1: %x, Val2: %x, Val3: %x, Val4: %x\n", valarray[0],valarray[1],valarray[2],valarray[3]);
								break;
							default: break;
						}
						break;
					case DACSET:{
						char buf[5];
						int mv;
						buf[0] = pBuf[(pUart->iGetPtr-3)&0xFF];buf[1] = pBuf[(pUart->iGetPtr-2)&0xFF];buf[2] = pBuf[(pUart->iGetPtr-1)&0xFF];buf[3] = pBuf[(pUart->iGetPtr-0)&0xFF];buf[4] = '\n';
						mv = atoi(buf);
						DAC_SetVout(mv);
						printf("DAC output is: %d mv\n",mv);
					}
						break;
					default: break;
				}
				pUart->iOrder = 0;
			}
		}
		pUart->iGetPtr = (pUart->iGetPtr+1)&0xFF;
	}
}
