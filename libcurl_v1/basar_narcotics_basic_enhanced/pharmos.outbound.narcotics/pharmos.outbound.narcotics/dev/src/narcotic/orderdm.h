//-------------------------------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  
 *  \author Harald Bayer
 *  \date   02.08.2012
 */
//-------------------------------------------------------------------------------------------------------------------------//
#ifndef GUARD_ORDERDM_H
#define GUARD_ORDERDM_H

//-------------------------------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------------------------------//
#include "logmodule.h"
#include <lockmanager/branchlockmanager.h>

//-------------------------------------------------------------------------------------------------------------------------//
// namespace section
//-------------------------------------------------------------------------------------------------------------------------//
namespace domMod {

//------------------------------------------------------------------------------------------------//
/*! \brief	class handles open orders for goods intake.

	\n This is a final class.  
	\n throws no-throw */
//-------------------------------------------------------------------------------------------------------------------------//
// class OpenOrderDM
//-------------------------------------------------------------------------------------------------------------------------//
class OrderDM
{
public:
	OrderDM();
	/*! \brief	method initializes the AccessorInstanceRef and counts up the number of init processes. \n no-throw */	
    void			init						( const basar::Int16 branchNo, const basar::db::aspect::ConnectionRef activeConn, boost::shared_ptr<domMod::LogDM> logdm, const basar::VarString& userName );

	/*! \brief	method counts down the number of init processes and after the last init process it resets the AccessorInstanceRef. 
				\n no-throw */	
	void										shutdown();

	const bool									isInitialized();

	//-------------------------------------------------------------------------------------------------------------------------//
	//	delete - methods
	//-------------------------------------------------------------------------------------------------------------------------//
	/*! \brief	method deletes an open order referenced by a purchaseorderno. \n no-throw */	
	bool										deleteOrder( basar::db::aspect::AccessorPropertyTable_YIterator yit );

	///*! \brief	method deletes several order positions in a certain area within a AccessorPropertyTable. 
	//			Every position to delete is referenced by a "1" in the selection column. \n no-throw */	
	bool										cancelSelectedOrderPositions( basar::db::aspect::AccessorPropertyTableRef orderPositions );

	///*! \brief	method deletes all open orders within a AccessorPropertyTable. 
	//			Every open order is referenced by a purchaseorderno. \n no-throw */	
	//bool										deleteOrder( basar::db::aspect::AccessorPropertyTableRef propTab );

	bool										setOrderStatusToForPrint( basar::db::aspect::AccessorPropertyTable_YIterator order );
	//-------------------------------------------------------------------------------------------------------------------------//
	//	get - methods
	//-------------------------------------------------------------------------------------------------------------------------//
	/*! \brief	returns the PropertyTable of the sole AccessorInstanceRef. \n no-throw */	
	basar::db::aspect::AccessorPropertyTableRef	getAccOpenOrderPropTab				();

	basar::db::aspect::AccessorPropertyTableRef	getAccOpenOrderPositionsPropTab		();
	
private:

	const bool									cancelOrderPosition( basar::db::aspect::AccessorPropertyTable_YIterator orderPosition);

	const bool									doStockUpdate( basar::db::aspect::AccessorPropertyTable_YIterator orderPosition );

	basar::db::aspect::AccessorPropertyTable_YIterator getPositionsForOrder(basar::db::aspect::AccessorPropertyTable_YIterator order);

	basar::db::aspect::AccessorInstanceRef		m_accNarOrder;				//!< instance of open order accessor
	basar::db::aspect::AccessorInstanceRef		m_accNarOrderPositions;		//!< instance of open order accessor

	basar::db::aspect::ConnectionRef			m_ActiveConn;

	basar::Int16								m_BranchNo;				//!< branch number 
	basar::Int32								m_Initialized;			//!< two meanings: a) to find out if the module is initialized at all and b) the method 'shutdown' is only run completly through if the module is not referenced any more.
    boost::shared_ptr<domMod::LogDM>			m_LogDM;

	boost::shared_ptr<libnarcotics::lockmanager::BranchLockManager>	m_LockManager;
};

} //namespace domMod 

#endif //GUARD_ORDERDM_H