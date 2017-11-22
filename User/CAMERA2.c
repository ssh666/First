#include "Mydefine.h"


void Camera2Init()
{
	uint16_t temp;
	uint8_t Flag = FAIL;



	// INIT START
ACKCAMERA2FAILED:
	// SEND ACK SIGNAL, CHECK IF THE CAMERA IS ALIVE 
	COUNTER = 0;
	Buffer_flg3 = 0;
	CmdCamera2(ACKCMD);
	Debug("ACKCMD was sent.\r\n");
	while(RESET == (Buffer_flg3 & 0x8000)){
		if(30 <= COUNTER){
			Debug("Haven't received any response, try again.\r\n");
			goto ACKCAMERA2FAILED;
		}
	}
	Debug("Have received something, going to checkout.\r\nHave rcved:");
	Debug((const char*)Rcv_buffer3);
	for(temp = (Buffer_flg3 & 0x7FFF); temp > 2; --temp){
		if((0x0D == Rcv_buffer3[temp - 1]) && (0x0E == Rcv_buffer3[temp - 2])\
			 && (0xAA == Rcv_buffer3[temp - 3])){
			Debug("The response is correct, go on.\r\n");
			Flag = SUCCESS;
			break;
		}
	}
	if(FAIL == Flag){
		Debug("The response is wrrong, sending text failed.\r\n");
		goto ACKCAMERA2FAILED;
	}else{
		Flag = FAIL;
	}



INITCAMERA2FAILED:
	// SEND INIT COMMAND, SET THE JPEG PICTURE AS 640 X 480
	COUNTER = 0;
	Buffer_flg3 = 0;
	CmdCamera2(INITCMD);
	Debug("INITCMD was sent.\r\n");
	while(0 == (Buffer_flg3 & 0x8000)){
		if(30 <= COUNTER){
			Debug("Haven't received any response, try again.\r\n");
			goto INITCAMERA2FAILED;
		}
	}
	Debug("Have received something, going to checkout.\r\nHave rcved:");
	Debug((const char*)Rcv_buffer3);
	for(temp = (Buffer_flg3 & 0x7FFF); temp > 2; --temp){
		if((0x01 == Rcv_buffer3[temp - 1]) && (0x0E == Rcv_buffer3[temp - 2]) &&\
				(0xAA == Rcv_buffer3[temp - 3])){
			Debug("The response is correct, go on.\r\n");
			Flag = SUCCESS;
			break;
		}
	}
	if(FAIL == Flag){
		Debug("The response is wrrong, sending text failed.\r\n");
		goto INITCAMERA2FAILED;
	}else{
		Flag = FAIL;
	}



INITDATASIZE2FAILED:
	// SEND DATASIZE INIT COMMAND, 0X0040 = 64B PER PACK
	COUNTER = 0;
	Buffer_flg3 = 0;
	CmdCamera2(DATASIZECMD);
	Debug("DATASIZECMD was sent.\r\n");
	while(0 == (Buffer_flg3 & 0x8000)){
		if(30 <= COUNTER){
			Debug("Haven't received any response, try again.\r\n");
			goto INITDATASIZE2FAILED;
		}
	}
	Debug("Have received something, going to checkout.\r\nHave rcved:");
	Debug((const char*)Rcv_buffer3);
	for(temp = (Buffer_flg3 & 0x7FFF); temp > 2; --temp){
		if((0x06 == Rcv_buffer3[temp - 1]) && (0x0E == Rcv_buffer3[temp - 2]) &&\
				(0xAA == Rcv_buffer3[temp - 3])){
			Debug("The response is correct, go on.\r\n");
			Flag = SUCCESS;
			break;
		}
	}
	if(FAIL == Flag){
		goto INITDATASIZE2FAILED;
	}else{
		Flag = FAIL;
	}
	Debug("The CAMERA2 is initialized, ready to work.\r\n");
	// DATASIZE INIT COMPLETE, NOW CAMERA2 IS READY TO WORK
}



void Cmd_Send3(char num, ...)
{
  va_list Cmd_ptr;
  uint8_t i;
  va_start(Cmd_ptr, num);
	USART_ClearFlag(USART3, USART_FLAG_TC);
  while(0 < num--){
		i = va_arg(Cmd_ptr, int);
		USART_SendData(USART3, i);
		while(RESET == USART_GetFlagStatus(USART3, USART_FLAG_TC));
  }
  va_end(Cmd_ptr);
}



uint32_t Camera2Snapshot(void)
{
	uint16_t temp;
	// SEND SNAPSHOT COMMAND, TAKING AN IMAGE
	COUNTER = 0;
	Buffer_flg3 = 0;
	CmdCamera2(IMGCMD);
	Debug("IMGCMD was sent.\r\n");
	delay1s();
	while(0 == (Buffer_flg3 & 0x8000)){
		if(30 <= COUNTER){
			Debug("Haven't received any response, FAILED.\r\n");
			return 0;
		}
	}
	Debug("Have received something, going to checkout.\r\n");
	for(temp = (Buffer_flg3 & 0x7FFF); temp > 2; --temp){
		if((0x05 == Rcv_buffer3[temp - 1]) && (0x0A == Rcv_buffer3[temp - 2]) &&\
				(0xAA == Rcv_buffer3[temp - 3])){
			Debug("The response is correct, taking image succeeded.\r\n");
			return (uint32_t)(((uint32_t)Rcv_buffer3[temp + 1] << 8)+(Rcv_buffer3[temp]));
		}
	}
	Debug("The response is wrrong, FAILED.\r\n");
	return 0;
}



void Camera2ImageProcess(void)
{
	uint16_t temp = 0;
	uint8_t lowbyte;
	uint8_t highbyte;
	do{
		lowbyte = (temp & 0x00FF);
		highbyte = ((temp++ >> 8) & 0x00FF);
		COUNTER = 0;
		Buffer_flg3 = 0;
		CmdCamera2(6, 0xAA, 0X0E, 0X00, 0X00, lowbyte, highbyte);
		while(0 == (Buffer_flg3 & 0x8000)){
			if(30 <= COUNTER){
				Debug("Haven't received any response, FAILED, stop processing.\r\n");
				break;
			}
		}
		Debug("Have received something, going to checkout.\r\n");
		if(0 == (Buffer_flg3 & 0x7FFF)){
			break;
		}else{
			GPRSSend((const char*)(Rcv_buffer3 + 4), (Buffer_flg3 & 0x7FFF) - 6);
		}
		Delayms(10);
	}while(1);
}



void Camera2Handler()
{
	Camera2Snapshot();
	Camera2ImageProcess();
}


