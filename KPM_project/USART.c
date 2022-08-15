#include "initDevice.h"

void USART_Init (void) {
	USART1->CR1 |= USART_CR1_UE;
	USART1->CR1 &= ~USART_CR1_M;
	USART1->CR2 &= ~USART_CR2_STOP;
// USARTDIV = Fck / (16 * BAUD) = 72000000 / (16 * 9600) = 468,75
// USART_BRR = 468,75 * 16 = 7500.
// speed 39,0625 for 57600 baud
	USART1->BRR = 1250;
// USART1->BRR = 29200; //2400
	USART1->CR1 |= USART_CR1_TE;
}
void coutTerminal(char trans_string[]){
	usartLen = strlen (trans_string);
	for (unsigned int j=0; j<usartLen; j++)
	{
		while ((USART1->SR & USART_SR_TXE) == 0) {}
		USART1->DR = trans_string[j];
	}
}
void UART_Transmit(char *arr, unsigned int size, const char *arrStr, int avarage, uint16_t TIME_OUT){
	snprintf(arr, size, arrStr, avarage);
	coutTerminal(arr);
	delay_ms(TIME_OUT);
}
