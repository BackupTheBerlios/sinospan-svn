/* Licensed under the Open Software License version 2.1 */
#include "oscfunc_pulse.h"

float OscFunc_Pulse(float phase)
{
	if(phase > 0.5)
	{
		return 1.0;
	}
	else // if (phase =< 0.5)
	{
		return -1.0;
	}
}
