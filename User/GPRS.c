#include "Mydefine.h"
void Cmd_Send1(char num, ...)
{
  va_list Cmd_ptr;
  uint8_t i;
  va_start(Cmd_ptr, num);
	USART_ClearFlag(USART1, USART_FLAG_TC);
  while(0 < num--){
		i = va_arg(Cmd_ptr, int);
		USART_SendData(USART1, i);
		while(RESET == USART_GetFlagStatus(USART1, USART_FLAG_TC));
  }
  va_end(Cmd_ptr);
}



void SIM800AInit()
{
	uint16_t temp;
	uint8_t Flag = FAIL;



	// INIT START
SIM800NONEEXIST:
	COUNTER = 0;
	Buffer_flg1 = 0x0000;
	// AT: IS SIM800A ALIVE?
	SendSIM800("AT\r\n", strlen("AT\r\n"));
	Debug("Have sent command: AT.\r\n");
	while(RESET == (Buffer_flg1 & 0x8000)){
		if(30 <= COUNTER){
			Debug("Haven't received any response, try again.\r\n");
			goto SIM800NONEEXIST;
		}
	}
	Debug("Have received something, going to checkout.\r\nHave rcved:");
	Debug((const char*)Rcv_buffer1);
	for(temp = (Buffer_flg1 & 0x7FFF); temp > 1; --temp){
		if(('K' == Rcv_buffer1[temp - 1]) && ('O' == Rcv_buffer1[temp - 2])){
			Debug("The response is correct, go on.\r\n");
			Flag = SUCCESS;
			break;
		}
	}
	if(SUCCESS != Flag){
		Debug("The response is wrrong, will try sending command again.\r\n");
		goto SIM800NONEEXIST;
	}else{
		Flag = FAIL;
	}



SIM800SIMCARDNONEEXIT:
	COUNTER = 0;
	Buffer_flg1 = 0x0000;
	// AT+CPIN?:IS SIM CARD EXIST?
	SendSIM800("AT+CPIN?\r\n", strlen("AT+CPIN?\r\n"));
	Debug("Have sent command: AT+CPIN?.\r\n");
	while(RESET == (Buffer_flg1 & 0x8000)){
		if(30 <= COUNTER){
			Debug("Haven't received any response, try again.\r\n");
			goto SIM800SIMCARDNONEEXIT;
		}
	}
	Debug("Have received something, going to checkout.\r\nHave rcved:");
	Debug((const char*)Rcv_buffer1);
	for(temp = (Buffer_flg1 & 0x7FFF); temp > 1; --temp){
		if(('Y' == Rcv_buffer1[temp - 1]) && ('D' == Rcv_buffer1[temp - 2])){
			Debug("The response is correct, go on.\r\n");
			Flag = SUCCESS;
			break;
		}
	}
	if(SUCCESS != Flag){
		Debug("The response is wrrong, will try sending command again.\r\n");
		goto SIM800SIMCARDNONEEXIT;
	}else{
		Flag = FAIL;
	}



SIM800GPRSFAIL:
	COUNTER = 0;
	Buffer_flg1 = 0x0000;
	// AT+CGATT?:IS GPRS ATTACHED?
	SendSIM800("AT+CGATT?\r\n", strlen("AT+CGATT?\r\n"));
	Debug("Have sent command: AT+CGATT?.\r\n");
	while(RESET == (Buffer_flg1 & 0x8000)){
		if(30 <= COUNTER){
			Debug("Haven't received any response, try again.\r\n");
			goto SIM800GPRSFAIL;
		}
	}
	Debug("Have received something, going to checkout.\r\nHave rcved:");
	Debug((const char*)Rcv_buffer1);
	for(temp = (Buffer_flg1 & 0x7FFF); temp > 3; --temp){
		if(('1' == Rcv_buffer1[temp - 1]) && ('T' == Rcv_buffer1[temp - 4])){
			Debug("The response is correct, go on.\r\n");
			Flag = SUCCESS;
			break;
		}
	}
	if(SUCCESS != Flag){
		Debug("The response is wrrong, will try sending command again.\r\n");
		goto SIM800GPRSFAIL;
	}else{
		Flag = FAIL;
	}



SIM800CIPMODEFAIL:
	delay1s();
	COUNTER = 0;
	Buffer_flg1 = 0x0000;
	// AT+CGATT?:IS GPRS ATTACHED?
	SendSIM800("AT+CIPMODE=1\r\n", strlen("AT+CIPMODE=1\r\n"));
	Debug("Have sent command: AT+CIPMODE=1.\r\n");
	while(RESET == (Buffer_flg1 & 0x8000)){
		if(30 <= COUNTER){
			Debug("Haven't received any response, try again.\r\n");
			goto SIM800CIPMODEFAIL;
		}
	}
	Debug("Have received something, going to checkout.\r\nHave rcved:");
	Debug((const char*)Rcv_buffer1);
	for(temp = (Buffer_flg1 & 0x7FFF); temp > 1; --temp){
		if(('K' == Rcv_buffer1[temp - 1]) && ('O' == Rcv_buffer1[temp - 2])){
			Debug("The response is correct, go on.\r\n");
			Flag = SUCCESS;
			break;
		}
	}
	if(SUCCESS != Flag){
		Debug("The response is wrrong, will try sending command again.\r\n");
		goto SIM800CIPMODEFAIL;
	}else{
		Flag = FAIL;
	}



SIM800APNFAIL:
	delay1s();
	COUNTER = 0;
	Buffer_flg1 = 0x0000;
	// AT+CIFSR:GET IP
	SendSIM800("AT+CSTT=\"CMNET\"\r\n", strlen("AT+CSTT=\"CMNET\"\r\n"));
	Debug("Have sent command: AT+CSTT=\"CMNET\".\r\n");
	while(RESET == (Buffer_flg1 & 0x8000)){
		if(50 <= COUNTER){
			Debug("Haven't received any response, try again.\r\n");
			goto SIM800APNFAIL;
		}
	}
	Debug("Have received something, go on.\r\nHave rcved:");
	Debug((const char*)Rcv_buffer1);
	Flag = FAIL;



SIM800GPRSSTARTFAIL:
	delay1s();
	COUNTER = 0;
	Buffer_flg1 = 0x0000;
	// AT+CIICR:START THE WIRLESS CONNECT
	SendSIM800("AT+CIICR\r\n", strlen("AT+CIICR\r\n"));
	Debug("Have sent command: AT+CIICR.\r\n");
	while(RESET == (Buffer_flg1 & 0x8000)){
		if(30 <= COUNTER){
			Debug("Haven't received any response, try again.\r\n");
			goto SIM800GPRSSTARTFAIL;
		}
	}
	Debug("Have received something, go on.\r\nHave rcved:");
	Debug((const char*)Rcv_buffer1);
	Flag = FAIL;



SIM800GETIPFAIL:
	delay1s();
	COUNTER = 0;
	Buffer_flg1 = 0x0000;
	// AT+CIFSR:GET IP
	SendSIM800("AT+CIFSR\r\n", strlen("AT+CIFSR\r\n"));
	Debug("Have sent command: AT+CIFSR.\r\n");
	while(RESET == (Buffer_flg1 & 0x8000)){
		if(50 <= COUNTER){
			Debug("Haven't received any response, try again.\r\n");
			goto SIM800GETIPFAIL;
		}
	}
	Debug("Have received something, go on.\r\nHave rcved:");
	Debug((const char*)Rcv_buffer1);
	Flag = FAIL;



SIM800GPRSCONNECTFAIL:
	delay1s();
	COUNTER = 0;
	Buffer_flg1 = 0x0000;
	// AT+CIPSTART=\"TCP\",\"1a5123747n.51mypc.cn\",\"34569\:CONNECT TO THE SERVER"1a5123747n.51mypc.cn", PORT:34569,
	SendSIM800("AT+CIPSTART=\"TCP\",\"1a5123747n.51mypc.cn\",\"34569\"\r\n", strlen("AT+CIPSTART=\"TCP\",\"1a5123747n.51mypc.cn\",\"34569\"\r\n"));
	Debug("Have sent command: AT+CIPSTART=\"TCP\",\"1a5123747n.51mypc.cn\",\"34569\".\r\n");
	while(RESET == (Buffer_flg1 & 0x8000)){
		if(30 <= COUNTER){
			Debug("Haven't received any response, try again.\r\n");
			goto SIM800GPRSCONNECTFAIL;
		}
	}
	Debug("Have received something, going to checkout.\r\nHave rcved:");
	Debug((const char*)Rcv_buffer1);
	for(temp = (Buffer_flg1 & 0x7FFF); temp > 1; --temp){
		if((('K' == Rcv_buffer1[temp - 1]) && ('O' == Rcv_buffer1[temp - 2])) \
			|| (('T' == Rcv_buffer1[temp - 1]) && ('C' == Rcv_buffer1[temp - 2]))){
			Debug("The response is correct, go on.\r\n");
			Flag = SUCCESS;
			break;
		}
	}
	if(SUCCESS != Flag){
		Debug("The response is wrrong, will try sending command again.\r\n");
		goto SIM800GPRSCONNECTFAIL;
	}else{
		Flag = FAIL;
	}
	Debug("The SIM800A is initialized, ready to work.\r\n");
}



void SIM800Handler()
{
	u8 strC[100];
	sprintf((char*)strC,"%d-%d-%d %d:%d:%d\r\nStart-up time:%d o'clock  Close time:%d o'clock\r\n",\
		calendar.w_year, calendar.w_month, calendar.w_date, calendar.hour, calendar.min, \
		calendar.sec, t_open, t_close);
	Usart1Send((const char*)strC,strlen((const char*)strC));
  Delayms(1000);
  Usart1Send("TIME", 4);
}


