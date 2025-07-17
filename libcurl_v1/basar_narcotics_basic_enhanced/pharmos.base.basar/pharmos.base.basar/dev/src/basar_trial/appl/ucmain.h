#include "ucsub.h"

using basar::appl::SystemEventManager;
/*
using basar::appl::SysEventExecuter;
using basar::appl::FuncExecuter;
using basar::appl::EventReturnType;
*/


class UCMain // In Produktiv-UCCs: public UseCaseController
{
public:
	void registerEventHandlers(const bool forSave);
	void run();

	SYSTEM_EVENT_HANDLER_DECLARATION(Cancel)
	SYSTEM_EVENT_HANDLER_DECLARATION(CustomerSelected)

	//basar::appl::EventReturnType handleCancel
	//	(basar::appl::IEventSource & rSource, basar::db::aspect::AccessorPropertyTable_YIterator yit);

	//basar::appl::EventReturnType handleCustomerSelected
	//	(basar::appl::IEventSource & rSource, basar::db::aspect::AccessorPropertyTable_YIterator yit);

private:
	UCSub m_SubUC;
};
