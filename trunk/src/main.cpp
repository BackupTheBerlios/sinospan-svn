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

bool mainInit();
bool mainGo(int argc, char *argv[]);
bool mainStop();
void mainDie();

int main(int argc, char *argv[])
{
	if(!mainInit() ) { return 64; }

	mainGo(argc, argv);

	UI::Loop();

	mainStop();

	mainDie();

	return 0;
}

// Note on the various error names:
// * FATALs happen when something happens that creates conditions under which
//   it's unsafe to continue normal execution.
// * ERRORs happen when something happens that _will_ cause unwanted behavior,
//   but it's still safe to continue normal execution.
// * WARNINGs happen when something happens that could cause unwanted behavior
//   to occur, but it's still safe to continue normal execution.
// * DBGs aren't errors at all, they just tell me what's happening inside the
//   program so I can track down bugs.

bool mainInit()
{
	// I know how ugly this is, but it works, and it's efficient.
	     if(!  ITC::Init() ) { printf("FATAL: main: Couldn't initialize ITC.\n"   ); }
	else if(!Synth::Init() ) { printf("FATAL: main: Couldn't initialize synth.\n" ); }
	else if(!  Out::Init() ) { printf("FATAL: main: Couldn't initialize output.\n"); }
	// else if(!  Seq::Init() ) { printf("FATAL: main: Couldn't initialize seq IF.\n"); }
	else if(!   UI::Init() ) { printf("FATAL: main: Couldn't initialize UI.\n"    ); }

	else { return true; }

	return false;
}

bool mainGo(int argc, char *argv[])
{
	bool ret = true;

	if(!  ITC::Go(          ) ) { printf("ERROR: main: Couldn't activate ITC.\n"   ); ret = false; }
	if(!Synth::Go(          ) ) { printf("ERROR: main: Couldn't activate synth.\n" ); ret = false; }
	if(!  Out::Go(          ) ) { printf("ERROR: main: Couldn't activate output.\n"); ret = false; }
	// if(!  Seq::Go(          ) ) { printf("ERROR: main: Couldn't activate seq IF.\n"); ret = false; }
	if(!   UI::Go(argc, argv) ) { printf("ERROR: main: Couldn't activate UI.\n"    ); ret = false; }

	return ret;
}

bool mainStop()
{
	bool ret = true;

	if(!   UI::Stop() ) { printf("ERROR: main: Couldn't deactivate UI.\n"    ); ret = false; }
	// if(!  Seq::Stop() ) { printf("ERROR: main: Couldn't deactivate seq IF.\n"); ret = false; }
	if(!  Out::Stop() ) { printf("ERROR: main: Couldn't deactivate output.\n"); ret = false; }
	if(!Synth::Stop() ) { printf("ERROR: main: Clound't deactivate synth.\n" ); ret = false; }
	if(!  ITC::Stop() ) { printf("ERROR: main: Couldn't deactivate ITC.\n"   ); ret = false; }

	return ret;
}

void mainDie()
{
	UI::Die();
	// Seq::Die();
	Out::Die();
	Synth::Die();
	ITC::Die();
}
