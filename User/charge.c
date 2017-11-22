#include "Mydefine.h"
AD_obj AD;
int charge_flag=0;// 0：不开放充电  1：mppt充电  2：恒压充电   3：浮充充电
float Pi=0.0,Pj=0.0;
int PWM=500,N=3;//N为步长
u8 Stop_charge=0;
void charge_switch()
{
   if((AD.L_V>AD.B_V)&&(charge_flag==0)&&(5<AD.B_V<14.4))
      {
         if(AD.B_V<=13)
            charge_flag=1;   
         if((AD.B_V>13)&&(AD.B_V<=13.8))
            charge_flag=2; 
         if((AD.B_V>13.8)&&(AD.B_V<14.4))
            charge_flag=3;
         COUNTER1=0;
         PWM=500;
         Pi=0.0;
         Pj=0.0;
      }
      switch(charge_flag)
      {
         case 0:
            break;
         case 1:
            MPPT();
            Debug("MPPT()");
         break;
         case 2:
            charge1();
            Debug("charge1()");
         break;
         case 3:
            charge2();
            Debug("charge2()");
         
         break;  
      }
}
void MPPT()
{
   u8 buff1[50];
//   Pi=AD.B_V*AD.B_I;
//   if(Pi>=Pj)
//   {
//      N=N;
//      PWM=PWM+N;
//      if(PWM>719)
//         PWM=719;
//   }
//   else
//   {
//      N=-N;
//      PWM=PWM+N;
//      if(PWM<0)
//         PWM=0;
//   }
   PWM=710;
//   sprintf((char *)buff1,"PWM:%d\r\n",PWM);
//   Debug((char*)buff1);
   TIM_SetCompare3(TIM4,PWM);
   Pj=Pi;
}
void charge1()
{
   u8 buff1[50];
   if(AD.B_V>=15)
   {
      PWM=PWM-3;
      if(PWM<0)
         PWM=0;
   }
   else
   {
      PWM=PWM+3;
      if(PWM>719)
         PWM=719;
   }
//   sprintf((char *)buff1,"PWM:%d\r\n",PWM);
//   Debug((char*)buff1);
   TIM_SetCompare3(TIM4,PWM); 
}
void charge2()
{
   u8 buff1[50];
   if(AD.B_V>=14.5)
   {
      PWM=PWM-3;
      if(PWM<0)
         PWM=0;
   }
   else
   {
      PWM=PWM+3;
      if(PWM>719)
         PWM=719;
   }
//   sprintf((char *)buff1,"PWM:%d\r\n",PWM);
//   Debug((char*)buff1);
   TIM_SetCompare3(TIM4,PWM); 
}
