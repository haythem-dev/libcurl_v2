#ifndef GUARD_ARTICLEMODULE_H
#define GUARD_ARTICLEMODULE_H
//-----------------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  main interface for domain class article
 *  \author Roland Kiefert
 *  \date   03.04.2006
 */
//-----------------------------------------------------------------------------------------------------------//

//-----------------------------------------------------------------------------------------------------------//
// namespace section
//-----------------------------------------------------------------------------------------------------------//
namespace domMod
{

//-----------------------------------------------------------------------------------------------------------//
/*! \brief  domain class for article module.

	\n This is a final class.  
	\n throws no-throw */
//-----------------------------------------------------------------------------------------------------------//
//	class ArticleDM
//-----------------------------------------------------------------------------------------------------------//
class ArticleDM
{
public:
	ArticleDM();
	
	/*! \brief	method initializes the AccessorInstanceRef, counts up the number of init processes. \n no-throw */	
	void												init( const basar::Int16 branchNo, const basar::db::aspect::ConnectionRef activeConn	//!< connection
															);

	/*! \brief	method counts down the number of init processes and after the last init process it resets the AccessorInstanceRef. 
				\n no-throw */	
	void												shutdown();

	//-------------------------------------------------------------------------------------------------------------------------//
	//	find - methods
	//-------------------------------------------------------------------------------------------------------------------------//
	/*! \brief	method searches a central article dataset (table 'artikelzentral') with the search 
				criteria contained in the passed parameter AccessorPropertyTable_YIterator. \n no-throw */
	basar::db::aspect::AccessorPropertyTable_YIterator	findArticleByPattern( basar::db::aspect::AccessorPropertyTable_YIterator yit //!< yiterator, containing where-statement
														);

	basar::Int32										findArticleNoByArticleCode( basar::db::aspect::AccessorPropertyTable_YIterator yit );
	
private:
	basar::db::aspect::AccessorInstanceRef				m_accArticle;	//!< instance of article-accessor
	basar::Int32										m_Initialized;	//!< two meanings: a) to find out if the module is initialized at all and b) the method 'shutdown' is only run completly through if the module is not referenced any more.
};

} //namespace domMod
#endif //GUARD_ARTICLEMODULE_H