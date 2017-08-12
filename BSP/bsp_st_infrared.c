#include "bsp_st_infrared.h"

__IO static uint16_t st_infrared_dma_buffer[30][2] = {0};
__IO static uint16_t st_infrared_filter_data[2] = {0};

//----------------------------------------------------------------- 
/*******
 *@brief Parameter config
 ******/
static GPIO_InitTypeDef ST_Infrared_OUT5_Config =
{
    .GPIO_Pin = ST_INFRARED_OUT5_PIN,
    .GPIO_Mode = GPIO_Mode_IN_FLOATING,
    .GPIO_Speed = GPIO_Speed_2MHz
};

static GPIO_InitTypeDef ST_Infrared_OUT6_Config =
{
    .GPIO_Pin = ST_INFRARED_OUT6_PIN,
    .GPIO_Mode = GPIO_Mode_IN_FLOATING,
    .GPIO_Speed = GPIO_Speed_2MHz
};

static GPIO_InitTypeDef ST_Infrared_AOUT5_Config =
{
    .GPIO_Pin = ST_INFRARED_AOUT5_PIN,
    .GPIO_Mode = GPIO_Mode_AIN,
    .GPIO_Speed = GPIO_Speed_50MHz
};

static GPIO_InitTypeDef ST_Infrared_AOUT6_Config =
{
    .GPIO_Pin = ST_INFRARED_AOUT6_PIN,
    .GPIO_Mode = GPIO_Mode_AIN,
    .GPIO_Speed = GPIO_Speed_50MHz
};

static DMA_InitTypeDef ST_Infrared_DMA_Config =
{
    .DMA_PeripheralBaseAddr = (uint32_t)ADC1_DR_ADDRESS,
    .DMA_MemoryBaseAddr = (uint32_t)(&st_infrared_dma_buffer[0][0]),
    .DMA_DIR = DMA_DIR_PeripheralSRC,
    .DMA_BufferSize = ST_INFRARED_DMA_BUFSIZE,
    .DMA_PeripheralInc = DMA_PeripheralInc_Disable,
    .DMA_MemoryInc = DMA_MemoryInc_Enable,
    .DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord,
    .DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord,
    .DMA_Mode = DMA_Mode_Circular,
    .DMA_Priority = DMA_Priority_VeryHigh,
    .DMA_M2M = DMA_M2M_Disable
};

static ADC_InitTypeDef ST_Infrared_ADC_Config =
{
    .ADC_Mode = ADC_Mode_Independent,
    .ADC_ScanConvMode = ENABLE,
    .ADC_ContinuousConvMode = ENABLE,
    .ADC_ExternalTrigConv = ADC_ExternalTrigConv_None, // use software to trig
    .ADC_DataAlign = ADC_DataAlign_Right,
    .ADC_NbrOfChannel = 2; // CH2 && CH3    
};



//----------------------------------------------------------------- 
/*******
 *@brief Initialization
 ******/
static void ST_Infrared_RCC_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |
    RCC_APB2Periph_AFIO |
    RCC_APB2Periph_ADC1 |
    RCC_APB2Periph_GPIOC, ENABLE);

    RCC_ADCCLKConfig(RCC_PCLK2_Div8); // 72MHz/8 = 9MHz
}

static void ST_Infrared_GPIO_Init(void)
{
    /*******
     *@brief Digital part
     ******/
    BKP_TamperPinCmd(DISABLE);
    PWR_BackupAccessCmd(ENABLE);

    GPIO_Init(ST_INFRARED_DOUT_PORT, &ST_Infrared_OUT5_Config);
    GPIO_Init(ST_INFRARED_DOUT_PORT, &ST_Infrared_OUT6_Config);

    GPIO_SetBits(ST_INFRARED_DOUT_PORT, ST_INFRARED_OUT5_PIN);
    GPIO_SetBits(ST_INFRARED_DOUT_PORT, ST_INFRARED_OUT6_PIN);

    /*******
     *@brief Analog part
     ******/
    GPIO_Init(ST_INFRARED_AOUT_PORT, &ST_Infrared_AOUT5_Config);
    GPIO_Init(ST_INFRARED_AOUT_PORT, &ST_Infrared_AOUT6_Config);
}

void ST_Infrared_DMA_Init(void)
{
    DMA_DeInit(ST_INFRARED_DMA_CH);
    DMA_Init(ST_INFRARED_DMA_CH, &ST_Infrared_DMA_Config);

    DMA_ITConfig(ST_INFRARED_DMA_CH, DMA_IT_TC, ENABLE);
    DMA_Cmd(ST_INFRARED_DMA_CH, DISABLE);
}

void ST_Infrared_ADC_Init(void)
{
    ADC_Init(ST_INFRARED_ADCX, &ST_Infrared_ADC_Config);

    ADC_RegularChannelConfig(ST_INFRARED_ADCX, ADC_Channel_2, 1, ADC_SampleTime_55Cycles5); // AOUT5: left side first
    ADC_RegularChannelConfig(ST_INFRARED_ADCX, ADC_Channel_3, 2, ADC_SampleTime_55Cycles5); // AOUT6: right side then

    ADC_DMACmd(ST_INFRARED_ADCX, ENABLE);
    ADC_Cmd(ST_INFRARED_ADCX, ENABLE);

    ADC_ResetCalibration(ST_INFRARED_ADCX);
    while (ADC_GetCalibrationStatus(ST_INFRARED_ADCX));
    ADC_StartCalibration(ST_INFRARED_ADCX);
    while (ADC_GetCalibrationStatus(ST_INFRARED_ADCX));

    ADC_SoftwareStartConvCmd(ST_INFRARED_ADCX, DISABLE); 
}

void ST_Infrared_Init(void)
{
    ST_Infrared_RCC_Init();
    ST_Infrared_GPIO_Init();
}

//----------------------------------------------------------------- 
/*******
 *@brief Control
 *@reten enum INFRED_DETC {FAR, NEAR}
 ******/
enum INFRED_DETC ST_Infrared_Left_DTEC(void)
{
    if (GPIO_ReadInputDataBit(ST_INFRARED_DOUT_PORT, ST_INFRARED_OUT5_PIN) == Bit_SET)
        return FAR;
    else
        return NEAR; 
}

enum INFRED_DETC ST_Infrared_Right_DTEC(void)
{
    if (GPIO_ReadInputDataBit(ST_INFRARED_DOUT_PORT, ST_INFRARED_OUT6_PIN) == Bit_SET)
        return FAR;
    else
        return NEAR; 
}

uint16_t* ST_Infrared_ADC_Filter(void)
{
    uint16_t sum = 0;
    uint16_t filter_out[2] = {0};
    uint8_t i = 0, j = 0;


    for (j; j<2; j++)
    {
        for (i; i<30; i++)
        {
            sum += st_infrared_dma_buffer[i][j];
        }

        filter_out[j] = sum;
        
        sum = 0;
    }

    return filter_out;    
}

//----------------------------------------------------------------- 
/*******
 *@brief Interrupt handler function
 ******/
void DMA1_Channel1_IRQHandler()
{
    if (DMA_GetITStatus(DMA1_IT_TC1) == SET)
    {
        st_infrared_filter_data = ST_Infrared_ADC_Filter();

        DMA_ClearFlag(DMA1_FLAG_TC1);
        DMA_ClearITPendingBit(DMA1_IT_TC1);
        memset(st_infrared_dma_buffer, 0, ST_INFRARED_DMA_BUFSIZE);
    }
                       
}

