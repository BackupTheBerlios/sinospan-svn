/* Licensed under the Open Software License version 2.1 */
#include "module.h"

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
