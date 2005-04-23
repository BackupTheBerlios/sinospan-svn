/*
A GTK UI.

Copyright (C) 2005 Ben Anderson
Licensed under the Open Software License version 2.1
*/
#ifndef SINOSPAN_UI_GTK_H
#define SINOSPAN_UI_GTK_H

#include "ui.h"

class UI_GTK : public UI::Driver {
public:
	UI_GTK();
	~UI_GTK();
	bool Go(int argc, char *argv[]);
	bool Stop();
	void Loop();
private:
	friend struct UI_GTK_CB;
};
#define USE_UI_GTK

#endif
