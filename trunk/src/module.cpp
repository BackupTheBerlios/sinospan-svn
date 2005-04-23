/* Copyright (C) 2005 Ben Anderson
   Licensed under the Open Software License version 2.1 */
#include "module.h"

#include "jack.h"
#include "plug.h"

Module::Module(): cel(0x0) {}

void Module::allocPorts()
{
	outputs = new Jack[outCt()];
	 inputs = new Plug[ inCt()];
}

void Module::deallocPorts()
{
	delete[]  inputs;
	delete[] outputs;
}

Module::~Module() {}
unsigned short int Module::inCt() const { return 0; }
unsigned short int Module::outCt() const { return 0; }
void Module::R_Render(float time) {}
