/* Licensed under the Open Software License version 2.1 */
#include "itc.h"

#include <stdio.h>	// printf()
#include <unistd.h>	// usleep()
#include <semaphore.h>	// sem_t, sem_init(), sem_destroy(), sem_getvalue(),
			// sem_post(), sem_trywait()

using namespace ITC;

// Total event slots to have.
#define NUM_EVENT_SLOTS 32

// Struct for event slots.
struct ITC_Event {
	// Primary semaphore; used to protect target state until everyone's
	// done reading it.
	sem_t sem1;
	// Secondary semaphore; 1 if it's a pending event, 0 otherwise.
	sem_t sem2;
	// Target.
	EvFlag target;
	// Function to be called
	void (*func)(void*);
	// Data for the function
	void *data;
	Event() { sem_init(&sem1, 0, 0); sem_init(&sem2, 0, 0); }
	~Event() { sem_destroy(&sem2); sem_destroy(&sem1); }
	int Grab(EvFlag rqTarget)
	{
		int flag;
		sem_getvalue(&sem1, flag);
		if(target != rqTarget)
		{
			int flag2;
			sem_getvalue(&sem2, flag2);
			if(flag != 0 || flag2 != 0) { return -1; }
			else { target = rqTarget; }
		}
		
		sem_post(&sem1);
		return flag;	// Returns number of threads watching this
				// before you.
	}
	void Ungrab() { sem_trywait(&sem1); }
};

// Event array.
static ITC_Event *pEvents[NUM_EVENT_SLOTS];

// Per-thread semaphores. Always set to the number of events pending for that
// thread.
static sem_t pThreadSems[NUM_ITC_OWNERS];
// Consume and produce indexes for each thread.

bool ITC::Init()
{
	int i = 0;
	while(i < NUM_EVENT_SLOTS)
	{
		pEvents[i] = new ITC_Event;
		i++;
	}
	i = 0;
	while(i < NUM_ITC_OWNERS)
	{
		sem_init(&pThreadSems, 0, 0);
		i++;
	}

	return true;
}

bool ITC::Go()
{
	return true;
}

bool ITC::Stop()
{
	return true;
}

void ITC::Die()
{
	int i;
	
	i = 0;
	while(i < NUM_EVENT_SLOTS)
	{
		delete pEvents[i];
		i++;
	}

	return;
}

void ITC::R_GetEvents(EvFlag owner)
{
	// For each time we successfully decrement our semaphore, move forward to the next grabbable slot that has a pending event.
	int flag;
	
	while(sem_trywait(&pThreadSems[owner]) == 0)
	{
		do {
			sem_getvalue(&pEvents[slot]->sem, &flag);
				// slot incremented here
		} while(flag != 2 || pEvents[slot]->target != owner);

		slot++;
		if(slot == NUM_EVENT_SLOTS) { slot = 0; }

		}
	}
}

void ITC::SendEvent(EvFlag target, void (*func)(void*), void *data)
{
	// A slot is empty if a) the pending event semaphore isn't up, and b)
	// if you successfully grab it and no other thread before you had it
	// grabbed when you grabbed it.
	// If the pending event semaphore isn't up and you successfully grab
	// the slot, but you weren't the first one there, the other thread is
	// either writing an event to this slot, or will be very shortly.
	// (NOTE: it could be another thread checking the slot. If that's the
	// case, that thread is a writing thread, and likely will begin writing
	// to it.)
	// If the pending event semaphore IS up, it's a pending event. If you
	// successfully grab the event, note the timestamp. If it's _smaller_
	// than the timestamp of an event earlier in the list, then this is the
	// consume point; you cannot go past this point when looking for a slot
	// to put your event.
	//
	// So: find the consume point (beware, it may be the only grabbable
	// non-empty slot, and there might not be one at all, in which case it
	// doesn't matter where you stick your event), then find the first
	// grabbable not-empty slot before it that you get to first. (It may
	// very well be the consume point again, so watch out.) Then walk back
	// towards the
	// consume point until you find an empty slot, and use that. If you get
	// all the way back to the consume point, there's nowhere for your
	// event to go, sorry. Once/if you've found an open slot, grab the slot
	// if you haven't done so already, stick in your event, and post to the
	// target thread's semaphore. Now leave it grabbed -- the reading
	// thread will un-grab it. You're done!
	//
	// It's not O(n) time, but it's close, and it's 100% blocking-free as
	// well. (It still has the caveat that there might not be an open slot,
	// however...)
	static int slot = 0;
	int flag;

	pthread_mutex_lock(&pWriteLock);

	sem_getvalue(&pEventFlags[slot], &flag);
	while(flag != 0)
	{
		// XXX: We might want a condition variable here. But I'm not
		// certain that would be RT-safe...
		printf("ERROR: render: All event slots are full!\n");
		usleep(100);	// Be careful not to infringe on the rendering
				// thread
		sem_getvalue(&pEventFlags[slot], &flag);
	}
	
	pEvents[slot] = event;
	sem_post(&pEventFlags[slot]);

	slot++;
	if(slot == NUM_EVENT_SLOTS) { slot = 0; }
	
	pthread_mutex_unlock(&pWriteLock);
}
