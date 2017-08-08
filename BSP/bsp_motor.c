#include "bsp_motor.h"

//----------------------------------------------------------------- 
/*******
 *@brief Parameter config 
 ******/
static GPIO_InitTypeDef Motor_AIN1_Config =
{
    .GPIO_Pin = MOTOR_AIN1_PIN,
    .GPIO_Mode = GPIO_Mode_Out_PP,
    .GPIO_Speed = MOTOR_GPIO_SPEED
};

static GPIO_InitTypeDef Motor_AIN2_Config =
{
    .GPIO_Pin = MOTOR_AIN2_PIN,
    .GPIO_Mode = GPIO_Mode_Out_PP,
    .GPIO_Speed = MOTOR_GPIO_SPEED
};

static GPIO_InitTypeDef Motor_PWMA_Config =
{
    .GPIO_Pin = MOTOR_PWMA_PIN,
    .GPIO_Mode = GPIO_Mode_AF_PP,
    .GPIO_Speed = GPIO_Speed_50MHz
};

static GPIO_InitTypeDef Motor_BIN1_Config =
{
    .GPIO_Pin = MOTOR_BIN1_PIN,
    .GPIO_Mode = GPIO_Mode_Out_PP,
    .GPIO_Speed = GPIO_Speed_2MHz // To be as GPIO, PC13 speed should be under 2MHz
};

static GPIO_InitTypeDef Motor_BIN2_Config =
{
    .GPIO_Pin = MOTOR_BIN2_PIN,
    .GPIO_Mode = GPIO_Mode_Out_PP,
    .GPIO_Speed = MOTOR_GPIO_SPEED
};

static GPIO_InitTypeDef Motor_PWMB_Config =
{
    .GPIO_Pin = MOTOR_PWMB_PIN,
    .GPIO_Mode = GPIO_Mode_AF_PP,
    .GPIO_Speed = GPIO_Speed_50MHz
};

static GPIO_InitTypeDef Motor_STBY_Config =
{
    .GPIO_Pin = MOTOR_STBY_PIN,
    .GPIO_Mode = GPIO_Mode_Out_PP,
    .GPIO_Speed = MOTOR_GPIO_SPEED
};

static TIM_TimeBaseInitTypeDef Motor_TIM3_Config =
{
    .TIM_Period = MOTOR_TIM3_PERIOD,
    .TIM_Prescaler = MOTOR_TIM3_PRESCALER,
    .TIM_ClockDivision = TIM_CKD_DIV1,
    .TIM_CounterMode = TIM_CounterMode_Up
};

static TIM_OCInitTypeDef Motor_TIM3_PWMA_Config =
{  
    .TIM_OCMode = TIM_OCMode_PWM1,
    .TIM_OutputState = TIM_OutputState_Enable,
    .TIM_Pulse = MOTOR_TIM3_PWMA_PULSE,
    .TIM_OCPolarity = TIM_OCPolarity_High
};

static TIM_OCInitTypeDef Motor_TIM3_PWMB_Config =
{  
    .TIM_OCMode = TIM_OCMode_PWM1,
    .TIM_OutputState = TIM_OutputState_Enable,
    .TIM_Pulse = MOTOR_TIM3_PWMB_PULSE,
    .TIM_OCPolarity = TIM_OCPolarity_High
};

//----------------------------------------------------------------- 
/*******
 *@brief Motor initialization function 
 ******/
static void Motor_RCC_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |
    RCC_APB2Periph_AFIO |
    RCC_APB2Periph_GPIOC, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    PWR_BackupAccessCmd(ENABLE);
    RCC_LSEConfig(RCC_LSE_OFF); // Close LSE to make PC13 as GPIO
    PWR_BackupAccessCmd(DISABLE);
}

static void Motor_GPIO_Init(void)
{
    GPIO_Init(MOTOR_PORT1, &Motor_AIN1_Config);
    GPIO_Init(MOTOR_PORT1, &Motor_AIN2_Config);
    GPIO_Init(MOTOR_PORT1, &Motor_PWMA_Config);
    GPIO_Init(MOTOR_PORT2, &Motor_BIN1_Config);
    GPIO_Init(MOTOR_PORT1, &Motor_BIN2_Config);
    GPIO_Init(MOTOR_PORT1, &Motor_PWMB_Config);
    GPIO_Init(MOTOR_PORT1, &Motor_STBY_Config);

    GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE); // Close JTAG and SW, make PA15 as GPIO
    BKP_TamperPinCmd(DISABLE); // PC13: pin state controlled by BKP register
    Motor_Off();
}

static void Motor_TIM3_Init(void)
{
    TIM_TimeBaseInit(TIM3, &Motor_TIM3_Config);

    TIM_OC2Init(TIM3, &Motor_TIM3_PWMA_Config);
    TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
    TIM_OC1Init(TIM3, &Motor_TIM3_PWMB_Config);
    TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);

    TIM_ARRPreloadConfig(TIM3, ENABLE);
    // TIM_CtrlPWMOutputs(TIM3, ENABLE);
    TIM_Cmd(TIM3, ENABLE);
}

void Motor_Init(void)
{
    Motor_RCC_Init();
    Motor_GPIO_Init();
    Motor_TIM3_Init();
}

//----------------------------------------------------------------- 
/*******
 *@brief Basic control function 
 ******/
static void Motor_A_Brake(void)
{
    GPIO_SetBits(MOTOR_PORT1, MOTOR_AIN1_PIN);
    GPIO_SetBits(MOTOR_PORT1, MOTOR_AIN2_PIN);
    // GPIO_SetBits(MOTOR_PORT1, MOTOR_STBY_PIN);
}

static void Motor_B_Brake(void)
{
    GPIO_SetBits(MOTOR_PORT2, MOTOR_BIN1_PIN);
    GPIO_SetBits(MOTOR_PORT1, MOTOR_BIN2_PIN);
    // GPIO_SetBits(MOTOR_PORT1, MOTOR_STBY_PIN);
}

static void Motor_A_CCW(void)
{
    GPIO_ResetBits(MOTOR_PORT1, MOTOR_AIN1_PIN);
    GPIO_SetBits(MOTOR_PORT1, MOTOR_AIN2_PIN);
    // GPIO_SetBits(MOTOR_PORT1, MOTOR_STBY_PIN);
}

static void Motor_B_CCW(void)
{
    GPIO_ResetBits(MOTOR_PORT2, MOTOR_BIN1_PIN);
    GPIO_SetBits(MOTOR_PORT1, MOTOR_BIN2_PIN);
    // GPIO_SetBits(MOTOR_PORT1, MOTOR_STBY_PIN);
}

static void Motor_A_CW(void)
{
    GPIO_SetBits(MOTOR_PORT1, MOTOR_AIN1_PIN);
    GPIO_ResetBits(MOTOR_PORT1, MOTOR_AIN2_PIN);
    // GPIO_SetBits(MOTOR_PORT1, MOTOR_STBY_PIN);
}

static void Motor_B_CW(void)
{
    GPIO_SetBits(MOTOR_PORT2, MOTOR_BIN1_PIN);
    GPIO_ResetBits(MOTOR_PORT1, MOTOR_BIN2_PIN);
    // GPIO_SetBits(MOTOR_PORT1, MOTOR_STBY_PIN);
}

void Motor_Off(void)
{
    GPIO_ResetBits(MOTOR_PORT1, MOTOR_STBY_PIN);
}

void Motor_Open(void)
{
    GPIO_SetBits(MOTOR_PORT1, MOTOR_STBY_PIN);
}

//----------------------------------------------------------------- 
/*******
 *@brief Advanced control function 
 *@param motor: mot_a, mot_b
         pwm: 0~255
         direction: fwd, bwd         
 ******/
static void Motor_Control(enum mot motor, uint8_t pwm, enum dir direction)
{
    float ccr_value = pwm * ((float)270/255);

    if (motor == mot_a)
    {
        TIM_SetCompare2(TIM3, (uint16_t)ccr_value);

        if (direction == fwd)
        {
            Motor_A_CCW();
        }
        else // bwd
        {
            Motor_A_CW();
        }
    }
    else // mot_b
    {
        TIM_SetCompare1(TIM3, (uint16_t)ccr_value);

        if (direction == fwd)
        {
            Motor_B_CCW();
        }
        else // bwd
        {
            Motor_B_CW();
        }
    }
}

void Motor_Go(enum act what)
{
    if (what == stop)
    {
        Motor_A_Brake();
        Motor_B_Brake();
        Motor_Off();
    }
    else
    {
        Motor_Open();

        if (what == forward)
        {
            Motor_Control(mot_a, 150, fwd);
            Motor_Control(mot_b, 150, fwd);
        }
        else if (what == backward)
        {
            Motor_Control(mot_a, 150, bwd);
            Motor_Control(mot_b, 150, bwd);
        }
        else if (what == left)
        {
            Motor_Control(mot_a, 100, fwd);
            Motor_Control(mot_b, 150, fwd);
        }
        else if (what == right)
        {
            Motor_Control(mot_a, 150, fwd);
            Motor_Control(mot_b, 100, fwd);
        }
        else
        {
            Motor_A_Brake();
            Motor_B_Brake();
            Motor_Off();
        }
    }
}

void Motor_Super_Go(enum act what, uint8_t pwm)
{
    Motor_Open();

    if (what == forward)
    {
        Motor_Control(mot_a, pwm, fwd);
        Motor_Control(mot_b, pwm, fwd);
    }
    else if (what == backward)
    {
        Motor_Control(mot_a, pwm, bwd);
        Motor_Control(mot_b, pwm, bwd);
    }
    else
    {
        Motor_A_Brake();
        Motor_B_Brake();
        Motor_Off();
    }
}

















