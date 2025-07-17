#ifndef GUARD_ISTATEMENTS_H
#define GUARD_ISTATEMENTS_H

//------------------------------------------------------------------------------
//namespaces
//------------------------------------------------------------------------------
namespace libnarcotics {
namespace lockmanager {

//------------------------------------------------------------------------------

class IStatements
{
public:
	virtual basar::VarString getStmtSelectLockByUser() = 0;
	virtual basar::VarString getStmtSelectAllLocksByUser() = 0;
	virtual basar::VarString getStmtInsertLock() = 0;
	virtual basar::VarString getStmtDeleteLock() = 0;
	virtual basar::VarString getStmtSelectForRenumber() = 0;
	virtual basar::VarString getStmtUpdateForRenumber() = 0;
	virtual basar::VarString getStmtDeleteAllLocksByBranch() = 0;
	virtual basar::VarString getStmtDeleteAllLocksByBusinessType() = 0;
	virtual basar::VarString getStmtDeleteAllLocksByBusinessTypeSpecialUserExcluded() = 0;
	virtual basar::VarString getStmtDeleteAllLocksByBusinessTypeAndUser() = 0;
	virtual basar::VarString getStmtDeleteAllLocksByBranchBusinessTypeAndUser() = 0;
};

//------------------------------------------------------------------------------

} //namespace lockmanager
} //namespace libnarcotics

//------------------------------------------------------------------------------

#endif //GUARD_ISTATEMENTS_H

//------------------------------------------------------------------------------
