/* Licensed under the Open Software License version 2.1 */
#include "oscfunc_sine.h"

#include <math.h>	// sinf(), M_PI

// 360 degrees in radians.
static const float CIRCLE_RAD = M_PI * 2;

float OscFunc_Sine(float phase)
{
	return sinf(phase * CIRCLE_RAD);
}
