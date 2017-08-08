#ifndef __BSP_BLUETOOTH_H
#define __BSP_BLUETOOTH_H

#include "bsp.h"

#define BLE_Rx_Pin GPIO_Pin_9 // USART1_Tx, PA9 
#define BLE_Tx_Pin GPIO_Pin_10 // USART1_Rx, PA10
#define BLE_PWRC_Pin GPIO_Pin_8 // BLE PWRC, PB8 
#define BLE_DISC_Pin GPIO_Pin_9 // BLE DISC, PB9 
#define BLE_DATA_PORT GPIOA
#define BLE_CTL_PORT GPIOB

#define BLE_RX_BUFFER_SIZE 20
#define BLE_TX_BUFFER_SIZE 20

static void BLE_GPIO_Init(void);
static void BLE_USART1_Init(void);

void BLE_Init(void);
void BLE_Enable_Connect(void);
void BLE_Disconnect(void);
void BLE_Awake(void);
void BLE_Sleep(void);

void BLE_Set_Name(void);

void USART1_IRQHandler(void);

#endif

