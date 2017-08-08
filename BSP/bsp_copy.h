#ifndef __BSP_COPY_H
#define __BSP_COPY_H

#include "bsp.h"

// USART utility
void COPY_usart_sendchar(USART_TypeDef* usart_typedef, uint8_t sChar);
void COPY_usart_sendstring(USART_TypeDef* usart_typedef, uint8_t* string, uint8_t length);

#endif


