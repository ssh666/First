#ifndef _DS1302_H
#define _DS1302_H
#include "MYdefine.h"
#include "sys.h"
#define ds1302clk GPIO_Pin_13
#define ds1302dat GPIO_Pin_14 
#define ds1302rst GPIO_Pin_12 

void ds1302_GPIO_Configuration(void);
void write_1302byte(uint8_t dat);//дһ���ֽڵ�����sck������д����
uint8_t read_1302(uint8_t add);//������
void write_1302(uint8_t add,uint8_t dat);//��ָ���Ĵ���д��һ���ֽڵ�����
void ds1302_init(uint8_t *write,uint8_t *time);//��ʼ��1302
#endif
