#ifndef __BSP_MOTOR_H
#define __BSP_MOTOR_H

#include "bsp.h"

#define MOTOR_PORT1 GPIOA
#define MOTOR_PORT2 GPIOC
#define MOTOR_AIN1_PIN GPIO_Pin_11
#define MOTOR_AIN2_PIN GPIO_Pin_12
#define MOTOR_PWMA_PIN GPIO_Pin_7
#define MOTOR_BIN1_PIN GPIO_Pin_13
#define MOTOR_BIN2_PIN GPIO_Pin_15
#define MOTOR_PWMB_PIN GPIO_Pin_6
#define MOTOR_STBY_PIN GPIO_Pin_8

#define MOTOR_GPIO_SPEED GPIO_Speed_10MHz

// (MOTOR_TIM3_PERIOD+1) * (MOTOR_TIM3_PRESCALER+1) * (1/72MHz) = 3 * (1/100KHz)
#define MOTOR_TIM3_PERIOD (270-1)
#define MOTOR_TIM3_PRESCALER (8-1)

#define MOTOR_TIM3_PWMA_PULSE (135) // CCR2 TIM3 CH2
#define MOTOR_TIM3_PWMB_PULSE (135) // CCR1 TIM3 CH1

enum mot {mot_a, mot_b};
enum dir {fwd, bwd};
enum act {forward, backward, stop, left, right};

static void Motor_RCC_Init(void);
static void Motor_GPIO_Init(void);
static void Motor_TIM3_Init(void);

void Motor_Init(void);

static void Motor_A_Brake(void);
static void Motor_B_Brake(void);
static void Motor_A_CCW(void);
static void Motor_B_CCW(void);
static void Motor_A_CW(void);
static void Motor_B_CW(void);

void Motor_Open(void);
void Motor_Off(void);

static void Motor_Control(enum mot motor, uint8_t pwm, enum dir direction);

void Motor_Go(enum act what);
void Motor_Super_Go(enum act what, uint8_t pwm);

#endif


