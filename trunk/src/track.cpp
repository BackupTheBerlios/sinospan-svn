/* Copyright (C) 2005 Ben Anderson
   Licensed under the Open Software License version 2.1 */
#include "track.h"

// NOPORT?
#include <stdio.h>	// printf()

#include "synth.h"
#include "itc.h"
#include "cell.h"
#include "ramdude.h"
#include "plug.h"

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
void Track::AddCell(Cell *cel)
{
	if(membCt >= TRACK_MAX_CELLS)
	{
		// DANGER: THIS SHOULD NOT HAPPEN, EVER!
		printf(
			"ERROR: Tried to add a cell to a full track?\n"
		);
		return;
	}
	membs[membCt] = cel;
	membCt++;
}

void Track::RemoveCell(Cell *cel)
{
	unsigned int i = 0;
	while(membs[i] != cel)
	{
		i++;
		if(i >= TRACK_MAX_CELLS) return;
	}
	
	membCt--;
	while(i < membCt)
	{
		membs[i] = membs[i+1];
		i++;
	}
	ITC_DEFER(callback_0, ITC::OWNER_RAMDUDE, cel, (void*) cel,
	{
		delete (Cell*) cel;
	});
	RAMDude::Whack();
}

void Track::R_Render(float time)
{
	unsigned short int i = 0;
	while(i < membCt)
	{
		membs[i]->R_Render(time);
	}
}

