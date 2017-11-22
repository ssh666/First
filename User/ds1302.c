#include"DS1302.h"
#include "delay.h"

uint8_t read[7]={0x81,0x83,0x85,0x87,0x89,0x8b,0x8d};//读秒、分、时、日、月、周、年的寄存器地址
uint8_t write[]={0x80,0x82,0x84,0x86,0x88,0x8a,0x8c};//写秒、分、时、日、月、周、年的寄存器地址
/*PA4.6为输出*/
/*PA5配置为开漏模式，此模式下能够实现真正的双向IO口*/
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

void write_1302byte(uint8_t dat)//写一个字节的数据sck上升沿写数据
{
 	uint8_t i=0;
	GPIO_ResetBits(GPIOB,ds1302clk);
 	//ds1302clk=0;
    delay_us(2);//延时大约2us
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
uint8_t read_1302(uint8_t add)//读数据
{
	uint8_t i=0,dat1=0x00;
	GPIO_ResetBits(GPIOB,ds1302rst);
	GPIO_ResetBits(GPIOB,ds1302clk);
	//ds1302rst=0;
	//ds1302clk=0;
	delay_us(3);//略微延时2us
	GPIO_SetBits(GPIOB,ds1302rst);
	//ds1302rst=1;
	delay_us(3);//时间要大约3us
	write_1302byte(add);//先写寄存器的地址
	for(i=0;i<8;i++)
	   {
	    GPIO_SetBits(GPIOB,ds1302clk);
		//ds1302clk=1;
		dat1>>=1;
		GPIO_ResetBits(GPIOB,ds1302clk);
		//ds1302clk=0;//拉低时钟线，以便于数据的读入
		if(GPIO_ReadInputDataBit(GPIOB,ds1302dat)==1)//数据线此时为高电平
		{dat1=dat1|0x80;}
	  }
	  delay_us(1);
	  GPIO_ResetBits(GPIOB,ds1302rst);
	  //ds1302rst=0;
	  return dat1;
	}
void write_1302(uint8_t add,uint8_t dat)//向指定寄存器写入一个字节的数据
{
	GPIO_ResetBits(GPIOB,ds1302rst);
	GPIO_ResetBits(GPIOB,ds1302clk);
	//ds1302rst=0;
	//ds1302clk=0;
	delay_us(1);//略微延时
	GPIO_SetBits(GPIOB,ds1302rst);
	//ds1302rst=1;
	delay_us(2);//时间大约2us
	write_1302byte(add);
	write_1302byte(dat);
	GPIO_ResetBits(GPIOB,ds1302rst);
	GPIO_ResetBits(GPIOB,ds1302clk);
	//ds1302clk=0;
	//ds1302rst=0;
	delay_us(1);

}
void ds1302_init(uint8_t *write,uint8_t *time)//初始化1302
{
	uint8_t i=0,j=0;
	write_1302(0x8e,0x00);//去除写保护
		for(i=0;i<7;i++)//转BCD码
	  {
		j=time[i]%10;//个位数部分
		time[i]=(time[i]/10)*16+j;	
	  }
	for(i=0;i<7;i++)//进行对时
	{
		write_1302(write[i],time[i]);
		
	}
    write_1302(0x8e,0x80);//加写保护
}
void ds1302_data(uint8_t *read)//处理数据并通过串口打印
{
//   u8 buffer3[50];
	uint8_t i=0,g[7],time[7];
	for(i=0;i<7;i++)
	    	    {
	    		 time[i]=read_1302(read[i]);
	    		}//读数据已经完成
			    for(i=0;i<7;i++)
	    		      {
		    			g[i]=time[i]%16;//秒个位数据
		    			time[i]=time[i]/16;//秒十位数据
	    			  }
	    			//此时已转换成10进制数，g[i]里面存放的是秒分时日月周年的各个位数据
	    			//而此时的time【i】里面存放的则是秒分时日月周年的十位数据
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
//	  sprintf((char *)buffer3,"20%d%d年%d%d月%d%d日%d%d:%d%d:%d%d 星期%d\r\n",\
//	  time[6],g[6],time[4],g[4],time[3],g[3],time[2],g[2],time[1],g[1],time[0],g[0],g[5]);
//     Debug((char *)buffer3);
//	  s=time[0]+g[0];	
}
