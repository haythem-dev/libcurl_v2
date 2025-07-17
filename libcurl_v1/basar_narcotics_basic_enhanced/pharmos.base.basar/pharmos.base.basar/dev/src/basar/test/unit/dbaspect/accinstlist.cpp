#include <UnitTest++/UnitTest++.h>
#include "libbasardbaspect.h"

#include "libbasarproperty.h"

#include "libbasardbaspect_exceptions.h"

#include "testconnection.h"

//------------------------------------------------------------------------------
using basar::db::aspect::Manager;
using basar::db::aspect::ConnectionRef;
using basar::db::aspect::AccessorInstanceRef;
using basar::db::aspect::AccessorDefinitionRef;
using basar::db::aspect::PropertyDescriptionListRef;
using basar::db::aspect::AccessorInstanceListRef;
using basar::db::aspect::FULL_CACHING;
using basar::db::aspect::UnknownAccessorInstanceException;
using basar::db::aspect::ExistAccessorInstanceException;
using basar::db::aspect::AccessorListSize;

using basar::test::unit::dbaspect::TestConnection;

//------------------------------------------------------------------------------
SUITE(DBAspect_AccessorInstanceList)
{

//------------------------------------------------------------------------------

TEST(TestList)
{
	ConnectionRef conn = TestConnection::createDefaultConnectionRef();

	try
	{
		AccessorDefinitionRef accdef1    = Manager::getInstance().createAccessorDefinition("test3_AccessorDefinitionRef");
		AccessorDefinitionRef accdef2    = Manager::getInstance().createAccessorDefinition("test4_AccessorDefinitionRef");

		PropertyDescriptionListRef pdl1  = accdef1.getPropertyDescriptionList();
		PropertyDescriptionListRef pdl2  = accdef2.getPropertyDescriptionList();

		AccessorInstanceRef     accInst1 = Manager::getInstance().createAccessorInstance ("test3_AccessorInstanceRef",	
																				          "test3_AccessorDefinitionRef", 
																				          conn,
																						  FULL_CACHING,
																						  true);
		AccessorInstanceRef     accInst2 = Manager::getInstance().createAccessorInstance ("test4_AccessorInstanceRef",	
																				          "test4_AccessorDefinitionRef", 
																				          conn,
																						  FULL_CACHING,
																						  true);
		CHECK_THROW(Manager::getInstance().createAccessorInstance ("test4_AccessorInstanceRef",	
																   "test4_AccessorDefinitionRef", 
																   conn,
																   FULL_CACHING,
																   true),
				    ExistAccessorInstanceException);

		AccessorInstanceListRef list     = Manager::getInstance().getAccessorInstanceList();
		AccessorListSize        no     = list.size();

		Manager::getInstance().releaseAccessorInstance("test4_AccessorInstanceRef");
		CHECK_EQUAL((unsigned)--no, (unsigned)list.size());

		Manager::getInstance().releaseAccessorInstance("test3_AccessorInstanceRef");
		CHECK_EQUAL((unsigned)--no, (unsigned)list.size());

		CHECK_THROW(Manager::getInstance().releaseAccessorInstance("nonsense"), UnknownAccessorInstanceException);

	}
	catch (basar::Exception & ex)
    {
		std::cout << "basar exception: " << ex.what().c_str() << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
    catch (...)
    {
        std::cout << "unknown exception" << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }

	conn.close();
}


//------------------------------------------------------------------------------
}	// SUITE
