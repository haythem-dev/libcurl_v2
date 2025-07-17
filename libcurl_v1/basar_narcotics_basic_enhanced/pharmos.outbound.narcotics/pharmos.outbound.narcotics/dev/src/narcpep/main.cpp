//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------

#include "libbasarcmnutil.h"
#include "libbasardbsql.h"

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------

const basar::VarString MODE_ABBA = "abba";
const basar::VarString MODE_NARC = "narc";

//----------------------------------------------------------------------------

basar::db::sql::ConnectionRef	dbconn;

static basar::Int16			s_BranchNo = 0;
static basar::VarString		s_FromDB;
static basar::VarString		s_ToDB;
static basar::VarString		s_InboundDB;
static basar::VarString		s_Mode;
static basar::Int32			s_FromDate = 0;
bool						s_NarcPEP = false;
bool						s_AbbaPEP = false;

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------

int narcPEP(basar::db::sql::ConnectionRef dbConn, const basar::Int16 branchNo, const basar::VarString& fromDB, const basar::VarString& toDB, const basar::Int32 fromDate = 0);
int abbaPEP(basar::db::sql::ConnectionRef dbConn, const basar::Int16 branchNo, const basar::VarString& fromDB, const basar::VarString& toDB, const basar::VarString& inboundDB);

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------

bool usage(const char* progname)
{
	basar::VarString msg;
	msg.format("usage: %s -branchno <branchno> -mode [narc|abba] -fromdb <fromdb> -todb <todb> [-inbounddb <inbounddb>] [-fromdate <yyyymmdd>] ", progname);
	std::cout << msg << std::endl;
	LOG4CPLUS_ERROR(log4cplus::Logger::getRoot(), msg);
	return false;
}

bool evalCmdLine(basar::Int32 argC, char* argV[])
{
	basar::cmnutil::CmdLineTokenizer cmdline;
	basar::VarString branchno;
	basar::VarString fromdate;

	cmdline.setCmdLine(argC, argV);
	cmdline.getValOfParam("-branchno", branchno);
	s_BranchNo = static_cast<basar::Int16>(branchno.stoi());
	cmdline.getValOfParam("-mode", s_Mode);
	cmdline.getValOfParam("-fromdb", s_FromDB);
	cmdline.getValOfParam("-todb", s_ToDB);
	if (cmdline.isParam("-inbounddb"))
	{
		cmdline.getValOfParam("-inbounddb", s_InboundDB);
	}
	if (cmdline.isParam("-fromdate"))
	{
		cmdline.getValOfParam("-fromdate", fromdate);
		s_FromDate = fromdate.stoi();
	}
	if (MODE_ABBA != s_Mode && MODE_NARC != s_Mode)
		return usage(argV[0]);
	if (s_BranchNo == 0)
		return usage(argV[0]);
	if (s_FromDB.empty())
		return usage(argV[0]);
	if (s_ToDB.empty())
		return usage(argV[0]);
	if (MODE_ABBA == s_Mode && s_InboundDB.empty())
		return usage(argV[0]);
	return true;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------

void connectDB()
{
	basar::cmnutil::CollBString connstr = s_FromDB.tokenize("@");

	basar::db::sql::DatabaseInfo	dbinfo;
	dbinfo.setInfx(connstr.at(1), connstr.at(0));

	dbconn = basar::db::sql::Manager::getInstance().createConnect(dbinfo);
	dbconn.setLockModeWait  (basar::WAITSECS, 20);
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------

void disconnectDB()
{
	dbconn.close();
	dbconn.reset();
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------

int main(int argc, char* argv[])
{
	if (false == evalCmdLine(argc, argv))
		return -1;

	connectDB();
	if (s_Mode == MODE_NARC)
	{
		narcPEP(dbconn, s_BranchNo, s_FromDB, s_ToDB, s_FromDate);
	}
	else if (s_Mode == MODE_ABBA)
	{
		abbaPEP(dbconn, s_BranchNo, s_FromDB, s_ToDB, s_InboundDB);
	}
	disconnectDB();
	return 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
