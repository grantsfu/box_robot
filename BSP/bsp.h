#ifndef _BSP_H
#define _BSP_H

/************************************************
 *@brief STM32F103C8T6 resources allocation (PART)
 *
 *       Motor: TIM3 channel1 && channel2
 *       Led: TIM4 DMA1 && DAM4
 *       Sonar: TIM2 && EXTI4
 *       PC13,14,15: PC13(out) -> Motor BIN1
 *                   PC14(in) -> ST_Infrared OUT5 
 *                   PC14(in) -> ST_Infrared OUT6 
 ***********************************************/
#ifndef max
    #define max(a, b) ((a)>(b) ? (a):(b))
#endif

#ifndef min
    #define min(a, b) ((a)<(b) ? (a):(b))
#endif

typedef enum {false = 0, true} boolean;

#include  <stdio.h>
#include  <string.h>
#include  <ctype.h>
#include  <stdlib.h>
#include  <stdarg.h>
#include  <math.h>

#include "stm32f10x.h"
#include "stm32f10x_it.h"

#include "bsp_systick.h"
#include "bsp_usart.h"
#include "bsp_led.h"
#include "bsp_bluetooth.h"
#include "bsp_mp3.h"
#include "bsp_motor.h"
#include "bsp_sonar.h"
#include "bsp_st_infrared.h"

#include "bsp_copy.h"

void BSP_Init(void);
void All_NVIC_Init(void);

#endif



