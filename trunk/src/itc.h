/*
Inter-thread communication engine. Events are passed from the MIDI and UI
threads via this system. Most of synth and its cronies have wrappers available
for rendering-thread-only functions that just generate events here.

Licensed under the Open Software License version 2.1
*/
#ifndef SINOSPAN_ITC_H
#define SINOSPAN_ITC_H

#include <pthread.h>	// pthread_mutex_t

#include "note.h"

namespace ITC
{

enum EvFlag {
	OWNER_UI,	// Event pending for UI thread
	OWNER_RENDER,	// Event pending for Render thread
	OWNER_RAMDUDE,	// Event pending for RAMDude thread
	NUM_ITC_OWNERS	// Leave this at the end
};

// Note on unit conventions:
// As I'm guessing you noticed by now, time is ALWAYS passed in microseconds,
// and frequency is ALWAYS passed in cycles per microsecond.

// And one last note:
// You're guaranteed that an event will be processed before the next rendering
// cycle begins. However, if the next rendering cycle never happens, the event
// will never be received. Keep this in mind.

bool Init(); bool Go(); bool Stop(); void Die();

// Run your pending callbacks.
void R_GetEvents(EvFlag owner);

// Defer a function call to another thread. Returns false on failure (most
// likely because all the slots are full), true otherwise.
bool R_SendEvent(EvFlag target, void (*func)(void*), void *data);
};

// Convenience macro. Give it a name that's unique in the current scope and the
// OWNER_ value for a thread and put a function body after it and that function
// will be called in the other thread. Third arg is void pointer to the data to
// pass to the function.
// For example:
// ITC_DEFER(callback5487835293284, ITC::OWNER_FOO, 0x0,
// {
//	printf("In Foo Thread.\n");
// });
#define ITC_DEFER(name, target, data, func) \
class name \
{ \
public: \
	static void callback(void *arg) func \
}; \
ITC::R_SendEvent(target, &name::callback, data);

#endif
