#include "main.h"

int testNum; // later think about trasfer this variable to another file

void DMA1_Channel1_IRQHandler(){ 
 if ((DMA1->ISR & DMA_ISR_TCIF1)) 
 { 
		flagDMA = 1;
		DMA1->IFCR |= DMA_IFCR_CGIF1; 
 } 
}
int main (){
	Init();
	ssd1306_Init();
	ssd1306_SetCursor (13,0);
	ssd1306_WriteString("ATE-engineering", Font_7x10, White);
	ssd1306_UpdateScreen(1,2);
	
	ssd1306_SetCursor (5,16);
	ssd1306_WriteString("Pressbutton", Font_11x18, White);
	ssd1306_SetCursor (20,35);
	ssd1306_WriteString("To start", Font_11x18, White);
	ssd1306_UpdateScreen(2,8);
	
	DMA1_Channel1->CCR |= DMA_CCR1_EN;
	
	while (testNum != NUM_OF_TESTS){
		while (butt != (GPIOB->IDR & GPIO_IDR_IDR15)){}
			delay_ms(100);
			ledBlinkTestCount(testNum);
			if (flagDMA == 1)
			{
				DMA1_Channel1->CCR &=~ (unsigned int)DMA_CCR1_EN;	
				DMA1_Channel1->CNDTR = MEASURE_NUM;
				DMA1_Channel1->CCR |= DMA_CCR1_EN;
				flagDMA = 0;
				
//				ssd1306_Fill(Black);															// следующие 7 строк просто вывожу Тест и №теста, и слово Model
//				ssd1306_UpdateScreen(3,3);
//				ssd1306_SetCursor (0,16);
//				ssd1306_ConvertVar(trans_str, 40, "TestN#%d", testNum);
//				ssd1306_WriteString(trans_str, Font_7x10, White);
//				ssd1306_SetCursor (64,16);
//				ssd1306_WriteString("Model:", Font_7x10, White);
//				ssd1306_UpdateScreen(3,3);
				
				// need some delay before measurent. Now delay is replaced by
				// displaying information on the screen. If this functions will stay before ssd, i will have last measurements
				// in next test
				ADC_GetAverageValue();				//эта основная функция, берёт все значения из DMAбуфера
				ADC_AvrValueCalibration();		// функция убирает смещение (калибрует измеренное значение)
				
				ssd1306_Fill(Black);
				ssd1306_UpdateScreen(3,3);
				ssd1306_SetCursor (0,16);
				ssd1306_ConvertVar(trans_str, 40, "TestN#%d", testNum);
				ssd1306_WriteString(trans_str, Font_7x10, White);
				ssd1306_SetCursor (64,16);
				ssd1306_WriteString("Model:", Font_7x10, White);
				ssd1306_UpdateScreen(3,3);






				ssd1306_Fill(Black);
				ssd1306_UpdateScreen(4,5);

				ssd1306_SetCursor (3,28);
				ssd1306_ConvertVar(trans_str, 5, "%d", (int)adcAVR);
				ssd1306_WriteString(trans_str, Font_11x18, White);
				
				ssd1306_SetCursor (64,25);
				ssd1306_ConvertVar(trans_str, 15, "Min:%d", (int)limitsTest[testNum][MIN]);
				ssd1306_WriteString(trans_str, Font_7x10, White);
				ssd1306_SetCursor (64,34);
				ssd1306_ConvertVar(trans_str, 15, "Max:%d", (int)limitsTest[testNum][MAX]);
				ssd1306_WriteString(trans_str, Font_7x10, White);
				ssd1306_UpdateScreen(4,6);
			
				ssd1306_Fill(Black);	// turn off comparasion
				ssd1306_UpdateScreen(7,8);
				ssd1306_SetCursor (0,50);
				ssd1306_WriteString("TRUE,press button", Font_7x10, White);
				ssd1306_UpdateScreen(7,8);					
				testNum++; 
//				if (standartComparasion()) 
//				{ 
//					ssd1306_Fill(Black);
//					ssd1306_UpdateScreen(7,8);
//					ssd1306_SetCursor (0,50);
//					ssd1306_WriteString("TRUE,press button", Font_7x10, White);
//					ssd1306_UpdateScreen(7,8);				
//					testNum++; 
//				}
//				else 
//				{ 
//					ssd1306_Fill(Black);
//					ssd1306_UpdateScreen(7,8);
//					ssd1306_SetCursor (0,50);
//					ssd1306_WriteString("ERROR!RocketKAPUT", Font_7x10, White);
//					ssd1306_UpdateScreen(7,8);
//					return 0; 
//				}
			}
	}
	UART_Transmit(trans_str, 63,  "END OF ALL TESTS %d\n", 0, 0);
	cycleBlink();
	
//	return 0;
}
/*
void ADC_Calibration() {
	//-------------Calibration ADC offset----------------//
	// reference for >0...<3,3 Volts
	if (adcAVR[testNum]>=1685 && adcAVR[testNum]<=1980){
		adcAVR[testNum] += (ADC_OFFSET-30);
		return;
	}
	else if (adcAVR[testNum]>=1320 && adcAVR[testNum]<=1645){
		adcAVR[testNum] -= 7;
		return;
	}
	// reference for >3,3...<5 Volts
	else if (adcAVR[testNum]>=1980 && adcAVR[testNum]<=2150){
		adcAVR[testNum] += (ADC_OFFSET-30);
		return;
	}
	else if (adcAVR[testNum]>=1150 && adcAVR[testNum]<=1320){
		adcAVR[testNum] -= 17;
		return;
	}
	// reference for >5...<9 Volts
	else if (adcAVR[testNum]>2150 && adcAVR[testNum]<=2400){
		adcAVR[testNum] += (ADC_OFFSET-1);
		return;
	}
	else if (adcAVR[testNum]<1200 && adcAVR[testNum]>=780){
		adcAVR[testNum] -= (ADC_OFFSET-35);
		return;
	}
	// reference for >9...<15 Volts
	else if (adcAVR[testNum]>2400 && adcAVR[testNum]<=3500){
		adcAVR[testNum] += (ADC_OFFSET+15);
		return;
	}
//	else if (adcAVR[testNum]<780 && adcAVR[testNum]>=0){
	else if (adcAVR[testNum]<780){
		adcAVR[testNum] -= (ADC_OFFSET-12);
		return;
	}
	else{return;}
}*/
__attribute__((noreturn)) void cycleBlink(){
	GPIOA->BSRR = GPIO_BSRR_BR5;
	GPIOA->BSRR = GPIO_BSRR_BR6;
	GPIOA->BSRR = GPIO_BSRR_BR7;
	while(1){
		GPIOA->BSRR = GPIO_BSRR_BS5;
		delay_ms(100);
		GPIOA->BSRR = GPIO_BSRR_BR5;
		delay_ms(100);
		GPIOA->BSRR = GPIO_BSRR_BS6;
		delay_ms(100);
		GPIOA->BSRR = GPIO_BSRR_BR6;
		delay_ms(100);
		GPIOA->BSRR = GPIO_BSRR_BS7;
		delay_ms(100);
		GPIOA->BSRR = GPIO_BSRR_BR7;
		delay_ms(100);
	}
}
void ledBlinkTestCount(int numOfTest){
	switch (numOfTest)
	{
		case 0:
			GPIOA->BSRR = GPIO_BSRR_BR5;
			GPIOA->BSRR = GPIO_BSRR_BR6;
			GPIOA->BSRR = GPIO_BSRR_BR7;
			GPIOA->BSRR = GPIO_BSRR_BS5;
		break;
		case 1:
			GPIOA->BSRR = GPIO_BSRR_BR5;
			GPIOA->BSRR = GPIO_BSRR_BS6;
		break;
		case 2:
			GPIOA->BSRR = GPIO_BSRR_BS5;
		break;
		case 3:
			GPIOA->BSRR = GPIO_BSRR_BR5;
			GPIOA->BSRR = GPIO_BSRR_BR6;
			GPIOA->BSRR = GPIO_BSRR_BS7;
		break;
		case 4:
			GPIOA->BSRR = GPIO_BSRR_BS5;
		break;
		case 5:
			GPIOA->BSRR = GPIO_BSRR_BR5;
			GPIOA->BSRR = GPIO_BSRR_BS6;
		break;
		case 6:
			GPIOA->BSRR = GPIO_BSRR_BS5;
		break;
		case 7:
			GPIOA->BSRR = GPIO_BSRR_BR5;
			GPIOA->BSRR = GPIO_BSRR_BR7;
			GPIOA->BSRR = GPIO_BSRR_BR6;
		break;
	}
}

// end
