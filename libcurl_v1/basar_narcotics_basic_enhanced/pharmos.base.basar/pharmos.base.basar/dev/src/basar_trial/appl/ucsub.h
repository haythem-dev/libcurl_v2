#ifndef TESTAPPL_UCSUB_H
#define TESTAPPL_UCSUB_H

#include <libbasarcmnutil.h>
#include <libbasardbaspect.h>
#include <libbasarappl.h>

using basar::appl::SystemEventManager;
/*
using basar::appl::SysEventExecuter;
using basar::appl::FuncExecuter;
using basar::appl::EventReturnType;
*/

class UCSub // In Produktiv-UCCs: public UseCaseController
{
public:

	void registerEventHandlers(const bool forSave);
	void run();

	SYSTEM_EVENT_HANDLER_DECLARATION(Cancel)
};

#endif
