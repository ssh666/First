#include "mydefine.h"
#include "dht11.h"
#include "myfunction.h"

u8 stop_flag = 0;

void SelfCheck(void)
{
  // ���ʪ��
  u8 temperature;
	u8 humidity;
  DHT11_Read_Data(&temperature,&humidity);
  if(humidity>95)
  {
    stop_output();
  }
  else
  {
    RTC_Get();
    if((10<calendar.hour) && (12>calendar.hour))
    {
      start_output();
    }
    else
    {
      stop_output();
    }
  }
  // �������ص�ѹ

  // ������ص�ѹ
  double AD_Value = 0;
  AD_Value=Get_Adc_Average(ADC_Channel_1,10)*0.00011841;
  if(AD_Value>14.3)
  {
      stop_charge();
  }
  else
  {
    if(AD_Value<14.2)
      start_charge();
  }
  
  // ��������
  if(stop_flag!=1)
  {
    AD_Value=Get_Adc_Average(ADC_Channel_5,10)*0.00011841;
    if(AD_Value>1.6)
    {
        stop_charge()  ;
    }
    else
    {
      if(AD_Value<1.4)
        start_charge();
    }
  }
  
  // ������ص���
  if(stop_flag!=1)
  {
    AD_Value=Get_Adc_Average(ADC_Channel_6,10)*0.00011841;
    if(AD_Value>1.3)
    {
        stop_charge();
    }
    else
    {
      if(AD_Value<1.15)
        start_charge();
    }
  }
  
  // �ж��Ƿ񷢳�����
  
  // �Լ����
}
