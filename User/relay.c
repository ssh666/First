#include "Mydefine.h"

u8 L_work_Flag;
void relay_init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
 	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE);	 
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;				 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);				 //PB7 负载开关
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_Init(GPIOC, &GPIO_InitStructure);	
   GPIO_SetBits(GPIOC,GPIO_Pin_7);	                //PC7 输出高   关闪光灯
   L_work_Flag=0;
 	GPIO_SetBits(GPIOB,GPIO_Pin_7);						 //PB7 输出高   关负载
   Data1.work_state[2]=0x30;
}
