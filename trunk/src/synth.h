/*
Synthesis core (goes through itc for event processing, etc)

Copyright (C) 2005 Ben Anderson
Licensed under the Open Software License, version 2.1
*/
#ifndef SINOSPAN_SYNTH_H
#define SINOSPAN_SYNTH_H

struct Track;

namespace Synth {
bool Init(); bool Go(); bool Stop(); void Die();
// Dependency info:
// Out CANNOT be running when Synth changes stage.

#define SYNTH_MAX_TRACKS 32

// Tracks in the synth. 
static Track *tracks[SYNTH_MAX_TRACKS];
static unsigned short int trackCt = 0;

// Convenience function: Add a track.
void AddTrack();

// Convenience function: Remove track at this index.
void RemoveTrack(int idx);

// Immediately silence everything!
void R_Panic(void);

// Get the number of output channels in the synth.
inline unsigned short int outCt() const;

// Render a frame for the given amount of time after the last frame.
float R_Render(float time);
};

#endif
