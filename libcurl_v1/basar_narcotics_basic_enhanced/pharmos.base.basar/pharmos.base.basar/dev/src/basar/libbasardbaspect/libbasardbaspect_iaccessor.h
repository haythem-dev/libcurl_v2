#ifndef GUARD_LIBBASARDBASPECT_IACCESSOR_H
#define GUARD_LIBBASARDBASPECT_IACCESSOR_H
//----------------------------------------------------------------------------
/*! \file
 *  \brief  internal interface accessor (pure virtual one)
 *  \author Marco Köppendörfer
 *  \date   15.10.2013
 */
//----------------------------------------------------------------------------

//---------------------------------------------------------------------------------------//
// includes
//---------------------------------------------------------------------------------------//
#include "libbasardbaspect_definitions.h"

//#include "libbasarproperty_propertydescriptionlistref.h"
//
//#include "libbasardbaspect_accessorpropertytable.h"
//#include "libbasardbaspect_connectionref.h"


//---------------------------------------------------------------------------------------//
// namespaces
//---------------------------------------------------------------------------------------//
namespace basar	 
{
	namespace db	 
	{
		namespace sql
		{
			struct ExecuteReturnInfo;
		}

		namespace aspect {

//---------------------------------------------------------------------------------------//
// forward declaration
//---------------------------------------------------------------------------------------//
class AccessorExecuteListener;
class AccessMethodListRef;
class ConnectionRef;
class AccessorPropertyTableRef;
class AccessorPropertyTable_YIterator;
class AccessorExecuteListenerRef;
class AccessorExecuteListener;

//---------------------------------------------------------------------------------------//
// class declaration IAccessor
//---------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////////////////////////
/*! \brief  the class IAccessor holds a list with the accessmethod objects.

	This is an interface (pure virtual) class.
	There is a difference between definition and instance in implementation.*/
/////////////////////////////////////////////////////////////////////////////////////////////////
class IAccessor
{

public:
	//----------------------------------------------------------//
	// object section
	//----------------------------------------------------------//
	/*! \brief d'tor	\n no-throw */
	virtual ~IAccessor(){};
	
	/*! \brief The method creates an instance of a definition. \n no-throw */
	virtual IAccessorSharedPtr					createInstance(	 const ConnectionRef     connRef, 
																 const CachingPolicyEnum eCachePolicy = ON_DEMAND_CACHING ) = 0;

	/*! \brief The method returns a reference to a property description list 
	    \n     no-throw */
	virtual basar::property::PropertyDescriptionListRef	getPropertyDescriptionList() = 0;

	/*! \brief The method returns a const reference to a property table/cacher. \n InvalidStateException */
	virtual const AccessorPropertyTableRef				getPropertyTable() const = 0; 

	/*! \brief The method returns a reference to a property table/cacher. \n InvalidStateException */
	virtual AccessorPropertyTableRef					getPropertyTable() = 0; 

	/*! \brief The method returns the created reference to the accessmethod with this name, if it is a definition - accessor.
	    \n     ExistAccessorMethodException, UnknownAccessMethodException */
	virtual AccessMethodRef								createAccessMethod( const AccessMethodName & Name ) = 0;

	/*! \brief The method returns the reference to the accessmethod with this name.
	    \n     UnknownAccessMethodException */
	virtual const AccessMethodRef						getAccessMethod( const AccessMethodName & Name ) const = 0;

	/*! \brief The method returns a pointer to a list of all accessmethods of this accessor object
	    \n     no-throw */
	virtual const AccessMethodListRef					getAccessMethodList() const = 0;

	/*! \brief The method creates a sql-statement for each stringbuilder in the method and executes it on the given yIt.
	    \n     successive CICS statements with same accessor instance: preceding statement, resultset is automatically closed
	    \n     throw UnknownMethodNameException */
	virtual ExecuteResultInfo							execute( const AccessMethodName          & name, 
													     AccessorPropertyTable_YIterator & yit,
														 bool                              clearBeforeSelect, 
														 bool                              flagExcept,
                                                         DatabaseHintEnum                  dbHint) = 0;
	/*! \brief The method creates a sql-statement for each stringbuilder in the method and executes it.
	    \n     successive CICS statements with same accessor instance: preceding statement, resultset is automatically closed
	    \n     throw UnknownMethodNameException */
	virtual ExecuteResultInfo							execute( const AccessMethodName & name, 
														 bool                     clearBeforeSelect, 
														 bool                     flagExcept,
                                                         DatabaseHintEnum         dbHint) = 0;

	/*! \brief The method creates a sql-statement for each stringbuilder in the method and executes it on given range 
		 between yIt-begin and yIt-end.
	    \n     successive CICS statements with same accessor instance: preceding statement, resultset is automatically closed
	    \n     throw UnknownMethodNameException */
	virtual ExecuteResultInfo						   execute( const AccessMethodName           & name, 
														 AccessorPropertyTable_YIterator & yBeg, 
														 AccessorPropertyTable_YIterator & yEnd,
														 bool                              clearBeforeSelect, 
														 bool                              flagExcept,
                                                         DatabaseHintEnum                             dbHint) = 0;

	/*! \brief The method creates a sql-aggregate-statement (count*, sum, max, min, avg) with one return value (decimal) 
				for each stringbuilder in method and executes it.
	    \n     successive CICS statements with same accessor instance: preceding statement, resultset is automatically closed
	    \n     throw InvalidStateException, UnknownMethodNameException */
	virtual std::pair<bool, Decimal>					executeAggregate( const AccessMethodName & rName ) = 0;

	/*! \brief The method creates a sql-aggregate-statement (count*, sum, max, min, avg) with one return value (decimal) 
				for each stringbuilder in method and executes it.
	    \n     successive CICS statements with same accessor instance: preceding statement, resultset is automatically closed
	    \n     throw InvalidStateException, UnknownMethodNameException */
	virtual std::pair<bool, Decimal>					executeAggregate( const AccessMethodName        & rName,
																  AccessorPropertyTable_YIterator yit) = 0;



	/*! \brief The method sets default parameters for the sqlbuilders of the given method.
	    \n     UnknownMethodNameException */
	virtual void					                    setDefaultParameter( cmnutil::ParameterList   paramList,
																	 const AccessMethodName & rName ) = 0;
	
	/*! \brief The method sets default parameters for the sqlbuilders of the given method.
	    \n     no-throw */
	virtual void					                    setDefaultParameter( const VarString        & rDefParams,
																	 const AccessMethodName & rName ) = 0;
	
	/*! \brief The method sets default parameters for the sqlbuilders of the given method.
	    \n     no-throw */
	virtual void				                     	setDefaultParameter( ConstString              defParams,
																	 const AccessMethodName & rName ) = 0;

	/*! \brief The method returns the  default parameters that are set by setDefaultParameter(.., methodname)
	    \n     UnknownMethodNameException */
	virtual cmnutil::ParameterList						getDefaultParameter(const AccessMethodName & rName) const = 0;

	/*! \brief The method returns an iterator to the first element of the AccessMethod list. 
 		\n     no-throw */
	virtual CollAccessorMethodRef::iterator				begin() = 0;

	/*! \brief The method returns an iterator to the last element of the AccessMethod list. 
 		\n     no-throw */
	virtual CollAccessorMethodRef::iterator				end() = 0;

	/*! \brief The method returns the size of the AccessMethod list. 
 		\n     no-throw */
	virtual AccessMethodListSize						size() const = 0;

	/*! \brief The method checks if there are elements in the AccessMethod list. 
 		\n     no-throw */
	virtual bool										empty() const = 0;


	/*! \brief The method returns a shared pointer to this by casting weak -> shared
		\n no-throw */
	virtual const IAccessorSharedPtr					getThis() const = 0;

	/*! \brief The method returns the name of the accessor
		\n no-throw */
	virtual const VarString &							getAccessorName() const = 0;

	/*! \brief The method sets the accessor name. \n no-throw */
	virtual void										setAccessorName( const VarString name ) = 0;
	

	//-----------------------------------------------------------------------------------//
	// snapshots methods
	//-----------------------------------------------------------------------------------//
	/*! \brief	The method turns the flag on (true) or off (false) to decide if the internal snapshot must 
				be created or not. \n NullReferenceException */
	virtual void										switchCreateInternalSnapshot( bool toCreate ) = 0;

	/*! \brief	The method returns a frozen state and values of the current data to reset it later. 
	    \n      throw SnapshotNotAllowedToCreateException */
	virtual const property::PropertyTableSnapshotRef	createSnapshot() const = 0;
	
	/*! \brief	The method restores the states and values of data taken earlier. 
	    \n      throw SnapshotNotAllowedToSetException */
	virtual void										resetToSnapshot( property::PropertyTableSnapshotRef snapShot ) = 0;

	/*! \brief	The method clears the internal propertytablesnapshot. \n no-throw */
	virtual void										clearInternalSnapshot() = 0;

	//-----------------------------------------------------------------------------------//
	// transaction methods
	//-----------------------------------------------------------------------------------//
	//! \brief	The method starts the transaction.		\n no-throw
	virtual void										beginTransaction() = 0;
	//! \brief	The method realizes the transaction.	\n no-throw
	virtual void										commitTransaction() = 0;
	//! \brief	The method cancels the transaction.		\n no-throw
	virtual void										rollbackTransaction() = 0;


	//!	\brief The method indicates whether a transaction is active or not. \n throws NullReferenceException.
	virtual bool										isInTransaction() const = 0;

	//! \brief	The method returns the connectionref. \n no-throw.
	virtual const ConnectionRef							getConnection() const = 0;

 
    //-----------------------------------------------------------------------------------//
	// serial id methods
	//-----------------------------------------------------------------------------------//
    //! \brief	returns the last inserted Serial 
    //! \return	the last inserted Serial 
    virtual Int64										getLastInsertedSerial	(Int32 nr //!< number within the serial list (0 to  getNumberOfLastInsertedSerials)
																		) = 0;

    //! \brief	are there Serials listed
    virtual bool										hasLastInsertedSerial() = 0;

    //! \brief	returns the number of last inserted Serial 
    virtual Int32										getNumberOfLastInsertedSerials() = 0;

    //! \brief	clears last inserted Serial  list
    virtual void										clearLastInsertedSerials() = 0;

    //-----------------------------------------------------------------------------------//
	// AccessorExecuteListener methods
	//-----------------------------------------------------------------------------------//
    //! \brief	 sets the AccessorExecuteListener
     virtual void										setAccessorExecuteListener		(const AccessorExecuteListener &) = 0;

    //! \brief	 removes  the AccessorExecuteListener
     virtual void										removeAccessorExecuteListener	() = 0;

    //! \brief	 returns the AccessorExecuteListener
     virtual AccessorExecuteListenerRef					getAccessorExecuteListener		() const = 0;
};

} // aspect
} // db
} // basar

#endif
