/*
One column of modules.

Licensed under the Open Software License version 2.1
*/
#ifndef SINOSPAN_CELL_H
#define SINOSPAN_CELL_H

#include "module.h"

#define MAX_MODULES_PER_CELL 64

struct Cell
{
	Module *membs[MAX_MODULES_PER_CELL];
	unsigned short int numMembs;

	// Add another module to this cell.
	void R_AddModule(Module *mdl);
	// Add another module to this cell, ITC-wrapped version.
	void AddModule(Module *mdl) const;

	// Render for this much time since the last pass.
	void R_Render(float time);
};

#endif
