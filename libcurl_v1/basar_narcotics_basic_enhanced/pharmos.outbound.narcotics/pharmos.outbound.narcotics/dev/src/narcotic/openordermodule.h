//-------------------------------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  DomainModule Original
 *  \author Roland Kiefert
 *  \date   23.03.2006
 */
//-------------------------------------------------------------------------------------------------------------------------//
#ifndef GUARD_OPENORDERMODULE_H
#define GUARD_OPENORDERMODULE_H

//-------------------------------------------------------------------------------------------------------------------------//
// namespace section
//-------------------------------------------------------------------------------------------------------------------------//
namespace domMod 
{

//------------------------------------------------------------------------------------------------//
/*! \brief	class handles open orders for goods intake.

	\n This is a final class.  
	\n throws no-throw */
//-------------------------------------------------------------------------------------------------------------------------//
// class OpenOrderDM
//-------------------------------------------------------------------------------------------------------------------------//
class OpenOrderDM
{
public:
	OpenOrderDM();
	/*! \brief	method initializes the AccessorInstanceRef and counts up the number of init processes. \n no-throw */	
	void			init						( const basar::Int16 branchNo, const basar::db::aspect::ConnectionRef activeConn );

	/*! \brief	method counts down the number of init processes and after the last init process it resets the AccessorInstanceRef. 
				\n no-throw */	
	void			shutdown					();

	//-------------------------------------------------------------------------------------------------------------------------//
	//	find - methods
	//-------------------------------------------------------------------------------------------------------------------------//
	/*! \brief	method searches for all open orders with search criteria of the passed AccessorPropertyTable_YIterator. 
				\n no-throw */	
	basar::db::aspect::AccessorPropertyTableRef	findAllOpenOrderByPattern( basar::db::aspect::AccessorPropertyTable_YIterator yit );
	
	//-------------------------------------------------------------------------------------------------------------------------//
	//	delete - methods
	//-------------------------------------------------------------------------------------------------------------------------//
	/*! \brief	method deletes an open order referenced by a purchaseorderno. \n no-throw */	
	basar::db::aspect::ExecuteResultInfo deleteOpenOrder( basar::db::aspect::AccessorPropertyTable_YIterator yit );

	//-------------------------------------------------------------------------------------------------------------------------//
	//	get - methods
	//-------------------------------------------------------------------------------------------------------------------------//
	/*! \brief	returns the PropertyTable of the sole AccessorInstanceRef. \n no-throw */	
	basar::db::aspect::AccessorPropertyTableRef	getAccOpenOrderPropTab();
	
private:
	//AccessorInstanceRef ;
	basar::db::aspect::AccessorInstanceRef		m_accNarOpenOrder;		//!< instance of open order accessor
	basar::Int32								m_Initialized;			//!< two meanings: a) to find out if the module is initialized at all and b) the method 'shutdown' is only run completly through if the module is not referenced any more.
};

} //namespace  domMod
#endif //GUARD_OPENORDERMODULE_H