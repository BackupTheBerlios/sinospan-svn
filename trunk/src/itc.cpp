/* Licensed under the Open Software License version 2.1 */
#include "itc.h"

#include <stdio.h>	// printf()
#include <unistd.h>	// usleep()
#include <pthread.h>	// pthread_rwlock_t, pthread_rwlock_init(),
			// pthread_rwlock_tryrdlock(),
			// pthread_rwlock_trywrlock(), pthread_rwlock_unlock(),
			// pthread_rwlock_destroy()
#include <semaphore.h>	// sem_t, sem_init(), sem_destroy(), sem_getvalue(),
			// sem_post(), sem_trywait()

#include "atomind.h"

using namespace ITC;

// Total event slots to have.
#define NUM_EVENT_SLOTS 32

// Struct for event slots.
struct ITC_Event {
	// Pending event semaphore; 1 if it's a completely filled pending
	// event, 0 otherwise.
	sem_t pending;
	// Target thread.
	EvFlag target;
	pthread_rwlock_t targetLock;
	// Function to be called
	void (*func)(void*);
	// Data for the function
	void *data;

	ITC_Event() { pthread_rwlock_init(&targetLock, NULL);
						sem_init(&pending, 0, 0); }
	~ITC_Event() { sem_destroy(&pending);
					pthread_rwlock_destroy(&targetLock); }

};

// Event array.
static ITC_Event *pEvents[NUM_EVENT_SLOTS];

// Consume AtomicIndexes for each thread.
static AtomicIndex *pConsIdxs[NUM_ITC_OWNERS];

// Global produce AtomicIndex.
static AtomicIndex *pProdIdx;

bool ITC::Init()
{
	// DANGER: During ITC initialization, no threads are supposed to be
	// running except the main thread! This is the primary reason why it's
	// initted first.
	int i = 0;
	while(i < NUM_EVENT_SLOTS)
	{
		pEvents[i] = new ITC_Event;
		i++;
	}
	i = 0;
	while(i < NUM_ITC_OWNERS)
	{
		pConsIdxs[i] = new AtomicIndex(NUM_EVENT_SLOTS);
		i++;
	}
	pProdIdx = new AtomicIndex(NUM_EVENT_SLOTS);

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
	delete pProdIdx;

	int i = 0;
	while(i < NUM_ITC_OWNERS)
	{
		delete pConsIdxs[i];
		i++;
	}
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
	// While we aren't at the production point:
	while(pConsIdxs[owner]->GetValue() != pProdIdx->GetValue() )
	{
		// What slot are we on?
		int consIdx = pConsIdxs[owner]->GetValue();
		// Is this a pending event?
		int flag;
		sem_getvalue(&pEvents[consIdx]->pending, &flag);
		if(flag == 1)
		{
			// It is! Is it for us?
			if(pthread_rwlock_tryrdlock(&pEvents[consIdx]->targetLock) == 0)
			{
				if(pEvents[consIdx]->target == owner)
				{
					// It is! Send it.
					pEvents[consIdx]->func(pEvents[consIdx]->data);
					sem_trywait(&pEvents[consIdx]->pending);
				}
				pthread_rwlock_unlock(&pEvents[consIdx]->targetLock);
			}
		}
		// Step forward.
		pConsIdxs[owner]->Inc();
	}
}

bool ITC::R_SendEvent(EvFlag target, void (*func)(void*), void *data)
{
	int slot = pProdIdx->GetValue();
	int flag;
	sem_getvalue(&pEvents[slot]->pending, &flag);
	while(pthread_rwlock_trywrlock(&pEvents[slot]->targetLock) != 0 || flag == 1)
	{
		slot++;
		if(slot >= NUM_EVENT_SLOTS) { slot = 0; }
		if(slot == pConsIdxs[target]->GetValue() && slot != pProdIdx->GetValue() )
		{
			// We're at the consumption point for the target! There's no room!
			return false;
		}
		sem_getvalue(&pEvents[slot]->pending, &flag);
	}

	pEvents[slot]->target = target;
	pEvents[slot]->func = func;
	pEvents[slot]->data = data;
	sem_post(&pEvents[slot]->pending);

	pthread_rwlock_unlock(&pEvents[slot]->targetLock);

	pProdIdx->Inc();

	return true;
}
