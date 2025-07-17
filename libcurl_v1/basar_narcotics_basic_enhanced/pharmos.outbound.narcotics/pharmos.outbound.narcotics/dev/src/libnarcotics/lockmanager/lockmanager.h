//------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  lockmanager class
 *  \author Benjamin Endlicher
 *  \date   21.05.2012
 */
//------------------------------------------------------------------------------------------------//
#ifndef GUARD_LOCKMANAGER_H
#define GUARD_LOCKMANAGER_H

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
class LockManager
{
	
public:
	LockManager(
		const basar::db::aspect::ConnectionRef activeConn,
		const basar::VarString& userName,
		const basar::VarString& tableName,
		const log4cplus::Logger& logger);
	
	virtual ~LockManager();

	virtual bool releaseLock(const basar::Int16 branchNo, const basar::VarString& businessType, const basar::Int32 idBusinessObject);

	virtual bool requestLock(const basar::Int16 branchNo, const basar::VarString& businessType, const basar::Int32 idBusinessObject, const basar::Int16 maximumCounterValue /*= 1*/);

	virtual bool releaseAllLocksByUser(const basar::Int16 branchNo);

	virtual bool releaseAllLocksByBranch(const basar::Int16 branchNo);

	virtual bool releaseAllLocksByBusinessType(const basar::Int16 branchNo, const basar::VarString& businessType);
	
	virtual bool releaseAllLocksByBusinessTypeSpecialUserExcluded(const basar::Int16 branchNo, const basar::VarString& businessType);

	virtual bool releaseAllLocksByBranchBusinessTypeAndUser(const basar::Int16 branchNo, const basar::VarString& businessType);

	virtual bool releaseAllLocksByBusinessTypeAndUser( const basar::VarString& businessType);

private:	
	//member variables
	basar::db::sql::ConnectionRef	m_SqlConnectionRef;			//!< active connection
	basar::VarString				m_UserName;
	boost::shared_ptr<IStatements>	m_Statements;				//!< semi-prepared sql statements, necessary to use lockmanager with different tables
	const log4cplus::Logger&		m_Logger;

	LockManager(const LockManager &);

	LockManager& operator= (LockManager&);

};

//------------------------------------------------------------------------------

} //namespace lockmanager
} //namespace libnarcotics

//------------------------------------------------------------------------------

#endif //GUARD_LOCKMANAGER_H

//------------------------------------------------------------------------------
