/*
UI (User Interface).

Copyright (C) 2005 Ben Anderson
Licensed under the Open Software License version 2.1
*/
#ifndef SINOSPAN_UI_H
#define SINOSPAN_UI_H

namespace UI
{
bool Init(); bool Go(int argc, char *argv[]); bool Stop(); void Die();

// Run the UI's main loop.
void Loop();

class Driver {
public:
	virtual ~Driver();
	virtual bool Go(int argc, char *argv[]);
	virtual bool Stop();

	// Run the UI's main loop.
	virtual void Loop();
};
};

#endif
