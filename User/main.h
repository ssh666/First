#ifndef _MAIN_H
#define _MAIN_H
#include "Mydefine.h"
#include "dht11.H"
#include "ds18b20.H"
#include "ds1302.h"
#include "stmflash.h"

u8 crc[2];
u8	t_open = 22;
u8	t_close = 24;
uint16_t MSG = 0;	 //  READ Mydefine.h ABOUT THE FUNCTION OF MSG
uint16_t COUNTER = 0;
uint16_t COUNTER1 = 0;
uint16_t COUNTER2 = 0;
uint16_t COUNTER4 = 0;
u16 COUNTER3=0;
unsigned long Sysus = 0;
unsigned long Sysms = 0;
uint16_t Buffer_flg1 = 0;		//  GPRS BUFFER FLAG
uint16_t Buffer_flg2 = 0;		//  CAMERA1 BUFFER FLAG
uint16_t Buffer_flg3 = 0;		//  CAMERA2 BUFFER FLAG
uint8_t Rcv_buffer1[BUFFERSIZE];
uint8_t Rcv_buffer2[BUFFERSIZE];
uint8_t Rcv_buffer3[BUFFERSIZE];
u8 Data_flag;
u8 Data1_Buffer[75];
Data1_obj  Data1;
War_obj War;
u8 Over_work;
void USART_Send(Data1_obj temp,u16 t)
{
   
}
u8 DHT11_flag,Ds18b20_flag,*p;
static u32 CpuID[3];
static  char ID[4];
u8 Wait30min=0;// 等待30分钟发数据
void GetLockCode()
{
   CpuID[0]=*(vu32*)(0x1ffff7e8);
   CpuID[1]=*(vu32*)(0x1ffff7ec);
   CpuID[2]=*(vu32*)(0x1ffff7f0);
   ID[0]=(char)((CpuID[0]>>24)&0XFF);
   ID[1]=(char)((CpuID[0]>>16)&0XFF);
   ID[2]=(char)((CpuID[0]>>8)&0XFF);
   ID[3]=(char)((CpuID[0]>>0)&0XFF);

}

void Data1Handle()                  //
{

}
void MSGHandle()
{
   uint8_t buff1[10];
   uint8_t time[7]={30,20,12,31,10,2,17};//默认设置时间
   GetLockCode();
	if(0 != (Buffer_flg1 & 0X8000))
	{
      if(Rcv_buffer1[0]==0x66)
      { 
      
         if((Rcv_buffer1[1]==0x31)&&(Rcv_buffer1[2]==0x32)&&(Rcv_buffer1[3]==0x33)&&(Rcv_buffer1[4]==0x36))
         {
            switch(Rcv_buffer1[5])
            {
               case 0x31:MSG |= 0x8000;
                     break;
               case 0x32:MSG |= 0x4000;  //获取实时测量数据
                     break;
               case 0x33:;MSG |= 0x0100; //设置工作时间
                     break;
               case 0x34:
                  if(Rcv_buffer1[7]==0x30)
                     MSG |= 0x1000;   //关负载
                  if(Rcv_buffer1[7]==0x31)
                     MSG |= 0x2000;  //开负载     
                     break;
               case 0x35:MSG |= 0x0800;
                     break;
               case 0x36:MSG |= 0x0400;  //设置报警上下限
                     break;
               case 0x37:MSG |= 0x0200;  //时间校准     
                     break;
//		switch(Rcv_buffer1[0])
//		{
//			case 'a': MSG |= 0x8000;
////                Usart1Send("Msg \'a\' received\r\n\r\n",strlen("Msg \'a\' received\r\n\r\n"));
//                break;
//			case 'b': MSG |= 0x4000;
////                Usart1Send("Msg \'b\' received\r\n\r\n",strlen("Msg \'b\' received\r\n\r\n"));
//                break;
//			case 'c': MSG |= 0x2000;
////                Usart1Send("Msg \'c\' received\r\n\r\n",strlen("Msg \'c\' received\r\n\r\n"));
//                break;
//			case 'd': MSG |= 0x1000;
////                Usart1Send("Msg \'d\' received\r\n\r\n",strlen("Msg \'d\' received\r\n\r\n"));
//                break;
//			case 'e': MSG |= 0x0800;
////                Usart1Send("Msg \'e\' received\r\n\r\n",strlen("Msg \'e\' received\r\n\r\n"));
//                break;
//			case 'f': MSG |= 0x0400;
////                Usart1Send("Msg \'e\' received\r\n\r\n",strlen("Msg \'e\' received\r\n\r\n"));
//                break;
//			case 'g': MSG |= 0x0200;
////                Usart1Send("Msg \'e\' received\r\n\r\n",strlen("Msg \'e\' received\r\n\r\n"));
//                break;
//			case 'h': MSG |= 0x0100;
////                Usart1Send("Msg \'e\' received\r\n\r\n",strlen("Msg \'e\' received\r\n\r\n"));
//                break;
//			case 'i': MSG |= 0x0080;
////                Usart1Send("Msg \'e\' received\r\n\r\n",strlen("Msg \'e\' received\r\n\r\n"));
//                break;
			default: Buffer_flg1=0;Rcv_buffer1[0]=0;break;
//		}
	}
            
}
}
      if(Rcv_buffer1[0]=='s') //收到图像传输指令
      {
         if((Rcv_buffer1[1]==0x31)&&(Rcv_buffer1[2]==0x32)&&(Rcv_buffer1[3]==0x33)&&(Rcv_buffer1[4]==0x36)&&(Rcv_buffer1[17]=='a'))
         {
            MSG |= 0x0080;     
         }
         if((Rcv_buffer1[1]==0x31)&&(Rcv_buffer1[2]==0x32)&&(Rcv_buffer1[3]==0x33)&&(Rcv_buffer1[4]==0x36)&&(Rcv_buffer1[17]=='f'))
         {
            MSG |= 0x0040;   
         }
      }
         
   if(RESET != (MSG & 0x8000))
   {
      Data1_Send();
//      Usart1Send((const char* )Data1_Buffer,sizeof(Data1_Buffer));
   }    
   if(RESET != (MSG & 0x4000)) //获取实时测量数据
   {
      Data2_Send(); 
   }
   if(RESET != (MSG & 0x2000)) 
   {
      if(Over_work==0)
      {
         GPIO_ResetBits(GPIOB,GPIO_Pin_7);	   //开负载
         Data1.work_state[2]=0x31;
         Debug("on");
      }
   }
   if(RESET != (MSG & 0x1000)) 
   {
      GPIO_SetBits(GPIOB,GPIO_Pin_7);        //关负载
      Data1.work_state[2]=0x30;
      Debug("off");
   }
   if(RESET != (MSG & 0x0080))               //发图
   {
      Photo_Send();
   }
   if(RESET != (MSG & 0x0800)) 
   {
      Get_All_Measure();
      Percision(); 
   }
   if(RESET != (MSG & 0x0400))    //设置报警上下限
   {
      War.RI_up=(Rcv_buffer1[8]-0X30)*10+(Rcv_buffer1[9]-0X30)+(Rcv_buffer1[10]-0X30)*0.1;
      War.BV_down=(Rcv_buffer1[11]-0X30)*10+(Rcv_buffer1[12]-0X30)+(Rcv_buffer1[13]-0X30)*0.1;
      War.BV_up=(Rcv_buffer1[14]-0X30)*10+(Rcv_buffer1[15]-0X30)+(Rcv_buffer1[16]-0X30)*0.1;
      War.AT_down=(Rcv_buffer1[17]-0X30)*10+(Rcv_buffer1[18]-0X30)+(Rcv_buffer1[19]-0X30)*0.1;
      War.AT_up=(Rcv_buffer1[20]-0X30)*10+(Rcv_buffer1[21]-0X30)+(Rcv_buffer1[22]-0X30)*0.1;
      War.AH_down=(Rcv_buffer1[23]-0X30)*10+(Rcv_buffer1[24]-0X30)+(Rcv_buffer1[25]-0X30)*0.1;
      War.AH_up=(Rcv_buffer1[26]-0X30)*10+(Rcv_buffer1[27]-0X30)+(Rcv_buffer1[28]-0X30)*0.1;
      War.WT_down=(Rcv_buffer1[29]-0X30)*10+(Rcv_buffer1[30]-0X30)+(Rcv_buffer1[31]-0X30)*0.1;
      War.WT_up=(Rcv_buffer1[32]-0X30)*10+(Rcv_buffer1[33]-0X30)+(Rcv_buffer1[34]-0X30)*0.1;
//   u32 buffer6[50];
//   sprintf((char*)buffer6,"War.RI_up: %f\r\n",War.RI_up);
//   Debug((char*)buffer6);
//   sprintf((char*)buffer6,"War.BV_down: %f\r\n",War.BV_down);
//   Debug((char*)buffer6);
//   sprintf((char*)buffer6,"War.BV_up: %f\r\n",War.BV_up);
//   Debug((char*)buffer6);
//   sprintf((char*)buffer6,"War.AT_up: %f\r\n",War.AT_up);
//   Debug((char*)buffer6);
//   sprintf((char*)buffer6,"War.AH_down: %f\r\n",War.AH_down);
//   Debug((char*)buffer6);
//   sprintf((char*)buffer6,"War.AH_up: %f\r\n",War.AH_up);
//   Debug((char*)buffer6);
//   sprintf((char*)buffer6,"War.WT_down: %f\r\n",War.WT_down);
//   Debug((char*)buffer6);
//   sprintf((char*)buffer6,"War.WT_up: %f\r\n",War.WT_up);
//   Debug((char*)buffer6);
   }
   if(RESET != (MSG & 0x0200))   //时间校准
   {
      buff1[0]=Rcv_buffer1[10]-0X30;
      buff1[1]=Rcv_buffer1[11]-0X30;
      buff1[2]=Rcv_buffer1[12]-0X30;
      buff1[3]=Rcv_buffer1[13]-0X30;
      buff1[4]=Rcv_buffer1[14]-0X30;
      buff1[5]=Rcv_buffer1[15]-0X30;
      buff1[6]=Rcv_buffer1[16]-0X30;
      buff1[7]=Rcv_buffer1[17]-0X30;
      buff1[8]=Rcv_buffer1[18]-0X30;
      buff1[9]=Rcv_buffer1[19]-0X30; 
      time[6]=buff1[0]*10+buff1[1];
      time[5]=2;
      time[4]=buff1[2]*10+buff1[3];
      time[3]=buff1[4]*10+buff1[5];
      time[2]=buff1[6]*10+buff1[7];
      time[1]=buff1[8]*10+buff1[9];
      time[0]=0;
      ds1302_init(write,time);//设置1302的时间  
//      Debug("123");
   }
   if(RESET != (MSG & 0x0100))//设置工作时间
   {
      Data1.set[0]=Rcv_buffer1[8];
      Data1.set[1]=Rcv_buffer1[9];
      Data1.set[2]=Rcv_buffer1[10];
      Data1.set[3]=Rcv_buffer1[11];
      Data1.set[4]=Rcv_buffer1[12];
      Data1.set[5]=Rcv_buffer1[13];
      Data1.set[6]=Rcv_buffer1[14];
      Data1.set[7]=Rcv_buffer1[15];  
   }
   if(RESET != (MSG & 0x0040))     //设置报警手机
   {
      Phone_Number[0]=Rcv_buffer1[6];
      Phone_Number[1]=Rcv_buffer1[7];
      Phone_Number[2]=Rcv_buffer1[8];
      Phone_Number[3]=Rcv_buffer1[9];
      Phone_Number[4]=Rcv_buffer1[10];
      Phone_Number[5]=Rcv_buffer1[11];
      Phone_Number[6]=Rcv_buffer1[12];
      Phone_Number[7]=Rcv_buffer1[13];
      Phone_Number[8]=Rcv_buffer1[14];
      Phone_Number[9]=Rcv_buffer1[15];
      Phone_Number[10]=Rcv_buffer1[16];
      STMFLASH_Write(FLASH_SAVE_ADDR,(u16*)Phone_Number,11);
      
   }
   
      Buffer_flg1 = 0;
       
   
// if(RESET != (MSG & 0x8000)) SIM800Handler();
//	if(RESET != (MSG & 0x4000)) Camera1Handler();
//	if(RESET != (MSG & 0x2000)) CurrentHandler();
//	if(RESET != (MSG & 0x1000)) VoltageHandler();
//	if(RESET != (MSG & 0x0800)) DHT11Handler();
//	if(RESET != (MSG & 0x0400)) relay1_on();
//	if(RESET != (MSG & 0x0200)) relay2_on();
//	if(RESET != (MSG & 0x0100)) relay1_off();
//	if(RESET != (MSG & 0x0080)) relay2_off();
  
//	if(RESET != (MSG & 0x0400)) SelfCheck();
	if(0 != MSG)
	{
		// Usart1Send("Msg handler complete.\r\n",strlen("Msg handler complete.\r\n"));
		MSG = 0;
	}
         }
      }


#endif
