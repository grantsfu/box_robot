#ifndef _BSP_H
#define _BSP_H

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

#include "bsp_copy.h"

void BSP_Init(void);
void All_NVIC_Init(void);

#endif



