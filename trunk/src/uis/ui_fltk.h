/*
A UI via FLTK.

Licensed under the Open Software License version 2.1
*/
#ifndef SINOSPAN_UI_FLTK_H
#define SINOSPAN_UI_FLTK_H

#include "ui.h"

// HACK: I don't want to have to include FLTK headers for UI core (makes the
// Jamfiles cleaner), but UI core needs to have this header. So, declare FLTK
// classes here...
class Fl_Window;
class Fl_Button;

class UI_FLTK : public UI::Driver
{
public:
	UI_FLTK(); ~UI_FLTK();
	bool Go(int argc, char *argv[]); bool Stop();
	void Loop();
private:
	Fl_Window *mainWin;
	Fl_Button *panicBut;
};
#define USE_UI_FLTK

#endif
