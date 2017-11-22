#include "Mydefine.h"
#include "dht11.h"
#include "ds1302.h"

void LOCALPeriphInit()
{
	
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	USART_InitTypeDef UART_InitStructure;
	TIM_TimeBaseInitTypeDef TIMER1_InitStructure;
   TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 | RCC_APB1Periph_USART2 | RCC_APB1Periph_USART3 | RCC_APB1Periph_TIM2| RCC_APB1Periph_TIM4, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_USART1, ENABLE);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
	Sysms = SystemCoreClock / 8000;
	Sysus = Sysms / 1000;
	
	USART_DeInit(USART1);
	USART_DeInit(USART2);
	USART_DeInit(USART3);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);           // USART1_Tx = PA9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOA, &GPIO_InitStructure);           // USART2_Tx = PA2
	GPIO_Init(GPIOB, &GPIO_InitStructure); 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);          // USART1_Rx = PA10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_Init(GPIOA, &GPIO_InitStructure);          // USART2_Rx = PA3
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_Init(GPIOB, &GPIO_InitStructure);         // RAINInput = PB5
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_Init(GPIOB, &GPIO_InitStructure);          //  PWM = PB8
	
	UART_InitStructure.USART_BaudRate = 115200;           //波特率115200
	UART_InitStructure.USART_Parity = USART_Parity_No;
	UART_InitStructure.USART_StopBits = USART_StopBits_1;
	UART_InitStructure.USART_WordLength = USART_WordLength_8b;
	UART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	UART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(USART1, &UART_InitStructure);
	USART_Init(USART2, &UART_InitStructure);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART1, ENABLE);
	USART_Cmd(USART2, ENABLE);
	
	TIMER1_InitStructure.TIM_Period = 2999;	// 72000000/(7199 + 1) * (2999 + 1) = 300000US = 300MS
	TIMER1_InitStructure.TIM_Prescaler = 7199;	// TICK PERIOD IS 72000000/(7199 + 1)
	TIMER1_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIMER1_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIMER1_InitStructure);
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
   TIMER1_InitStructure.TIM_Period = 2999;	// 72000000/(7199 + 1) * (2999 + 1) = 300000US = 300MS
	TIMER1_InitStructure.TIM_Prescaler = 7199;	// TICK PERIOD IS 72000000/(7199 + 1)
   TIM_TimeBaseInit(TIM2, &TIMER1_InitStructure);
   TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
   TIMER1_InitStructure.TIM_Period = 719;	// 72000000/(719 + 1) = 100k     PWM频率
	TIMER1_InitStructure.TIM_Prescaler = 0;	// TICK PERIOD IS 72000000
	TIMER1_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIMER1_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
   TIM_TimeBaseInit(TIM4, &TIMER1_InitStructure);
   TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
   TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
   TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;
   TIM_OC3Init(TIM4,&TIM_OCInitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_Init(&NVIC_InitStructure);
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStructure);	
   NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
   NVIC_Init(&NVIC_InitStructure);
	TIM_Cmd(TIM3, ENABLE);
   TIM_Cmd(TIM2, ENABLE);
   TIM_Cmd(TIM4, ENABLE);
   delay_init();
   relay_init();
   ds1302_GPIO_Configuration();//1302模拟时序引脚初始化
}
void Photo_Send()
{
   int i;
   GPIO_ResetBits(GPIOC,GPIO_Pin_7);	   //开补光灯
   Camera1Snapshot();
   Delayms(100);
   u8 Photo_Buffer[12];
   Photo_Buffer[0]=0x70;
   Photo_Buffer[1]=0x31;
   Photo_Buffer[2]=0x32;
   Photo_Buffer[3]=0x33;
   Photo_Buffer[4]=0x36;
   Photo_Buffer[5]=0x30;
   Photo_Buffer[6]=0x30;
   Photo_Buffer[7]=0x30;
   Photo_Buffer[8]=Camera_H;
   Photo_Buffer[9]=Camera_L;
   Photo_Buffer[10]=0x30;
   Photo_Buffer[11]=0x30;
   for(i=0;i<12;i++)
   {
      USART_SendData(USART1,*( Photo_Buffer+i));
      while(RESET == USART_GetFlagStatus(USART1, USART_FLAG_TC));
   }
   Camera1ImageProcess();
   GPIO_SetBits(GPIOC,GPIO_Pin_7);	   //关补光灯 
}
void Data2_Send()   //实时数据测量
{

   int i;
   u8 Data2_Buffer[5];
   Data2_Buffer[0]=0x66;  //头 地址 功能码
   Data2_Buffer[1]=0x31;
   Data2_Buffer[2]=0x32;
   Data2_Buffer[3]=0x33;
   Data2_Buffer[4]=0x36;
   for(i=0;i<5;i++)
   {
      USART_SendData(USART1,*(Data2_Buffer+i));
      while(RESET == USART_GetFlagStatus(USART1, USART_FLAG_TC));
   }
   for(i=0;i<70;i++)
   {
      USART_SendData(USART1,*((u8*)&Data1+i+5));
      while(RESET == USART_GetFlagStatus(USART1, USART_FLAG_TC));
   }
   
}
void Message(u16* Str,u16 t)
{
   uint16_t i = t;
	USART_ClearFlag(USART1, USART_FLAG_TC);
	for(; i>0; --i){
		USART_SendData(USART1, *Str++);
		while(RESET == USART_GetFlagStatus(USART1, USART_FLAG_TC));
   }
}

void Data1_Send()    //30min 发送数据无应答
{
   int i;
  for(i=0;i<sizeof(Data1);i++)
   {
      USART_SendData(USART1,*((u8*)&Data1+i));
      while(RESET == USART_GetFlagStatus(USART1, USART_FLAG_TC));
   }
}
   

void Usart1Send(const char *Str, const uint16_t length)
{
	uint16_t i = length;
	USART_ClearFlag(USART1, USART_FLAG_TC);
	for(; i>0; --i){
		USART_SendData(USART1, *Str++);
		while(RESET == USART_GetFlagStatus(USART1, USART_FLAG_TC));
  }
}


void Usart2Send(const char *Str, const uint16_t length)
{
	uint16_t i = length;
	USART_ClearFlag(USART2, USART_FLAG_TC);
	for(; i>0; --i){
		USART_SendData(USART2, *Str++);
		while(RESET == USART_GetFlagStatus(USART2, USART_FLAG_TC));
  }
}



void Usart3Send(const char *Str, const uint16_t length)
{
	uint16_t i = length;
	USART_ClearFlag(USART3, USART_FLAG_TC);
	for(; i>0; --i){
		USART_SendData(USART3, *Str++);
		while(RESET == USART_GetFlagStatus(USART3, USART_FLAG_TC));
  }
}



//void Delayus(unsigned long us)
//{
//	unsigned long temp;
//	SysTick->LOAD = us * Sysus;
//	SysTick->VAL = 0x00;
//	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
//	do{
//		temp = SysTick->CTRL;
//	}while( ( temp & 0x01 ) && !( temp & (1<<16) ));
//	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
//	SysTick->VAL = 0X00;
//}



//void Delayms(unsigned long ms)
//{
//	u32 temp;
//	SysTick->LOAD = (unsigned long)ms * Sysms;
//	SysTick->VAL = 0x00;
//	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
//	do{
//		temp=SysTick->CTRL;
//	}while( ( temp & 0x01 ) && !( temp & (1<<16) ));
//	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
//	SysTick->VAL = 0X00;
//}


