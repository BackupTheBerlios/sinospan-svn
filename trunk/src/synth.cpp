/* Licensed under the Open Software License version 2.1 */
#include "synth.h"

#include <stdio.h>	// printf()
#include <pthread.h>	// pthread_mutex_t, PTHREAD_MUTEX_INITIALIZER,
			// pthread_mutex_lock(), pthread_mutex_unlock()

#include "itc.h"
#include "track.h"

using namespace Synth;

#define MAX_NUM_TRACKS 32

static Track *allTracks[MAX_NUM_TRACKS];
static unsigned short int numTracks = 0;

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

void Synth::R_Panic()
{
	// TODO STUB
	printf("DBG: Synth: panicking!\n");
	return;
}

void Synth::R_AddTrack(Track *trk)
{
	if(numTracks >= MAX_NUM_TRACKS)
	{
		// DANGER: THIS SHOULD NOT HAPPEN, EVER!
		printf("ERROR: Tried to add another track when already at max?\n");
		return;
	}

	allTracks[numTracks] = trk;
	numTracks++;

	return;
}

float Synth::R_Render(float time)
{
	// Grab all pending events.
	ITC::R_GetEvents(ITC::OWNER_RENDER);

	if(pthread_mutex_trylock(&pSynUpLock) != 0)
	{
		printf("DBG: State change -- no data.\n");
		// During a state change. Don't be running yet.
		return 0.0;
	}
	else if(!pSynUp)
	{
		pthread_mutex_unlock(&pSynUpLock);
		// printf("DBG: Not running -- no data.\n");
		// We're not (supposed to be) running
		return 0.0;
	}
	pthread_mutex_unlock(&pSynUpLock);

	// XXX TODO DANGER STUB
	return 0.0;
}
