#include "bsp_usart.h"

// extern u8 Tx_Buffer[];
// extern size_t Tx_Buffer_LEN;

int fputc(int ch, FILE *f)
{
    USART_SendData(USART1, (unsigned char) ch);

    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) != SET);

    return (ch);
}

//----------------------------------------------------------------- 
void USART_Send_Char(USART_TypeDef* USARTx, u8 ch)
{
    USART_ClearFlag(USARTx, USART_FLAG_TC);
    
    USART_SendData(USARTx, (uint8_t)ch);

    while (USART_GetFlagStatus(USARTx, USART_FLAG_TC) != SET);
}

void USART_Send_String(USART_TypeDef* USARTx, u8 *str)
{
    uint8_t len = 0;
    len = strlen((char *)str);

    USART_ClearFlag(USART1, USART_FLAG_TC);
    
    while (len--)
    {
        USART_Send_Char(USARTx, *str);
        str++;
    }

    // strncpy(Tx_Buffer, str, Tx_Buffer_LEN);
    // USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
}


