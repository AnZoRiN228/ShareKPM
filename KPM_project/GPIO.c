#include "initDevice.h"

void GPIO_INIT (void){ 
/*--TURN OFF JTAG PINS FOR FALSE ALARM--*/
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
	AFIO->MAPR|=AFIO_MAPR_SWJ_CFG_JTAGDISABLE; 
	
/*------------ADC1 PA0------------*/
	GPIOA->CRL &= ~GPIO_CRL_CNF0;
	GPIOA->CRL &= ~GPIO_CRL_MODE0;
	
/*------------USART PINS------------*/	
	// TX PA9
	GPIOA->CRH &= (~GPIO_CRH_CNF9_0);
	GPIOA->CRH |= GPIO_CRH_CNF9_1;
	GPIOA->CRH &= ~GPIO_CRH_MODE9;
	GPIOA->CRH |= GPIO_CRH_MODE9;
	// RX PA10
/* TX
//	GPIOA->CRH &= (~GPIO_CRH_CNF10_0);
//	GPIOA->CRH |= GPIO_CRH_CNF10_1;
//	GPIOA->CRH &= (~(GPIO_CRH_MODE10));
//	GPIOA->BSRR |= GPIO_ODR_ODR10;
*/
/*------------Button PINS------------*/
	GPIOB->CRH &=~ GPIO_CRH_CNF15;
	GPIOB->CRH |= GPIO_CRH_CNF15_1;
	GPIOB->CRH &=~ GPIO_CRH_MODE15_1;
	GPIOB->ODR |= GPIO_ODR_ODR15;
/*------------LED PINS------------*/							
	GPIOA->CRL &= ~GPIO_CRL_CNF5;
	GPIOA->CRL &= ~GPIO_CRL_CNF6;
	GPIOA->CRL &= ~GPIO_CRL_CNF7;
	
	GPIOA->CRL &= ~GPIO_CRL_MODE5_1;
	GPIOA->CRL &= ~GPIO_CRL_MODE6_1;
	GPIOA->CRL &= ~GPIO_CRL_MODE7_1;
	
	GPIOA->CRL |= GPIO_CRL_MODE5_1;
	GPIOA->CRL |= GPIO_CRL_MODE6_1;
	GPIOA->CRL |= GPIO_CRL_MODE7_1;
	
/*------------I2C PINS------------*/		
	GPIOB->CRL &=~ (GPIO_CRL_CNF6 | GPIO_CRL_CNF7 | 
									GPIO_CRL_MODE7 | GPIO_CRL_MODE6);
  GPIOB->CRL |= GPIO_CRL_CNF6 | GPIO_CRL_CNF7 |
                GPIO_CRL_MODE7_0 | GPIO_CRL_MODE6_0;
}