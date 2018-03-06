#ifndef __usart_H
#define __usart_H

#include "system.h" 
#include "stdio.h" 

#define VOUT  1
#define VALOUT 5
#define DACSET  9
typedef struct{
	u8 iRxPtr;
	u8 iGetPtr;
	u8 iOrder;
	u8 iEndPtr;
}uartCtrl;

typedef enum {FALSE = 0, TRUE = 1} BOOL;

extern char RxBuf[256];
//extern u8 RxPtr; 
//extern u8 GetPtr; 
extern uartCtrl uart;
extern BOOL bUartNewData;

void USART1_Init(u32 bound);
void dataFrame_OK(uartCtrl* pUart, char* pBuf);



#endif


