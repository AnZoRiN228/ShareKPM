#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f10x.h"                  // Device header
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "initDevice.h"
#include "limits.h"
#include "ssd1306.h"

#define MEASURE_NUM 100
#define ADC_OFFSET 77
#define NUM_OF_TESTS 10

static char trans_str[64] = {0,};
static _Bool flagDMA;
static unsigned int usartLen;
static _Bool butt;
extern uint16_t buff[MEASURE_NUM];
extern uint32_t adcAVR;
extern int testNum;
//extern uint8_t SSD1306_Buffer[SSD1306_WIDTH * SSD1306_HEIGHT / 8];

void DMA1_Channel1_IRQHandler(void);
void UART_Transmit(char * arr, unsigned int size, const char *arrStr, int avarage, uint16_t TIME_OUT);
__attribute__((noreturn)) void cycleBlink(void);
void ledBlinkTestCount(int numOfTest);
void coutTerminal(char trans_str[]);
void ADC_GetAverageValue(void);
void ADC_AvrValueCalibration(void);
_Bool standartComparasion(void);

#ifdef __cplusplus
}
#endif

#endif /*__MAIN_H */
