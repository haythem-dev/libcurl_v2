//-------------------------------------------------------------------------------------------------//
// includes
//-------------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "branchlockmanager.h"

//------------------------------------------------------------------------------
//namespaces
//------------------------------------------------------------------------------
namespace libnarcotics {
namespace lockmanager {

//-------------------------------------------------------------------------------------------------//
BranchLockManager::BranchLockManager(
	const basar::Int16 branchNo,
	const basar::db::aspect::ConnectionRef activeConn,
	const basar::VarString& userName,
	const basar::VarString& tableName,
	const log4cplus::Logger& logger
	) :
	LockManager(activeConn, userName, tableName, logger),
	m_BranchNo(branchNo)
{
}

//-------------------------------------------------------------------------------------------------//
bool BranchLockManager::releaseLock(const basar::VarString& businessType, const basar::Int32 idBusinessObject)
{
	return LockManager::releaseLock(this->m_BranchNo, businessType, idBusinessObject);
}

//-------------------------------------------------------------------------------------------------//
bool BranchLockManager::requestLock(const basar::VarString& businessType, const basar::Int32 idBusinessObject, const basar::Int16 maximumCounterValue /*= 1*/)
{
	return LockManager::requestLock(this->m_BranchNo, businessType, idBusinessObject, maximumCounterValue);
}

//-------------------------------------------------------------------------------------------------//
bool BranchLockManager::releaseAllLocksByUser()
{
	return LockManager::releaseAllLocksByUser(this->m_BranchNo);
}

//-------------------------------------------------------------------------------------------------//
bool BranchLockManager::releaseAllLocksByBranch()
{
	 return LockManager::releaseAllLocksByBranch(this->m_BranchNo);
}

//-------------------------------------------------------------------------------------------------//
bool BranchLockManager::releaseAllLocksByBusinessType(const basar::VarString& businessType)
{
	return LockManager::releaseAllLocksByBusinessType(this->m_BranchNo, businessType);
}

//-------------------------------------------------------------------------------------------------//
bool BranchLockManager::releaseAllLocksByBusinessTypeSpecialUserExcluded(const basar::VarString& businessType)
{
	return LockManager::releaseAllLocksByBusinessTypeSpecialUserExcluded(this->m_BranchNo, businessType);
}

//-------------------------------------------------------------------------------------------------//
bool BranchLockManager::releaseAllLocksByBranchBusinessTypeAndUser( const basar::VarString& businessType )
{
	return LockManager::releaseAllLocksByBranchBusinessTypeAndUser(this->m_BranchNo, businessType);
}

//-------------------------------------------------------------------------------------------------//
BranchLockManager::~BranchLockManager()
{
}

//------------------------------------------------------------------------------

} //namespace lockmanager
} //namespace libnarcotics

//------------------------------------------------------------------------------
