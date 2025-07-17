//----------------------------------------------------------------------------
/*
 *  \author Michael Eichenlaub
 *  \date   04.08.2010
*/
//----------------------------------------------------------------------------

#include <UnitTest++/UnitTest++.h>
#include "libbasardbsql.h"

#include "testconnection.h"

//------------------------------------------------------------------------------
SUITE(DatabaseInfo)
{

//------------------------------------------------------------------------------
using basar::ConstString;
using basar::Int32;
using basar::db::sql::DatabaseInfo;
using basar::db::sql::ConnectionRef;

using basar::test::unit::dbsql::TestConnection;

//------------------------------------------------------------------------------

#ifdef HAS_CICS

static ConstString CICS_CONFIG			= CICS_CONFIG_WEST;
static ConstString CICS_STARTTRANS      = CICS_START;
static ConstString CICS_REMOTEHOST      = CICS_HOST;
static ConstString CICS_REMOTEPORT      = CICS_PORT;
static ConstString CICS_REMOTEPORT_FAIL = "3103";

static Int32       TIMEOUT              = 5;	// seconds


//------------------------------------------------------------------------------
TEST(CicsAvail_Ok)
{
	DatabaseInfo dbinfo;

	dbinfo.setCICS(CICS_CONFIG, 
				   CICS_REMOTEHOST, 
				   CICS_REMOTEPORT, 
				   CICS_STARTTRANS);

	CHECK(dbinfo.isCicsAvail(TIMEOUT));
}

//------------------------------------------------------------------------------
TEST(CicsAvail_Fail)
{
	DatabaseInfo dbinfo;

	dbinfo.setCICS(CICS_CONFIG, 
				   CICS_REMOTEHOST, 
				   CICS_REMOTEPORT_FAIL, 
				   CICS_STARTTRANS);

	CHECK(!dbinfo.isCicsAvail(TIMEOUT));
}

//------------------------------------------------------------------------------
TEST(CicsAvail_FollowingConn_Ok)
{
	DatabaseInfo dbinfo;

	dbinfo.setCICS(CICS_CONFIG, 
				   CICS_REMOTEHOST, 
				   CICS_REMOTEPORT, 
				   CICS_STARTTRANS);

	CHECK(dbinfo.isCicsAvail(TIMEOUT));

	// test if following connection is corrupt
    ConnectionRef conn = TestConnection::createTestConnection(TestConnection::CICS);
    CHECK(!conn.isOpen());

	conn.open();
    CHECK(conn.isOpen());

	conn.close();
    CHECK(!conn.isOpen());
}

//------------------------------------------------------------------------------
TEST(CicsAvail_FollowingConn_Fail)
{
	DatabaseInfo dbinfo;

	dbinfo.setCICS(CICS_CONFIG, 
				   CICS_REMOTEHOST, 
				   CICS_REMOTEPORT_FAIL, 
				   CICS_STARTTRANS);

	CHECK(!dbinfo.isCicsAvail(TIMEOUT));

	// test if following connection is corrupt
    ConnectionRef conn = TestConnection::createTestConnection(TestConnection::CICS);
    CHECK(!conn.isOpen());

	conn.open();
    CHECK(conn.isOpen());

	conn.close();
    CHECK(!conn.isOpen());
}

#endif
//------------------------------------------------------------------------------
}	// SUITE
