/* Licensed under the Open Software License version 2.1 */
#include "out_jack.h"

#include "synth.h"	// syn2Render()
#include "itc.h"	// itc2GetEvents()

#include <stdbool.h>	// true, false
#include <stdio.h>	// printf()

#include <jack/jack.h>	// jack_port_t, jack_client_t, jack_client_new(),
			// jack_set_process_callback(),
			// jack_set_sample_rate_callback(),
			// jack_port_register(), jack_get_sample_rate(),
			// jack_activate(), jack_nframes_t,
			// jack_client_close(), JACK_DEFAULT_AUDIO_TYPE,
			// JackPortIsOutput

Out_JACK::Out_JACK()
{
	return;
}

int R_pRender(jack_nframes_t,void*);
int R_pNewSrate(jack_nframes_t,void*);

bool Out_JACK::Go()
{
	SampleRate = 0;

	// Create a new JACK client.
	JackClient = jack_client_new("Sinospan");
	if(JackClient == NULL) { printf("FATAL: out_jack: Couldn't make JACK client\n"); return false; }

	// Associate our render function as the JACK process callback
	if(jack_set_process_callback(JackClient, R_pRender, (void*) this) != 0) { printf("ERROR: out_jack: Couldn't register processing callback\n"); }
	
	// Set our sample rate change handler
	if(jack_set_sample_rate_callback(JackClient, R_pNewSrate, (void*) this) != 0) { printf("WARNING: out_jack: Couldn't register sample rate callback.\n"); }

	// Make our output port
	JackOut1 = jack_port_register(JackClient, "Out", JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);
	if(JackOut1 == NULL) { printf("FATAL: out_jack: Couldn't register an output port.\n"); return false; }

	// Get the sample rate from the JACK server.
	R_pNewSrate(jack_get_sample_rate(JackClient), (void*) this);

	// It's go-time!
	if(jack_activate(JackClient) != 0) { printf("ERROR: out_jack: Couldn't activate JACK client.\n"); return false; }

	return true;
}

bool Out_JACK::Stop()
{
	// Stop!
	if(jack_deactivate(JackClient) != 0) {printf("ERROR: out_jack: Couldn't deactivate JACK client.\n"); return false; }

	jack_client_close(JackClient);

	return true;
}

Out_JACK::~Out_JACK()
{
	return;
}

// *** RENDERING THREAD ENTRY POINT *** //
int R_pRender(jack_nframes_t numFrames, void *pself)
{
	Out_JACK *self = (Out_JACK*) pself;
	jack_default_audio_sample_t *out;
	unsigned int i;

	out = (jack_default_audio_sample_t*) jack_port_get_buffer(self->JackOut1, numFrames);

	i = 0;
	while(i < numFrames)
	{
		out[i] = (jack_default_audio_sample_t) Synth::R_Render(self->SampleRate);
		i++;
	}
	
	return 0;
}

// JACK callback for sample rate change
int R_pNewSrate(jack_nframes_t newSrate, void *pself)
{
	Out_JACK *self = (Out_JACK*) pself;
	self->SampleRate = 1 / (newSrate / 1000000.0);
	// printf("DBG: Sample rate: %f\n", pSampleRate);
	
	return 0;
}
