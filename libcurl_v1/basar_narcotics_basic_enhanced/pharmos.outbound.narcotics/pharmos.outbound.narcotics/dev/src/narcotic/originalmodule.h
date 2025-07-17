//-------------------------------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  DomainModule Original
 *  \author Roland Kiefert
 *  \date   23.03.2006
 */
//-------------------------------------------------------------------------------------------------------------------------//
#ifndef GUARD_ORIGINALMODULE_H
#define GUARD_ORIGINALMODULE_H

//-------------------------------------------------------------------------------------------------------------------------//
// namespace section
//-------------------------------------------------------------------------------------------------------------------------//
namespace domMod 
{

//-------------------------------------------------------------------------------------------------------------------------//
/*! \brief	class handles the original narcotic bookings as they are produced by PHARMOS.

	\n This is a final class.  
	\n throws no-throw */
//-------------------------------------------------------------------------------------------------------------------------//
// class OriginalDM
//-------------------------------------------------------------------------------------------------------------------------//
class OriginalDM
{
public:
	OriginalDM();

	/*! \brief	method initializes the AccessorInstanceRef, counts up the number of init processes and creates the search iterator.
				\n no-throw */	
	void init( const basar::Int16 branchNo, const basar::db::aspect::ConnectionRef activeConn );

	/*! \brief	method counts down the number of init processes and after the last init process it resets the AccessorInstanceRef. 
				\n no-throw */	
	void												shutdown();

	/*! \brief returns clears, the search iterator i.e. unsets the properties. \n no-throw */
	void												clearSearchIterator();

	//-------------------------------------------------------------------------------------------------------------------------//
	//	find - methods
	//-------------------------------------------------------------------------------------------------------------------------//
	/*! \brief	method searches for all open orders with search criteria of the passed AccessorPropertyTable_YIterator. 
				\n no-throw */
	basar::db::aspect::AccessorPropertyTableRef			findCatalogByPattern(
															basar::db::aspect::AccessorPropertyTable_YIterator yit //!< yiterator, containing where-statement
														);
	/*! \brief	method searches for all open orders withsearch criteria of the passed ParameterList. 
				\n no-throw */
	basar::db::aspect::AccessorPropertyTableRef			findCatalogByPattern(
															basar::cmnutil::ParameterList where //!< parameter-list, containing where-statement
														);	
	//-------------------------------------------------------------------------------------------------------------------------//
	//	get - methods
	//-------------------------------------------------------------------------------------------------------------------------//
	/*! \brief	returns the PropertyTable of the sole AccessorInstanceRef. \n no-throw */	
	basar::db::aspect::AccessorPropertyTableRef			getAccOrgCatalogPropTab();
	
	/*! \brief returns current the search iterator. \n no-throw */
	basar::db::aspect::AccessorPropertyTable_YIterator	getSearchIterator();

private:
	basar::db::aspect::AccessorInstanceRef				m_accOrgCatalog;	//!< instance of original-booking-accessor
	basar::Int16										m_BranchNo;			//!< branch number 
	basar::db::aspect::AccessorPropertyTable_YIterator	m_searchIterator;	//!< search iterator
	basar::Int32										m_Initialized;		//!< two meanings: a) to find out if the module is initialized at all and b) the method 'shutdown' is only run completly through if the module is not referenced any more.
};

} //namespace domMod
#endif //GUARD_ORIGINALMODULE_H