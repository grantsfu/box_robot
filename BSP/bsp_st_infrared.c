#include "bsp_st_infrared.h"

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



//----------------------------------------------------------------- 
/*******
 *@brief Initialization
 ******/
static void ST_Infrared_RCC_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |
    RCC_APB2Periph_GPIOC, ENABLE);
}

static void ST_Infrared_GPIO_Init(void)
{
    BKP_TamperPinCmd(DISABLE);
    PWR_BackupAccessCmd(ENABLE);

    GPIO_Init(ST_INFRARED_DOUT_PORT, &ST_Infrared_OUT5_Config);
    GPIO_Init(ST_INFRARED_DOUT_PORT, &ST_Infrared_OUT6_Config);

    GPIO_SetBits(ST_INFRARED_DOUT_PORT, ST_INFRARED_OUT5_PIN);
    GPIO_SetBits(ST_INFRARED_DOUT_PORT, ST_INFRARED_OUT6_PIN);
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


