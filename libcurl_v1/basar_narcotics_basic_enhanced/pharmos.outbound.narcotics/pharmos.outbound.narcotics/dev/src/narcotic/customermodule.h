//----------------------------------------------------------------------------
/*! \file
 *  \brief  main interface for domain class customer
 *  \author Roland Kiefert
 *  \date   03.04.2006
 */
//----------------------------------------------------------------------------
#ifndef GUARD_CUSTOMERMODULE_H
#define GUARD_CUSTOMERMODULE_H

//-------------------------------------------------------------------------------------------------//
// namespaces
//-------------------------------------------------------------------------------------------------//
namespace domMod
{

//---------------------------------------------------------------------------
/*! \brief  domain class for customer module.

	\n This is a final class.  
	\n throws no-throw */
class CustomerDM
{

public:
	/*!\brief Default constructor.
		\n no-throw */
	CustomerDM();
	/*!\brief Default destructor.
		\n no-throw */
	virtual ~CustomerDM();
	/*! \brief This method init the module customerModule
		\n no-throw */
	void												init	( const basar::Int16 branchNo,						//!< branch number
																  const basar::db::aspect::ConnectionRef activeConn	//!< connection
																);

	void												shutdown();

	basar::db::aspect::AccessorPropertyTable_YIterator	findCustomerByPattern( basar::db::aspect::AccessorPropertyTable_YIterator yit //!< yiterator, containing where-statement
														);
	basar::db::aspect::AccessorPropertyTable_YIterator	findCustomerByPattern( basar::cmnutil::ParameterList where //!< parameter-list, containing where-statement
														);	
	basar::db::aspect::AccessorPropertyTable_YIterator	findCustomerForReprint( basar::db::aspect::AccessorPropertyTable_YIterator yit //!< yiterator, containing where-statement
														);
	basar::db::aspect::AccessorPropertyTableRef			findCustomerNoByName( const basar::db::aspect::AccessorPropertyTable_YIterator yit //!< param: given customerName for searching
														);
	basar::db::aspect::AccessorPropertyTableRef			findCustomerMapping( const basar::db::aspect::AccessorPropertyTable_YIterator yit
														);
	basar::db::aspect::ExecuteResultInfo				deleteCustomerMapping( const basar::db::aspect::AccessorPropertyTable_YIterator yit 
														);
	basar::db::aspect::ExecuteResultInfo				saveCustomerMapping( const basar::db::aspect::AccessorPropertyTable_YIterator yit 
														);
	basar::db::aspect::AccessorPropertyTableRef			getAccCustomerPropTab();
	
	basar::db::aspect::AccessorPropertyTable_YIterator	addEmptyCustomer();

	basar::db::aspect::AccessorPropertyTableRef			getAccCustMappingPropTab();
	
	basar::db::aspect::AccessorPropertyTable_YIterator	addEmptyCustMapping();

private:
	basar::db::aspect::AccessorInstanceRef				m_accCustomer;	
	basar::db::aspect::AccessorInstanceRef				m_accCustMapping;

	basar::Int32										m_Initialized;
};

} //namespace domMod

#endif //GUARD_CUSTOMERMODULE_H