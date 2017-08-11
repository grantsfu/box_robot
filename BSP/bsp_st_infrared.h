#ifndef __BSP_ST_INFRARED_H
#define __BSP_ST_INFRARED_H

#include "bsp.h"

#define ST_INFRARED_DOUT_PORT GPIOC
#define ST_INFRARED_AOUT_PORT GPIOA
#define ST_INFRARED_OUT5_PIN GPIO_Pin_14
#define ST_INFRARED_OUT6_PIN GPIO_Pin_15

enum INFRED_DETC {FAR, NEAR};

static void ST_Infrared_RCC_Init(void);
static void ST_Infrared_GPIO_Init(void);

void ST_Infrared_Init(void);
enum INFRED_DETC ST_Infrared_Left_DTEC(void);
enum INFRED_DETC ST_Infrared_Right_DTEC(void);

#endif



