#ifndef GUARD_LIBBASARDBASPECT_MANAGER_H
#define GUARD_LIBBASARDBASPECT_MANAGER_H
//---------------------------------------------------------------------------------------//
/*! \file
 *  \brief  public interface manager
 *  \author Roland Kiefert
 *  \date   18.07.2005
 */
//---------------------------------------------------------------------------------------//

//---------------------------------------------------------------------------------------//
// dll imports and exports
//---------------------------------------------------------------------------------------//
#ifndef LIBBASARDBASPECT_API
	#ifdef _WIN32
		#ifdef LIBBASARDBASPECT_EXPORTS
			#define LIBBASARDBASPECT_API __declspec(dllexport)	//!< dll exported
		#else
			#define LIBBASARDBASPECT_API __declspec(dllimport)	//!< dll imported
		#endif 
	#else 
		#define LIBBASARDBASPECT_API
	#endif 
#endif 

//---------------------------------------------------------------------------------------//
// includes 
//---------------------------------------------------------------------------------------//
#include "libbasardbsql_definitions.h"
#include "libbasardbaspect_definitions.h"
#include "libbasardbaspect_connectionref.h"

//---------------------------------------------------------------------------------------//
// namespaces
//---------------------------------------------------------------------------------------//
namespace basar  {
namespace db     {
	namespace sql
	{
		class ConnectionList_Iterator;
	}
namespace aspect {

//---------------------------------------------------------------------------------------//
// forward declaration
//---------------------------------------------------------------------------------------//
class AccessorDefinitionRef;
class AccessorInstanceRef;
class AccessorDefinitionListRef;
class AccessorInstanceListRef;
class AccessorPropertyTable_YIterator;
class ManagerImpl;

//---------------------------------------------------------------------------------------//
/*! \brief  Base class for manager.

	This is a final class Singleton */
//---------------------------------------------------------------------------------------//
// class implementation ManagerBase
//---------------------------------------------------------------------------------------//
class ManagerBase
{
	friend class basar::cmnutil::Singleton<ManagerBase>; //!< friend for calling create-methods

public:

	typedef db::sql::ConnectionList_Iterator ConnectionList_Iterator;	//!< iterator to list of connections
	typedef db::sql::ConnectionListSize		 ConnectionListSize;		//!< number of connections in list

	/*! \brief The method checks if the chosen connection is open. 
 		\n     no-throw */
	LIBBASARDBASPECT_API bool				isOpen( const ConnectionList_Iterator it ) const;
	/*! \brief The method returns an iterator to the first element of the connection list. 
 		\n     no-throw */
	LIBBASARDBASPECT_API  
	ConnectionList_Iterator					beginConn()		const;	
	
	/*! \brief The method returns an iterator to the last element of the connection list. 
 		\n     no-throw */
	LIBBASARDBASPECT_API 
	ConnectionList_Iterator					endConn()		const;

	/*! \brief	The method returns the size of the connection list. \n no-throw */
	LIBBASARDBASPECT_API 
	ConnectionListSize						sizeConn()		const;
	/*! \brief	The method returns the number of all open connections in CollConnection. \n no-throw */
	LIBBASARDBASPECT_API
	ConnectionListSize						sizeOpenConn()	const;	

	/*! \brief	The method returns the database info to given iterator and sets connId to connections unique id 
 				\n OutOfRangeIteratorException */
	LIBBASARDBASPECT_API
	const ConnectionRef::DatabaseInfo & 	getConnInfo( const ConnectionList_Iterator it, VarString& connId ) const;

	/*! \brief The method establishes a connection to given database info.
 		\n     NullReferenceException */
	LIBBASARDBASPECT_API 
	ConnectionRef							createConnect( const ConnectionRef::DatabaseInfo & rDbInfo, 
														   bool                                openConn = true );
	/*! \brief The method creates a reference of an accessor instance and returns it. 
 		\n     UnknownAccessorDefinitonException, ExistAccessorInstanceException, NullReferenceException */
	LIBBASARDBASPECT_API 
	AccessorInstanceRef						createAccessorInstance( const AccessorName    & insName, 
																	const AccessorName    & defName, 
																	const ConnectionRef     connRef, 
																	const CachingPolicyEnum eCachePolicy        = ON_DEMAND_CACHING,
																	bool                    storeInInstanceList = false); 
	
	/*! \brief The method returns the created reference to an accessor definition. 
 		\n     ExistAccessorDefinitionException, NullReferenceException */
	LIBBASARDBASPECT_API 
	AccessorDefinitionRef					createAccessorDefinition(const AccessorName& rName); 

	/*! \brief	The method returns the created reference to an accessorpropertytable yiterator. 
		\n no-throw */
	LIBBASARDBASPECT_API
	AccessorPropertyTable_YIterator			createAccessorYIterator( const basar::cmnutil::ParameterList& rParaList ); 

	/*! \brief The method returns a reference to an accessor definition. 
 		\n     NullReferenceException */
	LIBBASARDBASPECT_API 
	AccessorDefinitionRef					getAccessorDefinition(const AccessorName& rName); 

	/*! \brief The method returns a reference to an accessor instance. 
 		\n     UnknownAccessorInstanceException, NullReferenceException */
	LIBBASARDBASPECT_API 
	AccessorInstanceRef						getAccessorInstance(const AccessorName& rName); 

	/*! \brief The method deletes the reference to this accessor definition from manager. 
 		\n     UnknownAccessorInstanceException */
	LIBBASARDBASPECT_API 
	void									releaseAccessorDefinition(const AccessorName& rName); 

	/*! \brief The method deletes the reference to this accessor instance from manager. 
 		\n     UnknownAccessorInstanceException */
	LIBBASARDBASPECT_API 
	void									releaseAccessorInstance(const AccessorName& rName); 

	/*! \brief The method returns a list of accessor definitions. 
 		\n     NullReferenceException */
	LIBBASARDBASPECT_API 
	AccessorDefinitionListRef				getAccessorDefinitionList();

	/*! \brief The method returns a list of accessor instances.
 		\n     NullReferenceException */
	LIBBASARDBASPECT_API 
	AccessorInstanceListRef					getAccessorInstanceList();
	
	/*! \brief The method returns if an accessor definition is known by the manager
	    \n     NullReferenceException */
	LIBBASARDBASPECT_API 
	bool					                hasAccessorDefinition(const AccessorName& rName) const;

	/*! \brief The method returns if an accessor instance is stored in manager's instance list
	    \n     NullReferenceException */
	LIBBASARDBASPECT_API 
	bool					                hasAccessorInstance(const AccessorName& rName) const;

	
private:
	/*! \brief std-constructor 
 		\n     no-throw */
	LIBBASARDBASPECT_API					ManagerBase();
	
	/*! \brief The method returns the pointer to a manager. 
 		\n     NullReferenceException */
	const boost::shared_ptr<ManagerImpl>&	getImpl() const;

	boost::shared_ptr<ManagerImpl>			m_pManager; //!< pointer to manageraccessMethod
	

	
};

//---------------------------------------------------------------------------------------//
/*! \brief  derived class from ManagerBase.

	This is a final class. Singleton 
 *  \note  class is necessary for dll exporting Singleton\<ManagerBase\> and 
 *         instantiating Manager::m_Instance */
//-----------------------------------------------------------------------------------------------------//
// class Manager
//-----------------------------------------------------------------------------------------------------//
class LIBBASARDBASPECT_API Manager : public basar::cmnutil::Singleton<ManagerBase>
{
private:
									/*! \brief standard constructor 
 										\n     no-throw */
									Manager    		();
									/*! \brief copy constructor 
 										\n     no-throw */
									Manager    		(const Manager     & r);
									/*! \brief assign operator 
 									\n     no-throw */
	Manager &    					operator =		(const Manager     & r);
};

//---------------------------------------------------------------------------------------//
} // aspect
} // db
} // basar

//---------------------------------------------------------------------------------------//
#endif	// GUARD
