/*
An output from something.

Copyright (C) 2005 Ben Anderson
Licensed under the Open Software License version 2.1
*/
#ifndef SINOSPAN_JACK_H
#define SINOSPAN_JACK_H

class Plug;

class Jack
{
public:
	Jack(); ~Jack();
	// Set the value on this jack.
	Jack &operator=(float val);
	// Is anything connected to this jack?
	inline bool IsUsed() const;
private:
	// The actual data value on this jack.
	float data;
	// Reference to the top plug connected to us. 0x0 if no plugs are
	// connected.
	Plug *plugsTop;
	// Appropriately edits plugsTop when the top plug is removed, and also
	// accesses data for reading.
	friend class Plug;
};

#endif
