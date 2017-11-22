#include "Mydefine.h"
u8 Camera_H,Camera_L;

void Camera1Init()
{
	uint16_t temp;
	uint8_t Flag = FAIL;
	// INIT START



ACKCAMERA1FAILED:
	// SEND ACK SIGNAL, CHECK IF THE CAMERA IS ALIVE 
	COUNTER = 0;
	Buffer_flg2 = 0;
	CmdCamera1(ACKCMD);
	Debug("ACKCMD was sent.\r\n");
	while(RESET == (Buffer_flg2 & 0x8000)){
		if(30 <= COUNTER){
			Debug("Haven't received any response, try again.\r\n");
			goto ACKCAMERA1FAILED;
		}
	}
	Debug("Have received something, going to checkout.\r\nHave rcved:");
	Debug((const char*)Rcv_buffer2);
	for(temp = (Buffer_flg2 & 0x7FFF); temp > 2; --temp){
		if((0x0D == Rcv_buffer2[temp - 1]) && (0x0E == Rcv_buffer2[temp - 2])\
			 && (0xAA == Rcv_buffer2[temp - 3])){
			Debug("The response is correct, go on.\r\n");
			Flag = SUCCESS;
			break;
		}
	}
	if(FAIL == Flag){
		Debug("The response is wrrong, sending text failed.\r\n");
		goto ACKCAMERA1FAILED;
	}else{
		Flag = FAIL;
	}



INITCAMERA1FAILED:
	// SEND INIT COMMAND, SET THE JPEG PICTURE AS 640 X 480
	COUNTER = 0;
	Buffer_flg2 = 0;
	CmdCamera1(INITCMD);
	Debug("INITCMD was sent.\r\n");
	while(0 == (Buffer_flg2 & 0x8000)){
		if(30 <= COUNTER){
			Debug("Haven't received any response, try again.\r\n");
			goto INITCAMERA1FAILED;
		}
	}
	Debug("Have received something, going to checkout.\r\nHave rcved:");
	Debug((const char*)Rcv_buffer2);
	for(temp = (Buffer_flg2 & 0x7FFF); temp > 2; --temp){
		if((0x01 == Rcv_buffer2[temp - 1]) && (0x0E == Rcv_buffer2[temp - 2]) &&\
				(0xAA == Rcv_buffer2[temp - 3])){
			Debug("The response is correct, go on.\r\n");
			Flag = SUCCESS;
			break;
		}
	}
	if(FAIL == Flag){
		Debug("The response is wrrong, sending text failed.\r\n");
		goto INITCAMERA1FAILED;
	}else{
		Flag = FAIL;
	}



INITDATASIZE1FAILED:
	// SEND DATASIZE INIT COMMAND, 0X0040 = 64B PER PACK
	COUNTER = 0;
	Buffer_flg2 = 0;
	CmdCamera1(DATASIZECMD);
	Debug("DATASIZECMD was sent.\r\n");
	while(0 == (Buffer_flg2 & 0x8000)){
		if(30 <= COUNTER){
			Debug("Haven't received any response, try again.\r\n");
			goto INITDATASIZE1FAILED;
		}
	}
	Debug("Have received something, going to checkout.\r\nHave rcved:");
	Debug((const char*)Rcv_buffer2);
	for(temp = (Buffer_flg2 & 0x7FFF); temp > 2; --temp){
		if((0x06 == Rcv_buffer2[temp - 1]) && (0x0E == Rcv_buffer2[temp - 2]) &&\
				(0xAA == Rcv_buffer2[temp - 3])){
			Debug("The response is correct, go on.\r\n");
			Flag = SUCCESS;
			break;
		}
	}
	if(FAIL == Flag){
		goto INITDATASIZE1FAILED;
	}else{
		Flag = FAIL;
	}
	Debug("The CAMERA1 is initialized, ready to work.\r\n");
	// DATASIZE INIT COMPLETE, NOW CAMERA1 IS READY TO WORK
}



void Cmd_Send2(char num, ...)
{
  va_list Cmd_ptr;
  uint8_t i;
  va_start(Cmd_ptr, num);
	USART_ClearFlag(USART2, USART_FLAG_TC);
  while(0 < num--){
		i = va_arg(Cmd_ptr, int);
		USART_SendData(USART2, i);
		while(RESET == USART_GetFlagStatus(USART2, USART_FLAG_TC));
  }
  va_end(Cmd_ptr);
}



void Camera1Handler()
{
	Camera1Snapshot();
	Camera1ImageProcess();
  Delayms(500);
  Usart1Send("CAMERA1", 7);
}



uint32_t Camera1Snapshot(void)
{
	uint16_t temp;
	// SEND SNAPSHOT COMMAND, TAKING AN IMAGE
	COUNTER = 0;
	Buffer_flg2 = 0;
	CmdCamera1(IMGCMD);
	Debug("IMGCMD was sent.\r\n");
	delay1s();
	while(0 == (Buffer_flg2 & 0x8000)){
		if(30 <= COUNTER){
			Debug("Haven't received any response, FAILED.\r\n");
			return 0;
		}
	}
	Debug("Have received something, going to checkout.\r\n");
	for(temp = (Buffer_flg2 & 0x7FFF); temp > 2; --temp){
		if((0x05 == Rcv_buffer2[temp - 1]) && (0x0A == Rcv_buffer2[temp - 2]) &&\
				(0xAA == Rcv_buffer2[temp - 3])){
         Debug("The response is correct, taking image succeeded.\r\n");
         Camera_H = Rcv_buffer2[temp+1];
         Camera_L = Rcv_buffer2[temp];
			return 1;
		}
	}
	Debug("The response is wrrong, FAILED.\r\n");
	return 0;
}



void Camera1ImageProcess(void)
{
	uint16_t temp = 0;
	uint8_t lowbyte;
	uint8_t highbyte;
	do{
		lowbyte = (temp & 0x00FF);
		highbyte = ((temp++ >> 8) & 0x00FF);
		COUNTER = 0;
		Buffer_flg2 = 0;
		CmdCamera1(6, 0xAA, 0X0E, 0X00, 0X00, lowbyte, highbyte);
		while(0 == (Buffer_flg2 & 0x8000)){
			if(30 <= COUNTER){
				Debug("Haven't received any response, FAILED, stop processing.\r\n");
				break;
			}
		}
		Debug("Have received something, going to checkout.\r\n");
		if(0 == (Buffer_flg2 & 0x7FFF)){
			break;
		}else{
			GPRSSend((const char*)(Rcv_buffer2 + 4), (Buffer_flg2 & 0x7FFF) - 6);
		}
//		Delayms(10);
	}while(1);
}


