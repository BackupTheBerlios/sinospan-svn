/* Copyright (C) 2005 Ben Anderson
   Licensed under the Open Software License version 2.1 */
#include "plug.h"

#include "jack.h"

Plug::Plug(): connjack(0x0), above(0x0), below(0x0) {}

Plug::~Plug()
{
	this->Disconnect();
}

void Plug::Connect(Jack &jk)
{
	if(IsConnected() ) { Disconnect(); }
	// Let's stick ourselves at the top of the list, as that's the easiest
	// and fastest place.
	below = jk.plugsTop;
	// the plug above the plug below this...
	below->above = this;
	jk.plugsTop = this;
	connjack = &jk;
}

void Plug::Disconnect()
{
	if(!IsConnected() ) { return; }
	// the plug above the plug below this, which won't be here anymore
	// (we're disconnecting!)
	if(below != 0x0) below->above = above;
	// the plug below the plug above this, which won't be here anymore
	if(above != 0x0) above->below = below;
	else connjack->plugsTop = below;	// Might be 0x0 -- intended
						// (we're the only plug
						// connected and we're
						// disconnecting).
	connjack = 0x0;
	below = above = 0x0;
}

bool Plug::IsConnected() const
{
	return connjack != 0x0;
}

void Plug::DisconnectAll()
{
	// We don't have to do as much error checking here, as everyone is
	// disconnecting.
	if(!IsConnected() ) { return; }
	if(below != 0x0) below->DisconnectAll();

	above = 0x0;
	connjack = 0x0;
	below = 0x0;
}

const float &Plug::read() const
{
	static const float silence = 0.0;
	if(!IsConnected() ) { return silence; }
	return connjack->data;
}
