#include "Mydefine.h"
// #define _DEBUG_USART1_

void Debug(const char* x){
	
	
	
	#ifdef _DEBUG_USART1_
	Usart1Send(x,strlen(x));
	#endif
	
	
	
	#ifdef _DEBUG_USART2_
	Usart2Send(x,strlen(x));
	#endif
	
	
	
	#ifdef _DEBUG_USART3_
	Usart3Send(x,strlen(x));
	#endif
	
	
	
}

