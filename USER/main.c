#include "bsp.h"

//----------------------------------------------------------------- 
/*******
 *@brief TEMP function definition
 ******/

void UartSendChar ( u8 ch, USART_TypeDef* USARTx );
void UartSendString ( u8 *str, u8 len, USART_TypeDef* USARTx );

//----------------------------------------------------------------- 
/*******
 *@brief Global variable 
 ******/

// u8 Tx_Buffer[] = {"AT+VER\r\n"};
// u8 Tx_Buffer_LEN;
// Tx_Buffer_LEN = strlen(Tx_Buffer);
// extern enum INFRED_DETC {FAR, NEAR};

int main(void)	
{
    BSP_Init();        

//----------------------------------------------------------------- 
/*******
 *@brief Advanced application 
 ******/	
// uint8_t pwm = 240;
// // uint8_t i;

// for (;;)
// {
// 	Motor_Super_Go(forward, pwm);
// 	Led_Bright(pwm, 50, 205, 50);
// 	Delay_10us(100*500);

// 	if (pwm > 0)
// 	{
// 		pwm -= 20;
// 	}
// 	else
// 	{
// 		pwm = 240;
// 	}
// }

// // while (1);

//----------------------------------------------------------------- 
/*******
 *@brief ST188_Infrared Test
 ******/
 	enum INFRED_DETC left_value, right_value; 

	for (;;)
	{
		left_value = ST_Infrared_Left_DTEC();
		right_value = ST_Infrared_Right_DTEC();

		if ((left_value == FAR) && (right_value == FAR))
		{
			Led_Whole_Light(255, 0, 0);
		}
		else if ((left_value == NEAR) || (right_value == NEAR))
		{
			Led_Whole_Light(0, 0, 255);
		}
		else
		{
			Led_Whole_Light(0, 255, 0);
		}

		Delay_10us(100*500);
	} 

//----------------------------------------------------------------- 
/*******
 *@brief Sonar Test
 ******/
	// float distance = 0;

	// Led_Whole_Light(0, 0, 0);

	// for (;;)
	// {
	// 	distance = Sonar_Measurement();

	// 	// distance is limited, less than 200
	// 	Led_Bright((uint8_t)distance, 255, 0, 0);

	// 	// Delay_10us(100*700);
	// }
//----------------------------------------------------------------- 
/*******
 *@brief Motor test 
 ******/
 	// for (;;)
	// {
	// 	// Motor_A_CCW();
	// 	// Motor_B_CCW();
	// 	// Delay_10us(100*2000);
	// 	// Motor_A_CW();
	// 	// Motor_B_CW();
	// 	// Delay_10us(100*2000);
	// 	// Motor_A_Brake();
	// 	// Motor_B_Brake();
	// 	// Delay_10us(100*2000);

	// 	// Motor_Control(mot_a, 55, fwd);
	// 	// Motor_Control(mot_b, 55, bwd);
	// 	// Delay_10us(100*2000);
	// 	// Motor_Control(mot_a, 55, bwd);
	// 	// Motor_Control(mot_b, 55, fwd);
	// 	// Delay_10us(100*2000);

	// 	// Motor_Go(forward);
	// 	// Delay_10us(100*1000);
	// 	// Motor_Go(backward);
	// 	// Delay_10us(100*1000);
	// 	Motor_Go(left);
	// 	Delay_10us(100*3000);
	// 	Motor_Go(right);
	// 	Delay_10us(100*3000);
	// 	Motor_Go(stop);
	// 	Delay_10us(100*2000);
	// }

	// // while (1);

//----------------------------------------------------------------- 
/*******
 *@brief MP3 test 
 ******/

	// // uint8_t cmd_name[20] = {"hello human\r\n"};
	// // uint8_t cmd_name[20] = {"7E020DEF"};
	// uint8_t cmd_play[6] = {0x7E,0x02,0x0D,0xEF};
	// uint8_t cmd_specify_vol[6] = {0x7E,0x03,0x06,0x1E,0xEF};
	// uint8_t cmd_specify_son[6] = {0x7E,0x04,0x03,0x00,0x02,0xEF};
	// uint8_t cmd_loop_play[6] = {0x7E,0x03,0x11,0x00,0xEF};
	// uint8_t cmd_rst[6] = {0x7E,0x02,0x0C,0xEF};
	

	// // char temp_data[40] = {0};
	// // uint8_t data = 77;
	// uint8_t i = 0;
	// // size_t len = strlen(cmd_name);
	// // FlagStatus flag_state = RESET;
	// // ITStatus it_state = RESET;
	 
	// for (i = 1; i>0; i--)
	// // for (;;)
	// {
	// 	// USART_SendData(USART1, (unsigned char) data);
	// 	// USART_SendData(USART1, (unsigned char) data);
	// 	// while (USART_GetFlagStatus(USART1, USART_FLAG_TC) != SET);
		
	// 	// USART_Send_Char(USART3, data);
	// 	// USART_Send_String(USART1, cmd_name);
	// 	// COPY_usart_sendstring(USART3, cmd_name, len);

	// 	// COPY_usart_sendstring(USART3, cmd_rst, 4);
	// 	// // UartSendString(cmd_specify_vol, 5, USART3);
	// 	// Delay_10us(100*1500);
	// 	// COPY_usart_sendstring(USART3, cmd_specify_vol, 5);
	// 	// // UartSendString(cmd_specify_vol, 5, USART3);
	// 	// Delay_10us(100*150);
	// 	// COPY_usart_sendstring(USART3, cmd_specify_son, 6);
	// 	// // UartSendString(cmd_specify_son, 6, USART3);
	// 	// Delay_10us(100*150);
	// 	// COPY_usart_sendstring(USART3, cmd_loop_play, 5);
	// 	// // UartSendString(cmd_loop_play, 5, USART3);
	// 	// Delay_10us(100*150);
		
	// 	/*******
	// 	 *@brief MP3 MSG pack function control
	// 	 ******/
	// 	MP3_Reset();
	// 	Delay_10us(100*1500);
	// 	MP3_Specify_Volume(15);
	// 	Delay_10us(100*150);
	// 	MP3_Specify_Song(8);
	// 	Delay_10us(100*150);
	// 	MP3_Loop_Play(LOOP_ONE);
	// 	Delay_10us(100*150);
	// 	MP3_Specify_EQ(JAZZ);
	// 	Delay_10us(100*150);

	// 	// strncpy(&temp_data[(2-i)*len], cmd_name, len); 
	// 	// flag_state = USART_GetFlagStatus(USART1, USART_FLAG_TC);
	// 	// printf("Flag_RXNE = %d\r\n", flag_state);
	// 	// flag_state = USART_GetFlagStatus(USART1, USART_FLAG_TXE);
	// 	// printf("Flag_ORE = %d\r\n", flag_state);
	// 	// it_state = USART_GetITStatus(USART1, USART_IT_TC);
	// 	// printf("IT_RXNE = %d\r\n", it_state);
	// 	// it_state = USART_GetITStatus(USART1, USART_IT_TXE);
	// 	// printf("IT_ORE = %d\r\n", it_state);

	// 	Delay_10us(100*1000);
	// }

	// // MP3_Play();
	// // MP3_Specify_Song(6);

	// printf("CODE_STATE: EOF\r\n");

	// while (1);
//----------------------------------------------------------------- 
/*******
*@brief Led test 
******/
	// for (;;)
	// {
	// 	// Led_Whole_Light(255, 48, 48); // Red.Firebrick1
	// 	// Delay_10us(100*1000);
	// 	// Led_Whole_Light(255, 69, 0); // Red.OrangeRed
	// 	// Delay_10us(100*1000);
	// 	// Led_Whole_Light(255, 193, 37); // Yellow.Goldenrod1
	// 	// Delay_10us(100*1000);
	// 	// Led_Whole_Light(50, 205, 50); // Green.LimeGreen
	// 	// Delay_10us(100*1000);
	// 	// Led_Whole_Light(0, 139, 0); // Green.Green4
	// 	// Delay_10us(100*1000);
	// 	// Led_Whole_Light(0, 191, 255); // Blue.DeepSkyBlue
	// 	// Delay_10us(100*1000);
	// 	// Led_Whole_Light(224, 102, 255); // Purple.MediumOrchild1
	// 	// Delay_10us(100*1000);
	// 	// Led_Breath(255, 0, 255); // Purple.Magenta
	// 	// Delay_10us(100*1000);
	// 	// Led_Breath(0, 139, 0);
	// 	// Delay_10us(100*1000);

	// 	// Led_Whole_Light(0, 191, 255);
	// 	// Delay_10us(100*1000);
	// 	// Led_Single_Light(0, 255, 48, 48);
	// 	// Delay_10us(100*1000);
	// 	// Led_Single_Light(1, 50, 205, 50);
	// 	// Delay_10us(100*1000);
	// 	// Led_Single_Light(2, 255, 69, 0);
	// 	// Delay_10us(100*1000);
	// 	// Led_Single_Light(3, 0, 139, 0);
	// 	// Delay_10us(100*1000);

	// 	Led_Bright(20, 0, 191, 255);
	// 	Delay_10us(100*1000);
	// 	Led_Bright(255, 0, 191, 255);
	// 	Delay_10us(100*1000);
	// }

    // // while (1);
//----------------------------------------------------------------- 
/*******
*@brief BLE test 
******/	

	// uint8_t cmd_talk[20] = {"hello human\r\n"};
	// uint8_t cmd_baud[20] = {"AT+VER\r\n"};
	// uint16_t i = 0;
	// size_t len = strlen(cmd_baud);

	// BLE_Disconnect();

	// for (i = 400; i>0; i--)
	// {
	// 	printf("This is a BLE Test\r\n");
	// 	// COPY_usart_sendstring(USART1, cmd_talk, 20);	

	// 	COPY_usart_sendstring(USART1, cmd_baud, len);
	// 	Delay_10us(100*150);
	// }

	// while (1);
}

//----------------------------------------------------------------- 
/*******
 *@brief Temp function 
 ******/
void UartSendChar ( u8 ch, USART_TypeDef* USARTx )
{
    // CPU_SR_ALLOC();
    // OS_CRITICAL_ENTER();

    USART_SendData ( USARTx, ( uint8_t ) ch );

    while ( USART_GetFlagStatus ( USARTx, USART_FLAG_TC ) == RESET );
    // OS_CRITICAL_EXIT();

}

void UartSendString ( u8 *str, u8 len, USART_TypeDef* USARTx )
{
    // CPU_SR_ALLOC();
    // OS_CRITICAL_ENTER();

    while ( len -- )
    {
        UartSendChar ( *str, USARTx );
        str++;
    }

    // OS_CRITICAL_EXIT();

}




