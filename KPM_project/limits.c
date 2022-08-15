#include "limits.h"

_Bool standartComparasion(){
	if (adcAVR > limitsTest[testNum][MIN])
	{
//		UART_Transmit(trans_str, 63,  "ERROR: Minimum marriage %d\n", 0, 0);
		return 0;
	}
	else if (adcAVR < limitsTest[testNum][MAX])
	{
//		UART_Transmit(trans_str, 63,  "ERROR: Marriage to the max %d\n", 0, 0);
		return 0;
	}
//			else { testNum++; 
//						 return 1;}
	return 1;
}
