/*
Output driver and rendering thread manager (as our rendering thread might be
externally managed by e.g. JACK, CA).

This is used by render.

Copyright (C) 2005 Ben Anderson
Licensed under the Open Software License version 2.1
*/
#ifndef SINOSPAN_OUT_H
#define SINOSPAN_OUT_H

namespace Out {
bool Init(); bool Go(); bool Stop(); void Die();

// Out calls Synth::R_Render in the rendering thread repeatedly, and that's how
// rendering happens.

// Driver interface. Pretty basic, really -- the driver is expected to start
// and manage the worker thread, so very little has to go through this
// interface.
class Driver {
public:
	virtual     ~Driver();
	virtual bool Go();
	virtual bool Stop();
};

};

#endif
