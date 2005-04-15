/* Copyright (C) 2005 Ben Anderson
   Licensed under the Open Software License version 2.1 */
#include "module.h"

// NOPORT?
#include <stdlib.h>	// malloc(), free()

void Module::allocPorts()
{
	if(inCt() > 0)
	{
		// Allocate
		inputs = (Port***) malloc(sizeof(Port*) * inCt() );

		// Initialize
		int i = 0;
		while(i < inCt() )
		{
			*inputs[i] = 0x0;
			i++;
		}
	}
	if(outCt() > 0)
	{
		// Allocate
		outputs = (Port**) malloc(sizeof(Port) * outCt() );

		// Initialize
		int i = 0;
		while(i < outCt() )
		{
			*outputs[i] = 0.0;
			i++;
		}
	}
}

void Module::deallocPorts()
{
	if(inCt() > 0)
	{
		free(inputs);
	}
	if(outCt() > 0)
	{
		free(outputs);
	}
}

Module::~Module() {}
unsigned short int Module::inCt() const { return 0; }
unsigned short int Module::outCt() const { return 0; }
void Module::R_Render(float time) {}
