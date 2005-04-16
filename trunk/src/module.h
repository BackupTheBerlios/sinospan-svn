/*
Module -- data goes in, data comes out.

Copyright (C) 2005 Ben Anderson
Licensed under the Open Software License version 2.1
*/
#ifndef SINOSPAN_MODULE_H
#define SINOSPAN_MODULE_H

struct Plug;
struct Jack;

struct Module
{
	virtual ~Module();
	// Get the total number of input ports.
	virtual unsigned short int inCt() const;
	// Get the total number of output ports.
	virtual unsigned short int outCt() const;
	// This module's input plugs. Always inCt() number of elements.
	Plug *inputs;
	// The module's output jacks. Always outCt() number of elements.
	Jack *outputs;
	// Render for this much time after the last pass.
	virtual void R_Render(float time);
protected:
	// Convenience functions for derived classes.
	void allocPorts();
	void deallocPorts();
};

#endif
