/* Copyright (C) 2005 Ben Anderson
   Licensed under the Open Software License version 2.1 */
#include "cell.h"

// NOPORT?
#include <stdio.h>	// printf()

#include "itc.h"
#include "module.h"
#include "ramdude.h"

Cell::Cell(): membCt(0), trk(0x0) {}

Cell::~Cell()
{
	// Deleting Tracks/Cells is supposed to be recursive.
	unsigned int i = 0;
	while(i < membCt)
	{
		delete membs[i];
		i++;
	}
}

// No, I'm not going to combine Track, Cell, and part of Synth into a container
// class. GCC / GDB get confused as all hell as it is.
void Cell::AddModule(Module *mdl)
{
	if(membCt >= CELL_MAX_MODULES)
	{
		// DANGER: THIS SHOULD NOT HAPPEN, EVER!
		printf(
			"ERROR: Tried to add a module to a full cell?\n"
		);
		return;
	}
	membs[membCt] = mdl;
	membCt++;
	
	mdl->cel = this;
}

void Cell::RemoveModule(Module *mdl)
{
	unsigned int i = 0;
	while(membs[i] != mdl)
	{
		i++;
		if(i >= CELL_MAX_MODULES) return;
	}
	
	membCt--;
	while(i < membCt)
	{
		membs[i] = membs[i+1];
		i++;
	}
	ITC_DEFER(callback_0, ITC::OWNER_RAMDUDE, mdl, (void*) mdl,
	{
		delete (Module*) mdl;
	});
	RAMDude::Whack();
}

void Cell::R_Render(float time)
{
	unsigned short int i = 0;
	while(i < membCt)
	{
		membs[i]->R_Render(time);
		i++;
	}
}
