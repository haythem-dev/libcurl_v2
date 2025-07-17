#include <UnitTest++/UnitTest++.h>
#include "libbasardbaspect.h"

#include "libbasardbaspect_exceptions.h"

//------------------------------------------------------------------------------
using basar::db::aspect::AccessorDefinitionRef;
using basar::db::aspect::AccessMethodRef;
using basar::db::aspect::AccessMethodListRef;
using basar::db::aspect::AccessMethodListSize;
using basar::db::aspect::Manager;
using basar::db::aspect::ExistAccessorMethodException;

//------------------------------------------------------------------------------
SUITE(DBAspect_AccessMethod)
{

//------------------------------------------------------------------------------
TEST (CreateMethod)
{
	AccessorDefinitionRef accdef  = Manager::getInstance().createAccessorDefinition("accdef1");

	AccessMethodListRef   list    = accdef.getAccessMethodList();
	AccessMethodListSize  no      = list.size();
	AccessMethodRef       accmeth;

	accmeth = accdef.createAccessMethod("accmethod1");
	CHECK_EQUAL((unsigned)++no, (unsigned)list.size());

	accmeth = accdef.createAccessMethod("accmethod2");
	CHECK_EQUAL((unsigned)++no, (unsigned)list.size());

	CHECK_THROW(accdef.createAccessMethod("accmethod1"), ExistAccessorMethodException);
}

//------------------------------------------------------------------------------
}	// SUITE
