/* Licensed under the Open Software License version 2.1 */
#include "oscfunc_triangle.h"

float OscFunc_Triangle(float phase)
{
	if(phase < 0.25)
	{
		return phase * 4;
	}
	else if(phase >= 0.75)
	{
		return ( (phase - 0.75) * 4) - 1.0;
	}
	else // if(phase < 0.75 && phase >= 0.25)
	{
		return ( (1.0 - phase) - 0.5) * 4.0;
	}
}
