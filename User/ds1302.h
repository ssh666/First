#ifndef _DS1302_H
#define _DS1302_H
#include "MYdefine.h"
#include "sys.h"
#define ds1302clk GPIO_Pin_13
#define ds1302dat GPIO_Pin_14 
#define ds1302rst GPIO_Pin_12 

void ds1302_GPIO_Configuration(void);
void write_1302byte(uint8_t dat);//写一个字节的数据sck上升沿写数据
uint8_t read_1302(uint8_t add);//读数据
void write_1302(uint8_t add,uint8_t dat);//向指定寄存器写入一个字节的数据
void ds1302_init(uint8_t *write,uint8_t *time);//初始化1302
#endif
