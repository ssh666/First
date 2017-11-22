#include"DS1302.h"
#include "delay.h"

uint8_t read[7]={0x81,0x83,0x85,0x87,0x89,0x8b,0x8d};//���롢�֡�ʱ���ա��¡��ܡ���ļĴ�����ַ
uint8_t write[]={0x80,0x82,0x84,0x86,0x88,0x8a,0x8c};//д�롢�֡�ʱ���ա��¡��ܡ���ļĴ�����ַ
/*PA4.6Ϊ���*/
/*PA5����Ϊ��©ģʽ����ģʽ���ܹ�ʵ��������˫��IO��*/
void ds1302_GPIO_Configuration(void)
{
	
  GPIO_InitTypeDef GPIO_InitStruct;

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = ds1302clk|ds1302rst;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStruct.GPIO_Pin = ds1302dat;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
		
}

void write_1302byte(uint8_t dat)//дһ���ֽڵ�����sck������д����
{
 	uint8_t i=0;
	GPIO_ResetBits(GPIOB,ds1302clk);
 	//ds1302clk=0;
    delay_us(2);//��ʱ��Լ2us
 	for(i=0;i<8;i++)
 	    {
			GPIO_ResetBits(GPIOB,ds1302clk);
 	    	//ds1302clk=0;
			if(dat&0x01)
			GPIO_SetBits(GPIOB,ds1302dat);
			else
			GPIO_ResetBits(GPIOB,ds1302dat);
	 		//ds1302dat=(dat&0x01);
	 		delay_us(2);
			GPIO_SetBits(GPIOB,ds1302clk);
	 		//ds1302clk=1;
	 		dat>>=1;
	 		delay_us(1);
 		}	
	}
uint8_t read_1302(uint8_t add)//������
{
	uint8_t i=0,dat1=0x00;
	GPIO_ResetBits(GPIOB,ds1302rst);
	GPIO_ResetBits(GPIOB,ds1302clk);
	//ds1302rst=0;
	//ds1302clk=0;
	delay_us(3);//��΢��ʱ2us
	GPIO_SetBits(GPIOB,ds1302rst);
	//ds1302rst=1;
	delay_us(3);//ʱ��Ҫ��Լ3us
	write_1302byte(add);//��д�Ĵ����ĵ�ַ
	for(i=0;i<8;i++)
	   {
	    GPIO_SetBits(GPIOB,ds1302clk);
		//ds1302clk=1;
		dat1>>=1;
		GPIO_ResetBits(GPIOB,ds1302clk);
		//ds1302clk=0;//����ʱ���ߣ��Ա������ݵĶ���
		if(GPIO_ReadInputDataBit(GPIOB,ds1302dat)==1)//�����ߴ�ʱΪ�ߵ�ƽ
		{dat1=dat1|0x80;}
	  }
	  delay_us(1);
	  GPIO_ResetBits(GPIOB,ds1302rst);
	  //ds1302rst=0;
	  return dat1;
	}
void write_1302(uint8_t add,uint8_t dat)//��ָ���Ĵ���д��һ���ֽڵ�����
{
	GPIO_ResetBits(GPIOB,ds1302rst);
	GPIO_ResetBits(GPIOB,ds1302clk);
	//ds1302rst=0;
	//ds1302clk=0;
	delay_us(1);//��΢��ʱ
	GPIO_SetBits(GPIOB,ds1302rst);
	//ds1302rst=1;
	delay_us(2);//ʱ���Լ2us
	write_1302byte(add);
	write_1302byte(dat);
	GPIO_ResetBits(GPIOB,ds1302rst);
	GPIO_ResetBits(GPIOB,ds1302clk);
	//ds1302clk=0;
	//ds1302rst=0;
	delay_us(1);

}
void ds1302_init(uint8_t *write,uint8_t *time)//��ʼ��1302
{
	uint8_t i=0,j=0;
	write_1302(0x8e,0x00);//ȥ��д����
		for(i=0;i<7;i++)//תBCD��
	  {
		j=time[i]%10;//��λ������
		time[i]=(time[i]/10)*16+j;	
	  }
	for(i=0;i<7;i++)//���ж�ʱ
	{
		write_1302(write[i],time[i]);
		
	}
    write_1302(0x8e,0x80);//��д����
}
void ds1302_data(uint8_t *read)//�������ݲ�ͨ�����ڴ�ӡ
{
//   u8 buffer3[50];
	uint8_t i=0,g[7],time[7];
	for(i=0;i<7;i++)
	    	    {
	    		 time[i]=read_1302(read[i]);
	    		}//�������Ѿ����
			    for(i=0;i<7;i++)
	    		      {
		    			g[i]=time[i]%16;//���λ����
		    			time[i]=time[i]/16;//��ʮλ����
	    			  }
	    			//��ʱ��ת����10��������g[i]�����ŵ������ʱ��������ĸ���λ����
	    			//����ʱ��time��i�������ŵ��������ʱ���������ʮλ����
                 Data1.YR[0]=0X32; 
                 Data1.YR[1]=0X30; 
                 Data1.YR[2]=0X30+time[6];   
                 Data1.YR[3]=0X30+g[6];
                 Data1.MN[0]=0X30+time[4];
                 Data1.MN[1]=0X30+g[4];
                 Data1.MN[2]=0X30+time[3];
                 Data1.MN[3]=0X30+g[3];
                 Data1.HM[0]=0X30+time[2];
                 Data1.HM[1]=0X30+g[2];
                 Data1.HM[2]=0X30+time[1];
                 Data1.HM[3]=0X30+g[1];                   
//	  sprintf((char *)buffer3,"20%d%d��%d%d��%d%d��%d%d:%d%d:%d%d ����%d\r\n",\
//	  time[6],g[6],time[4],g[4],time[3],g[3],time[2],g[2],time[1],g[1],time[0],g[0],g[5]);
//     Debug((char *)buffer3);
//	  s=time[0]+g[0];	
}
