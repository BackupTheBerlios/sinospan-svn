/* Licensed under the Open Software License version 2.1 */
#include "cell.h"

#include <stdio.h>	// printf()

void Cell::R_AddModule(Module *mod)
{
	if(numMembs >= MAX_MODULES_PER_CELL)
	{
		// DANGER: THIS SHOULD NOT HAPPEN, EVER!
		printf("ERROR: Tried to add a module to a full cell?\n");
		return;
	}

	membs[numMembs] = mod;
	numMembs++;
}

static void DeferAddModule(Module *mdl)
{
	
}

void Cell::AddModule(Module *mdl) const
{
	
}

void Cell::R_Render(float time)
{
	int i = 0;
	while(i < numMembs)
	{
		membs[i]->R_Render(time);
	}
}
