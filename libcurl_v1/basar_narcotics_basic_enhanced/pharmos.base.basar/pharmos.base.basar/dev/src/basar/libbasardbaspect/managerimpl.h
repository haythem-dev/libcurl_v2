#ifndef GUARD_MANAGER_H
#define GUARD_MANAGER_H
//---------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  internal interface manager
 *  \author Roland Kiefert
 *  \date   18.07.2005
 */
//---------------------------------------------------------------------------------------------------//

//---------------------------------------------------------------------------------------------------//
// includes
//---------------------------------------------------------------------------------------------------//
#include "libbasardbsql_definitions.h"
#include "libbasardbaspect_definitions.h"
#include "libbasardbaspect_accessorpropertytable_yiterator.h"
#include "libbasardbaspect_connectionref.h"

//---------------------------------------------------------------------------------------------------//
// namespace section
//---------------------------------------------------------------------------------------------------//
namespace basar  {
namespace db	 {
	namespace sql
	{
		class ConnectionList_Iterator;
	}
namespace aspect {

//---------------------------------------------------------------------------------------------------//
// forward declaration
//---------------------------------------------------------------------------------------------------//
class AccessorDefinitionListRef;
class AccessorInstanceRef;
class AccessorInstanceListRef;

//---------------------------------------------------------------------------------------------------//
// class declaration
//---------------------------------------------------------------------------------------------------//
/*! \brief  implementation of manager.

	This is a final class.*/
//---------------------------------------------------------------------------------------------------//
class ManagerImpl
{
	friend class ManagerBase; //!< friend for calling create-methods

public:
	//------------------------------------------------------------------//
	// typedef section
	//------------------------------------------------------------------//
	typedef boost::shared_ptr< ManagerImpl > ManagerImplSharedPtr;	//!< shared pointer to AccessMethod
	

	/*! \brief The method creates a ManagerImpl. 
 		\n     no-throw */
	static ManagerImplSharedPtr create();

	//------------------------------------------------------------------//
	// c'tor and d'tor
	//------------------------------------------------------------------//	
	/*! \brief destructor
		\n	   no-throw */
	~ManagerImpl();

	//------------------------------------------------------------------//
	// iterators
	//------------------------------------------------------------------//	
	/*! \brief The method returns an iterator to the first element of the connection list. \n no-throw */
	const basar::db::sql::ConnectionList_Iterator		beginConn	() const;	
	
	/*! \brief The method returns an iterator to the last element of the connection list. \n no-throw */
	const basar::db::sql::ConnectionList_Iterator		endConn		() const;
	
	/*! \brief The method returns an iterator to the last element of the connection list. \n no-throw */
	basar::db::sql::ConnectionListSize			        sizeConn	() const;
	
	//------------------------------------------------------------------//
	// connection specific methods
	//------------------------------------------------------------------//	
	/*! \brief	The method checks if connection to given iterator is open.
 				\n OutOfRangeIteratorException */
	bool				 								isOpen( const sql::ConnectionList_Iterator it ) const;
	/*! \brief	The method returns the number of all open connections in CollConnection. \n no-throw */
	basar::db::sql::ConnectionListSize			        sizeOpenConn() const;	

	/*! \brief	The method returns infos about the current Connection. \n no-throw */
	const ConnectionRef::DatabaseInfo &					getConnInfo( const sql::ConnectionList_Iterator it, 
																	 VarString                        & connId ) const;
	
	//------------------------------------------------------------------//
	// create methods
	//------------------------------------------------------------------//	
	/*! \brief	The Method establishes a connection to given database info. \n no-throw */
	ConnectionRef								createConnect( const ConnectionRef::DatabaseInfo& rDbInfo, 
															   bool openConn = true );

	/*! \brief	The method creates a reference of an accessor instance and returns it. 
				\n UnknownAccessorDefinitonException, ExistAccessorInstanceException */
	AccessorInstanceRef							createAccessorInstance( const AccessorName   & insName, 
																		const AccessorName   & defName, 
																		const ConnectionRef     connRef, 
																		const CachingPolicyEnum eCachePolicy        = ON_DEMAND_CACHING,
																		bool                    storeInInstanceList = false); 
	
	/*! \brief	The method returns the created reference to an accessor definition. 
	    \n      ExistAccessorDefinitionException */
	AccessorDefinitionRef						createAccessorDefinition( const AccessorName& rName ); 
	
	/*! \brief	The method returns the created reference to an accessor yiterator. \n no-throw */
	AccessorPropertyTable_YIterator				createAccessorYIterator( const cmnutil::ParameterList & rParaList ); 
	
	//------------------------------------------------------------------//
	// get methods
	//------------------------------------------------------------------//	
	/*! \brief	The method returns a reference to an accessor definition. \n no-throw */
	AccessorDefinitionRef						getAccessorDefinition( const AccessorName& rName );
	
	/*! \brief	The method returns a reference to an accessor instance. \n UnknownAccessorInstanceException */
	AccessorInstanceRef							getAccessorInstance( const AccessorName& rName );

	/*! \brief The method returns a list of accessor definitions. \n no-throw */
	const AccessorDefinitionListRef				getAccessorDefinitionList();
	/*! \brief The method returns an iterator to the first element of the accessor definition list. \n no-throw */
	CollAccessorDefinitionRefIt					beginDefinition();
	/*! \brief The method returns an iterator to the last element of the accessor definition list. \n no-throw */
	CollAccessorDefinitionRefIt					endDefinition();
	/*! \brief The method returns the size of the accessor definition list. \n no-throw */
	AccessMethodListSize					    sizeDefinition() const;
	
	/*! \brief The method checks if the accessor definition list is empty \n no-throw */
	bool										emptyDefinition() const;
	
	
	/*! \brief The method returns a list of the accessor instances. \n no-throw */
	const AccessorInstanceListRef				getAccessorInstanceList();
	/*! \brief The method returns an iterator to the first element of the accessor instance list. \n no-throw */
	CollAccessorInstanceRefIt					beginInstance();
	/*! \brief The method returns an iterator to the last element of the accessor instance list. \n no-throw */
	CollAccessorInstanceRefIt					endInstance();
	/*! \brief The method returns the size of the accessor instance list. \n no-throw */
	AccessMethodListSize					    sizeInstance() const;
	/*! \brief The method checks if the accessor instance list is empty \n no-throw */
	bool										emptyInstance() const;


	/*! \brief	The method deletes the reference to this accessor definition from manager. \n UnknownAccessorDefinitionException */
	void										releaseAccessorDefinition( const AccessorName& rName ); 
	
	/*! \brief The method deletes the reference to this accessor instance from manager. 
 				\n UnknownAccessorInstanceException */
	void										releaseAccessorInstance( const AccessorName& rName );
	
	
	/*! \brief The method returns if an accessor definition is known by the manager
	    \n no-throw */
	bool					                    hasAccessorDefinition(const AccessorName& rName) const;

	/*! \brief The method returns if an accessor instance is stored in manager's instance list
	    \n no-throw */	
	bool                                        hasAccessorInstance(const AccessorName& rName) const;
	

private:
	/*! \brief returning shared pointer to this by casting weak -> shared
		\n no-throw */
	ManagerImplSharedPtr						getThis( );
	
	
private:
	//------------------------------------------------------------------//
	// typedefs
	//------------------------------------------------------------------//	
	/*! "weak" reference to object (itself) that's already managed by a shared_ptr (ManagerImpl), 
		not visible from the outside, initialized at static create */
	typedef boost::weak_ptr<ManagerImpl>		ManagerImplWeakPtr;	//!< weak pointer to ManagerImpl

	//------------------------------------------------------------------//
	// private methods
	//------------------------------------------------------------------//	
	ManagerImpl					();								//!< forbidden constructor
	ManagerImpl					( const ManagerImpl & rCopy );	//!< forbidden copy constructor
	ManagerImpl & operator =	( const ManagerImpl & rCopy );	//!< forbidden assignment operator

	/*! \brief set member weak pointer.\n no-throw */
	void						setWeakPtr( ManagerImplWeakPtr weakPtr );		

	//------------------------------------------------------------------//
	// private members
	//------------------------------------------------------------------//	
	ManagerImplWeakPtr							m_WeakPtr;					//!< weak pointer to this, needed for common reference group 
	CollAccessorDefinitionRef					m_AccessorDefinitionList;	//!< list of definition refs of an accessor
	CollAccessorInstanceRef						m_AccessorInstanceList;		//!< list of instance refs of an accessor
	AccessorPropertyTable_YIterator				m_NullYit;					//!< empty yiterator
};

//--------------------------------------------------------------------------------------------------
} // aspect
} // db
} // basar

//--------------------------------------------------------------------------------------------------
#endif
