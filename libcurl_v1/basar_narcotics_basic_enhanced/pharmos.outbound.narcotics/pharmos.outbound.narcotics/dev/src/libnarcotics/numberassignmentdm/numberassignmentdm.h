//----------------------------------------------------------------------------
/*! \file
 *  \brief  
 *  \author  
 *  \date   
 */
//----------------------------------------------------------------------------
#ifndef GUARD_NUMBERASSIGNMENTDM_H
#define GUARD_NUMBERASSIGNMENTDM_H

//---------------------------------------------------------------------------
using basar::db::aspect::AccessorPropertyTable_YIterator;
using basar::db::aspect::AccessorPropertyTableRef;
using basar::db::aspect::AccessorInstanceRef;

//-------------------------------------------------------------------------------------------------------------------------//
// namespace section
//-------------------------------------------------------------------------------------------------------------------------//
namespace domMod
{

//---------------------------------------------------------------------------
/*! \brief  domain class for table nummerek

	\n This is a final class.  
	\n throws no-throw */
class NumberAssignmentDM //: public DomainModuleBase
{
public:
//---------------------------------------------------------------------------
// global functions of NumberAssignmentDM
//---------------------------------------------------------------------------
	/*! \brief std constructor
		\n no-throw */
	NumberAssignmentDM();

	virtual ~NumberAssignmentDM();

	/*! \brief This method inits the module
		\n no-throw */
	void							init					( const basar::db::aspect::ConnectionRef activeConn,	//!< connection
															  const basar::Int16 branchNo							//!< branch number
															);

	//! \brief	resets all AccessorInstanceRef members. \n no-throw
	void							shutdown();

//---------------------------------------------------------------------------
// INTERFACE - METHODS
//---------------------------------------------------------------------------
	basar::Int32					findAndIncrementNoForContext ( const AccessorPropertyTable_YIterator yit	//!< yit containing numbertype + tablename
																);

private:
	/*! \brief copy constructor 
 		\n     no-throw */
	NumberAssignmentDM    				( const NumberAssignmentDM & r );
	/*! \brief assign operator 
	 	\n     no-throw */
	NumberAssignmentDM &    operator =	( const NumberAssignmentDM & r );

	//---------------------------------------------------------------------------
	// accessor instances ( created in init() )
	//---------------------------------------------------------------------------
	basar::db::aspect::AccessorInstanceRef m_NumberACC; //!< instance of accessor for selecting current contributionvoucherno from narcnumber

	basar::VarString m_defNameNumberAssignment;  //!< name of the accessor definition AccNumber
	basar::VarString m_instNameNumberAssignment; //!< name of the accessor instance AccNumber
	  
	basar::VarString m_defaultParameter; //!< the default parameter for the accessor
	
	basar::Int32 m_Initialized;	//!< two meanings: a) to find out if the module is initialized at all and b) the method 'shutdown' is only run completly through if the module is not referenced any more.
};

} //namespace domMod

#endif //GUARD_NUMBERASSIGNMENTDM_H
