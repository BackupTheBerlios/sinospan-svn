/* Licensed under the Open Software License version 2.1 */
#include "oscfunc_noise.h"

#include <stdlib.h>	// rand(), RAND_MAX

float OscFunc_Noise(float phase)
{
	return rand() / (float) RAND_MAX * 2.0 - 1.0;
}
