#include "bsp_copy.h"

//----------------------------------------------------------------- 
/*******
 *@brief USART utility 
 ******/
uint8_t nCount = 0;

void COPY_usart_sendchar(USART_TypeDef* usart_typedef, uint8_t sChar) {
	USART_SendData(usart_typedef, (uint8_t) sChar);
	while (USART_GetFlagStatus(usart_typedef, USART_FLAG_TXE) == RESET) {
	}
}

void COPY_usart_sendstring(USART_TypeDef* usart_typedef, uint8_t* string, uint8_t length) {
	uint8_t* p = string;
	uint8_t len;

    // USART_ITConfig(usart_typedef, USART_IT_TXE, ENABLE);
	
	if(length==0){
		while( *p++ != '\0' ){
			USART_SendData(usart_typedef, (uint8_t)(*p));
			while (USART_GetFlagStatus(usart_typedef, USART_FLAG_TXE) == RESET) {
			}
		}
	}
	else
	{
		for(len=0;len<length;len++){
			USART_SendData(usart_typedef, (uint8_t)(*p));
			p++;
			while (USART_GetFlagStatus(usart_typedef, USART_FLAG_TXE) == RESET) {
			}
			
			// TEST
			nCount++;
		}
	}
}


