#include "initDevice.h"

void RCC_Start (void){
//	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
//	AFIO->MAPR|=AFIO_MAPR_SWJ_CFG_JTAGDISABLE;
	/*------------ADC RCC------------*/
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
	RCC->CFGR &=~ RCC_CFGR_ADCPRE_0;
	RCC->CFGR |= RCC_CFGR_ADCPRE_1;
//	RCC->CFGR |= RCC_CFGR_ADCPRE_0;
	
	/*------------USART RCC------------*/
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
	
	/*------------I/O RCC------------*/
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN; // Ports A
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;	// Ports B
	
	/*------------DMA RCC------------*/
	RCC->AHBENR |= RCC_AHBENR_DMA1EN;	// DMA1
	
	/*------------I2C RCC------------*/
	RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN; // look to 1st string of function
	/*------------Disable JTAG PINS------------*/	
	AFIO->MAPR|=AFIO_MAPR_SWJ_CFG_JTAGDISABLE;
}
