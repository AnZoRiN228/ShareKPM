#include "initDevice.h"

uint32_t adcAVR;

void ADC_Init (void){
	ADC1->CR2 |= ADC_CR2_CONT;					// start in continues mode
	ADC1->SMPR2 &= ~ADC_SMPR2_SMP0;			// sample time 1.5 cycles
//	ADC1->SMPR2 |= ADC_SMPR2_SMP0_1;		// sample time 13.5 cycles
//	ADC1->SMPR2 |= ADC_SMPR2_SMP0_2;			//41.5 cycles
//	ADC1->SMPR2 |= ADC_SMPR2_SMP0;			// 239.5 microSec
	ADC1->SQR3 &= ~ADC_SQR3_SQ1;				// select PA0 channel internal ADC as 1st conversion

//	ADC1->SQR1 &= ~ADC_SQR1_L_0; 			// number of conversions (for only 1 conversion is not necessary)
//	ADC1->CR1 |= ADC_CR1_SCAN;				// scan all channel start from SQ1
	ADC1->CR2 |= ADC_CR2_DMA;					// it's necessary to set bit in scan mode to escape loss of data in ADC_DR

	/*------------Start calibration------------*/
	ADC1->CR2 |= ADC_CR2_ADON;
	delay_ms (1);
	ADC1->CR2 |= ADC_CR2_CAL;
	while ((ADC1->CR2 & ADC_CR2_CAL) != 0) {} 
		
/*------------Start conversion------------*/
		ADC1->CR2 |= ADC_CR2_EXTTRIG;
		ADC1->CR2 |= ADC_CR2_EXTSEL;
		ADC1->CR2 |= ADC_CR2_SWSTART;
}
void ADC_GetAverageValue(){
	adcAVR=0;
	for (uint16_t i=0; i<MEASURE_NUM; i++)
	{
		adcAVR += buff[i];
	}
	adcAVR *= 3360;
	adcAVR /= 4096;
	adcAVR/=MEASURE_NUM;
}
void ADC_AvrValueCalibration() {
	//-------------Calibration ADC offset----------------//
	// reference for >0...<3,3 Volts
	if (adcAVR>=1685 && adcAVR<=1980){
		adcAVR += (ADC_OFFSET-30);
		return;
	}
	else if (adcAVR>=1320 && adcAVR<=1645){
		adcAVR -= 5;
		return;
	}
	// reference for >3,3...<5 Volts
	else if (adcAVR>=1980 && adcAVR<=2150){
		adcAVR += (ADC_OFFSET-30);
		return;
	}
	else if (adcAVR>=1150 && adcAVR<1320){
		adcAVR -= 17;
		return;
	}
	// reference for >5...<9 Volts
	else if (adcAVR>2150 && adcAVR<=2400){
		adcAVR += (ADC_OFFSET-2);
		return;
	}
	else if (adcAVR<1200 && adcAVR>=780){
		adcAVR -= (ADC_OFFSET-35);
		return;
	}
	// reference for >9...<15 Volts
	else if (adcAVR>2400 && adcAVR<=3500){
		adcAVR += (ADC_OFFSET+15);
		return;
	}
	//	else if (adcAVR<780 && adcAVR>=0){
	else if (adcAVR<780){
		adcAVR -= (ADC_OFFSET-12);
		return;
	}
	else{return;}
}
