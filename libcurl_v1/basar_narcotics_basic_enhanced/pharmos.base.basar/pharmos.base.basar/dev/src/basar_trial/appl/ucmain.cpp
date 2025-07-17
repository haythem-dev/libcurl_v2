#include "testappl.h"
#include "ucmain.h"

void UCMain::run()
{
	// Schema: 1. Zeile in run() muss lauten:
	registerEventHandlers(true);
	// Dadurch werden die Event-Handler der aktuelle Liste hinzugefügt,
	// es wird nichts überschrieben!


	// Tue was, z.B. matchen


	// Simuliere: VC feuert Cancel
	SystemEventManager::getInstance().fire("Cancel");
	// Simuliere: VC feuert CustomerSelected
	SystemEventManager::getInstance().fire("CustomerSelected", basar::appl::NullEventSource::getInstance());

	// Sub-Use-Case rufen: Einfach run() rufen
	m_SubUC.run();
	// Use-Case 2 ist abgelaufen
	// Cancel-Event muss wieder in unserem Kontext sein

	// Simuliere: VC feuert Cancel
	SystemEventManager::getInstance().fire("Cancel");
	// Simuliere: VC feuert CustomerSelected
	SystemEventManager::getInstance().fire("CustomerSelected", basar::appl::NullEventSource::getInstance());

	// Schema: letzte Zeile in run() muss lauten:
	registerEventHandlers(false);
}

/////////////////////////////////////
// Section: System Event Handling ///
/////////////////////////////////////

//-----------------------------------------------------
// Insert a comma separated list of System Events
// While UCMain is actice, these Events are handled
//HANDLE_2_SYSTEM_EVENTS(UCMain,
//	Cancel, CustomerSelected)


void UCMain::registerEventHandlers(const bool forSave)
{
	
	SystemEventManager::getInstance().registerExecuter("Cancel", 
		basar::appl::FuncExecuter(new basar::appl::SysEventExecuter<UCMain>(this,& UCMain :: handleCancel)), forSave);

	SystemEventManager::getInstance().registerExecuter("CustomerSelected", 
		basar::appl::FuncExecuter(new basar::appl::SysEventExecuter<UCMain>(this,& UCMain :: handleCustomerSelected)),
		forSave);
	


	/*basar::appl::SysEventExecuter<UCMain>(this,& UCMain :: handleCustomerSelected);

	basar::appl::FuncExecuter executer = 
		basar::appl::FuncExecuter(new basar::appl::SysEventExecuter<UCMain>(this,& UCMain :: handleCustomerSelected));
	
	SystemEventManager::getInstance().registerExecuter("CustomerSelected", executer, forSave);*/
	
}


/*
BEGIN_HANDLERS_REGISTRATION(UCMain)
	SYSTEM_EVENT_HANDLER_REGISTRATION_WITH_SOURCE(UCMain, Cancel, basar::appl::NullEventSource::getInstance())
	SYSTEM_EVENT_HANDLER_REGISTRATION(UCMain, CustomerSelected)
END_HANDLERS_REGISTRATION()
*/



//-----------------------------------------------------

SYSTEM_EVENT_HANDLER_DEFINITION(UCMain,Cancel)
//basar::appl::EventReturnType UCMain::handleCancel
//		(basar::appl::IEventSource & rSource, basar::db::aspect::AccessorPropertyTable_YIterator yit)
{
	printf("cancel - main\n");
	return basar::appl::EventReturnStruct (); //std with ret = OK, msg = ""
}

SYSTEM_EVENT_HANDLER_DEFINITION(UCMain,CustomerSelected)
//basar::appl::EventReturnType UCMain::handleCustomerSelected
//		(basar::appl::IEventSource & rSource, basar::db::aspect::AccessorPropertyTable_YIterator yit)
{
	rSource;

	basar::appl::EventReturnStruct str;
	str.ret = basar::appl::HANDLER_INFO;
	str.message ="info message";

	printf("customerselected - main\n");

	return str;
}



/*basar::appl::EventReturnType 
	UCMain::handleCustomerSelected
	(basar::appl::IEventSource *, basar::db::aspect::AccessorPropertyTable_YIterator )
{
	return basar::appl::EventReturnStruct();
}*/


//-----------------------------------------------------

