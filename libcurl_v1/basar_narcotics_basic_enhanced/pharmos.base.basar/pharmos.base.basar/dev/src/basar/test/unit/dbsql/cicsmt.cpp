//----------------------------------------------------------------------------
/** $Id: cicsmt.cpp 5900 2008-07-02 07:41:19Z michael.eichenlaub $
 *  \author Michael Eichenlaub
 *  \date   09.07.2008
*/
//----------------------------------------------------------------------------

#include <UnitTest++/UnitTest++.h>
#include "libbasardbsql.h"

#include "testconnection.h"

#include <stdexcept>
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

//#define VERBOSE

#define CICSMT_ENABLED

#ifdef CICSMT_ENABLED

//------------------------------------------------------------------------------
SUITE(CicsMt)
{

//------------------------------------------------------------------------------
using basar::ConstString;
using basar::VarString;
using basar::Int32;
using basar::ULong32;
using basar::db::sql::ConnectionRef;
using basar::db::sql::StatementRef;
using basar::db::sql::ResultsetRef;
using basar::db::sql::DatabaseInfo;

using basar::test::unit::dbsql::TestConnection;


#ifdef HAS_CICS

//------------------------------------------------------------------------------
class MyThread : public QThread
{
public:
	enum ModeEnum
	{
		MODE_SLEEP,		// with sleep between connect and disconnect
		MODE_WO_SLEEP	// without sleep
	};
					MyThread	();
    void			run			();
	void			setMode		(ModeEnum eMode);
	void            mysleep     (ULong32 msec);

protected:
	ModeEnum		m_eMode;

private:

					MyThread	(const MyThread & rCopy);
	MyThread &    	operator =	(const MyThread & rCopy);			//!< forbidden
};

//------------------------------------------------------------------------------
MyThread::MyThread() : m_eMode(MODE_SLEEP)
{
}

//------------------------------------------------------------------------------
void MyThread::setMode(ModeEnum eMode)
{
	m_eMode = eMode;
}

//------------------------------------------------------------------------------
void MyThread::mysleep(ULong32 msec)
{
	QThread::msleep(msec);
}

//------------------------------------------------------------------------------
void MyThread::run()
{
	try
	{
		ConnectionRef connect;
		DatabaseInfo dbinfo;

		{
			connect = TestConnection::createTestConnection(TestConnection::CICS);
			connect.open();

			dbinfo = connect.getDbInfo();

#ifdef VERBOSE
			std::cout << "thread "       << QThread::currentThreadId() 
                      << " opened slot " << dbinfo.connectionId 
                      << std::endl;
#endif
		}
		
		if (MODE_SLEEP == m_eMode)
			msleep(300);

		{	
			connect.close();

#ifdef VERBOSE
			std::cout << "thread "       << QThread::currentThreadId() 
                      << " closed slot " << dbinfo.connectionId 
                      << std::endl;
#endif
		}
	}
	catch (basar::Exception & ex)
    {
		std::cout << "basar exception in MyThread::run(): " << ex.what().c_str() 
                  << std::endl;
    }
	catch (std::exception & ex)
    {
		std::cout << "std exception in MyThread::run(): " << ex.what() 
                  << std::endl;
    }
    catch (...)
    {
        std::cout << "unknown exception in MyThread::run()" 
                  << std::endl;
    }
}

//------------------------------------------------------------------------------
class MySafeThread : public MyThread
{
public:
					MySafeThread();
    void			run			();

private:

					MySafeThread(const MySafeThread & );
	MySafeThread & 	operator =	(const MySafeThread & );			//!< forbidden

	static QMutex	mut;
};

//------------------------------------------------------------------------------
QMutex MySafeThread::mut;

//------------------------------------------------------------------------------
MySafeThread::MySafeThread()
{
}

//------------------------------------------------------------------------------
void MySafeThread::run()
{
	try
	{
		ConnectionRef connect;
		DatabaseInfo dbinfo;

		{
			QMutexLocker lock(&mut);
		
			connect = TestConnection::createTestConnection(TestConnection::CICS);
			connect.open();

			dbinfo = connect.getDbInfo();

#ifdef VERBOSE
			std::cout << "thread "       << QThread::currentThreadId() 
                      << " opened slot " << dbinfo.connectionId 
                      << std::endl;
#endif
		}
		
		if (MODE_SLEEP == m_eMode)
			msleep(300);

		{	
			QMutexLocker lock(&mut);
			
			connect.close();

#ifdef VERBOSE
			std::cout << "thread "       << QThread::currentThreadId() 
                      << " closed slot " << dbinfo.connectionId 
                      << std::endl;
#endif
		}
	}
	catch (basar::Exception & ex)
    {
		std::cout << "basar exception in MyThread::run(): " << ex.what().c_str() 
                  << std::endl;
    }
	catch (std::exception & ex)
    {
		std::cout << "std exception in MyThread::run(): " << ex.what() 
                  << std::endl;
    }
    catch (...)
    {
        std::cout << "unknown exception in MyThread::run()" 
                  << std::endl;
    }
}

//------------------------------------------------------------------------------
class MyDataThread : public MyThread
{
public:
					MyDataThread(Int32 loop,
                                 bool  storedproc);
					~MyDataThread();


    void			run			();

private:

					MyDataThread(const MyDataThread & );
	MyDataThread & 	operator =	(const MyDataThread & );			//!< forbidden
    
    void            execSql       ();
    void            execStoredProc();

    const Int32     repeat;
    const bool      mode;
	ConnectionRef   conn;

	static QMutex	mut;
};

//------------------------------------------------------------------------------
QMutex MyDataThread::mut;

//------------------------------------------------------------------------------
MyDataThread::MyDataThread(Int32 loop,
                           bool  storedproc)
    : repeat(loop),
      mode  (storedproc)
{
	try
	{
        QMutexLocker lock(&mut);
    
        conn = TestConnection::createTestConnection(TestConnection::CICS);
        conn.open();
    }
	catch (basar::Exception & ex)
    {
		std::cout << "basar exception in MyThread::run(): " << ex.what() 
                  << std::endl;
    }
	catch (std::exception & ex)
    {
		std::cout << "std exception in MyThread::run(): " << ex.what() 
                  << std::endl;
    }
    catch (...)
    {
        std::cout << "unknown exception in MyThread::run()" 
                  << std::endl;
    }
}

//------------------------------------------------------------------------------
MyDataThread::~MyDataThread()
{
    try
    {
        QMutexLocker lock(&mut);
        
        conn.close();
	}
	catch (basar::Exception & ex)
    {
		std::cout << "basar exception in MyThread::run(): " << ex.what()
                  << std::endl;
    }
	catch (std::exception & ex)
    {
		std::cout << "std exception in MyThread::run(): " << ex.what() 
                  << std::endl;
    }
    catch (...)
    {
        std::cout << "unknown exception in MyThread::run()" 
                  << std::endl;
    }
}

//------------------------------------------------------------------------------
void MyDataThread::execSql()
{
    ConstString sql = 
        "SELECT par_k_nr "
        "FROM   vsam.dkpar_ut "
        "WHERE  par_k_fil = '01' "
        "ORDER BY par_k_nr;";

    try
    {
        StatementRef stmt = conn.createStatement();
        ResultsetRef res  = stmt.executeQuery(sql);

        Int32 cnt = 0;

        while (res.next())
        {
            ++cnt;
        }

        CHECK_EQUAL(3, cnt);
    }
    catch (basar::Exception & ex)
    {
        std::cout << ex.what() << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
    catch ( ... )
    {
        std::cout << "unknown exception" << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }

}

//------------------------------------------------------------------------------
void MyDataThread::execStoredProc()
{
    ConstString sql = "CALL STP.TCASTP ('PSH501','19270000000SH5011');";

    StatementRef stmt = conn.createStatement();
    ResultsetRef res  = stmt.executeSingleQuery(sql);

    Int32 cnt = 0;

    VarString cola;
    VarString colb;
    VarString hlp1,
              hlp2;

    const VarString::value_type CMP[] = "19270000000SH50";  // + "93KEINE DATEN"
    const VarString::size_type  LEN   = sizeof(CMP) - 1;    // minus '\0'

    while (res.next())
    {
        cola = res.getString(0);
        colb = res.getString("param");
        hlp1 = cola.substr( 0, 4);
        hlp2 = colb.substr(11, 4);

        CHECK_EQUAL(cola.c_str(), colb.c_str());
        CHECK(LEN < cola.size());

        CHECK_EQUAL(VarString(CMP).substr( 0, 4).c_str(), hlp1.c_str());    // "1927"
        CHECK_EQUAL(VarString(CMP).substr(11, 4).c_str(), hlp2.c_str());    // "SH50"

        ++cnt;
    }

    CHECK_EQUAL(1, cnt);
}

//------------------------------------------------------------------------------
void MyDataThread::run()
{
	try
	{
        for (Int32 i = 0; i < repeat; ++i)
        {
            if (mode)
            {
                execStoredProc();
            }
            else 
            {
                execSql();
            }
        }
	}
	catch (basar::Exception & ex)
    {
		std::cout << "basar exception in MyThread::run(): " << ex.what().c_str() 
                  << std::endl;
    }
	catch (std::exception & ex)
    {
		std::cout << "std exception in MyThread::run(): " << ex.what() 
                  << std::endl;
    }
    catch (...)
    {
        std::cout << "unknown exception in MyThread::run()" 
                  << std::endl;
    }
}

//------------------------------------------------------------------------------

#ifdef CRASH

// not synchronized connections: tcACCESS error 0x0, 0x10700000

TEST(Repeated_UnsafeThread_CicsConnect)
{
	try
	{
		static const Int32 numThrd = 10;
		static const Int32 loops   =  5;

		MyThread * thrd[numThrd];

		Int32 j;

		for (j = 0; j < loops; ++j)
		{
#ifdef VERBOSE
			std::cout << std::endl << "loop " << j << std::endl;
#endif

			for (Int32 i = 0; i < numThrd; ++i)
			{
				thrd[i] = new MyThread;
				thrd[i]->setMode(MyThread::MODE_WO_SLEEP);
				
				thrd[i]->start();
			}

#ifdef VERBOSE
			std::cout << "threads started" << std::endl;
#endif
			
			for (int i = 0; i < numThrd; ++i)
			{
#ifdef VERBOSE
				std::cout <<  QThread::currentThreadId() << ": waiting for thread " 
                          << i 
                          << std::endl;
#endif
				thrd[i]->wait();

				delete thrd[i];
			}
		}

		CHECK_EQUAL(loops, j);
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
}

#endif

//------------------------------------------------------------------------------
TEST(RepeatedSafeThreadCICSConnect)
{
	try
	{
		static const Int32 numThrd = 4;
		static const Int32 loops   = 5;

		MySafeThread * thrd[numThrd];

		Int32 j;

		for (j = 0; j < loops; ++j)
		{
#ifdef VERBOSE
			std::cout << std::endl << "loop " << j << std::endl;
#endif

			for (Int32 i = 0; i < numThrd; ++i)
			{
				thrd[i] = new MySafeThread;
				thrd[i]->setMode(MyThread::MODE_SLEEP);
				thrd[i]->start();
			}

#ifdef VERBOSE
			std::cout << "threads started" << std::endl;
#endif
			
			for (int i = 0; i < numThrd; ++i)
			{
#ifdef VERBOSE
				std::cout <<  QThread::currentThreadId() << ": waiting for thread " 
                          << i 
                          << std::endl;
#endif
				thrd[i]->wait();

				delete thrd[i];
			}
		}

		CHECK_EQUAL(loops, j);
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
}

//------------------------------------------------------------------------------
TEST(RepeatedSafeThreadCICSConnect_wo_sleep_started_with_gap)
{
	try
	{
		static const Int32 numThrd = 4;
		static const Int32 loops   = 5;

		MySafeThread * thrd[numThrd];

		Int32 j;

		for (j = 0; j < loops; ++j)
		{
#ifdef VERBOSE
			std::cout << std::endl << "loop " << j << std::endl;
#endif

			for (Int32 i = 0; i < numThrd; ++i)
			{
				thrd[i] = new MySafeThread;
				thrd[i]->setMode(MyThread::MODE_WO_SLEEP);
				
				switch (i)
				{
				case 2:
					thrd[i]->mysleep(300);
					break;

				case 0:
				case 1:
				case 3:
				default:
					break;
				}

				thrd[i]->start();
			}

#ifdef VERBOSE
			std::cout << "threads started" << std::endl;
#endif
			
			for (int i = 0; i < numThrd; ++i)
			{
#ifdef VERBOSE
				std::cout <<  QThread::currentThreadId() << ": waiting for thread " 
                          << i 
                          << std::endl;
#endif
				thrd[i]->wait();

				delete thrd[i];
			}
		}

		CHECK_EQUAL(loops, j);
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
}

//------------------------------------------------------------------------------
TEST(RepeatedSafeThreadCICSConnect_w_sleep_started_with_gap)
{
	try
	{
		static const Int32 numThrd = 4;
		static const Int32 loops   = 5;

		MySafeThread * thrd[numThrd];

		Int32 j;

		for (j = 0; j < loops; ++j)
		{
#ifdef VERBOSE
			std::cout << std::endl << "loop " << j << std::endl;
#endif

			for (Int32 i = 0; i < numThrd; ++i)
			{
				thrd[i] = new MySafeThread;
				thrd[i]->setMode(MyThread::MODE_SLEEP);
				
				switch (i)
				{
				case 2:
					thrd[i]->mysleep(300);
					break;

				case 0:
				case 1:
				case 3:
				default:
					break;
				}

				thrd[i]->start();
			}

#ifdef VERBOSE
			std::cout << "threads started" << std::endl;
#endif
			
			for (int i = 0; i < numThrd; ++i)
			{
#ifdef VERBOSE
				std::cout <<  QThread::currentThreadId() << ": waiting for thread " 
                          << i 
                          << std::endl;
#endif
				thrd[i]->wait();

				delete thrd[i];
			}
		}

		CHECK_EQUAL(loops, j);
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
}

//------------------------------------------------------------------------------
TEST(CICSConnect_DataThreads)
{
	try
	{
		static const Int32 loops   = 5;

		MyDataThread thrdsql   (loops, false);
		MyDataThread thrdstproc(loops, true );

        thrdsql   .start();
        thrdstproc.start();
        
        thrdsql   .run();
        thrdstproc.run();

        thrdsql   .wait();
        thrdstproc.wait();
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
}

//------------------------------------------------------------------------------
#endif	// CICS

//------------------------------------------------------------------------------
}	// SUITE

#endif	// CICSMT_ENABLED

