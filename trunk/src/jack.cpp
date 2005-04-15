/* Copyright (C) 2005 Ben Anderson
   Licensed under the Open Software License version 2.1 */
#include "jack.h"

#include "plug.h"

Jack::Jack(): data(0.0), plugsTop(0x0) {}

Jack::~Jack()
{
	if(plugsTop != 0x0) plugsTop->DisconnectAll();
}

Jack &Jack::operator=(float val)
{
	data = val;
	return *this;
}

inline bool Jack::IsUsed() const
{
	return plugsTop != 0x0;
}
