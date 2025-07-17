#include <UnitTest++/UnitTest++.h>
#include "libbasardbsql.h"

#include "testconnection.h"
#include "testinfxconnection.h"

#include <iostream>

#ifdef _WIN32
#pragma warning (push)
#pragma warning(disable : 4311 4312 4512 4127)
#else    
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wlong-long"
#endif

#include <QtCore/QThread>
#include <QtCore/QMutex>

#ifdef _WIN32
#pragma warning (pop)
#else    
#pragma GCC diagnostic pop
#endif

// #define VERBOSE

//------------------------------------------------------------------------------
using basar::Int32;
using basar::Int64;
using basar::VarString;

using basar::db::sql::ConnectionRef;
using basar::db::sql::StatementRef;
using basar::db::sql::ExecuteReturnInfo;

using basar::test::unit::dbsql::TestConnection;
using basar::test::unit::dbsql::TestInfxConnection;

//------------------------------------------------------------------------------
SUITE(ConnectionMt)
{

//------------------------------------------------------------------------------
class DataFixture
{
public:
    DataFixture(TestConnection::TestConnectionType conntype) : m_conntype(conntype)
	{
		ConnectionRef connect = TestConnection::createTestConnection(m_conntype);
		StatementRef  stmt    = connect.createStatement();

		m_Ret = stmt.execute("INSERT INTO but_var (colser) VALUES (0);");
		CHECK(!m_Ret.hasError());
		CHECK_EQUAL(1, (int)m_Ret.m_lastSerial.size());
	}

	~DataFixture()
	{
		// CHECK_EQUAL(1, (int)m_Ret.m_lastSerial.size()); C4297: function assumed not to throw an exception but does
		ConnectionRef connect = TestConnection::createTestConnection(m_conntype);
		StatementRef  stmt    = connect.createStatement();
		
		VarString sql;
		sql.format("DELETE FROM but_var "
			       "WHERE colser = %d",
				   m_Ret.m_lastSerial[0]);
		m_Ret = stmt.execute(sql);
		// CHECK(!m_Ret.hasError());                       C4297: function assumed not to throw an exception but does
		// CHECK_EQUAL(1, (int)m_Ret.getAffectedRows());   C4297: function assumed not to throw an exception but does
	}

	ExecuteReturnInfo                        m_Ret;
    const TestConnection::TestConnectionType m_conntype;

private:
    DataFixture &  operator = (const DataFixture &);
};

//------------------------------------------------------------------------------
class DataFixtureDefault : public DataFixture
{
public:
    DataFixtureDefault() : DataFixture(TestConnection::INFX) {}
};

//------------------------------------------------------------------------------
class DataFixtureBg : public DataFixture
{
public:
    DataFixtureBg() : DataFixture(TestConnection::INFX_BG) {}
};

//------------------------------------------------------------------------------
class MyThread : public QThread
{
public:
					MyThread	(const Int32                        no,
						         const Int64                        id, 
                                 TestConnection::TestConnectionType conntype);
    void			run			();

private:

					MyThread	(const MyThread & rCopy);
	MyThread &    	operator =	(const MyThread & rCopy);			//!< forbidden

	static QMutex		     mut;

	const Int32			                     m_No;
	const Int64                              m_Id;
    const TestConnection::TestConnectionType m_conntype;

public:
	Int32				m_Exit;
};

//------------------------------------------------------------------------------
QMutex MyThread::mut;

//------------------------------------------------------------------------------
MyThread::MyThread(const Int32                        no,
				   const Int64                        id,
                   TestConnection::TestConnectionType conntype)
	: m_No      (no),
	  m_Id      (id),
      m_conntype(conntype),
	  m_Exit    (0)
{
}

//------------------------------------------------------------------------------
void MyThread::run()
{
	try
	{
		ConnectionRef connect;

		// open
		{
		    QMutexLocker lock(&mut);

            connect = TestConnection::createTestConnection(m_conntype);
			connect.setIsolationLevel(basar::INFX_COMITTEDREAD);
			connect.setLockModeWait  (basar::WAITSECS, 20); 
		}

#ifdef VERBOSE
		std::cout << "thread " << m_No 
			      << "("       << QThread::currentThreadId()
				  << ") opened connection " 
				  << std::endl;
#endif

		// execute
		{
			QMutexLocker lock(&mut);

			connect.begin();

			StatementRef stmt = connect.createStatement();
			
			VarString sql;
			sql.format("UPDATE but_var "
				       "SET colsmint = %d "
					   "WHERE colser = %d",
                       m_No,
					   m_Id);

			ExecuteReturnInfo ret = stmt.execute(sql);
			CHECK(!ret.hasError());
			CHECK_EQUAL(1, (int)ret.getAffectedRows());

			connect.commit();
		}

		// close
		{
			QMutexLocker lock(&mut);

            using namespace basar::db::sql;
            ConnectionListSize prev = Manager::getInstance().size();

			connect.close();
            CHECK_EQUAL((int)prev      , (int)Manager::getInstance().size());

            connect.reset();
            CHECK_EQUAL((int)(prev - 1), (int)Manager::getInstance().size());
		}

#ifdef VERBOSE
		std::cout << "thread "  << m_No 
			      << "("        << QThread::currentThreadId() 
				  << ") closed connection "
			      << std::endl;
#endif
	}
	catch (basar::Exception & ex)
    {
		std::cerr << "thread "  << m_No 
			      << "("        << QThread::currentThreadId() 
				  << "): basar exception in MyThread::run(): " 
			      << ex.what().c_str() 
			      << std::endl;
		m_Exit = 1;
    }
	catch (std::exception & ex)
    {
		std::cerr << "thread "  << m_No 
			      << "("        << QThread::currentThreadId() 
				  << "): std exception in MyThread::run(): " 
			      << ex.what()
			      << std::endl;
		m_Exit = 2;
    }
    catch (...)
    {
		std::cerr << "thread "  << m_No 
			      << "("        << QThread::currentThreadId() 
				  << "): unknown exception in MyThread::run()" 
			      << std::endl;
		m_Exit = 3;
    }
}

//------------------------------------------------------------------------------
class MainThread
{
public:
    static void exec(ExecuteReturnInfo                & exret, 
                     TestConnection::TestConnectionType conntype);
};

//------------------------------------------------------------------------------
void MainThread::exec(ExecuteReturnInfo                & exret,
                      TestConnection::TestConnectionType conntype)
{
    static const Int32 numThrd = 20;
    static const Int32 loops   =  3;

    CHECK_EQUAL(0, (int)basar::db::sql::Manager::getInstance().size());
    CHECK_EQUAL(0, (int)basar::db::sql::Manager::getInstance().sizeOpen());

    try
	{
		MyThread * thrd[numThrd];

        Int32 j;
		for (j = 0; j < loops; ++j)
		{
#ifdef VERBOSE
			std::cout << std::endl 
				      << QThread::currentThreadId()
					  << ": loop " << j << std::endl;
#endif

			for (Int32 i = 0; i < numThrd; ++i)
			{
				thrd[i] = new MyThread(i, exret.m_lastSerial[0], conntype);
				thrd[i]->start();
			}

#ifdef VERBOSE
			std::cout << std::endl 
				      << QThread::currentThreadId() << ": "
					  << numThrd << " threads started" << std::endl;
#endif
			
			for (int i = 0; i < numThrd; ++i)
			{
#ifdef VERBOSE
				std::cout <<  QThread::currentThreadId() << ": waiting for thread " << i << std::endl;
#endif
				thrd[i]->wait();

				CHECK_EQUAL(0, (thrd[i]->m_Exit));

				delete thrd[i];
			}
		}

		CHECK_EQUAL(loops, j);
	}
	catch (basar::Exception & ex)
    {
		std::cerr << "thread "  << QThread::currentThreadId() 
				  << ": basar exception: " 
				  << ex.what().c_str()
			      << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
	catch (std::exception & ex)
    {
		std::cerr << "thread "  << QThread::currentThreadId() 
				  << ": std exception: " 
			      << ex.what()
			      << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
    catch (...)
    {
		std::cerr << "thread "  << QThread::currentThreadId() 
				  << ": unknown exception" 
			      << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
}

//------------------------------------------------------------------------------
TEST_FIXTURE(DataFixtureDefault, Repeated_Thread_Operations)
{
    MainThread::exec(m_Ret, TestConnection::INFX_CONCURRENT_TRANSACTION);
}

//------------------------------------------------------------------------------
#ifdef HAS_CICS
TEST_FIXTURE(DataFixtureDefault, Repeated_Thread_Operations_with_configured_connects)
{
    {
        ConnectionRef conn = TestConnection::createTestConnection(TestConnection::CICS_OPENED);
        conn.close();
        conn.reset();
    }

    {
        basar::db::sql::DatabaseInfo dbInfo = TestInfxConnection::getDefaultDataBaseInfo();
        dbInfo.setInfxTimeout("12", "3");

		ConnectionRef connect = basar::db::sql::Manager::getInstance().createConnect(dbInfo);

		connect.setIsolationLevel(basar::INFX_COMITTEDREAD);
		connect.setLockModeWait  (basar::WAITSECS, 20);

        connect.close();
        connect.reset();
    }

    MainThread::exec(m_Ret, TestConnection::INFX_CONCURRENT_TRANSACTION);
}
#endif

//------------------------------------------------------------------------------
TEST_FIXTURE(DataFixtureBg, Repeated_Thread_Operations_Bg)
{
    MainThread::exec(m_Ret, TestConnection::INFX_CONCURRENT_TRANSACTION_BG);
}

//------------------------------------------------------------------------------
#ifdef HAS_CICS
TEST_FIXTURE(DataFixtureBg, Repeated_Thread_Operations_with_configured_connects_Bg)
{
    {
        ConnectionRef conn = TestConnection::createTestConnection(TestConnection::CICS_OPENED);
        conn.close();
        conn.reset();
    }

    {
        basar::db::sql::DatabaseInfo dbInfo = TestInfxConnection::getBgDataBaseInfo();
        dbInfo.setInfxTimeout("12", "3");

		ConnectionRef connect = basar::db::sql::Manager::getInstance().createConnect(dbInfo);

		connect.setIsolationLevel(basar::INFX_COMITTEDREAD);
		connect.setLockModeWait  (basar::WAITSECS, 20);

        connect.close();
        connect.reset();
    }

    MainThread::exec(m_Ret, TestConnection::INFX_CONCURRENT_TRANSACTION_BG);
}
#endif
//------------------------------------------------------------------------------
}	// SUITE
