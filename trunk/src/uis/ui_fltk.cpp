/* Licensed under the Open Software License version 2.1 */
#include "ui_fltk.h"

#include <FL/Fl.h>
#include <FL/Fl_Window.h>
#include <FL/Fl_Button.h>

#include "itc.h"
#include "synth.h"

UI_FLTK::UI_FLTK()
{
	return;
}

UI_FLTK::~UI_FLTK()
{
	return;
}

static void panicButCB(Fl_Widget*,void*);

bool UI_FLTK::Go(int argc, char *argv[])
{
	mainWin = new Fl_Window(100, 100);
	panicBut = new Fl_Button(10, 10, 80, 80, "Panic!");
	panicBut->callback(panicButCB, 0);
	mainWin->end();
	mainWin->show(argc, argv);

	return true;
}

bool UI_FLTK::Stop()
{
	// FLTK doesn't want any cleanup (!?)
	return true;
}

void UI_FLTK::Loop()
{
	Fl::run();
}

static void panicButCB(Fl_Widget *w, void *nul)
{
	ITC::DeferVoid(&Synth::R_Panic);
}
