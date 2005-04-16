/* Copyright (C) 2005 Ben Anderson
   Licensed under the Open Software License version 2.1 */
#include "cell.h"

// NOPORT?
#include <stdio.h>	// printf()

#include "itc.h"
#include "module.h"
#include "ramdude.h"

// No, I'm not going to combine Track, Cell, and part of Synth into a container
// class. GCC / GDB get confused as all hell as it is.
void Cell::AddModule(Module *mdl)
{
	struct funcdata { Cell *self; Module *mdl; };
	funcdata *dt = new funcdata;
	dt->self = this; dt->mdl = mdl;
	ITC_DEFER(callback_0, ITC::OWNER_RENDER, pdt, (void*) dt,
	{
		funcdata *dt = (funcdata*) pdt;
		if(dt->self->membCt >= CELL_MAX_MODULES)
		{
			// DANGER: THIS SHOULD NOT HAPPEN, EVER!
			printf(
			       "ERROR: Tried to add a module to a full cell?\n"
			);
			return;
		}

		dt->self->membs[dt->self->membCt] = dt->mdl;
		dt->self->membCt++;
		ITC_DEFER(callback_1, ITC::OWNER_RAMDUDE, pdt, (void*) dt,
		{
			funcdata *dt = (funcdata*) pdt;
			delete dt;
		});
		RAMDude::Whack();
	});
}

void Cell::RemoveModule(unsigned short int idx)
{
	struct funcdata { Cell *self; unsigned short int idx; Module *mdl; };
	funcdata *dt = new funcdata;
	dt->self = this;
	dt->idx = idx;
	ITC_DEFER(callback_0, ITC::OWNER_RENDER, pdt, (void*) dt,
	{
		funcdata *dt = (funcdata*) pdt;
		dt->mdl = dt->self->membs[dt->idx];
		dt->self->membCt--;

		while(dt->idx < dt->self->membCt)
		{
			dt->self->membs[dt->idx] = dt->self->membs[dt->idx+1];
			dt->idx++;
		}
		ITC_DEFER(callback_1, ITC::OWNER_RAMDUDE, pdt, (void*) dt,
		{
			funcdata *dt = (funcdata*) pdt;
			delete dt->mdl;
			delete dt;
		});
		RAMDude::Whack();
	});
}

void Cell::R_Render(float time)
{
	unsigned short int i = 0;
	while(i < membCt)
	{
		membs[i]->R_Render(time);
	}
}
