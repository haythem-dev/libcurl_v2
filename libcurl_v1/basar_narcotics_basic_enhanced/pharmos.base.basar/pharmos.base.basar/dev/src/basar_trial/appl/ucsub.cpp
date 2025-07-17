#include <libbasarcmnutil.h>
#include <libbasardbaspect.h>
#include <libbasarappl.h>

#include "testappl.h"
#include "ucsub.h"

void UCSub::run()
{
	// Schema: 1. Zeile in run() muss lauten:
	registerEventHandlers(true);
	// Dadurch werden die Event-Handler der aktuelle Liste hinzugefügt,
	// es wird nichts überschrieben!

	basar::appl::IEventSource & ref = basar::appl::NullEventSource::getInstance();

	// Simuliere: VC feuert Cancel
	SystemEventManager::getInstance().fire("Cancel", ref);
	// Simuliere: VC feuert CustomerSelected
	SystemEventManager::getInstance().fire("CustomerSelected", ref);

	SystemEventManager::getInstance().fireMulti("Cancel", basar::appl::NullEventSource::getInstance());

	// Schema: letzte Zeile in run() muss lauten:
	registerEventHandlers(false);
}

/////////////////////////////////////
// Section: System Event Handling ///
/////////////////////////////////////


HANDLE_1_SYSTEM_EVENT(UCSub,
// TO DO BEGIN: Insert a comma separated list of System Events
// While UCMain is actice, these Events are handled
	Cancel
// TO DO END	
)

/*
void UCSub::registerEventHandlers(const bool forSave)
{
	SystemEventManager::getInstance().registerExecuter("Cancel", 
		basar::appl::FuncExecuter(new basar::appl::SysEventExecuter<UCSub>(this,handleCancel)),
		forSave);
}
*/

SYSTEM_EVENT_HANDLER_DEFINITION(UCSub,Cancel)
{
	rSource;

	basar::appl::EventReturnStruct str;
	//str.ret = basar::appl::HANDLER_ERROR;
	//str.message ="error";
	str.ret = basar::appl::HANDLER_OK;
	
	printf("cancel - sub\n");

	return str;
}

