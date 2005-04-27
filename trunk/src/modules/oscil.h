/*
An oscillator. Duh?

Copyright (C) 2005 Ben Anderson
Licensed under the Open Software License version 2.1
*/
#ifndef SINOSPAN_OSCIL_H
#define SINOSPAN_OSCIL_H

#include "module.h"
#include "oscfunc.h"

struct Oscil : public Module
{
	// Osc function, phase, and frequency
	Oscil(OscFunc*, float, float);
	// Or just the function.
	Oscil(OscFunc*);
	~Oscil();

	unsigned short int inCt() const;
	unsigned short int outCt() const;
	OscFunc *func;
	float phase;
	float freq; // In oscillations per microsecond

	// Render for this much time after the last frame.
	void R_Render(float time);
};

#endif
