#ifndef GUARD_BRANCHLOCKMANAGER_H
#define GUARD_BRANCHLOCKMANAGER_H

//-------------------------------------------------------------------------------------------------//
// includes
//-------------------------------------------------------------------------------------------------//
#include "lockmanager.h"

//------------------------------------------------------------------------------
//namespaces
//------------------------------------------------------------------------------
namespace libnarcotics {
namespace lockmanager {

//-------------------------------------------------------------------------------------------------//
class BranchLockManager : public LockManager
{
public:
	BranchLockManager(
		const basar::Int16 branchNo,
		const basar::db::aspect::ConnectionRef activeConn,
		const basar::VarString& userName,
		const basar::VarString& tableName,
		const log4cplus::Logger& logger);
	virtual ~BranchLockManager();
	
	using LockManager::releaseLock; //avoids solaris compiler errors
	bool releaseLock(const basar::VarString& businessType, const basar::Int32 idBusinessObject);

	using LockManager::requestLock;
	bool requestLock(const basar::VarString& businessType, const basar::Int32 idBusinessObject, const basar::Int16 maximumCounterValue /*= 1*/);

	using LockManager::releaseAllLocksByUser;
	bool releaseAllLocksByUser();

	using LockManager::releaseAllLocksByBranch;
	bool releaseAllLocksByBranch();

	using LockManager::releaseAllLocksByBusinessType;
	bool releaseAllLocksByBusinessType(const basar::VarString& businessType);

	using LockManager::releaseAllLocksByBusinessTypeSpecialUserExcluded;
	bool releaseAllLocksByBusinessTypeSpecialUserExcluded(const basar::VarString& businessType);
    
    using LockManager::releaseAllLocksByBranchBusinessTypeAndUser;
	bool releaseAllLocksByBranchBusinessTypeAndUser(const basar::VarString& businessType);

private:
	//member variables
	basar::Int16	m_BranchNo;

	BranchLockManager& operator= (BranchLockManager&);
	BranchLockManager(const BranchLockManager &);
};

//------------------------------------------------------------------------------

} //namespace lockmanager
} //namespace libnarcotics

//------------------------------------------------------------------------------

#endif //GUARD_BRANCHLOCKMANAGER_H 

//------------------------------------------------------------------------------
