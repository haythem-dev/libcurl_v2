//----------------------------------------------------------------------------
/*! \file
 *  \brief  main interface for domain class branch (de: filiale)
 *  \author Bjoern Bischof
 *  \date   29.06.2006
 */
//----------------------------------------------------------------------------
#ifndef GUARD_BRANCHMODULE_H
#define GUARD_BRANCHMODULE_H

//-------------------------------------------------------------------------------------------------//
// namespaces
//-------------------------------------------------------------------------------------------------//
namespace domMod
{

//---------------------------------------------------------------------------
/*! \brief  domain class for branch module.

	\n This is a final class. \n throws no-throw */

class BranchDM
{

public:
	BranchDM();
	/*! \brief This method init the module articleModule. \n no-throw */
	void												init	(	const basar::Int16 branchNo, //!< connection
																	const basar::db::aspect::ConnectionRef activeConn //!< connection
																);

	void												shutdown();

	basar::db::aspect::AccessorPropertyTable_YIterator	findBranch();
	basar::db::aspect::AccessorPropertyTable_YIterator	findBranchAddress();
	basar::db::aspect::AccessorPropertyTableRef			getAccBranchAddressPropTab();

private:
	basar::db::aspect::AccessorInstanceRef				m_accBranch;			//!< instance of branch-accessor
	basar::db::aspect::AccessorInstanceRef				m_accBranchAddress;		//!< instance of branch-accessor for selecting address
	basar::Int32										m_Initialized;
	basar::Int16										m_BranchNo;				//!< branch number 
	basar::db::aspect::AccessorPropertyTable_YIterator	m_yitBranch;
	basar::db::aspect::AccessorPropertyTable_YIterator	m_yitBranchAddress;
	bool												m_Loaded;
};

} //namespace domMod

#endif //GUARD_BRANCHMODULE_H