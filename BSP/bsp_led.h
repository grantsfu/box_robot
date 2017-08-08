#ifndef __BSP_LED_H
#define	__BSP_LED_H

#include "bsp.h"

#ifndef max
    #define max(a, b) ((a)>(b) ? (a):(b))
#endif

#ifndef min
    #define min(a, b) ((a)<(b) ? (a):(b))
#endif

#define WS2812_FREQ (800000) //800KHz
#define TIMER_CLOCK_FREQ (8000000)
#define LED_TIM4_PERIOD (90) // 72MHz/WS2812_FREQ
#define LED_NUMBER (4)			       
#define LED_DATA_SIZE (LED_NUMBER*24)
#define LED_RESET_SLOTS_BEGIN (50)
#define LED_RESET_SLOTS_END (50)
#define LED_LAST_SLOT (1)
#define LED_BUFFER_SIZE (LED_RESET_SLOTS_BEGIN + LED_DATA_SIZE + LED_LAST_SLOT + LED_RESET_SLOTS_END)
#define WS2812_HIGH (LED_TIM4_PERIOD*2/3)
#define WS2812_LOW (LED_TIM4_PERIOD/3)
#define WS2812_RESET (0)

// #define IS_LED_GROUP(GROUP) (((GROUP) == LOGO_LED) || ((GROUP) == FOUR_LED))
#define LOGO_LED 0
#define FOUR_LED 1

static void WS2812_Update(uint8_t group);
static void Set_Led_Color(uint8_t index, uint8_t red, uint8_t green, uint8_t blue);

void WS2812_Init(void);
void Led_Whole_Light(uint8_t red, uint8_t green, uint8_t blue);
void Led_Single_Light(uint8_t index, uint8_t red, uint8_t green, uint8_t blue);

/*******
 *@brief RGB and HSV conversion 
 ******/
typedef struct
{
    float hue;
    float saturation;
    float value;
}   hsv_struct;

typedef struct
{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
}   rgb_struct;

// rgb_struct *RGB_To_Pack(uint8_t red, uint8_t green, uint8_t blue);
// hsv_struct *RGB_To_HSV(rgb_struct *rgb);
// rgb_struct *HSV_To_RGB(hsv_struct *hsv);

// float threeway_max(float a, float b, float c);
// float threeway_min(float a, float b, float c);
void Led_Breath(uint8_t red, uint8_t green, uint8_t blue);
void Led_Bright(uint8_t bright, uint8_t red, uint8_t green, uint8_t blue);

#endif
