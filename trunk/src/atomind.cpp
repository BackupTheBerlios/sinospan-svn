/* Copyright (C) 2005 Ben Anderson
   Licensed under the Open Software License version 2.1 */
#include "atomind.h"

// NOPORT
#include <semaphore.h>	// sem_init(), sem_destroy(), sem_trywait(), sem_post()

AtomicIndex::AtomicIndex(int _max): max(_max)
{
	sem_init(&sem1, 0, 0);
	sem_init(&sem2, 0, 0);
}

AtomicIndex::~AtomicIndex()
{
	sem_destroy(&sem2);
	sem_destroy(&sem1);
}

// The first semaphore is incremented up to our internal maximum, then the
// second semaphore is incemented up to our internal maximum. Then the first is
// decremented back to 0, and finally the second is decremented back to 0.

void AtomicIndex::Inc()
{
	int vSem1, vSem2;
	pGetSemVals(vSem1, vSem2);
	if(vSem1 < max && vSem2 == 0)
	{
		// We're incrementing the first semaphore.
		sem_post(&sem1);
	}
	else if(vSem1 == max && vSem2 < max)
	{
		// We're incrementing the second semaphore.
		sem_post(&sem2);
	}
	else if(vSem1 > 0 && vSem2 == max)
	{
		// We're decrementing the first semaphore.
		sem_trywait(&sem1);
	}
	else // if(vSem1 == 0 && vSem2 > 0)
	{
		// We're decrementing the second semaphore.
		sem_trywait(&sem2);
	}

	return;
}

int AtomicIndex::GetValue()
{
	int vSem1, vSem2;
	pGetSemVals(vSem1, vSem2);
	if(vSem1 < max && vSem2 == 0)
	{
		// We're incrementing the first semaphore.
		return vSem1;
	}
	else if(vSem1 == max && vSem2 < max)
	{
		// We're incrementing the second semaphore.
		return vSem2;
	}
	else if(vSem1 > 0 && vSem2 == max)
	{
		// We're decrementing the first semaphore.
		return max - vSem1;
	}
	else // if(vSem1 == 0 && vSem2 > 0)
	{
		// We're decrementing the second semaphore.
		return max - vSem2;
	}
}

inline void AtomicIndex::pGetSemVals(int &val1, int &val2)
{
	sem_getvalue(&sem1, &val1);
	sem_getvalue(&sem2, &val2);
}
