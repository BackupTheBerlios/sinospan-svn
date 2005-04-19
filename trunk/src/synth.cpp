/* Copyright (C) 2005 Ben Anderson
   Licensed under the Open Software License version 2.1 */
#include "synth.h"

// NOPORT
#include <stdio.h>	// printf()
#include <pthread.h>	// pthread_mutex_t, PTHREAD_MUTEX_INITIALIZER,
			// pthread_mutex_lock(), pthread_mutex_unlock()

#include "itc.h"
#include "track.h"
#include "plug.h"
#include "ramdude.h"

using namespace Synth;

Track *Synth::tracks[SYNTH_MAX_TRACKS];
unsigned short int Synth::trackCt = 0;

// XXX: Wouldn't a semaphore a la RAMDude be more efficient here?
static bool pSynUp = false;
static pthread_mutex_t pSynUpLock = PTHREAD_MUTEX_INITIALIZER;

bool Synth::Init()
{
	return true;
}

bool Synth::Go()
{
	pthread_mutex_lock(&pSynUpLock);
	pSynUp = true;
	pthread_mutex_unlock(&pSynUpLock);

	return true;
}

bool Synth::Stop()
{
	pthread_mutex_lock(&pSynUpLock);
	pSynUp = false;
	pthread_mutex_unlock(&pSynUpLock);

	return true;
}

void Synth::Die()
{
	Stop();
	return;
}

// No, I'm not going to combine Track, Cell, and part of Synth into a container
// class. GCC / GDB get confused as all hell as it is.
void Synth::AddTrack()
{
	Track *trk = new Track;
	ITC_DEFER(callback_0, ITC::OWNER_RENDER, trk, (void*) trk,
	{
		if(trackCt >= SYNTH_MAX_TRACKS)
		{
			// DANGER: THIS SHOULD NOT HAPPEN, EVER!
			printf(
		     "ERROR: Tried to add another track when already at max?\n"
			);
			return;
		}

		tracks[trackCt] = (Track*) trk;
		trackCt++;
	});

	return trk;
}

void Synth::RemoveTrack(unsigned short int idx)
{
	struct funcdata { unsigned short int idx; Track *trk; };
	funcdata *dt = new funcdata;
	dt->idx = idx;
	ITC_DEFER(callback_0, ITC::OWNER_RENDER, pdt, (void*) dt,
	{
		funcdata *dt = (funcdata*) pdt;
		dt->trk = tracks[dt->idx];
		trackCt--;

		while(dt->idx < trackCt)
		{
			tracks[dt->idx] = tracks[dt->idx+1];
			dt->idx++;
		}
		ITC_DEFER(callback_1, ITC::OWNER_RAMDUDE, pdt, (void*) dt,
		{
			funcdata *dt = (funcdata*) pdt;
			delete dt->trk;
			delete dt;
		});
		RAMDude::Whack();
	});
}

inline unsigned short int Synth::outCt()
{
	// TODO: Stereo (or more!) support
	return 1;
}

float Synth::R_Render(float time)
{
	// Grab all pending events.
	ITC::R_GetEvents(ITC::OWNER_RENDER);

	if(pthread_mutex_trylock(&pSynUpLock) != 0)
	{
		// During a state change. Don't be running yet.
		return 0.0;
	}
	else if(!pSynUp)
	{
		pthread_mutex_unlock(&pSynUpLock);
		// We're not (supposed to be) running
		return 0.0;
	}
	pthread_mutex_unlock(&pSynUpLock);

	float workBuf = 0.0;
	unsigned short int i = 0;
	while(i < trackCt)
	{
		tracks[i]->R_Render(time);
		unsigned short int j = 0;
		while(j < outCt() )
		{
			workBuf += tracks[i]->outs[j]->read();
		}
		i++;
	}

	return workBuf;
}
