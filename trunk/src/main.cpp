/*Licensed under the Open Software License version 2.1

STATUS:
No technology implemented
Still working on test environment.
*/

#include <stdio.h>	// printf()

#include "itc.h"
#include "synth.h"
#include "out.h"
#include "ui.h"

// Note on the various error names:
// * FATALs happen when something happens that creates conditions under which
//   it's unsafe to continue normal execution.
// * ERRORs happen when something happens that _will_ cause unwanted behavior,
//   but it's still safe to continue normal execution.
// * WARNINGs happen when something happens that could cause unwanted behavior
//   to occur, but it's still safe to continue normal execution.
// * DBGs aren't errors at all, they just tell me what's happening inside the
//   program so I can track down bugs.

int main(int argc, char *argv[])
{
	// --- INITIALIZATION --- //
	// I know how ugly this is, but it works, and it's efficient.
	     if(!  ITC::Init() ) { printf("FATAL: main: Couldn't initialize ITC.\n"   ); }
	else if(!Synth::Init() ) { printf("FATAL: main: Couldn't initialize synth.\n" ); }
	else if(!  Out::Init() ) { printf("FATAL: main: Couldn't initialize output.\n"); }
	// else if(!  Seq::Init() ) { printf("FATAL: main: Couldn't initialize seq IF.\n"); }
	else if(!   UI::Init() ) { printf("FATAL: main: Couldn't initialize UI.\n"    ); }

	else
	{
		// --- ACTIVATION --- //
		if(!  ITC::Go(          ) ) { printf("ERROR: main: Couldn't activate ITC.\n"   ); }
		if(!Synth::Go(          ) ) { printf("ERROR: main: Couldn't activate synth.\n" ); }
		if(!  Out::Go(          ) ) { printf("ERROR: main: Couldn't activate output.\n"); }
		// if(!  Seq::Go(          ) ) { printf("ERROR: main: Couldn't activate seq IF.\n"); ret = false; }
		if(!   UI::Go(argc, argv) ) { printf("ERROR: main: Couldn't activate UI.\n"    ); }


		// main loop...
		UI::Loop();

		// --- DEACTIVATION --- //
		if(!   UI::Stop() ) { printf("ERROR: main: Couldn't deactivate UI.\n"    ); }
		// if(!  Seq::Stop() ) { printf("ERROR: main: Couldn't deactivate seq IF.\n"); ret = false; }
		if(!  Out::Stop() ) { printf("ERROR: main: Couldn't deactivate output.\n"); }
		if(!Synth::Stop() ) { printf("ERROR: main: Clound't deactivate synth.\n" ); }
		if(!  ITC::Stop() ) { printf("ERROR: main: Couldn't deactivate ITC.\n"   ); }

		// -- DEINITIALIZATION --- //
		UI::Die();
		// Seq::Die();
		Out::Die();
		Synth::Die();
		ITC::Die();

		return 0;
	}

	// If initialization fails:
	return 64;
}
