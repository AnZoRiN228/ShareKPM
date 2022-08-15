#include "initDevice.h"

uint16_t buff[MEASURE_NUM];

void DMA_ADC_Init(void){
	
	DMA1_Channel1->CPAR = (uint32_t)&ADC1->DR;
	DMA1_Channel1->CMAR = (uint32_t)buff;
	DMA1_Channel1->CCR &=~ (unsigned int)DMA_CCR1_DIR;
	DMA1_Channel1->CNDTR = MEASURE_NUM;
	DMA1_Channel1->CCR &=~ (unsigned int)DMA_CCR1_PINC;
	DMA1_Channel1->CCR |= DMA_CCR1_MINC;
	DMA1_Channel1->CCR |= DMA_CCR1_PSIZE_0;
	DMA1_Channel1->CCR |= DMA_CCR1_MSIZE_0;
	DMA1_Channel1->CCR |= DMA_CCR1_PL;
	DMA1_Channel1->CCR &=~ (unsigned int)DMA_CCR1_CIRC;
//	DMA1_Channel1->CCR |= DMA_CCR1_CIRC;	// need to use in single mode
	
	NVIC_EnableIRQ(DMA1_Channel1_IRQn);
	DMA1_Channel1->CCR |= DMA_CCR1_TCIE;
	
//	DMA1_Channel1->CCR |= DMA_CCR1_EN;
}
