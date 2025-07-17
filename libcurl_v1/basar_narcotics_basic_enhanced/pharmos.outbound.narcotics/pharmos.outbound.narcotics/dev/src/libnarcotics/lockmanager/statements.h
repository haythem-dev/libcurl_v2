//----------------------------------------------------------------------------
/*! \file
 *  \brief  definitions for prepared statements
 *  \author Marco Köppendörfer
 *  \date   21.06.2012
 */
//----------------------------------------------------------------------------

#ifndef GUARD_STATEMENTS_H
#define GUARD_STATEMENTS_H

//-------------------------------------------------------------------------------------------------//
// includes
//-------------------------------------------------------------------------------------------------//
#include "istatements.h"

//------------------------------------------------------------------------------
//namespaces
//------------------------------------------------------------------------------
namespace libnarcotics {
namespace lockmanager {

//-------------------------------------------------------------------------------------------------//
	class Statements : public IStatements
	{
	public:
		Statements(const basar::VarString& tableName);
		
		virtual ~Statements();

		basar::VarString getStmtSelectLockByUser();

		basar::VarString getStmtSelectAllLocksByUser();

		basar::VarString getStmtInsertLock();

		basar::VarString getStmtDeleteLock();

		basar::VarString getStmtSelectForRenumber();

		basar::VarString getStmtUpdateForRenumber();

		basar::VarString getStmtDeleteAllLocksByBranch();

		basar::VarString getStmtDeleteAllLocksByBusinessType();

		basar::VarString getStmtDeleteAllLocksByBusinessTypeSpecialUserExcluded();

		basar::VarString getStmtDeleteAllLocksByBusinessTypeAndUser();

		basar::VarString getStmtDeleteAllLocksByBranchBusinessTypeAndUser();

	private:
		Statements(const Statements &copy);

		Statements& operator=(Statements&);

		basar::VarString m_stmtSelectLockByUser;

		basar::VarString m_stmtSelectAllLocksByUser;

		basar::VarString m_stmtInsertLock;

		basar::VarString m_stmtDeleteLock;

		basar::VarString m_stmtSelectForRenumber;

		basar::VarString m_stmtUpdateRenumber;

		basar::VarString m_stmtDeleteAllLocksByBranch;

		basar::VarString m_stmtDeleteAllLocksByBusinessType;

		basar::VarString m_stmtDeleteAllLocksByBusinessTypeSpecialUserExcluded;

		basar::VarString m_stmtDeleteAllLocksByBusinessTypeAndUser;

		basar::VarString m_stmtDeleteAllLocksByBranchBusinessTypeAndUser;

		static basar::ConstString m_stmtSelectLockByUserRaw;
		
		static basar::ConstString m_stmtSelectAllLocksByUserRaw;

		static basar::ConstString m_stmtInsertLockRaw;

		static basar::ConstString m_stmtDeleteLockRaw;

		static basar::ConstString m_stmtSelectForRenumberRaw;

		static basar::ConstString m_stmtUpdateRenumberRaw;

		static basar::ConstString m_stmtDeleteAllLocksByBranchRaw;

		static basar::ConstString m_stmtDeleteAllLocksByBusinessTypeRaw;

		static basar::ConstString m_stmtDeleteAllLocksByBusinessTypeSpecialUserExcludedRaw;

		static basar::ConstString m_stmtDeleteAllLocksByBusinessTypeAndUserRaw;	

		static basar::ConstString m_stmtDeleteAllLocksByBranchBusinessTypeAndUserRaw;

	};

//------------------------------------------------------------------------------

} //namespace lockmanager
} //namespace libnarcotics

//------------------------------------------------------------------------------

#endif // GUARD_STATEMENTS_H

//------------------------------------------------------------------------------
