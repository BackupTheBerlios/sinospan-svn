/*
Connects to a Jack to get data. Might not necessarily be connected, and under
certain semi-common circumstances might be disconnected without warning.

Copyright (C) 2005 Ben Anderson
Licensed under the Open Software License version 2.1
*/
#ifndef SINOSPAN_PLUG_H
#define SINOSPAN_PLUG_H

class Jack;

class Plug
{
	Plug(); ~Plug();
	// Connect to a jack, disconnecting from the current one first if we
	// are presently connected.
	void Connect(Jack &jk);
	// Disconnect from the current jack.
	void Disconnect();
	// Is it connected?
	inline bool IsConnected() const;
	// Get the data of the jack this is connected to. Safe to call even if
	// not connected (returns 0.0 in that case.)
	inline const float &read() const;
private:
	// The Jack we're connected to. 0x0 if not connected.
	Jack *connjack;
	// The plug above us. 0x0 if none.
	Plug *above;
	// The plug below us. 0x0 if none.
	Plug *below;
	// Disconnect this plug and all plugs below it. Used to disconnect
	// plugs recursively when the jack is dying.
	void DisconnectAll();
	// Calls DisconnectAll() on the top plug when dying.
	friend class Jack;
};

#endif
