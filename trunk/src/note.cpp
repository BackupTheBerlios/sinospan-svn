/* Licensed under the Open Software License version 2.1 */
#include "note.h"

static const float A5_NATURAL_FREQ = 0.00044; // 440Hz

static const float NOTE_RATIOS[12] = 
{
	1.0,			// A_NAT
	1.0594630943593182,	// A_SH
	1.1224620483093182,	// B_NAT
	1.1892071150027272,	// C_NAT
	1.2599210498947728,	// C_SH
	1.33483985417,		// D_NAT
	1.414213562373182,	// D_SH
	1.4983070768765909,	// E_NAT
	1.5874010519681818,	// F_NAT
	1.6817928305074998,	// F_SH
	1.7817974362806819,	// G_NAT
	1.8877486253634093	// G_SH
};

Note& Note::SetFromMidiN(unsigned short int num)
{
	static const int A_ZERO = 9;
	int workNum = num - A_ZERO;
	
	this->octave = workNum / 12;
	workNum += 12; // Make sure it's positive.
	this->interval = (Note::interval_t) (workNum % 12);

	return *this;
}

float Note::Freq()
{
	float octCore;
	int i;

	octCore = A5_NATURAL_FREQ;
	i = this->octave;

	while(i > 5)
		{ octCore *= 2.0; i--; }
	while(i < 5)
		{ octCore /= 2.0; i++; }

	return octCore * NOTE_RATIOS[(int) this->interval];
}
