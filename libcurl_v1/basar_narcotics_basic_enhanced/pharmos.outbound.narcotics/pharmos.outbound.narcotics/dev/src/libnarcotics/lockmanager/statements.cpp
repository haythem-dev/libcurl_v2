//------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  wrapper object for sql statements
 *  \author Marco Köppendörfer
 *  \date   26.06.2012
 */
//------------------------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------------------------//
// includes
//-------------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "statements.h"

//------------------------------------------------------------------------------
//namespaces
//------------------------------------------------------------------------------
namespace libnarcotics {
namespace lockmanager {

//------------------------------------------------------------------------------
//statics
//------------------------------------------------------------------------------
	basar::ConstString Statements::m_stmtSelectLockByUserRaw = "select businesstype, idbusinessobject, counter from %s where username = ? and branchno = ? and businesstype = ? and idbusinessobject = ?";

	basar::ConstString Statements::m_stmtSelectAllLocksByUserRaw = "select branchno, businesstype, idbusinessobject, username, counter from %s where username = ?";

	basar::ConstString Statements::m_stmtInsertLockRaw = "insert into <tblName> (branchno, username, businesstype, idbusinessobject, lockdate, locktime, counter) "
			"select %d, '%s', '%s', %d, %d, %d, (select count(*) from <tblName> where branchno = %d and businesstype = '%s' and idbusinessobject = %d) +1 from systables where tabid = 1 "
			"and %d > (select count(*) from <tblName> where branchno = %d and businesstype = '%s' and idbusinessobject = %d)";

	basar::ConstString Statements::m_stmtDeleteLockRaw = "delete from %s where username = ? and businesstype = ? and idbusinessobject = ? and branchno = ? and counter = ?";

	basar::ConstString Statements::m_stmtSelectForRenumberRaw = "select counter, branchno, businesstype, idbusinessobject, username from %s where businesstype = ? and idbusinessobject = ? and branchno = ? for update";

	basar::ConstString Statements::m_stmtUpdateRenumberRaw = "update %s set counter = ? where branchno = ? and businesstype = ? and idbusinessobject = ? and username = ? and counter = ?";

	basar::ConstString Statements::m_stmtDeleteAllLocksByBranchRaw = "delete from %s where branchno = ?";

	basar::ConstString Statements::m_stmtDeleteAllLocksByBusinessTypeRaw = "delete from %s where branchno = ? and businesstype = ?";

	basar::ConstString Statements::m_stmtDeleteAllLocksByBusinessTypeSpecialUserExcludedRaw = "delete from %s where branchno = ? and businesstype = ? and username != 'narc.transfer' "
																								"and ( lockdate < CAST(TO_CHAR(CURRENT, '%%Y%%m%%d') AS INTEGER) OR "
																								"( lockdate = CAST(TO_CHAR(CURRENT, '%%Y%%m%%d') AS INTEGER)  "
																								"AND locktime < CAST(TO_CHAR(CURRENT - INTERVAL(00:05) HOUR TO MINUTE, '%%H%%M%%S') AS INTEGER) * 1000)) ";

	basar::ConstString Statements::m_stmtDeleteAllLocksByBusinessTypeAndUserRaw = "delete from %s where businesstype = ? and username = ?";

	basar::ConstString Statements::m_stmtDeleteAllLocksByBranchBusinessTypeAndUserRaw = "delete from %s where branchno = ? and businesstype = ? and username = ?";

	//-------------------------------------------------------------------------------------------------//
	Statements::Statements(const basar::VarString& tableName)
	{
		m_stmtSelectLockByUser				.format(m_stmtSelectLockByUserRaw, tableName.c_str());

		m_stmtSelectAllLocksByUser			.format(m_stmtSelectAllLocksByUserRaw, tableName.c_str());

		m_stmtDeleteLock					.format(m_stmtDeleteLockRaw, tableName.c_str());

		m_stmtSelectForRenumber				.format(m_stmtSelectForRenumberRaw, tableName.c_str());

		m_stmtUpdateRenumber				.format(m_stmtUpdateRenumberRaw, tableName.c_str());

		m_stmtDeleteAllLocksByBranch		.format(m_stmtDeleteAllLocksByBranchRaw, tableName.c_str());

		m_stmtDeleteAllLocksByBusinessType	.format(m_stmtDeleteAllLocksByBusinessTypeRaw, tableName.c_str());

		m_stmtDeleteAllLocksByBusinessTypeSpecialUserExcluded
											.format(m_stmtDeleteAllLocksByBusinessTypeSpecialUserExcludedRaw, tableName.c_str());

		m_stmtDeleteAllLocksByBusinessTypeAndUser .format(m_stmtDeleteAllLocksByBusinessTypeAndUserRaw, tableName.c_str());

		m_stmtDeleteAllLocksByBranchBusinessTypeAndUser.format(m_stmtDeleteAllLocksByBranchBusinessTypeAndUserRaw, tableName.c_str());

		//insert must be handled special
		m_stmtInsertLock = m_stmtInsertLockRaw;
		m_stmtInsertLock.exchange("<tblName>", tableName.c_str());

	}

	//-------------------------------------------------------------------------------------------------//
	Statements::~Statements()
	{
	}

	//-------------------------------------------------------------------------------------------------//
	basar::VarString Statements::getStmtDeleteAllLocksByBusinessTypeAndUser()
	{
		return m_stmtDeleteAllLocksByBusinessTypeAndUser;
	}

	//-------------------------------------------------------------------------------------------------//
	basar::VarString Statements::getStmtSelectLockByUser()
	{
		return m_stmtSelectLockByUser;
	}

	//-------------------------------------------------------------------------------------------------//
	basar::VarString Statements::getStmtSelectAllLocksByUser()
	{
		return m_stmtSelectAllLocksByUser;
	}

	//-------------------------------------------------------------------------------------------------//
	basar::VarString Statements::getStmtInsertLock()
	{
		return m_stmtInsertLock;
	}

	//-------------------------------------------------------------------------------------------------//
	basar::VarString Statements::getStmtDeleteLock()
	{
		return m_stmtDeleteLock;
	}

	//-------------------------------------------------------------------------------------------------//
	basar::VarString Statements::getStmtSelectForRenumber()
	{
		return m_stmtSelectForRenumber;
	}

	//-------------------------------------------------------------------------------------------------//
	basar::VarString Statements::getStmtUpdateForRenumber()
	{
		return m_stmtUpdateRenumber;
	}

	//-------------------------------------------------------------------------------------------------//
	basar::VarString Statements::getStmtDeleteAllLocksByBranch()
	{
		return m_stmtDeleteAllLocksByBranch;
	}

	//-------------------------------------------------------------------------------------------------//
	basar::VarString Statements::getStmtDeleteAllLocksByBusinessType()
	{
		return m_stmtDeleteAllLocksByBusinessType;
	}

	//-------------------------------------------------------------------------------------------------//
	basar::VarString Statements::getStmtDeleteAllLocksByBusinessTypeSpecialUserExcluded()
	{
		return m_stmtDeleteAllLocksByBusinessTypeSpecialUserExcluded;
	}	

	//-------------------------------------------------------------------------------------------------//
	basar::VarString Statements::getStmtDeleteAllLocksByBranchBusinessTypeAndUser()
	{
		return m_stmtDeleteAllLocksByBranchBusinessTypeAndUser;
	}

//------------------------------------------------------------------------------

} //namespace lockmanager
} //namespace libnarcotics

//------------------------------------------------------------------------------
