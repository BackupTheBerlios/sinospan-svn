/*
JACK output.

Licensed under the Open Software License version 2.1
*/
#ifndef SINOSPAN_OUT_JACK_H
#define SINOSPAN_OUT_JACK_H

#include "out.h"

#include <jack/jack.h>

class Out_JACK : public Out::Driver
{
public:
	friend int R_pRender(jack_nframes_t,void*);
	friend int R_pNewSrate(jack_nframes_t,void*);
	Out_JACK(); ~Out_JACK(); bool Go(); bool Stop();
private:
	// Output port
	jack_port_t *JackOut1;
	// Our JACK client
	jack_client_t *JackClient;

	// JACK's system sample rate, converted to microseconds per sample
	float SampleRate;
};

#define USE_OUT_JACK

#endif
