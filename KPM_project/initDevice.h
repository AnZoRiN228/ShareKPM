#ifndef INIT_DEVICE_H
#define INIT_DEVICE_H

#include "main.h"

static __IO uint16_t delay_count;

void Init(void);

void SysTick_Handler(void);
void delay_ms(uint16_t delay_temp);

void GPIO_INIT (void);
void RCC_Start (void);
void DMA_ADC_Init(void);
void ADC_Init (void);
void USART_Init (void);
void NVIC_Init (void);

void I2C_Init(void);
void I2C_InitFastMode(void);
void I2C_BusyError(void);

#endif
