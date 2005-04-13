/*
Atomic indexer system. You can read or increment them. As soon as they reach
max, they reset to 0. (So, they're always in the range 0...max-1.)

DANGER: if more than one thread increments one of these, there's (VERY VERY
SLIGHT) potential for a race condition nullifying one of the increments. (For
it to happen, thread 1 and thread 2 must increment it at very precise times
relative to each other.) However, as we use them, only one thread ever
increments them.

Licensed under the Open Software License version 2.1
*/
#ifndef SINOSPAN_ATOMIND_H
#define SINOSPAN_ATOMIND_H

#include <semaphore.h>	// sem_t

class AtomicIndex {
public:
	AtomicIndex(int _max); ~AtomicIndex();
	// Increment this index
	void Inc();
	// Get the value of this index
	int GetValue();
protected:
	int max;
	sem_t sem1;
	inline void pGetSemVals(int &val1, int &val2);
};

#endif
