/* Licensed under the Open Software License version 2.1 */
#include "ramdude.h"

#include <pthread.h>	// pthread_create
#include <semaphore.h>	// sem_t, sem_init(), sem_destroy(), sem_wait(),
			// sem_post()

#include "itc.h"

using namespace RAMDude;

// The thread itself.
static pthread_t pRAMDudeThread;
bool pRDThreadUp;

// Used to sleep.
static sem_t pSleepSem;

// Used to signal the thread to exit.
static sem_t pQuitSem;

bool RAMDude::Init()
{
	sem_init(&pSleepSem, 0, 0);
	sem_init(&pQuitSem, 0, 0);

	return true;
}

static void *pRAMDudeMain(void *nul);

bool RAMDude::Go()
{
	if(pRDThreadUp) { return false; }
	// Reset the quit semaphore.
	while(sem_trywait(&pQuitSem) == 0) {}
	pthread_create(&pRAMDudeThread, 0x0, &pRAMDudeMain, 0x0);
	
	return true;
}

bool RAMDude::Stop()
{
	if(!pRDThreadUp) { return false; }
	sem_post(&pQuitSem);
	Whack();
	// Note that TrashDude may continue working for quite some time after
	// the quit semaphore is posted and we whack the thread. This is
	// intentional -- there may be a few other things it needs to dealloc!
	pthread_join(pRAMDudeThread, 0x0);

	return true;
}

void RAMDude::Die()
{
	sem_destroy(&pSleepSem);
	sem_destroy(&pQuitSem);
}

void RAMDude::Whack()
{
	int sleepSemVal;
	sem_getvalue(&pSleepSem, &sleepSemVal);
	if(sleepSemVal == 0)
	{
		sem_post(&pSleepSem);
	}
}

static void *pRAMDudeMain(void *nul)
{
	while(sem_trywait(&pQuitSem) )
	{
		sem_wait(&pSleepSem);
		ITC::R_GetEvents(ITC::OWNER_RAMDUDE);
	}

	return 0x0;
}
