#include "Mydefine.h"
#include "dht11.h"
#include "ds18b20.h"
#include "ds1302.h"
u8 LV_H,LV_M,LV_L,BV_H,BV_M,BV_L,LI_H,LI_M,LI_L,BI_H,BI_M,BI_L,RI_H,RI_M,RI_L,AT_H,AT_M,AT_L,\
AH_H,AH_M,AH_L,WT_H,WT_M,WT_L,YR_H,YR_L,MN_H,MN_L,HM_H,HM_L;/*光伏电压，蓄电池电压，光伏电流，电池电流，
                                         负载电流，气温，湿度，水温,年份，月日，时分*/
void Adc_Init(void)
{ 	
	ADC_InitTypeDef ADC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOC	, ENABLE );
 
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);

                         
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_5 | GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
   
   GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	ADC_DeInit(ADC1);

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_Init(ADC1, &ADC_InitStructure);

  
	ADC_Cmd(ADC1, ENABLE);
	
	ADC_ResetCalibration(ADC1);
	
	while(ADC_GetResetCalibrationStatus(ADC1));
	
	ADC_StartCalibration(ADC1);
  
	while(ADC_GetCalibrationStatus(ADC1));
 
}				  


u16 Get_Adc(u8 ch)   
{
  ADC_RegularChannelConfig(ADC1,ch,1,ADC_SampleTime_239Cycles5);
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));
	return ADC_GetConversionValue(ADC1);
}


u16 Get_Adc_Average(u8 ch,u8 times)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		temp_val+=Get_Adc(ch);
		Delayms(5);
	}
	return temp_val/times;
}





void Get_All_Measure()
{
    u32 buffer5[50];
   AD.L_V=(float)(Get_Adc_Average(ADC_Channel_6,10)*0.00080566*7.20493*1.02);
//   sprintf((char*)buffer5,"AD_LV: %f\r\n",AD.L_V);
//   Debug((char*)buffer5);
   AD.B_V=(float)(Get_Adc_Average(ADC_Channel_5,10)*0.00080566*7.13979);
//   sprintf((char*)buffer5,"AD_BV: %f\r\n",AD.B_V);
//   Debug((char*)buffer5);
   AD.L_I=(float)(Get_Adc_Average(ADC_Channel_10,15)*0.00008055*10);
//   sprintf((char*)buffer5,"AD_LI: %f\r\n",AD.L_I);
//   Debug((char*)buffer5);
   AD.B_I=(float)(Get_Adc_Average(ADC_Channel_11,30)*0.00008055*15*1.30);
//   sprintf((char*)buffer5,"AD_BI: %f\r\n",AD.B_I);
//   Debug((char*)buffer5);
   AD.R_I=(float)(Get_Adc_Average(ADC_Channel_12,10)*0.00008055*15);
//   sprintf((char*)buffer5,"AD_RI: %f\r\n",AD.R_I);
//   Debug((char*)buffer5);
   AD.Rain=(float)(Get_Adc_Average(ADC_Channel_13,10)*0.00008055);
 

}
void Percision()   //
{
//   u32 buffer6[50];
//   sprintf((char*)buffer6,"AD_LV: %f\r\n",AD.L_V);
//   Debug((char*)buffer6);
//   sprintf((char*)buffer6,"AD_BV: %f\r\n",AD.B_V);
//   Debug((char*)buffer6);
//   sprintf((char*)buffer6,"AD_LI: %f\r\n",AD.L_I);
//   Debug((char*)buffer6);
//   sprintf((char*)buffer6,"AD_BI: %f\r\n",AD.B_I);
//   Debug((char*)buffer6);
//   sprintf((char*)buffer6,"AD_RI: %f\r\n",AD.R_I);
//   Debug((char*)buffer6);
   u8 Temp1,Temp2;
   Get_All_Measure();
   u8 buffer5[50];
   u16 Temp;
   u8 a,b,c,d;
   Temp=floor(AD.L_I*100);
   a=Temp/1000;
   b=Temp/100%10;
   c=Temp/10%10;
   d=Temp%10;
   if(d>=5)
      c=c+1;
   if(c>=10)
   {
      b=b+1;
      c=0;
   }
   if(b>=10)
   {
      a=a+1;
      b=0;
   }
   Data1.LI[0]=0x30+a;
   Data1.LI[1]=0x30+b;
   Data1.LI[2]=0x30+c;
   //光伏电流
   Temp=floor(AD.L_V*100);
   a=Temp/1000;
   b=Temp/100%10;
   c=Temp/10%10;
   d=Temp%10;
   if(d>=5)
      c=c+1;
   if(c>=10)
   {
      b=b+1;
      c=0;
   }
   if(b>=10)
   {
      a=a+1;
      b=0;
   }
   Data1.LV[0]=0x30+a;
   Data1.LV[1]=0x30+b;
   Data1.LV[2]=0x30+c;
   //光伏电压
   Temp=floor(AD.B_I*100);
   a=Temp/1000;
   b=Temp/100%10;
   c=Temp/10%10;
   d=Temp%10;
   if(d>=5)
      c=c+1;
   if(c>=10)
   {
      b=b+1;
      c=0;
   }
   if(b>=10)
   {
      a=a+1;
      b=0;
   }
   Data1.BI[0]=0x30+a;
   Data1.BI[1]=0x30+b;
   Data1.BI[2]=0x30+c;
   
   Temp=floor(AD.B_V*100);
   a=Temp/1000;
   b=Temp/100%10;
   c=Temp/10%10;
   d=Temp%10;
   if(d>=5)
      c=c+1;
   if(c>=10)
   {
      b=b+1;
      c=0;
   }
   if(b>=10)
   {
      a=a+1;
      b=0;
   }
   Data1.BV[0]=0x30+a;
   Data1.BV[1]=0x30+b;
   Data1.BV[2]=0x30+c;
   
   Temp=floor(AD.R_I*100);
   a=Temp/1000;
   b=Temp/100%10;
   c=Temp/10%10;
   d=Temp%10;
   if(d>=5)
      c=c+1;
   if(c>=10)
   {
      b=b+1;
      c=0;
   }
   if(b>=10)
   {
      a=a+1;
      b=0;
   }
   Data1.RI[0]=0x30+a;
   Data1.RI[1]=0x30+b;
   Data1.RI[2]=0x30+c;
   
   
   DHT11_Read_Data(&Temp1,&Temp2);
   AD.AT=Temp1;
   a=Temp1/10%10;
   b=Temp1%10;
   Data1.AT[0]=0x30+a;
   Data1.AT[1]=0x30+b;
   Data1.AT[2]=0x30+0;
   
   AD.AH=Temp2;
   a=Temp2/10%10;
   b=Temp2%10;
   Data1.AH[0]=0x30+a;
   Data1.AH[1]=0x30+b;
   Data1.AH[2]=0x30+0;
   
   Temp=DS18B20_Get_Temp();
   AD.WT=Temp/10;
   a=Temp/100%10;
   b=Temp/10%10;
   c=Temp%10;
   Data1.WT[0]=0x30+a;
   Data1.WT[1]=0x30+b;
   Data1.WT[2]=0x30+c;  
}
