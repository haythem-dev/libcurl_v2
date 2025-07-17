/* @file managerimpl.cpp                                                
* @brief ...                                       
* @author Anke Klink                      
* @date 2007                            
*/ 

#include <UnitTest++/UnitTest++.h>
#include "libbasardbsql.h"
#include "libbasar_definitions.h"
#include "libbasardbsql_connectionlist_iterator.h"
#include "libbasardbsql_manager.h"

#include "testconnection.h"

using basar::db::sql::ConnectionRef;
using basar::db::sql::DatabaseInfo;
using basar::db::sql::ManagerImpl;
using basar::db::sql::Manager;
using basar::db::sql::ConnectionListSize;
using basar::db::sql::ConnectionList_Iterator;

using basar::test::unit::dbsql::TestConnection;

#define TESTDBSQLMANAGERIMPL
#ifdef TESTDBSQLMANAGERIMPL
//------------------------------------------------------------------------------
SUITE(Manager)
{

	// -------------------------------------------------------------------
	TEST(manager_size)
	{
		Manager::clear();
		ManagerImpl & manager   = Manager::getInstance();

		ConnectionRef conn = TestConnection::createDefaultTestConnection();
		CHECK(conn.isOpen());

		CHECK_EQUAL(1U, (unsigned int)manager.size    ());
		CHECK_EQUAL(1U, (unsigned int)manager.sizeOpen());

		conn.close();
		CHECK_EQUAL(1U, (unsigned int)manager.size    ());
		CHECK_EQUAL(0U, (unsigned int)manager.sizeOpen());
	}

	// -------------------------------------------------------------------
	TEST(manager_size_scope)
	{
		Manager::clear();
		ManagerImpl & manager   = Manager::getInstance();
		
		{
			ConnectionRef conn = TestConnection::createDefaultTestConnection();
			CHECK(conn.isOpen());

			CHECK_EQUAL(1U, (unsigned int)manager.size    ());
			CHECK_EQUAL(1U, (unsigned int)manager.sizeOpen());

			conn.close();
			CHECK_EQUAL(1U, (unsigned int)manager.size    ());
			CHECK_EQUAL(0U, (unsigned int)manager.sizeOpen());
		}

		CHECK_EQUAL(0U, (unsigned int)manager.size    ());
		CHECK_EQUAL(0U, (unsigned int)manager.sizeOpen());
	}

	// -------------------------------------------------------------------
	TEST(testdbsqlmanagerimpl_createRef_open_close_size_ConnectionList_Iterator)
	{		
		Manager::clear();
		ManagerImpl & manager = Manager::getInstance();
		
		{
			CHECK(manager.begin() == manager.end());
			ConnectionList_Iterator iter = manager.begin();

			CHECK(iter.isBegin());
			CHECK(iter.isEnd());
		}

		// add ConnectionRef
		{
			// uses create internal:
			ConnectionRef connRef = TestConnection::createDefaultTestConnection();

			CHECK(connRef.isOpen());
			CHECK_EQUAL(1U, (unsigned int)manager.size());
			CHECK_EQUAL(1U, (unsigned int)manager.sizeOpen());

			connRef.close();
			CHECK_EQUAL(1U, (unsigned int)manager.size());
			CHECK_EQUAL(0U, (unsigned int)manager.sizeOpen());

			// ConnectionList_Iterator
			ConnectionList_Iterator iter = manager.begin();
			CHECK(!iter.isNull());	
			CHECK(iter.isBegin());
			ConnectionList_Iterator iter2 = manager.begin();
			CHECK(iter == iter2);

			iter2.reset();
            CHECK(iter2.isNull());	
            CHECK_THROW(iter2.isBegin(), basar::InvalidIteratorException);
		
			CHECK_THROW(iter != iter2, basar::DifferentContainersIteratorException);
			
			CHECK(manager.begin() != manager.end());
		
			iter++;

			CHECK(iter ==  manager.end());
			CHECK(iter.isEnd());
			CHECK_THROW(iter->first, basar::OutOfRangeIteratorException);
            CHECK_THROW(iter2->first, basar::InvalidIteratorException);
			CHECK_THROW(iter++, basar::OutOfRangeIteratorException);
			CHECK(iter ==  manager.end());
		}

		CHECK_EQUAL(0U, (unsigned int)manager.size());
		CHECK_EQUAL(0U, (unsigned int)manager.sizeOpen());
	}

	// -------------------------------------------------------------------
	TEST(testdbsqlmanagerimpl_open_close_shutdown)
	{
		try
		{
			ManagerImpl & manager = Manager::getInstance();
			{
				// uses create internal:
				ConnectionRef connRef = TestConnection::createDefaultTestConnection();

				CHECK(connRef.isOpen());
				CHECK_EQUAL(1U, (unsigned int)manager.size    ());
				CHECK_EQUAL(1U, (unsigned int)manager.sizeOpen());

				manager.shutdown();

				CHECK(!connRef.isOpen());
				CHECK_EQUAL(1U, (unsigned int)manager.size    ());
				CHECK_EQUAL(0U, (unsigned int)manager.sizeOpen());

				connRef.reset();
				manager.shutdown();

				CHECK(connRef.isNull());
				CHECK_EQUAL(0U, (unsigned int)manager.size    ());
				CHECK_EQUAL(0U, (unsigned int)manager.sizeOpen());
			}

			manager.shutdown();
			CHECK_EQUAL(0U, (unsigned int)manager.size    ());
			CHECK_EQUAL(0U, (unsigned int)manager.sizeOpen());
		}
		catch (...)
		{
			const bool ExceptionOccured = false;
			CHECK(ExceptionOccured);
		}
	}
	
	// maybe todo: define more tests with DatabaseInfo and Iterator

}
#endif
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
