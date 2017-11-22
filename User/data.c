#include "Mydefine.h"
#include "stmflash.h"
u16 Steal_Send[45]={0x75,0x73,0x72,0x2E,0X63,0X6E,0X23,0X41,0X54,0X2B,0X53,0X4D,0X53,0X45,\
   0X4E,0X44,0X3D ,0X22,0X31,0X33,0X37,0X35,0X38,0X32,0X39,0X35,0X38,0X31,0X30,0X22,\
   0X2C,0X33,0X2C,0X22,0XB5,0XE7,0XB3,0XD8,0XB1,0XBB,0XB5,0XC1,0X22,0X0D,0X0A};    //手机从第十九个开始  电池被盗
u8 Phone_Number[11];
u8 Already_War=0;   //已经发送过警报的话  延时30min在次发警报
void DataInit()               //数据初始化
   {
      int i;
      for(i=0;i<75;i++)
      {
         *((u8*)&Data1+i)=0x30;
      }
      Data1.head=0X55;
      Data1.address[0]=0x31;   //默认地址1235
      Data1.address[1]=0x32;
      Data1.address[2]=0x33;
      Data1.address[3]=0x36;
      for(i=5;i<75;i++)         //赋初始值
      {
         *((u8*)&Data1+i)=0x30;
      }
      War.LI_up=5.0;
      War.BI_up=5.0;
      War.RI_up=5.0;
      War.LV_up=24.0;
      War.BV_up=15.0;
      War.AT_up=50.0;
      War.AH_up=80.0;
      War.WT_up=50.0;
      War.BV_down=10.8;
      War.AT_down=5.0;
      War.AH_down=10.8;
      War.WT_down=5.0;         //初始警报值
      STMFLASH_Read(FLASH_SAVE_ADDR,(u16*)Phone_Number,11);
      Over_work=0;
      Already_War=0;
      Data_flag=0;
   }
void War_compare()            //警报判断
{
    Data1.war_state[14]=0x30;  //保留
   if(AD.L_I>=War.LI_up)
      Data1.war_state[1]=0x31;
   else
      Data1.war_state[1]=0x30;
   if(AD.B_I>=War.BI_up)
      Data1.war_state[2]=0x31;
   else
      Data1.war_state[2]=0x30;
   if(AD.L_V>=War.LV_up)
      Data1.war_state[3]=0x31;
   else
      Data1.war_state[3]=0x30;
   if(AD.B_V<=War.BV_down)
      Data1.war_state[4]=0x31;
   else
      Data1.war_state[4]=0x30;
   if(AD.B_V>=War.BV_up)
      Data1.war_state[5]=0x31;
   else
      Data1.war_state[5]=0x30;
   if(AD.R_I>=War.RI_up)
      Data1.war_state[6]=0x31;
   else
      Data1.war_state[6]=0x30;
    if(AD.AT<=War.AT_down)
      Data1.war_state[7]=0x31;
   else
      Data1.war_state[7]=0x30;
    if(AD.AT>=War.AT_up)
      Data1.war_state[8]=0x31;
   else
      Data1.war_state[8]=0x30;
    if(AD.WT<=War.WT_down)
      Data1.war_state[9]=0x31;
   else
      Data1.war_state[9]=0x30;
   if(AD.WT>=War.WT_up)
      Data1.war_state[10]=0x31;
   else
      Data1.war_state[10]=0x30;
   if(AD.AH<=War.AH_down)
      Data1.war_state[11]=0x31;
   else
      Data1.war_state[11]=0x30;
   if(AD.AH>=War.AH_up)
      Data1.war_state[12]=0x31;
   else
      Data1.war_state[12]=0x30;
   if(AD.B_V<5)                                     //电池被盗 发短信
   {
      Data1.war_state[13]=0x31;
      Steal_Send[18]=Phone_Number[0];
      Steal_Send[19]=Phone_Number[1];
      Steal_Send[20]=Phone_Number[2];
      Steal_Send[21]=Phone_Number[3];
      Steal_Send[22]=Phone_Number[4];
      Steal_Send[23]=Phone_Number[5];
      Steal_Send[24]=Phone_Number[6];
      Steal_Send[25]=Phone_Number[7];
      Steal_Send[26]=Phone_Number[8];
      Steal_Send[27]=Phone_Number[9];
      Steal_Send[28]=Phone_Number[10];
      if(Already_War==0)
      {
         Message((u16*)Steal_Send,45);
         Already_War=1;
         COUNTER3=0;
      }
   }
   else
      Data1.war_state[13]=0x30;
   if((Data1.war_state[1]==0x30)&&(Data1.war_state[2]==0x30)&&(Data1.war_state[3]==0x30)&&(Data1.war_state[4]==0x30)&&\
      (Data1.war_state[5]==0x30)&&(Data1.war_state[6]==0x30)&&(Data1.war_state[7]==0x30)&&(Data1.war_state[8]==0x30)&&\
      (Data1.war_state[9]==0x30)&&(Data1.war_state[10]==0x30)&&(Data1.war_state[11]==0x30)&&(Data1.war_state[12]==0x30)&&\
      (Data1.war_state[13]==0x30))
      Data1.war_state[0]=0x30;
   else
      Data1.war_state[0]=0x31;
}