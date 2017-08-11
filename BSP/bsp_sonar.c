#include "bsp_sonar.h"

__IO uint16_t ms_count;
// // Trace the stage of sonar process, 0 means before rising trigger, 1 means after falling triggers
__IO uint8_t sonar_stage_trace;
__IO uint32_t us_time = 0;
__IO float sum = 0;

//----------------------------------------------------------------- 
/*******
 *@brief Parameter config 
 ******/
static GPIO_InitTypeDef Sonar_Triger_Config =
{
    .GPIO_Mode = GPIO_Mode_Out_PP,
    .GPIO_Pin = SONAR_TRIGER_PIN,
    .GPIO_Speed = GPIO_Speed_10MHz
};

static GPIO_InitTypeDef Sonar_Echo_Config =
{
    .GPIO_Mode = GPIO_Mode_IPD,
    .GPIO_Pin = SONAR_ECHO_PIN,
    .GPIO_Speed = GPIO_Speed_10MHz
};

static TIM_TimeBaseInitTypeDef Sonar_TIM2_Config =
{
    .TIM_Period = SONAR_TIM2_PERIOD,
    .TIM_Prescaler = SONAR_TIM2_PRESCALER,
    .TIM_ClockDivision = TIM_CKD_DIV1,
    .TIM_CounterMode = TIM_CounterMode_Up
};

static EXTI_InitTypeDef Sonar_EXTI4_Config =
{
    .EXTI_Line = SONAR_EXTI_LINE,
    .EXTI_Mode = EXTI_Mode_Interrupt,
    .EXTI_LineCmd = ENABLE,
    .EXTI_Trigger = EXTI_Trigger_Rising_Falling
};

//----------------------------------------------------------------- 
/*******
 *@brief Initialization
 ******/
static void Sonar_RCC_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |
    RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
}

static void Sonar_GPIO_Init(void)
{
    GPIO_Init(SONAR_PORT, &Sonar_Triger_Config);
    GPIO_Init(SONAR_PORT, &Sonar_Echo_Config);

    // GPIO_ResetBits(SONAR_PORT, SONAR_TRIGER_PIN);
    GPIO_SetBits(SONAR_PORT, SONAR_TRIGER_PIN);
    // GPIO_ResetBits(SONAR_PORT, SONAR_ECHO_PIN);

    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource4);
}

static void Sonar_TIM2_Init(void)
{
    TIM_DeInit(TIM2);
    TIM_TimeBaseInit(TIM2, &Sonar_TIM2_Config);

    TIM_ClearFlag(TIM2, TIM_FLAG_Update);
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

    // TIM_ARRPreloadConfig(TIM2, ENABLE);
    // TIM_Cmd(TIM2, ENABLE);
}

static void Sonar_EXTI4_Init(void)
{
    EXTI_DeInit();
    EXTI_Init(&Sonar_EXTI4_Config);

    EXTI_ClearFlag(SONAR_EXTI_LINE);
}

void Sonar_Init(void)
{
    Sonar_RCC_Init();
    Sonar_GPIO_Init();
    Sonar_TIM2_Init();
    Sonar_EXTI4_Init();
}

//----------------------------------------------------------------- 
/*******
 *@brief Control function
 ******/
static void Sonar_Triger_Emit(void)
{
    sonar_stage_trace = 0;

    GPIO_ResetBits(SONAR_PORT, SONAR_TRIGER_PIN);
    // Sonar_Counter_Start();
    Delay_10us(2);
    GPIO_SetBits(SONAR_PORT, SONAR_TRIGER_PIN);
}

static void Sonar_Counter_Start(void)
{
    TIM_SetCounter(TIM2, 0);
    ms_count = 0;

    TIM_Cmd(TIM2, ENABLE);
}

static void Sonar_Counter_Stop(void)
{
    TIM_Cmd(TIM2, DISABLE);
}

static uint32_t Sonar_Echo_Time_Get(void)
{
    uint32_t us_count = 0;

    us_count =  ms_count*1000;
    us_count += TIM_GetCounter(TIM2);

    return us_count;
}

float Sonar_Measurement(void)
{  
    uint8_t i = 0;
    float distance = 0;

    us_time = 0;
    sum = 0;

    /*******
     *@brief Measure 5 times to get average value
     ******/
    while (i<5)
    {
        Sonar_Triger_Emit();
        // Sonar_Counter_Start();

        // while (EXTI_GetFlagStatus(EXTI_Line4) != SET); // Echo start
        // Sonar_Counter_Start();
        // while (EXTI_GetFlagStatus(EXTI_Line4) != SET); // Echo stop
        // Sonar_Counter_Stop();

        // while (EXTI_GetITStatus(EXTI_Line4) != SET); // wait EXTI4 happen

        // us_time = Sonar_Echo_Time_Get();

        /*******
         *@brief distance(cm) = Echo(us_time)/58
         ******/
        // distance = (float)us_time/58;
        // distance = ((float)us_time*340) / (2.0*10000); // uint: cm

        // sum += ((float)us_time*340) / (2.0*10000);
        i++;

        Delay_10us(100*60); // 60ms measurement cycle
    }

    distance = sum/(58*5.0);

    distance = min(distance, 200);

    return distance;
}

//----------------------------------------------------------------- 
/*******
 *@brief Interrupt function
 ******/
void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
    {
        ms_count++;

        TIM_ClearFlag(TIM2, TIM_FLAG_Update);
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    }
}

void EXTI4_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_Line4) == SET)
    {
        EXTI_ClearFlag(EXTI_Line4);
        EXTI_ClearITPendingBit(EXTI_Line4);

        // if (GPIO_ReadInputDataBit(SONAR_PORT, SONAR_ECHO_PIN) == 1)
        // {
        //     Sonar_Counter_Start();
        // }
        // else
        // {
        //     Sonar_Counter_Stop();
        // }

        if (sonar_stage_trace == 0)
        {
            Sonar_Counter_Start();

            sonar_stage_trace = 1;
        }
        else
        {
            Sonar_Counter_Stop();

            us_time = Sonar_Echo_Time_Get();
            sum += us_time;

            sonar_stage_trace = 0;
        }
    }
}









