/*
A row of cells. Each cell takes data from the previous cell (if any) and makes
data for the next cell (or output if there isn't a next cell).

Licensed under the Open Software License version 2.1
*/
#ifndef SINOSPAN_TRACK_H
#define SINOSPAN_TRACK_H

#include "cell.h"

#define MAX_CELLS_PER_TRACK 256

class Track
{
	Cell *membs[MAX_CELLS_PER_TRACK];
	unsigned short int numMembs;

	// Add another cell to this track.
	void R_AddCell(Cell *cel);

	// Render for this much time since the last pass.
	void R_Render(float time);
};

#endif
