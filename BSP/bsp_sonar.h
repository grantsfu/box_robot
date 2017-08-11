#ifndef __BSP_SONAR_H
#define __BSP_SONAR_H

#include "bsp.h"

#define SONAR_PORT GPIOA
#define SONAR_TRIGER_PIN GPIO_Pin_5
#define SONAR_ECHO_PIN GPIO_Pin_4
#define SONAR_EXTI_LINE EXTI_Line4
#define SONAR_TIM2_PERIOD (1000-1) // time uint 1ms
#define SONAR_TIM2_PRESCALER (72-1) // clock unit 1us

//----------------------------------------------------------------- 
/*******
 *@brief Control function
 ******/
static void Sonar_RCC_Init(void);
static void Sonar_GPIO_Init(void);
static void Sonar_TIM2_Init(void);
static void Sonar_EXTI4_Init(void);
static void Sonar_Triger_Emit(void);
static void Sonar_Counter_Start(void);
static void Sonar_Counter_Stop(void);
static uint32_t Sonar_Echo_Time_Get(void);

void Sonar_Init(void);
float Sonar_Measurement(void);

//----------------------------------------------------------------- 
/*******
 *@brief Interrupt function
 ******/
void TIM2_IRQHandler(void);
void EXTI4_IRQHandler(void);

#endif


