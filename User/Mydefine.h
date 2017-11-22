#ifndef _MYDEFINE_H
#define _MYDEFINE_H



#include "stm32f10x.h"
#include "delay.h"
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include "stdlib.h"
#include "relay.h"
#include "myfunction.h"
#include  <math.h>



extern uint16_t MSG;
extern u8 t_open;
extern u8 t_close;



/*
 -----------------------------------------------------
|  MSG  |	               FUNCTION                     |
|-------+---------------------------------------------|_
| bit15 | SET TRUE IF A MSG FROM SIM800A IS RECEIVED  |	\
|-------+---------------------------------------------|  |
| bit14 | SET TRUE IF A MSG FROM CAMERA1 IS RECEIVED  |  |
|-------+---------------------------------------------|   \  EXECUTE THESE FOUR
| bit13 | SET TRUE IF A MSG FROM CAMERA2 IS RECEIVED  |		/  MESSAGES FIRST
|-------+---------------------------------------------|	 |
| bit12 | SET TRUE IF A/D CONVERTER NEEDS TO WORK     |  |
|-------+---------------------------------------------|_/
| bit11 | 
|-------+---------------------------------------------|
| bit10 |
|-------+---------------------------------------------|
| bit09 |
|-------+---------------------------------------------|
| bit08 |
|-------+---------------------------------------------|
| bit07 |
|-------+---------------------------------------------|
| bit06 |
|-------+---------------------------------------------|
| bit05 |
|-------+---------------------------------------------|
| bit04 |
|-------+---------------------------------------------|
| bit03 |
|-------+---------------------------------------------|
| bit02 |
|-------+---------------------------------------------|
| bit01 |
|-------+---------------------------------------------|
| bit00 |
 -------+---------------------------------------------|
*/



#define SETSIM800AMSG() (MSG |= 0x8000)
#define RESETSIM800AMSG() (MSG &= 0x7FFF)
#define SETCAMERA1MSG() (MSG |= 0x4000)
#define RESETCAMERA1MSG() (MSG &= 0xBFFF)
#define SETCAMERA2MSG() (MSG |= 0x2000)
#define RESETCAMERA2MSG() (MSG &= 0xDFFF)

extern uint16_t COUNTER;
extern uint16_t COUNTER1;
extern uint16_t COUNTER2;
extern u16 COUNTER3;
extern uint16_t COUNTER4;
extern u8 Data_flag;
extern unsigned long Sysus;
extern unsigned long Sysms;
extern uint16_t Buffer_flg1;
extern uint16_t Buffer_flg2;
extern uint16_t Buffer_flg3;
extern uint8_t Rcv_buffer1[];
extern uint8_t Rcv_buffer2[];
extern uint8_t Rcv_buffer3[];
extern uint8_t read[];//读秒、分、时、日、月、周、年的寄存器地址
extern uint8_t write[];//写秒、分、时、日、月、周、年的寄存器地址
#define Delayus(x) delay_us(x)
#define Delayms(x) delay_ms(x)

#define FAIL 0
#define SUCCESS 1
#define BUFFERSIZE 1030
#define CmdSIM800 Cmd_Send1
#define GPRSSend Usart1Send
#define CmdCamera1 Cmd_Send2
#define CmdCamera2 Cmd_Send3
#define BUFFERSIZESIM800A 80
#define SendSIM800 Usart1Send
#define SendCamera1 Usart2Send
#define SendCamera2 Usart3Send
#define delay1s() Delayms(1000)
#define ACKCMD 6, 0xAA, 0x0D, 0x00, 0x00, 0x00, 0x00
#define IMGCMD 6, 0xAA, 0x04, 0x05, 0x00, 0x00, 0x00
#define INITCMD 6, 0xAA, 0x01, 0X00, 0x07, 0x00, 0x07
#define DATASIZECMD 6, 0xAA, 0x06, 0x08, 0x20, 0x03, 0x00
#define delay3s() Delayms(1000);Delayms(1000);Delayms(1000)
#define delay5s() Delayms(1000);Delayms(1000);Delayms(1000);Delayms(1000);Delayms(1000)
#define FLASH_SAVE_ADDR  0X08070000		//设置FLASH 保存地址(必须为偶数，且其值要大于本代码所占用FLASH的大小+0X08000000)

#define relay1_on() GPIO_SetBits(GPIOB,GPIO_Pin_5);
#define relay2_on() GPIO_SetBits(GPIOE,GPIO_Pin_5);
#define relay1_off() GPIO_ResetBits(GPIOB,GPIO_Pin_5);
#define relay2_off() GPIO_ResetBits(GPIOE,GPIO_Pin_5);
#define KEY0  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5)


#define stop_output() relay2_off()
#define stop_charge() relay1_off()
#define start_output() relay2_on()
#define start_charge() relay1_on()
void ds1302_data(uint8_t *read);
u8 RTC_Get(void);
u8 RTC_Init(void);
void ADWork(void);
void Adc_Init(void);
void SelfCheck(void);
void MSGHandle(void);
void ADHandler(void);
void relay_init(void);
void Camera1Init(void);
void Camera2Init(void);
void SIM800AInit(void);
void Debug(const char*);
void DataInit(void);
void War_compare(void);
void DHT11Handler(void);
void SIM800Handler(void);
void Camera1Handler(void);
void Camera2Handler(void);
void CurrentHandler(void);
void VoltageHandler(void);
void Get_All_Measure(void);
uint16_t Get_Adc(uint8_t);
void Cmd_Send1(char, ...);
void Cmd_Send2(char, ...);
void Cmd_Send3(char, ...);
u8 Is_Leap_Year(u16 year);
void LOCALPeriphInit(void);
uint32_t Camera1Snapshot(void);
uint32_t Camera2Snapshot(void);
void Camera1ImageProcess(void);
void Camera2ImageProcess(void);
uint16_t Get_Adc_Average(uint8_t,uint8_t);
u8 RTC_Get_Week(u16 year,u8 month,u8 day);
void Message(u16* str,u16 t);
void Usart1Send(const char * ,const uint16_t);
void Usart2Send(const char *, const uint16_t);
void Usart3Send(const char *, const uint16_t);
void MPPT(void);
void charge1(void);
void charge2(void);
void charge_switch(void);
void GetLockCode(void);
u8 RTC_Set(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec);
void Percision(void);
extern u8 LV_H,LV_L,BV_H,BV_L,LI_H,LI_L,BI_H,BI_L,RI_H,RI_L,AT_H,AT_L,AH_H,AH_L,\
WT_H,WT_L,YR_H,YR_L,MN_H,MN_L,HM_H,HM_L;/*光伏电压，蓄电池电压，光伏电流，电池电流，
                                         负载电流，气温，湿度，水温,年份，月日，时分*/
extern u8 L_work_Flag;
typedef struct 
{
	vu8 hour;
	vu8 min;
	vu8 sec;
	vu16 w_year;
	vu8  w_month;
	vu8  w_date;
	vu8  week;
}_calendar_obj;
 typedef struct 
{
	float L_V;
   float L_I;
   float B_V;
   float B_I;
   float R_I;
   float Rain;
   float AT;
   float AH;
   float WT;
}AD_obj;   //测量的浮点数
 typedef struct 
{
   u8  head;
   u8  address[4];
   u8  work_state[3];
   u8  war_state[15];
   u8  LI[3];
   u8  LV[3];
   u8  BI[3]; 
   u8  BV[3];
   u8  RI[3];
   u8  AT[3];
   u8  AH[3];
   u8  WT[3];
   u8  YR[4];
   u8  MN[4];
   u8  HM[4];
   u8  set[8];
   u8  spare[6];
   u8  crc[2];
}Data1_obj;  //测量数据75byte
typedef struct
{
   float LV_up;
   float LI_up;
   float RI_up;
   float BI_up;
   float BV_down;
   float BV_up;
   float AT_down;
   float AT_up;
   float AH_down;
   float AH_up;
   float WT_down;
   float WT_up;  
}War_obj;     //12个警报数据
extern u8 crc[2];
void Data1_Send(void);
void Photo_Send(void);
void Data2_Send(void);
extern u8 Camera_H,Camera_L;
extern Data1_obj Data1;
extern u8 DHT11_flag;
extern _calendar_obj calendar;
extern AD_obj AD;
extern War_obj War;
extern int charge_flag;
extern u8 Stop_charge;
extern u8 Phone_Number[11];
extern u8 Already_War; 
extern u8 Over_work;
#endif
