#include "bsp_mp3.h"

static uint8_t counter = 0; 
uint8_t mp3_recv_buffer[20] = {0};

//----------------------------------------------------------------- 
/*******
 *@brief Config 
 ******/
static GPIO_InitTypeDef MP3_GPIO_Rx_Config =
{
    .GPIO_Pin = MP3_RX_PIN,
    .GPIO_Mode = GPIO_Mode_AF_PP,
    .GPIO_Speed = GPIO_Speed_50MHz
};
static GPIO_InitTypeDef MP3_GPIO_Tx_Config =
{
    .GPIO_Pin = MP3_TX_PIN,
    .GPIO_Mode = GPIO_Mode_IN_FLOATING,
    .GPIO_Speed = GPIO_Speed_50MHz
};
static USART_InitTypeDef MP3_USART3_Config =
{
    .USART_BaudRate = 9600,
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
static void MP3_GPIO_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE); 

    GPIO_Init(MP3_PORT, &MP3_GPIO_Rx_Config);
    GPIO_Init(MP3_PORT, &MP3_GPIO_Tx_Config);
}
 
static void MP3_USART3_Init(void)
{

    USART_DeInit(USART3);
    USART_Init(USART3, &MP3_USART3_Config);

    // Clear flag TC 
    // USART_GetFlagStatus(USART1, USART_FLAG_TC);
    // USART_ClearFlag(USART3, USART_FLAG_TC | USART_FLAG_TXE);
    // USART_ClearITPendingBit(USART3, USART_IT_TC | USART_IT_TXE);
    USART_ClearFlag(USART3, USART_FLAG_TC);
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
    USART_ITConfig(USART3, USART_IT_TXE, DISABLE);

    USART_Cmd(USART3, ENABLE);
}

void MP3_Init(void)
{
    MP3_GPIO_Init();
    MP3_USART3_Init();
}

//----------------------------------------------------------------- 
/*******
 *@brief MP3 control function 
 ******/
static void MP3_MSG_Pack(uint8_t cmd, ...)
{
    uint8_t msg[6] = {0x7E, 0, 0, 0, 0, 0};

    va_list pList;
    va_start(pList, cmd);

    switch (cmd)
    {
    case NEXT_SONG:
    case PREVIOUS_SONG:
    case INC_VOLUME:
    case DEC_VOLUME:
    case PAUSE:
    case RST:
    case PLAY:
    case STOP:
    case CURRENT_STATUS:
    case CURRENT_VOLUME:
    case CURRENT_EQ:
    case CURRENT_PLAY_MODE:
    case CURRENT_VERSION:
    case TF_FILE_COUNT:
    case UDISK_FILE_COUNT:
    case FLASH_FILE_COUNT:
    case TF_CURRENT_SONG:
    case FLASH_CURRENT_SONG:
    case PLAY_TIME:
    case TOTAL_TIME:
    case SONG_NAME:
    case FOLDER_COUNT:
    // {
        msg[1] = 0x02;
        msg[2] = cmd;
        msg[3] = 0xEF;

        COPY_usart_sendstring(USART3, msg, 0x4);
        break;
    // }
    case SPECIFY_DEVICE:
    case SPECIFY_VOLUME:
    case SPECIFY_EQ:
    case FOLDER_EXCHANGE:
    case LOOP_PLAY:
    // {
        msg[1] = 0x03;
        msg[2] = cmd;
        msg[3] = (uint8_t) va_arg(pList, uint32_t);
        msg[4] = 0xEF;

        COPY_usart_sendstring(USART3, msg, 0x5);
        break;
    // }
    case SPECIFY_SONG:
    case SPECIFY_FOLDER:
    // {
        msg[1] = 0x04;
        msg[2] = cmd;
        msg[3] = (uint8_t) va_arg(pList, uint32_t);
        msg[4] = (uint8_t) va_arg(pList, uint32_t);
        msg[5] = 0xEF;

        COPY_usart_sendstring(USART3, msg, 0x6);
        break;
    // }
    default:
        break;
    }

    va_end (pList);    
}

//----------------------------------------------------------------- 
/*******
 *@brief MP3 specific function
 ******/

// MP3 reset
void MP3_Reset(void)
{
    MP3_MSG_Pack(RST);
}

// MP3 pause
void MP3_Pause(void)
{
    MP3_MSG_Pack(PAUSE);
}

// MP3 play
void MP3_Play(void)
{
    MP3_MSG_Pack(PLAY);
}

// MP3 looplay, mode: LOOP_ALL, LOOP_FOL, LOOP_ONE, LOOP_RAM, ONE_STOP
void MP3_Loop_Play(uint8_t mode)
{
    MP3_MSG_Pack(LOOP_PLAY, mode);
}

// MP3 stop
void MP3_Stop(void)
{
    MP3_MSG_Pack(STOP);
}

// MP3 next song
void MP3_Next_Song(void)
{
    MP3_MSG_Pack(NEXT_SONG);
}

// MP3 previous song
void MP3_Previous_Song(void)
{
    MP3_MSG_Pack(PREVIOUS_SONG);
}

// MP3 play a specific song, song_nbr(SPI): 0~200 
void MP3_Specify_Song(uint8_t song_nbr)
{
    MP3_MSG_Pack(SPECIFY_SONG, 0x00, song_nbr);
}

// MP3 play a specific song of specific folder  
void MP3_Specify_Folder_Song(uint8_t folder_nbr, uint8_t song_nbr)
{
    MP3_MSG_Pack(SPECIFY_FOLDER, folder_nbr, song_nbr);
}

// MP3 increase volume
void MP3_Increase_Volume(void)
{
    MP3_MSG_Pack(INC_VOLUME);
}

// MP3 decrease volume
void MP3_Decrease_Volume(void)
{
    MP3_MSG_Pack(DEC_VOLUME);
}

// MP3 play with specific volume, strong: 0x00~0x1E
void MP3_Specify_Volume(uint8_t strong)
{
    MP3_MSG_Pack(SPECIFY_VOLUME, strong);
}

// MP3 specify EQ, mode: NORMAL，POP，ROCK，JAZZ，CLASSIC，BASE
void MP3_Specify_EQ(uint8_t mode)
{
    MP3_MSG_Pack(SPECIFY_EQ, mode);
}

// MP3 specify device, device: UDISK, TF, SPIFLASH
void MP3_Specify_Storage(uint8_t device)
{
    MP3_MSG_Pack(SPECIFY_DEVICE, device);
}

//----------------------------------------------------------------- 
/*******
 *@brief USART3 IRQ handler 
 ******/
void USART3_IRQHandler(void)
{
    if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
    {
        if (counter < 20)
        {
            mp3_recv_buffer[counter++] = USART_ReceiveData(USART3);
        }
        else
        {
            memset(mp3_recv_buffer, 0, 20);
            counter = 0;

            mp3_recv_buffer[counter++] = USART_ReceiveData(USART3);
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
}


