#ifndef GUARD_NARCOTICSGROUPMODULE_H
#define GUARD_NARCOTICSGROUPMODULE_H

//-------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  main interface for domain class narcotics group
 *  \author Roland Kiefert
 *  \date   06.03.2006
 */
//-------------------------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------------------------//
// namespace
//-------------------------------------------------------------------------------------------------//
namespace domMod
{

//-------------------------------------------------------------------------------------------------//
// class declaration
//-------------------------------------------------------------------------------------------------//
/*! \brief  domain class for narcotics module.

	\n This is a final class.  
	\n throws no-throw */
//-------------------------------------------------------------------------------------------------//
class NarcoticsGroupDM
{
public:
	/*!\brief Default constructor.
		\n no-throw */
	NarcoticsGroupDM();
	/*!\brief Default destructor.
		\n no-throw */
	virtual ~NarcoticsGroupDM();
	/*! \brief	method initializes the AccessorInstanceRef, counts up the number of init processes. \n no-throw */	
	void													init		( const basar::db::aspect::ConnectionRef activeConn	//!< connection
															);

	/*! \brief	method counts down the number of init processes and after the last init process it resets the AccessorInstanceRef. 
				\n no-throw */	
	void													shutdown	();

	//-------------------------------------------------------------------------------------------------------------------------//
	//	find - methods
	//-------------------------------------------------------------------------------------------------------------------------//
	/*! \brief	method searches all articles for a given narcotic group. \n no-throw */
	basar::db::aspect::AccessorPropertyTableRef				findNarcGroupByPattern( basar::db::aspect::AccessorPropertyTable_YIterator yit //!< searchIterator of NarcoticsDM
															);
	/*! \brief	method searches article specific data concerning narcotic group and 
		\n name for all articles matching the search criteria (a given narcotic name). \n no-throw */
	basar::db::aspect::AccessorPropertyTableRef				findGroupByName(  basar::db::aspect::AccessorPropertyTable_YIterator searchIt //!< searchIterator of NarcoticsDM
															);
	/*! \brief	method searches a narcotic group and for a given article with its branch specific qty in stock. \n no-throw */
	basar::db::aspect::AccessorPropertyTable_YIterator		findStockLocationByArticleNo(	const basar::Int32 articleNo,	//!< the article number 
																							const basar::Int16 branchNo		//!< the branch number 
																						);
	
	//-------------------------------------------------------------------------------------------------------------------------//
	//	get - methods
	//-------------------------------------------------------------------------------------------------------------------------//
	/*! \brief	returns the PropertyTable of the sole AccessorInstanceRef. \n no-throw */
	basar::db::aspect::AccessorPropertyTableRef				getAccNarCatalogPropTab();

private:
	basar::db::aspect::AccessorInstanceRef					m_accNarGroup;	//!< instance of narcotics-group-accessor
	basar::Int32											m_Initialized;	//!< two meanings: a) to find out if the module is initialized at all and b) the method 'shutdown' is only run completly through if the module is not referenced any more.
};

} //namespace domMod

#endif //GUARD_NARCOTICSGROUPMODULE_H