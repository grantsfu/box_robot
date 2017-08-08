#include "bsp_bluetooth.h"

//----------------------------------------------------------------- 
/*******
 *@brief Config 
 ******/ 
u8 Rx_Buffer[BLE_RX_BUFFER_SIZE] = {0};
u8 RxCounter = 0;
// extern u8 Tx_Buffer[];
// extern size_t Tx_Buffer_LEN;
// u8 TxCounter;

static GPIO_InitTypeDef BLE_GPIO_Rx_Config =
{
    .GPIO_Pin = BLE_Rx_Pin,
    .GPIO_Mode = GPIO_Mode_AF_PP,
    .GPIO_Speed = GPIO_Speed_50MHz
};
static GPIO_InitTypeDef BLE_GPIO_Tx_Config =
{
    .GPIO_Pin = BLE_Tx_Pin,
    .GPIO_Mode = GPIO_Mode_IN_FLOATING,
    .GPIO_Speed = GPIO_Speed_50MHz
};
static GPIO_InitTypeDef BLE_PWRC_Config =
{
    .GPIO_Pin = BLE_PWRC_Pin,
    .GPIO_Mode = GPIO_Mode_Out_PP,
    .GPIO_Speed = GPIO_Speed_50MHz
};
static GPIO_InitTypeDef BLE_DISC_Config =
{
    .GPIO_Pin = BLE_DISC_Pin,
    .GPIO_Mode = GPIO_Mode_Out_PP,
    .GPIO_Speed = GPIO_Speed_50MHz
};
static USART_InitTypeDef BLE_USART1_Config =
{
    .USART_BaudRate = 115200,
    .USART_WordLength = USART_WordLength_8b,
    .USART_StopBits = USART_StopBits_1,
    .USART_Parity = USART_Parity_No,
    .USART_HardwareFlowControl = USART_HardwareFlowControl_None,
    .USART_Mode = USART_Mode_Rx | USART_Mode_Tx
};

//-----------------------------------------------------------------
/*******
 *@brief Initialization 
 ******/  
static void BLE_GPIO_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); 

    GPIO_Init(BLE_DATA_PORT, &BLE_GPIO_Rx_Config);
    GPIO_Init(BLE_DATA_PORT, &BLE_GPIO_Tx_Config);
    GPIO_Init(BLE_CTL_PORT, &BLE_PWRC_Config);
    GPIO_Init(BLE_CTL_PORT, &BLE_DISC_Config);

    // GPIO_ResetBits(BLE_CTL_PORT, BLE_DISC_Pin);
    // GPIO_SetBits(BLE_CTL_PORT, BLE_PWRC_Pin);
}
 
static void BLE_USART1_Init(void)
{
    USART_Init(USART1, &BLE_USART1_Config);

    // Clear flag TC 
    // USART_GetFlagStatus(USART1, USART_FLAG_TC);
    USART_ClearFlag(USART1, USART_FLAG_TC);
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    USART_ITConfig(USART1, USART_IT_TXE, DISABLE);

    USART_Cmd(USART1, ENABLE);
}

//-----------------------------------------------------------------  
/*******
 *@brief Basic control 
 ******/
void BLE_Disconnect(void)
{
    GPIO_ResetBits(BLE_CTL_PORT, BLE_DISC_Pin);
}
 
void BLE_Enable_Connect(void)
{
    GPIO_SetBits(BLE_CTL_PORT, BLE_DISC_Pin);
}
 
void BLE_Awake(void)
{
    GPIO_ResetBits(BLE_CTL_PORT, BLE_PWRC_Pin);
}
 
void BLE_Sleep(void)
{
    GPIO_SetBits(BLE_CTL_PORT, BLE_PWRC_Pin);
}

void BLE_Init(void)
{
    BLE_GPIO_Init();
    BLE_USART1_Init();

    BLE_Awake();
    BLE_Enable_Connect();

    // Wait BLE initialization
    // Delay_10us(100*5000);
}

//----------------------------------------------------------------- 
/*******
 *@brief Advanced function 
 ******/
void BLE_Set_Name(void)
{
    uint8_t cmd[] = "AT+VER\r\n";

    BLE_Disconnect();
    Delay_10us(100*100);
    USART_Send_String(USART1, cmd);
    Delay_10us(100*100);
    
    BLE_Enable_Connect();
}

//----------------------------------------------------------------- 
/*******
 *@brief USART1 interrupt function 
 ******/
void USART1_IRQHandler(void)
{
    // uint8_t data; 

    // Receive interrupt
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
        if (RxCounter < BLE_RX_BUFFER_SIZE)
        {
            Rx_Buffer[RxCounter++] = USART_ReceiveData(USART1);
        }
        else
        {
            memset(Rx_Buffer, 0, BLE_RX_BUFFER_SIZE);
            RxCounter = 0;

            Rx_Buffer[RxCounter++] = USART_ReceiveData(USART1);
        }

        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
        USART_ClearFlag(USART1, USART_FLAG_RXNE);
    }

    if (USART_GetFlagStatus(USART1, USART_FLAG_ORE) != RESET)
    {
       if (USART_GetITStatus(USART1, USART_IT_ORE) != RESET)
       {
            USART_ClearITPendingBit(USART1, USART_IT_ORE);
            USART_ClearFlag(USART1, USART_FLAG_ORE);
       }
    }
    
    // if (USART_GetITStatus(USART1, USART_IT_TXE) != RESET)
    // {
    //     USART_ClearITPendingBit(USART1, USART_IT_TXE | USART_IT_TC);       

    //     if (TxCounter == (Tx_Buffer_LEN-1))
    //     {
    //         TxCounter = 0;

    //         USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
    //         USART_SendData(USART1, Tx_Buffer[Tx_Buffer_LEN-1]);
    //     }
    //     else
    //     {
    //         USART_SendData(USART1, Tx_Buffer[TxCounter++]);
    //     }
    // }
}












