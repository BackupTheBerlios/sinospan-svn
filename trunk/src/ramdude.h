/*
The RAMDude. Does (de)allocating for RT threads.

Copyright (C) 2005 Ben Anderson
Licensed under the Open Software License version 2.1
*/
#ifndef SINOSPAN_RAMDUDE_H
#define SINOSPAN_RAMDUDE_H

namespace RAMDude
{
bool Init(); bool Go(); bool Stop(); void Die();

// Wake up the RAMDude so he'll carry out tasks.
void Whack();

// To use: Put your allocation / deallocation code in a function, and pass a
// pointer to it to RAMDude through ITC. Then, whack RAMDude. Simple.
};

#endif
