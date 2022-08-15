#include "initDevice.h"

void I2C_Init(void){
//	DBGMCU->CR |= DBGMCU_CR_DBG_I2C1_SMBUS_TIMEOUT;
	I2C1->CR1 &= ~I2C_CR1_SMBUS;
	
	I2C1->CR2 &=~ I2C_CR2_FREQ;
	I2C1->CR2 |= 36;	// need |=
	
	// formula to calcilate clock of SCL (100kHz) 10 000nS/(2* TPCLK1) TPCLK1 = 1 / (frequency of APB1) (1 / 36 MHz)
	I2C1->CCR &= ~I2C_CCR_CCR;
	I2C1->CCR = 180; //10 000ns/55,(5)ns = 180
	I2C1->CCR &=~ I2C_CCR_FS;
	I2C1->CCR &=~ I2C_CCR_DUTY;
	//Standart_Mode = 1000nS, Fast_Mode = 300nS, 1/42MHz = 24nS
	I2C1->TRISE = 37; //(1000nS/28nS)+1

	I2C1->CR1 |= I2C_CR1_PE;
// initialization finished. Then it's needed to set START->transfer Data->set STOP bit
}

void I2C_InitFastMode(void){
//	DBGMCU->CR |= DBGMCU_CR_DBG_I2C1_SMBUS_TIMEOUT;
	I2C1->CR1 &= ~I2C_CR1_SMBUS;

	I2C1->CR2 &=~ I2C_CR2_FREQ;
	I2C1->CR2 |= 36;	// need |=

	// formula to calcilate clock of SCL (100kHz) 10 000nS/(2* TPCLK1); TPCLK1 = 1 / (frequency of APB1) (1 / 36 MHz)
	I2C1->CCR &= ~I2C_CCR_CCR;
	I2C1->CCR = 2; // for 400kHz it's 4. Now it's 800kHz
	I2C1->CCR |= I2C_CCR_FS;
	I2C1->CCR |= I2C_CCR_DUTY;
	//Standart_Mode = 1000nS, Fast_Mode = 300nS, 1/36MHz = 28nS
	I2C1->TRISE = 11; //(1000nS/28nS)+1

	I2C1->CR1 |= I2C_CR1_PE;
// initialization finished. Then it's needed to set START->transfer Data->set STOP bit
}
void I2C_BusyError(void){
	I2C1->CR1 &=~ I2C_CR1_PE;	// 1
	GPIOB->CRL &=~ (GPIO_CRL_CNF6 | GPIO_CRL_CNF7 | 
									GPIO_CRL_MODE7 | GPIO_CRL_MODE6);
	GPIOB->CRL |= GPIO_CRL_CNF6_0 | GPIO_CRL_CNF7_0 |
                GPIO_CRL_MODE7_0 | GPIO_CRL_MODE6_0;
	GPIOB->ODR |= GPIO_ODR_ODR6; // SCL	2
	GPIOB->ODR |= GPIO_ODR_ODR7; // SDA	2
	while (!(GPIOB->IDR & GPIO_IDR_IDR6) &&
				 !(GPIOB->IDR & GPIO_IDR_IDR7)){} // 3
	GPIOB->ODR &=~ (unsigned int)GPIO_ODR_ODR7;	// 4
	while (GPIOB->IDR & GPIO_IDR_IDR7){}	// 5
	GPIOB->ODR &=~ (unsigned int)GPIO_ODR_ODR6;	// 6
	while (GPIOB->IDR & GPIO_IDR_IDR6){}	// 7
	GPIOB->ODR |= (unsigned int)GPIO_ODR_ODR7;	// 8
	while (!(GPIOB->IDR & GPIO_IDR_IDR7)){}	// 9
	GPIOB->ODR |= (unsigned int)GPIO_ODR_ODR6;	// 10
	while (!(GPIOB->IDR & GPIO_IDR_IDR6)){}	// 11
  GPIOB->CRL |= GPIO_CRL_CNF6_1 | GPIO_CRL_CNF7_1;// 12
	I2C1->CR1 |= I2C_CR1_SWRST; // 13
	I2C1->CR1 &=~ I2C_CR1_SWRST; // 14
	I2C1->CR1 |= I2C_CR1_PE; // 15
}
