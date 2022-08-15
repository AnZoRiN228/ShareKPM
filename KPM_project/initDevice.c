#include "initDevice.h"

void SysTick_Handler(void) { //1ms
	if (delay_count>0)
	{  delay_count--; } 
}

void delay_ms(uint16_t delay_temp){
	delay_count=delay_temp;
 	while (delay_count) {}
}

void Init(){
	SysTick_Config (SystemCoreClock/1000);

	RCC_Start();
	GPIO_INIT();
	ADC_Init ();
	delay_ms(100);
	USART_Init();
//	NVIC_Init ();
	DMA_ADC_Init();
//	I2C_Init();
	I2C_InitFastMode();
}
