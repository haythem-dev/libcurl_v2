#ifndef GUARD_LIBBASARDBASPECT_ACCESSOR_H
#define GUARD_LIBBASARDBASPECT_ACCESSOR_H
//----------------------------------------------------------------------------
/*! \file
 *  \brief  public interface accessor
 *  \author Roland Kiefert
 *  \date   18.07.2005
 */
//----------------------------------------------------------------------------

//------------------------------------------------------------------------------//
// dll exports and imports
//------------------------------------------------------------------------------//
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

//------------------------------------------------------------------------------//
// includes
//------------------------------------------------------------------------------//
#include "libbasardbaspect_definitions.h"

//------------------------------------------------------------------------------//
// namespaces
//------------------------------------------------------------------------------//
namespace basar  {
namespace db	 {
namespace aspect {

//------------------------------------------------------------------------------//
// forward declaration
//------------------------------------------------------------------------------//
class AccessorPropertyTableRef;
class AccessorPropertyTable_YIterator;
class AccessMethodListRef;
class AccessMethodRef;
class ManagerImpl;
class ConnectionRef;
class AccessorExecuteListener;
class AccessorExecuteListenerRef;

//------------------------------------------------------------------------------//
// class declaration AccessorInstanceRef
//------------------------------------------------------------------------------//
//////////////////////////////////////////////////////////////////////////////////
/*! \brief  Reference class to an accessor instance.

  This is a final class.
  This is a value class, copying is allowed. The class is generated by the compiler */
//////////////////////////////////////////////////////////////////////////////////
class AccessorInstanceRef
{
	friend class ManagerImpl;
	friend class AccessorDefinitionRef;
public:

	/*! \brief The method constructs reference object and initializes shared pointer to implementation object to null
 		\n     no-throw */
	LIBBASARDBASPECT_API 
	AccessorInstanceRef();
	
	/*! \brief destructor
 		\n     no-throw */
	LIBBASARDBASPECT_API 
	~AccessorInstanceRef();
	
	/*! \brief	The method turns the flag on (true) or off (false). Due to this 
				the internal snapshot is created or not.\n no-throw */
	LIBBASARDBASPECT_API void					switchCreateInternalSnapshot( const bool toCreate ); 


	/*! \brief The method returns a reference to a property description list 
	    \n     NullReferenceException */
	LIBBASARDBASPECT_API 
	const property::PropertyDescriptionListRef	getPropertyDescriptionList() const; 

	/*! \brief The method returns a const reference to a property table/cacher. \n InvalidStateException */
	LIBBASARDBASPECT_API 
	const AccessorPropertyTableRef				getPropertyTable() const ;

	/*! \brief The method returns a reference to a property table/cacher. \n InvalidStateException */
	LIBBASARDBASPECT_API 
	AccessorPropertyTableRef					getPropertyTable();

	/*! \brief The method returns the reference to the accessmethod with this name.
		\n	   If the accessmethod does not exist, this method will create an accessmethod, if it is a definition - accessor.
	    \n     UnknownAccessMethodException, NullReferenceException */
	LIBBASARDBASPECT_API 
	const AccessMethodRef						getAccessMethod( const AccessMethodName& Name ) const;
	
	/*! \brief The method returns a pointer to a list of all accessmethods of this access object
	    \n     NullReferenceException */
	LIBBASARDBASPECT_API
	const AccessMethodListRef					getAccessMethodList() const ;

	/*! \brief The method creates a sql-statement for each stringbuilder in method and executes it on given yIt.
	    \n     successive CICS statements with same accessor instance: preceding statement, resultset is automatically closed
	    \n     throw UnknownPropertyNameException, NullReferenceException */
	LIBBASARDBASPECT_API 
	const ExecuteResultInfo						execute( const AccessMethodName        & name, 
														 AccessorPropertyTable_YIterator yit,
														 bool                            clearBeforeSelect = true, 
														 bool                            flagExcept = true,
                                                         DatabaseHintEnum                dbHint = db::aspect::NO_HINT);
	/*! \brief The method creates a sql-statement for each stringbuilder in method and executes it.
	    \n     successive CICS statements with same accessor instance: preceding statement, resultset is automatically closed
	    \n     throw UnknownPropertyNameException, NullReferenceException */
	LIBBASARDBASPECT_API
	const ExecuteResultInfo						 execute( const AccessMethodName & name,
														 bool                      clearBeforeSelect = true, 
														 bool                      flagExcept = true,
                                                         DatabaseHintEnum          dbHint = db::aspect::NO_HINT);
	/*! \brief The method creates a sql-statement for each stringbuilder in method and executes it between given yIt-begin and yIt-end.
	    \n     successive CICS statements with same accessor instance: preceding statement, resultset is automatically closed
	    \n     no-throw */
	LIBBASARDBASPECT_API
	const ExecuteResultInfo						 execute( const AccessMethodName& name, 
														 AccessorPropertyTable_YIterator yBeg, 
														 AccessorPropertyTable_YIterator yEnd,
														 bool clearBeforeSelect = true, 
														 bool flagExcept = true,
                                                         DatabaseHintEnum dbHint = db::aspect::NO_HINT);
	
	/*! \brief The method creates a sql-aggregate-statement (count*, sum, max, min, avg) with one return value (decimal) 
				for each stringbuilder in method and executes it.
	    \n     successive CICS statements with same accessor instance: preceding statement, resultset is automatically closed
	    \n     throw UnknownPropertyNameException, NullReferenceException */
	LIBBASARDBASPECT_API
	const std::pair<bool, Decimal>				executeAggregate( const AccessMethodName& rName );


	/*! \brief The method creates a sql-aggregate-statement (count*, sum, max, min, avg) with one return value (decimal) 
				for each stringbuilder in method and executes it.
	    \n     successive CICS statements with same accessor instance: preceding statement, resultset is automatically closed
	    \n     throw UnknownPropertyNameException, NullReferenceException */
	LIBBASARDBASPECT_API
	const std::pair<bool, Decimal>				executeAggregate( const AccessMethodName& rName,
																	AccessorPropertyTable_YIterator yit);

	/*! \brief The method sets default parameters for sqlbuilders of given method (default empty -> all methods)
	    \n     throw UnknownMethodNameException */
	LIBBASARDBASPECT_API  void					setDefaultParameter( cmnutil::ParameterList paramList,
																	 const AccessMethodName& rName = "" );
	
	/*! \brief The method sets default parameters for sqlbuilders of given method (default empty -> all methods)
	    \n     throw UnknownMethodNameException */
	LIBBASARDBASPECT_API  void					setDefaultParameter( VarString & rDefParams,
																	 const AccessMethodName& rName = "" );
	
	/*! \brief The method sets default parameters for sqlbuilders of given method (default empty -> all methods)
	    \n     UnknownMethodNameException */
	LIBBASARDBASPECT_API  void					setDefaultParameter( ConstString defParams,
																	 const AccessMethodName& rName = "" );

     /*! \brief The method returns the  default parameters that are set by setDefaultParameter(.., methodname)
	    \n     UnknownMethodNameException */
	LIBBASARDBASPECT_API cmnutil::ParameterList getDefaultParameter(const AccessMethodName& rName= "") const ;

	
	/*! \brief The method returns the name of the instance accessor
		\n no-throw */
	LIBBASARDBASPECT_API 
	const VarString&							getAccessorName() const;
	/*! \brief The method checks if the shared pointer to the implementation class is null
		\n	   no-throw */
	LIBBASARDBASPECT_API
	bool									    isNull() const;
	/*! \brief The method sets the shared pointer to null.
		\n	   no-throw */
	LIBBASARDBASPECT_API 
	void										reset();

	/*! \brief	The method returns a frozen state and values of the current data to reset them later
				\n no-throw */
	LIBBASARDBASPECT_API 
	property::PropertyTableSnapshotRef			createSnapshot();
	
	/*! \brief	The method restores the states and values of data saved earlier
				\n no-throw */
	LIBBASARDBASPECT_API 
	void										resetToSnapshot( property::PropertyTableSnapshotRef snapShot );

	//!	\brief The method indicates whether a transaction is active or not. \n throws NullReferenceException.
	LIBBASARDBASPECT_API bool				    isInTransaction() const;

	//! \brief	The method returns the connectionref. \n no-throw.
	LIBBASARDBASPECT_API const ConnectionRef	getConnection() const;

	//-----------------------------------------------------------------------------------//
	// transaction methods
	//-----------------------------------------------------------------------------------//
	//! \brief	The method starts the transaction. \n NullReferenceException.
	LIBBASARDBASPECT_API void					beginTransaction();
	//! \brief	The method realizes the transaction. \n NullReferenceException.
	LIBBASARDBASPECT_API void					commitTransaction();
	//! \brief	The method cancels the transaction. \n NullReferenceException.
	LIBBASARDBASPECT_API void					rollbackTransaction();

    //-----------------------------------------------------------------------------------//
	// Serial id methods
	//-----------------------------------------------------------------------------------//
   
    //! \brief	returns the last inserted Serial 
    LIBBASARDBASPECT_API Int64 getLastInsertedSerial(Int32 nr);

     //! \brief	are there Serials listed
    LIBBASARDBASPECT_API bool hasLastInsertedSerial();

     //! \brief	returns the count of last inserted Serials 
    LIBBASARDBASPECT_API Int32 getNumberOfLastInsertedSerials();

     //! \brief	clears last inserted Serial  list
    LIBBASARDBASPECT_API void clearLastInsertedSerials();

    //-----------------------------------------------------------------------------------//
	// AccessorExecuteListener methods
	//-----------------------------------------------------------------------------------//
   
    //! \brief	 sets the AccessorExecuteListener
   LIBBASARDBASPECT_API void setAccessorExecuteListener(const AccessorExecuteListener &);

    //! \brief	 removes  the AccessorExecuteListener
   LIBBASARDBASPECT_API void removeAccessorExecuteListener();

    //! \brief	 returns the AccessorExecuteListener
   LIBBASARDBASPECT_API AccessorExecuteListenerRef getAccessorExecuteListener() const;

private:
	/*! \brief The method creates the pointer to the accessor instance class. \n no-throw */
	AccessorInstanceRef( const boost::shared_ptr<IAccessor> pAccessor );
	/*! \brief The method sets the accessorname \n no-throw */
	void										setAccessorName( const VarString name );
	/*! \brief	The method returns the pointer to an accessor.
				\n NullReferenceException */
	const boost::shared_ptr<IAccessor>&			getImpl() const;

	boost::shared_ptr<IAccessor>					m_pAccessor; //!< pointer to accessor 
};
// end class AccessorInstanceRef


/*--------------------------------------------------------------------------------------------------*/
/*! \brief  Reference class to an accessor definition.

  This is a final class.
  This is a value class, copying is allowed. The class is generated by the compiler */

class AccessorDefinitionRef
{
	friend class ManagerImpl;

public:
	/*! \brief The method creates an instance of a definition
	    \n     NullReferenceException */
	LIBBASARDBASPECT_API 
	const AccessorInstanceRef createInstance(	const ConnectionRef connRef, 
												const CachingPolicyEnum eCachePolicy = ON_DEMAND_CACHING ) const;

	/*! \brief The method returns a reference to the PropertyDescriptionList
	    \n     NullReferenceException */
	LIBBASARDBASPECT_API 
	property::PropertyDescriptionListRef					getPropertyDescriptionList(); 

	/*! \brief The method returns the created reference to the accessmethod with this name
	    \n     UnknownAccessMethodException, NullReferenceException */
	LIBBASARDBASPECT_API 
	AccessMethodRef											createAccessMethod( const AccessMethodName& Name );
	/*! \brief The method returns the reference to the accessmethod with this name
	    \n     ExistAccessorMethodException, UnknownAccessMethodException, NullReferenceException */
	LIBBASARDBASPECT_API AccessMethodRef					getAccessMethod(const AccessMethodName& Name);
	/*! \brief The method returns a pointer to a list of all accessmethods of this access object
	    \n     NullReferenceException */
	LIBBASARDBASPECT_API const AccessMethodListRef			getAccessMethodList() const;

	/*! \brief The method constructs the reference object and initializes the shared pointer to the 
		 implementation object to null
 		\n     no-throw */
	LIBBASARDBASPECT_API AccessorDefinitionRef();
	/*! \brief The method returns the name of the accessor definition
		\n no-throw */
	LIBBASARDBASPECT_API const VarString &					getAccessorName() const;
	/*! \brief The method checks if the shared pointer to the implementation class is null 
		\n	   no-throw */
	LIBBASARDBASPECT_API bool								isNull() const;
	/*! \brief The method sets the sharedpointer to null.
		\n	   no-throw */
	LIBBASARDBASPECT_API void								reset();
private:
	/*! \brief The method sets the accessorname
		\n no-throw */
	void													setAccessorName(const VarString name);
	/*! \brief The method creates the pointer of the class accessor definition. 
 		\n     no-throw */
	AccessorDefinitionRef(boost::shared_ptr<IAccessor> pAccessor);
	/*! \brief The method returns the pointer to an accessor. 
 		\n     NullReferenceException */
	const boost::shared_ptr<IAccessor>&						getImpl() const;

	boost::shared_ptr<IAccessor>							m_pAccessor; //!< pointer to accessor 
};

/*--------------------------------------------------------------------------------------------------*/
/*! \brief  Iterator through list of accessor instances

	This is a final class. */
class AccessorInstanceList_Iterator
{
public:
	/*! \brief The method initializes the Iterator with the value of the parameter list. 
 		\n     NullReferenceException */
	LIBBASARDBASPECT_API AccessorInstanceList_Iterator (const boost::shared_ptr<ManagerImpl>, 
														const std::map<AccessorName,AccessorInstanceRef>::iterator);

	/*! \brief standard constructor 
 		\n      no-throw */
	LIBBASARDBASPECT_API AccessorInstanceList_Iterator ();

	/*! \brief The method checks if the internal iterator is equal to rhp. 
 		\n     no-throw */
	LIBBASARDBASPECT_API bool operator == (const AccessorInstanceList_Iterator& rhp) const;
	/*! \brief The method checks if the internal Iterator isn't equal to rhp. 
 		\n     no-throw */
	LIBBASARDBASPECT_API bool operator != (const AccessorInstanceList_Iterator& rhp) const;

	/*! \brief The method returns the next pointer. 
 		\n     OutOfRangeIteratorException */
	LIBBASARDBASPECT_API AccessorInstanceList_Iterator	operator ++ (); 
	/*! \brief The method returns the current pointer and increases it.
 		\n     OutOfRangeIteratorException */
	LIBBASARDBASPECT_API AccessorInstanceList_Iterator	operator ++ (int); 
	/*! \brief The method returns the current pointer
 		\n     OutOfRangeIteratorException */
	LIBBASARDBASPECT_API AccessorInstanceRef			operator*();

	/*! \brief The method checks if the iterator points to begin() of the inner container
		\n     no-throw */
	LIBBASARDBASPECT_API bool					isBegin			() const;

	/*! \brief The method checks if the iterator points to end() of the inner container
		\n     no-throw */
	LIBBASARDBASPECT_API bool					isEnd			() const;

	/*! \brief The method checks if the iterator points to NULL
		\n     no-throw */
	LIBBASARDBASPECT_API bool isNull() const;

	/*! \brief The method sets the iterator points to NULL
		\n     no-throw */
	LIBBASARDBASPECT_API void reset();

private:
	/*! \brief The method checks if the iterator is valid
 		\n     no-throw */
	void checkValid() const;
	std::map<AccessorName,AccessorInstanceRef>::iterator m_It;	//!< the current iterator
	boost::shared_ptr<ManagerImpl> m_Manager;					//!< pointer to the manager 
};

/*--------------------------------------------------------------------------------------------------*/
/*! \brief  A class which holds a list of accessor instances.

	This is a final class. */
class AccessorInstanceListRef
{
public:
	/*! \brief The method initializes an accessor instance list with the values of the parameter list.
 		\n     no-throw */
	LIBBASARDBASPECT_API AccessorInstanceListRef(const boost::shared_ptr<ManagerImpl> Manager );
	/*! \brief The method returns an iterator to the first element of the AccessorInstanceList.
 		\n     no-throw */
	LIBBASARDBASPECT_API AccessorInstanceList_Iterator begin();
	/*! \brief The method returns an iterator to the last element of the AccessorInstanceList.
 		\n     no-throw */
	LIBBASARDBASPECT_API AccessorInstanceList_Iterator end();
	/*! \brief The method returns the size of the AccessorInstanceList.
 		\n     no-throw */
	LIBBASARDBASPECT_API AccessorListSize size() const;
private:
	boost::shared_ptr<ManagerImpl> m_Manager; //!< pointer to the manager 
};

/*--------------------------------------------------------------------------------------------------*/
/*! \brief  Iterator through list of accessor definitions

	This is a final class. */
class AccessorDefinitionList_Iterator
{
public:
	/*! \brief The method inits the Iterator with the values from the parameter list. 
 		\n     NullReferenceException */
	LIBBASARDBASPECT_API AccessorDefinitionList_Iterator(   const boost::shared_ptr<ManagerImpl>, 
															const std::map<AccessorName,AccessorDefinitionRef>::iterator );

	/*! \brief standard constructor 
 		\n      no-throw*/
	LIBBASARDBASPECT_API AccessorDefinitionList_Iterator ();

	/*! \brief The method checks if the internal iterator is equal to rhp. 
 		\n     no-throw */
	LIBBASARDBASPECT_API bool operator == (const AccessorDefinitionList_Iterator& rhp) const;
	/*! \brief The method checks if the internal Iterator isn't equal to rhp. 
 		\n     no-throw */
	LIBBASARDBASPECT_API bool operator != (const AccessorDefinitionList_Iterator& rhp) const;

	/*! \brief The method returns the next pointer. 
 		\n     OutOfRangeIteratorException */
	LIBBASARDBASPECT_API AccessorDefinitionList_Iterator operator ++ (); 
	/*! \brief The method returns the current pointer and increases it.
 		\n     OutOfRangeIteratorException */
	LIBBASARDBASPECT_API AccessorDefinitionList_Iterator operator ++ (int); 
	/*! \brief The method returns the current pointer:
 		\n     OutOfRangeIteratorException */
	LIBBASARDBASPECT_API AccessorDefinitionRef operator*();

	/*! \brief The method checks if the iterator points to begin() of the inner container
		\n     no-throw */
	LIBBASARDBASPECT_API bool					isBegin			() const;

	/*! \brief The method checks if the iterator points to end() of the inner container
		\n     no-throw */
	LIBBASARDBASPECT_API bool					isEnd			() const;

	/*! \brief The method checks if the iterator points to NULL
		\n     no-throw */
	LIBBASARDBASPECT_API bool isNull() const;

	/*! \brief The method sets the iterator points to NULL
		\n     no-throw */
	LIBBASARDBASPECT_API void reset();

private:
	/*! \brief The method checks if the iterator is valid
 		\n     no-throw */
	void checkValid() const;
	std::map<AccessorName,AccessorDefinitionRef>::iterator m_It;	//!< the current iterator
	boost::shared_ptr<ManagerImpl> m_Manager;						//!< pointer to the manager 
};

/*--------------------------------------------------------------------------------------------------*/
/*! \brief  A class which holds a list of accessor definitions.

	This is a final class. */
class AccessorDefinitionListRef
{
public:
	/*! \brief The method inits an accessor definition list with the values of the parameter list.
 		\n     no-throw */
	LIBBASARDBASPECT_API AccessorDefinitionListRef( const boost::shared_ptr<ManagerImpl> Manager );
	/*! \brief The method returns an iterator to the first element of the AccessorDefinitionList.
 		\n     no-throw */
	LIBBASARDBASPECT_API AccessorDefinitionList_Iterator begin();
	/*! \brief The method returns an iterator to the last element of the AccessorDefinitionList.
 		\n     no-throw */
	LIBBASARDBASPECT_API AccessorDefinitionList_Iterator end();
	/*! \brief The method returns the size of the iterator list.
 		\n     no-throw */
	LIBBASARDBASPECT_API AccessorListSize size() const ;
	
	/*! \brief The method checks if the shared pointer to the implementation class is null?\n no-throw */
	LIBBASARDBASPECT_API bool isNull() const;

	/*! \brief	The method sets the sharedpointer to null. \n no-throw */
	LIBBASARDBASPECT_API void reset();

private:
	/*! \brief	The method returns the pointer to a ManagerImpl. 
 				\n NullReferenceException */
	const boost::shared_ptr<ManagerImpl>& getImpl() const;
	boost::shared_ptr<ManagerImpl> m_Manager; //!< pointer to the manager 
};


} // aspect
} // db
} // basar

#endif
