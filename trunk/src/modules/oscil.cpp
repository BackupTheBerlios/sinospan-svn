/* Licensed under the Open Software License version 2.1 */
#include "oscil.h"

#include <math.h>	// floor()

Oscil::Oscil(OscFunc *f, float p, float r): func(f), phase(p), freq(r)
{
	allocPorts();
}

Oscil::~Oscil()
{
	deallocPorts();
}

unsigned short int Oscil::inCt() const { return 0; }
unsigned short int Oscil::outCt() const { return 1; }

void Oscil::R_Render(float time)
{
	phase += freq * time;
	
	// Phase is always between 0 and 1; if it's not, put it back there.
	// NOTE: I do things this way so as to always take constant time, which
	// is a big plus in realtime situations.
	phase -= floorf(phase / 1.0);

	*outputs[0] = func(phase);
}
