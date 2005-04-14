/* Copyright (C) 2005 Ben Anderson
   Licensed under the Open Software License version 2.1 */
#include "oscfunc_sawdown.h"

float OscFunc_SawDown(float phase)
{
	return (1.0 - phase) * 2 - 1.0;
}
