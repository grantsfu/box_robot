#ifndef __BSP_MP3_H
#define __BSP_MP3_H

#include "bsp.h"

//----------------------------------------------------------------- 
/*******
 *@brief Hardware definition 
 ******/
#define MP3_PORT GPIOB
#define MP3_RX_PIN GPIO_Pin_10
#define MP3_TX_PIN GPIO_Pin_11

//----------------------------------------------------------------- 
/*******
 *@brief CMD control 
 ******/
#define CMD_START                   0x7E            // 开始发送指令
#define CMD_END                     0xEF            // 结束发送指令

#define NEXT_SONG				    0X01            // 下一首
#define PREVIOUS_SONG				0X02            // 上一首
#define SPECIFY_SONG				0X03            // 指定曲目    spiflash参数范围0-200
#define INC_VOLUME			        0X04            // 音量加
#define DEC_VOLUME		            0X05            // 音量减
#define SPECIFY_VOLUME			    0X06            // 指定音量
#define SPECIFY_EQ                  0X07            // 指定音效 （参数：01:normal, 02:pop, 03:rock, 04:jazz, 05:classic, 06:base）
#define SPECIFY_DEVICE              0x09            // 指定播放设备

#define PAUSE				        0X0A            // 暂停播放（休眠）
#define RST	                        0X0C            // 复位芯片
#define PLAY				        0X0D            // 播放
#define STOP				        0X0E            // 停止
#define FOLDER_EXCHANGE	            0X0F            // 切换文件夹
#define LOOP_PLAY			        0X11	        // 循环播放
#define SPECIFY_FOLDER				0X12            // 播放指定文件夹下的歌曲

/*******
 *@brief CMD status check 
 ******/
#define CURRENT_STATUS		        0X42            // 查询当前播放状态（播放、停止、暂停等）
#define	CURRENT_VOLUME		        0X43            // 查询当前音量
#define	CURRENT_EQ				    0X44            // 查询当前音效 （参数：01:normal, 02:pop, 03:rock, 04:jazz, 05:classic, 06:base）
#define	CURRENT_PLAY_MODE	        0X45            // 查询当前播放模式（参数：0:all, 1:fol, 2:one ,3:ram, 4:one_stop）        
#define	CURRENT_VERSION		        0X46            // 查询当前软件版本
#define	TF_FILE_COUNT				0X47            // 查询TF卡歌曲数量
#define	UDISK_FILE_COUNT			0X48            // 查询udisk歌曲数量
#define	FLASH_FILE_COUNT			0X49            // 查询spiflash歌曲数量
#define	TF_CURRENT_SONG				0X4B            // 查询TF卡的当前曲目
#define	FLASH_CURRENT_SONG			0X4D            // 查询flash中的当前曲目
#define	PLAY_TIME			        0X50            // 查询当前播放时间
#define	TOTAL_TIME			        0X51            // 查询当前播放歌曲的总时间
#define	SONG_NAME			        0X52            // 查询当前歌曲名字（spiflash不支持）
#define	FOLDER_COUNT				0X53            // 查询当前文件夹下总文件夹数量

/*******
 *@brief Parameter config 
 ******/
//播放设备
#define UDISK                       0x00
#define TF                          0x01
#define AUX                         0x02
#define SLEEP                       0x03
#define SPIFLASH                    0x04

//循环模式
#define LOOP_ALL                    0x00
#define LOOP_FOL                    0x01
#define LOOP_ONE                    0x02
#define LOOP_RAM                    0x03
#define ONE_STOP                    0x04

//音效模式
#define NORMAL                      0x00
#define POP                         0x01
#define ROCK                        0x02
#define JAZZ                        0x03
#define CLASSIC                     0x04
#define BASE                        0x05

//播放音量大小
#define VOLUME_0                    0
#define VOLUME_1                    1
#define VOLUME_2                    2
#define VOLUME_3                    3
#define VOLUME_4                    4
#define VOLUME_5                    5
#define VOLUME_6                    6
#define VOLUME_7                    7
#define VOLUME_8                    8
#define VOLUME_9                    9
#define VOLUME_10                   10
#define VOLUME_11                   11
#define VOLUME_12                   12
#define VOLUME_13                   13
#define VOLUME_14                   14
#define VOLUME_15                   15
#define VOLUME_16                   16
#define VOLUME_17                   17
#define VOLUME_18                   18
#define VOLUME_19                   19
#define VOLUME_20                   20
#define VOLUME_21                   21
#define VOLUME_22                   22
#define VOLUME_23                   23
#define VOLUME_24                   24
#define VOLUME_25                   25
#define VOLUME_26                   26
#define VOLUME_27                   27
#define VOLUME_28                   28
#define VOLUME_29                   29
#define VOLUME_30                   30 

//----------------------------------------------------------------- 
/*******
 *@brief Hardware function 
 ******/
static void MP3_GPIO_Init(void);
static void MP3_USART3_Init(void);

void MP3_Init(void);

//----------------------------------------------------------------- 
/*******
 *@brief Control function 
 ******/
static void MP3_MSG_Pack(uint8_t cmd, ...);

void MP3_Reset(void);
void MP3_Pause(void);
void MP3_Play(void);
void MP3_Loop_Play(uint8_t mode);
void MP3_Stop(void);
void MP3_Next_Song(void);
void MP3_Previous_Song(void);
void MP3_Specify_Song(uint8_t song_nbr);
void MP3_Specify_Folder_Song(uint8_t folder_nbr, uint8_t song_nbr);
void MP3_Increase_Volume(void);
void MP3_Decrease_Volume(void);
void MP3_Specify_Volume(uint8_t strong);
void MP3_Specify_EQ(uint8_t mode);
void MP3_Specify_Storage(uint8_t device);

#endif


