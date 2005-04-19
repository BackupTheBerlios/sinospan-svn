/* Copyright (C) 2005 Ben Anderson
   Licensed under the Open Software License version 2.1 */
#include "track.h"

// NOPORT?
#include <stdio.h>	// printf()

#include "itc.h"
#include "cell.h"
#include "ramdude.h"

Track::Track()
{
	outs = new Plug[Synth::outCt()];
}

Track::~Track()
{
	delete[] outs;
}

// No, I'm not going to combine Track, Cell, and part of Synth into a container
// class. GCC gets confused enough as it is, and the debugging symbols will
// already quickly give you carpal tunnel.
void Track::AddCell()
{
	struct funcdata { Track *self; Cell *cel; };
	funcdata *dt = new funcdata;
	Cell *cel = new Cell;
	dt->cel = cel;
	dt->self = this;
	ITC_DEFER(callback_0, ITC::OWNER_RENDER, pdt, (void*) dt,
	{
		funcdata *dt = (funcdata*) pdt;
		if(dt->self->membCt >= TRACK_MAX_CELLS)
		{
			// DANGER: THIS SHOULD NOT HAPPEN, EVER!
			printf(
				"ERROR: Tried to add a cell to a full track?\n"
			);
			return;
		}

		dt->self->membs[dt->self->membCt] = dt->cel;
		dt->self->membCt++;
		ITC_DEFER(callback_1, ITC::OWNER_RAMDUDE, pdt, (void*) dt,
		{
			funcdata *dt = (funcdata*) pdt;
			delete dt;
		});
		RAMDude::Whack();
	});
	
	return cel;
}

void Track::RemoveCell(unsigned short int idx)
{
	struct funcdata { Track *self; unsigned short int idx; Cell *cel; };
	funcdata *dt = new funcdata;
	dt->self = this;
	dt->idx = idx;
	ITC_DEFER(callback_0, ITC::OWNER_RENDER, pdt, (void*) dt,
	{
		funcdata *dt = (funcdata*) pdt;
		dt->cel = dt->self->membs[dt->idx];
		dt->self->membCt--;

		while(dt->idx < dt->self->membCt)
		{
			dt->self->membs[dt->idx] = dt->self->membs[dt->idx+1];
			dt->idx++;
		}
		ITC_DEFER(callback_1, ITC::OWNER_RAMDUDE, pdt, (void*) dt,
		{
			funcdata *dt = (funcdata*) pdt;
			delete dt->cel;
			delete dt;
		});
		RAMDude::Whack();
	});
}

void Track::R_Render(float time)
{
	unsigned short int i = 0;
	while(i < membCt)
	{
		membs[i]->R_Render(time);
	}
}

