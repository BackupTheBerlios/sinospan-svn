/*
Note passing, note processing, et cetera.

Licensed under the Open Software License version 2.1
*/
#ifndef SINOSPAN_NOTE_H
#define SINOSPAN_NOTE_H

// The basic data type for passing notes around.
struct Note {
	unsigned short int octave; // Valid range is -1 to 9
	enum interval_t {
		A_NAT=0,
			A_SH=1,
		B_NAT=2,
		C_NAT=3,
			C_SH=4,
		D_NAT=5,
			D_SH=6,
		E_NAT=7,
		F_NAT=8,
			F_SH=9,
		G_NAT=10,
			G_SH=11
	} interval;

	// Set this to the Note value that corresponds to this MIDI note
	// number.
	Note& SetFromMidiN(unsigned short int num);

	// Get the frequency of this Note.
	float Freq();
};

#endif
