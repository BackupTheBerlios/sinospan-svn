/* Licensed under the Open Software License version 2.1 */
#include "out.h"

#include "outs/config_out.h"

using namespace Out;

static Driver *pMakeOutDriver();
static Driver *pCurOutDriver;

bool Out::Init()
{
	pCurOutDriver = pMakeOutDriver();
	if(pCurOutDriver == 0) { return false; }

	return true;
}

bool Out::Go()
{
	return pCurOutDriver->Go();
}

bool Out::Stop()
{
	return pCurOutDriver->Stop();
}

void Out::Die()
{
	delete pCurOutDriver;
}

static Driver *pMakeOutDriver()
{
	Driver *ret = 0;

#ifdef USE_OUT_JACK
	ret = new Out_JACK;
	if(ret != 0) { return ret; }
#endif

	return ret;
}

Out::Driver::~Driver() {}
bool Out::Driver::Go() { return false; }
bool Out::Driver::Stop() { return false; }
