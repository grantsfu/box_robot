#include "bsp_systick.h"

static __IO u32 Timing_Delay = 0;

void SysTick_Init(void)
{
   // unit = 10us
   if (SysTick_Config(SystemCoreClock/100000))
   {
       while (1);
   }

   SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}

void Delay_10us(__IO u32 nTime)
{
    Timing_Delay = nTime;

    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;

    while(Timing_Delay != 0);
}

// Systick interrupt function
void Timing_Delay_Decrement(void)
{
    if (Timing_Delay != 0x00)
    {
        Timing_Delay--;
    }
    else
    {
        SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
    }
} 

void SysTick_Handler(void)
{
    Timing_Delay_Decrement();
}



