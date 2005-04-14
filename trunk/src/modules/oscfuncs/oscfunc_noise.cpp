/* Copyright (C) 2005 Ben Anderson
   Licensed under the Open Software License version 2.1 */
#include "oscfunc_noise.h"

// NOPORT?
#include <stdlib.h>	// rand(), RAND_MAX

float OscFunc_Noise(float phase)
{
	return rand() / (float) RAND_MAX * 2.0 - 1.0;
}
