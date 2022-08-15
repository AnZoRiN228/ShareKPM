#ifndef LIMITS_H
#define LIMITS_H

#define MIN 0
#define MAX 1

#include "main.h"

// -9.5 (9.36)		+10.5(10.4 on PS)		-3.9 (3.85 on PS)	 +1.0  	+7.5
// static double model [] = {2,6}; 

/*------------ZERO IS 1655 VOLTS. ALL------------*/
//  All ranges are shifted to zero
// this are new ranges +- 5mV
static double limitsTest [5][2] = {		{2633, 2427},
																			{1285, 565},
																			{2733, 2015},
																			{1760, 1532},
																			{1417, 879},
																												};
//static double limitsTest [5][2] = {		{3700, 3000},
//																			{3700, 3000},
//																			{1700, 1400},
//																			{3700, 3000},
//																			{1700, 1400},
//																												};
#endif
