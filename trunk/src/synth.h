/*
Synthesis core (goes through itc for event processing, etc)

Licensed under the Open Software License, version 2.1
*/
#ifndef SINOSPAN_SYNTH_H
#define SINOSPAN_SYNTH_H

#include "track.h"
#include "module.h"

namespace Synth {
bool Init(); bool Go(); bool Stop(); void Die();
// Dependency info:
// Out CANNOT be running when Synth changes stage.

// Immediately silence everything!
void R_Panic(void);

// Add a track
void R_AddTrack(Track *trk);

// Connect output to this port.
void R_ConnectOut(Module::Port *port);
	
// Render a frame for the given amount of time after the last frame.
float R_Render(float time);
};

#endif
