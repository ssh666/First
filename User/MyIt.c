#include "Mydefine.h"


void USART1_IRQHandler()		// GPRS IRQ
{
	if(RESET != USART_GetITStatus(USART1, USART_IT_RXNE)){
		TIM_SetCounter(TIM3, 0);
		if(0 == (Buffer_flg1 & 0x8000)){
			Rcv_buffer1[Buffer_flg1++ & 0x7FFF] = USART_ReceiveData(USART1);
		}else{
			if(BUFFERSIZE < (Buffer_flg1&0x7FFF)){
				USART_ReceiveData(USART1);
			}else{
				Rcv_buffer1[Buffer_flg1 & 0x7FFF] = USART_ReceiveData(USART1);
			}
		}
	}
}



void USART2_IRQHandler()		// CAMERA1 IRQ
{
	if(RESET != USART_GetITStatus(USART2, USART_IT_RXNE)){
		TIM_SetCounter(TIM3, 0);
		if(0 == (Buffer_flg2 & 0x8000)){
			Rcv_buffer2[Buffer_flg2++ & 0x7FFF] = USART_ReceiveData(USART2);
		}else{
			if(BUFFERSIZE <= (Buffer_flg2&0x7FFF)){
				USART_ReceiveData(USART2);
			}else{
				Rcv_buffer2[Buffer_flg2 & 0x7FFF] = USART_ReceiveData(USART2);
			}
		}
	}
}



void USART3_IRQHandler()		// CAMERA2 IRQ
{
	if(RESET != USART_GetITStatus(USART3, USART_IT_RXNE)){
		TIM_SetCounter(TIM3, 0);
		if(0 == (Buffer_flg3 & 0x8000)){
			Rcv_buffer3[Buffer_flg3++ & 0x7FFF] = USART_ReceiveData(USART3);
		}else{
			if(BUFFERSIZE <= (Buffer_flg3&0x7FFF)){
				USART_ReceiveData(USART3);
			}else{
				Rcv_buffer3[Buffer_flg3 & 0x7FFF] = USART_ReceiveData(USART3);
			}
		}
	}
}



void TIM3_IRQHandler()
{
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET){
		if(0 != (Buffer_flg1 & 0x7FFF)){
			Buffer_flg1 |= 0x8000;
		}
		if(0 != (Buffer_flg2 & 0x7FFF)){
			Buffer_flg2 |= 0x8000;
		}
		if(0 != (Buffer_flg3 & 0x7FFF)){
			Buffer_flg3 |= 0x8000;
		}
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
		COUNTER > 100? COUNTER = 0: ++COUNTER;
	}
}
void TIM2_IRQHandler()
{
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
      {
         u8 buffer7 [50];
         if(MSG==0)
         {
            COUNTER2++;
            COUNTER4++;
           if(charge_flag!=0)
            {
               COUNTER1++;
               if(COUNTER1>400)      //120s 充电停下 方便测VB
               {  
                  charge_flag=0;
                  COUNTER1=0;
                  TIM_SetCompare3(TIM4,0);
                  delay_ms(3000);
                  AD.B_V=(float)(Get_Adc_Average(ADC_Channel_5,10)*0.00080566*7.13979);
   //               sprintf((char*)buffer7,"Stop_AD_BV: %f\r\n",AD.B_V);
   //               Debug((char*)buffer7);
               } 
            }
               if(COUNTER2>10)
               {
                  Percision(); 
                  War_compare(); 
                  ds1302_data(read);//读时间
                  COUNTER2=0;
               }
               if(Already_War==1)
               {
                  COUNTER3++;
                  if(COUNTER3>12000)      //1h 发短信间隔
                  {
                     Already_War=0;
                     COUNTER3=0;
                  }
               }
               if(COUNTER4>6000)  //30Min发数据
               {
                  COUNTER4=0;
                  Data_flag=1;
               }
         }            
         TIM_ClearITPendingBit(TIM2, TIM_IT_Update);         
        }  
}

      



//void RTC_IRQHandler(void)
//{		 
//	if (RTC_GetITStatus(RTC_IT_SEC) != RESET)//秒钟中断
//	{							
//		RTC_Get();//更新时间   
// 	}
//	if(RTC_GetITStatus(RTC_IT_ALR)!= RESET)//闹钟中断
//	{
//		RTC_ClearITPendingBit(RTC_IT_ALR);		//清闹钟中断	  	   
//  	} 				  								 
//	RTC_ClearITPendingBit(RTC_IT_SEC|RTC_IT_OW);		//清闹钟中断
//	RTC_WaitForLastTask();	  	    						 	   	 
//}

