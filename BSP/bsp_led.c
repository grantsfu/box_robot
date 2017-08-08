#include "bsp.h"

volatile  uint8_t Led_Buffer[LED_BUFFER_SIZE];

/***************************************
 *@brief Parameter config 
 **************************************/
static GPIO_InitTypeDef Logo_Led_GPIO_Config =
{
    .GPIO_Pin = GPIO_Pin_6,
    .GPIO_Speed = GPIO_Speed_50MHz,
    .GPIO_Mode = GPIO_Mode_AF_PP
};
static GPIO_InitTypeDef Four_Led_GPIO_Config =
{
    .GPIO_Pin = GPIO_Pin_7,
    .GPIO_Speed = GPIO_Speed_50MHz,
    .GPIO_Mode = GPIO_Mode_AF_PP
};
static DMA_InitTypeDef Logo_Led_DMA_Config =
{
    .DMA_PeripheralBaseAddr = (uint32_t)&TIM4->CCR1,
    .DMA_MemoryBaseAddr = (uint32_t)(&Led_Buffer[0]),
    .DMA_DIR = DMA_DIR_PeripheralDST,
    .DMA_BufferSize = LED_BUFFER_SIZE,
    .DMA_PeripheralInc = DMA_PeripheralInc_Disable,
    .DMA_MemoryInc = DMA_MemoryInc_Enable,
    .DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord,
    .DMA_MemoryDataSize = DMA_MemoryDataSize_Byte,
    .DMA_Mode = DMA_Mode_Circular,
    .DMA_Priority = DMA_Priority_High,
    .DMA_M2M = DMA_M2M_Disable
};
static DMA_InitTypeDef Four_Led_DMA_Config =
{
    .DMA_PeripheralBaseAddr = (uint32_t)&TIM4->CCR2,
    .DMA_MemoryBaseAddr = (uint32_t)(&Led_Buffer[0]),
    .DMA_DIR = DMA_DIR_PeripheralDST,
    .DMA_BufferSize = LED_BUFFER_SIZE,
    .DMA_PeripheralInc = DMA_PeripheralInc_Disable,
    .DMA_MemoryInc = DMA_MemoryInc_Enable,
    .DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord,
    .DMA_MemoryDataSize = DMA_MemoryDataSize_Byte,
    .DMA_Mode = DMA_Mode_Circular,
    .DMA_Priority = DMA_Priority_High,
    .DMA_M2M = DMA_M2M_Disable
};
static TIM_TimeBaseInitTypeDef Led_TIM4_Config =
{
    .TIM_Period = LED_TIM4_PERIOD - 1,
    .TIM_Prescaler = 0,
    .TIM_ClockDivision = TIM_CKD_DIV1,
    .TIM_CounterMode = TIM_CounterMode_Up
};
static TIM_OCInitTypeDef Led_OC_Config =
{
    .TIM_OCMode = TIM_OCMode_PWM1,
    .TIM_OutputState = TIM_OutputState_Enable,
    .TIM_Pulse = 0,
    .TIM_OCPolarity = TIM_OCPolarity_High
};

/***************************************
 *@brief Led initialization
 **************************************/
void WS2812_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    
    GPIO_Init(GPIOB, &Logo_Led_GPIO_Config);
    GPIO_Init(GPIOB, &Four_Led_GPIO_Config);

    TIM_TimeBaseInit(TIM4, &Led_TIM4_Config);

    TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
    TIM_OC1Init(TIM4, &Led_OC_Config);

    TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);
    TIM_OC2Init(TIM4, &Led_OC_Config);

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    DMA_Init(DMA1_Channel1, &Logo_Led_DMA_Config);
    DMA_Init(DMA1_Channel4, &Four_Led_DMA_Config);

    TIM_DMACmd(TIM4, TIM_DMA_CC1, ENABLE);
    TIM_DMACmd(TIM4, TIM_DMA_CC2, ENABLE);

    DMA_ITConfig(DMA1_Channel1, DMA_IT_TC | DMA_IT_TE, ENABLE);
    DMA_ITConfig(DMA1_Channel4, DMA_IT_TC | DMA_IT_TE, ENABLE);
}

/***************************************
 *@brief Led control
 **************************************/
static void WS2812_Update(uint8_t group)
{
    TIM_Cmd(TIM4, ENABLE);

    if (group == LOGO_LED)                                             
    {
        DMA_SetCurrDataCounter(DMA1_Channel1, LED_BUFFER_SIZE);      

        DMA_Cmd(DMA1_Channel1, ENABLE);                              
        TIM_DMACmd(TIM4, TIM_DMA_Update, ENABLE);                    
    }
    else // FOUR_LED
    {
        DMA_SetCurrDataCounter(DMA1_Channel4, LED_BUFFER_SIZE);      

        DMA_Cmd(DMA1_Channel4, ENABLE);                            
        TIM_DMACmd(TIM4, TIM_DMA_Update, ENABLE);                
    }
}

static void Set_Led_Color(uint8_t index, uint8_t red, uint8_t green, uint8_t blue)
{
    uint8_t TEMP_Buffer[24] = {0};
    uint8_t i;

    /***
     *@brief Green 
     **/
    for (i = 0; i < 8; i++)
    {
        TEMP_Buffer[i] = ((green<<i) & 0x80) ? WS2812_HIGH : WS2812_LOW;
    }
    /***
     *@brief Red 
     **/
    for (i = 0; i < 8; i++)
    {
        TEMP_Buffer[8+i] = ((red<<i) & 0x80) ? WS2812_HIGH : WS2812_LOW;
    }
    /***
     *@brief Blue
     **/
    for (i = 0; i < 8; i++)
    {
        TEMP_Buffer[16+i] = ((blue<<i) & 0x80) ? WS2812_HIGH : WS2812_LOW;
    }
    /***
     *@brief Fill data into Led_Buffer from TEMP_Buffer
     **/
    for ( i = 0; i < 24; i++ )
    {
        Led_Buffer[LED_RESET_SLOTS_BEGIN + index*24 + i] = TEMP_Buffer[i];
    }
}

void Led_Whole_Light(uint8_t red, uint8_t green, uint8_t blue)
{
    uint32_t index;

    for (index = 0; index < LED_NUMBER; index++)
    {
        Set_Led_Color(index, red, green, blue);
    }

    /*******
     *@brief Update
     ******/
    WS2812_Update(FOUR_LED);
}

void Led_Single_Light(uint8_t index, uint8_t red, uint8_t green, uint8_t blue)
{
    uint8_t TEMP_Buffer[24] = {0};
    uint8_t i;

    /***
     *@brief Green 
     **/
    for (i = 0; i < 8; i++)
    {
        TEMP_Buffer[i] = ((green<<i) & 0x80) ? WS2812_HIGH : WS2812_LOW;
    }
    /***
     *@brief Red 
     **/
    for (i = 0; i < 8; i++)
    {
        TEMP_Buffer[8+i] = ((red<<i) & 0x80) ? WS2812_HIGH : WS2812_LOW;
    }
    /***
     *@brief Blue
     **/
    for (i = 0; i < 8; i++)
    {
        TEMP_Buffer[16+i] = ((blue<<i) & 0x80) ? WS2812_HIGH : WS2812_LOW;
    }
    /***
     *@brief Fill data into Led_Buffer from TEMP_Buffer
     **/
    for (i=0; i<LED_DATA_SIZE; i++) // 96
    {
        Led_Buffer[LED_RESET_SLOTS_BEGIN + i] = WS2812_LOW;
    }

    for ( i = 0; i < 24; i++ )
    {
        Led_Buffer[LED_RESET_SLOTS_BEGIN + index*24 + i] = TEMP_Buffer[i];
    }

    /*******
     *@brief Update 
     ******/
    WS2812_Update(FOUR_LED);
}

/***************************************
 *@brief NVIC IRQHandler
 **************************************/
void DMA1_Channel1_IRQHandler()
{
    TIM_Cmd(TIM4, DISABLE);

    TIM_DMACmd(TIM4, TIM_DMA_Update, DISABLE);         
    DMA_Cmd(DMA1_Channel1, DISABLE);                    
    DMA_ClearFlag(DMA1_FLAG_TC1);
    memset((char *)Led_Buffer, 0, LED_BUFFER_SIZE);
}

void DMA1_Channel4_IRQHandler()
{
    TIM_Cmd(TIM4, DISABLE);

    TIM_DMACmd(TIM4, TIM_DMA_Update, DISABLE);           
    DMA_Cmd(DMA1_Channel4, DISABLE);                     
    DMA_ClearFlag(DMA1_FLAG_TC4);
    memset((char *) Led_Buffer, 0, LED_BUFFER_SIZE);
}

/************************************************
 *@brief RGB and HSV conversion, for breathing Led
 ***********************************************/

/*******
 *@brief Math 
 ******/ 
float threeway_max(float a, float b, float c)
{
    return max(a, max(b, c));
}

float threeway_min(float a, float b, float c)
{
    return min(a, min(b, c));
}

/*******
 *@brief Conversion function
 ******/
rgb_struct RGB_To_Pack(uint8_t red, uint8_t green, uint8_t blue)
{
    rgb_struct rgb;

    rgb.red = red;
    rgb.green = green;
    rgb.blue = blue;

    return rgb;
}

hsv_struct RGB_To_HSV(rgb_struct rgb)
{
    float rd = (float) rgb.red/255;
    float gd = (float) rgb.green/255;
    float bd = (float) rgb.blue/255;
    float max = threeway_max(rd, gd, bd);
    float min = threeway_min(rd, gd, bd);
    hsv_struct hsv;

    hsv.value = max;

    float d = max-min;
    hsv.saturation = max == 0 ? 0 : d/max;

    if (max == min) 
    {
        hsv.hue = 0;
    }
    else
    {
        if (max == rd) {
            hsv.hue = (gd - bd)/d + (gd < bd ? 6 : 0);
        }
        else if (max == gd)
        {
            hsv.hue = (bd - rd)/d + 2;
        }
        else // max == bd 
        {
            hsv.hue = (rd - gd)/d + 4;
        }

        hsv.hue /= 6;
    }

    return hsv;
}

rgb_struct HSV_To_RGB(hsv_struct hsv)
{
    float r, g, b;
    rgb_struct rgb;

    int i = (int)(hsv.hue*6);
    float f = hsv.hue*6 - i;
    float p = hsv.value*(1 - hsv.saturation);
    float q = hsv.value*(1 - f*hsv.saturation);
    float t = hsv.value*(1 - (1-f)*hsv.saturation);

    switch (i % 6)
    {
        case 0: r = hsv.value, g = t,         b = p; break; 
        case 1: r = q,         g = hsv.value, b = p; break; 
        case 2: r = p,         g = hsv.value, b = t; break; 
        case 3: r = p,         g = q,         b = hsv.value; break; 
        case 4: r = t,         g = p,         b = hsv.value; break; 
        case 5: r = hsv.value, g = p,         b = q; break; 
    }

    rgb.red = r*255;
    rgb.green = g*255;
    rgb.blue = b*255;

    return rgb;
}

void Led_Breath(uint8_t red, uint8_t green, uint8_t blue)
{
    rgb_struct rgb;
    hsv_struct hsv;
    float temp_value;

    rgb = RGB_To_Pack(red, green, blue);
    hsv = RGB_To_HSV(rgb);
    temp_value = hsv.value;

    // for (;;)
    // {
        // From bright to dark
        while (hsv.value >= 0)
        {
            rgb = HSV_To_RGB(hsv);
            Led_Whole_Light(rgb.red, rgb.green, rgb.blue);
            Delay_10us(100*40); // 40ms*50*1 = 2s

            hsv.value -= temp_value*0.02;
        }
        // From dark to bright
        hsv.value = 0;


        while (hsv.value <= temp_value)
        {
            rgb = HSV_To_RGB(hsv);
            Led_Whole_Light(rgb.red, rgb.green, rgb.blue);
            Delay_10us(100*40); 

            hsv.value += temp_value*0.02;
        }
    // }
}

/*******
 *@brief Led flash with configured brightness 
 *@param bright: 0~255, hsv.value belongs to 0~1
 ******/
void Led_Bright(uint8_t bright, uint8_t red, uint8_t green, uint8_t blue)
{
    rgb_struct rgb;
    hsv_struct hsv;

    rgb = RGB_To_Pack(red, green, blue);
    hsv = RGB_To_HSV(rgb);

    hsv.value = (float)bright * ((float)1/255);
    rgb = HSV_To_RGB(hsv);
    Led_Whole_Light(rgb.red, rgb.green, rgb.blue);   
}








