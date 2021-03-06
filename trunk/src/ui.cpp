/* Copyright (C) 2005 Ben Anderson
   Licensed under the Open Software License version 2.1 */
#include "ui.h"

#include "uis/config_ui.h"

using namespace UI;

static Driver *pMakeUiDriver();
static Driver *pCurUiDriver;

bool UI::Init()
{
	pCurUiDriver = pMakeUiDriver();
	if(pCurUiDriver == 0) { return false; }

	return true;
}

bool UI::Go(int argc, char *argv[])
{
	return pCurUiDriver->Go(argc, argv);
}

bool UI::Stop()
{
	return pCurUiDriver->Stop();
}

void UI::Die()
{
	delete pCurUiDriver;
}

void UI::Loop()
{
	pCurUiDriver->Loop();
}

static Driver *pMakeUiDriver()
{
	Driver *ret = 0;

#ifdef USE_UI_GTK
	ret = new UI_GTK;
	if(ret != 0) { return ret; }
#endif
	
	return ret;
}


UI::Driver::~Driver() {}
bool UI::Driver::Go(int argc, char *argv[]) { return false; }
bool UI::Driver::Stop() { return false; }
void UI::Driver::Loop() {}
