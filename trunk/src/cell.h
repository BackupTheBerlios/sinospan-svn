/*
One column of modules.

Copyright (C) 2005 Ben Anderson
Licensed under the Open Software License version 2.1
*/
#ifndef SINOSPAN_CELL_H
#define SINOSPAN_CELL_H

struct Module;

#define CELL_MAX_MODULES 64

struct Cell
{
	// Modules in this cell.
	Module *membs[CELL_MAX_MODULES];
	unsigned short int membCt;

	// Add another module to this cell.
	void R_AddModule(Module *md);

	// Remove module at this index from this cell.
	void R_RemoveModule(int id);

	// Render for this much time since the last pass.
	void R_Render(float time);
};

#endif
