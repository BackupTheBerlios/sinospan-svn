/*
Module -- data goes in, data comes out.

Copyright (C) 2005 Ben Anderson
Licensed under the Open Software License version 2.1
*/
#ifndef SINOSPAN_MODULE_H
#define SINOSPAN_MODULE_H

struct Module
{
	typedef float Port;

	virtual ~Module() {}
	// Get the total number of input ports.
	virtual unsigned short int inCt() const { return 0; }
	// Get the total number of output ports.
	virtual unsigned short int outCt() const { return 0; }
	// This module's inputs. (array)
	Port ***inputs;
	// The module's outputs. (array)
	Port **outputs;
	// Render for this much time after the last pass.
	virtual void R_Render(float time) {}
protected:
	// Constructor utility functions.
	void allocPorts();
	void deallocPorts();
};

#endif
