/* Copyright (C) 2005 Ben Anderson
   Licensed under the Open Software License version 2.1 */
#include "synth.h"

// NOPORT
#include <stdio.h>	// printf()
#include <pthread.h>	// pthread_mutex_t, PTHREAD_MUTEX_INITIALIZER,
			// pthread_mutex_lock(), pthread_mutex_unlock()
#include <semaphore.h>	// sem_t, sem_init(), sem_destroy(), sem_post(),
			// sem_trywait(), sem_getvalue()

#include "itc.h"
#include "track.h"
#include "plug.h"
#include "ramdude.h"

using namespace Synth;

Track *Synth::tracks[SYNTH_MAX_TRACKS];
unsigned short int Synth::trackCt = 0;

sem_t pSynUp;

bool Synth::Init()
{
	sem_init(&pSynUp);
	return true;
}

bool Synth::Go()
{
	sem_post(&pSynUp);

	return true;
}

bool Synth::Stop()
{
	if(!sem_trywait(&pSynUp) )
		{ printf("ERROR: Failed to down SynUp semaphore!\n");
								return false; }

	return true;
}

void Synth::Die()
{
	sem_destroy(&pSynUp);
	return;
}

// No, I'm not going to combine Track, Cell, and part of Synth into a container
// class. GCC / GDB get confused as all hell as it is.
void Synth::AddTrack(Track *trk)
{
	if(trackCt >= SYNTH_MAX_TRACKS)
	{
		// DANGER: THIS SHOULD NOT HAPPEN, EVER!
		printf(
			"ERROR: Tried to add a track when full?\n"
		);
		return;
	}
	tracks[trackCt] = trk;
	trackCt++;
}

void Synth::RemoveTrack(Track *trk)
{
	unsigned int i = 0;
	while(tracks[i] != trk)
	{
		i++;
		if(i >= SYNTH_MAX_TRACKS) return;
	}
	
	trackCt--;
	while(i < trackCt)
	{
		tracks[i] = tracks[i+1];
		i++;
	}
	ITC_DEFER(callback_0, ITC::OWNER_RAMDUDE, trk, (void*) trk,
	{
		delete (Track*) trk;
	});
	RAMDude::Whack();
}

inline unsigned short int Synth::outCt()
{
	// TODO: Stereo (or more!) support
	return 1;
}

float Synth::R_Render(float time)
{
	int synUpVal;
	sem_getvalue(&SynUp, &synUpVal);
	if(synUpVal == 0) { return 0.0; } // Synth is disabled.
	
	float workBuf = 0.0;
	unsigned short int i = 0;
	while(i < trackCt)
	{
		tracks[i]->R_Render(time);
		unsigned short int j = 0;
		while(j < outCt() )
		{
			workBuf += tracks[i]->outs[j].read();
		}
		i++;
	}
	
	return workBuf;
}
