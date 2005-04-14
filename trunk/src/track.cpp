/* Copyright (C) 2005 Ben Anderson
   Licensed under the Open Software License version 2.1 */
#include "track.h"

// NOPORT?
#include <stdio.h>	// printf()

void Track::R_AddCell(Cell *cel)
{
	if(numMembs >= TRACK_MAX_CELLS)
	{
		// DANGER: THIS SHOULD NOT HAPPEN, EVER!
		printf("ERROR: Tried to add a cell to a full track?\n");
		return;
	}

	membs[numMembs] = cel;
	numMembs++;
}

void Track::R_Render(float time)
{
	int i = 0;
	while(i < numMembs)
	{
		membs[i]->R_Render(time);
	}
}
