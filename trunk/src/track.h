/*
A row of cells. Each cell takes data from the previous cell (if any) and makes
data for the next cell (or output if there isn't a next cell).

Copyright (C) 2005 Ben Anderson
Licensed under the Open Software License version 2.1
*/
#ifndef SINOSPAN_TRACK_H
#define SINOSPAN_TRACK_H

struct Cell;
struct Plug;

#define TRACK_MAX_CELLS 256

struct Track
{
	Track(); ~Track();
	// Cells in this track.
	Cell *membs[TRACK_MAX_CELLS];
	unsigned short int membCt;
	// Outputs of this track. Always Synth::outCt() number of elements.
	Plug *outs[];
	// Add another empty cell to this track. Returns a pointer to the new 
	// cell.
	Cell *AddCell();

	// Remove the cell at this index from this track.
	void RemoveCell(unsigned short int idx);

	// Render for this much time since the last pass.
	void R_Render(float time);
};

#endif
