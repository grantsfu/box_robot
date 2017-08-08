#ifndef __BSP_USART_H
#define __BSP_USART_H

#include "bsp.h"

void USART_Send_Char(USART_TypeDef* USARTx, u8 ch);
void USART_Send_String(USART_TypeDef* USARTx, u8 *str);

#endif

