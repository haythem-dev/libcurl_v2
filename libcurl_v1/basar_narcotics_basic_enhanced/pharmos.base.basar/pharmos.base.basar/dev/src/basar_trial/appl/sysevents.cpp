#include <libbasarcmnutil.h>
#include <libbasardbaspect.h>
#include <libbasarappl.h>

#include "testappl.h"
#include "ucmain.h"

//------------------------------------------------------------------------------
void test_systemevents()
{
	UCMain mainUC;
	
	mainUC.run();

	// alle SytemEvents sind deregistriert, d.h die nachfolgenden Dinge
	// müssen Exceptions werfen
	SystemEventManager::getInstance().fire("CustomerSelected");
	SystemEventManager::getInstance().fire("Cancel",			basar::appl::NullEventSource::getInstance());

}
