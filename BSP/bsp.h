#ifndef _BSP_H
#define _BSP_H

/************************************************
 *@brief STM32F103C8T6 resources allocation (PART)
 *
 *       Motor: TIM3_CH1 && TIM3_CH2
 *       Led: TIM4 && DMA1_CH1 && DMA1_CH4(DMA_Priority_High)
 *       Sonar: TIM2 && EXTI4
 *       PC13,14,15: PC13(out) -> Motor BIN1
 *                   PC14(in) -> ST_Infrared OUT5 
 *                   PC14(in) -> ST_Infrared OUT6 
 *       ADC1 CH2 && CH3: ADC1_IN2 -> ST_Infrared AOUT5
 *                        ADC1_IN3 -> ST_Infrared AOUT6
 *       DMA1_CH1: TIM4_CH1 && ADC1(DMA_Priority_VeryHigh)
 *                 TODO: exist conflict
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



