#ifndef GUARD_ACCESSOR_H
#define GUARD_ACCESSOR_H
//----------------------------------------------------------------------------
/*! \file
 *  \brief  internal interface accessor
 *  \author Roland Kiefert
 *  \date   18.07.2005
 */
//----------------------------------------------------------------------------

//---------------------------------------------------------------------------------------//
// includes
//---------------------------------------------------------------------------------------//
#include <boost/unordered_map.hpp>

#include "libbasarproperty_propertydescriptionlistref.h"
#include "libbasardbaspect_definitions.h"
#include "libbasardbaspect_accessorpropertytable.h"
#include "libbasardbaspect_connectionref.h"
#include "libbasardbaspect_accessor.h"
#include "libbasardbaspect_accessorexecutelistener.h"

#include "libbasardbaspect_iaccessor.h"

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
			class PreparedStatementRef;
			
		}

		namespace aspect {

//---------------------------------------------------------------------------------------//
// forward declaration
//---------------------------------------------------------------------------------------//
class AccessMethodListRef;
class ConnectionRef;
class AccessorPropertyTableRef;
class AccessorPropertyTable_YIterator;
class AccessorExecuteListenerRef;
class AccessorExecuteListener;
class SQLStringBuilderList_Iterator;

//-------------------------------------------------------------------------------------------------------//
// typedef section
//-------------------------------------------------------------------------------------------------------//
typedef std::pair<basar::VarString, sql::CursorTypeEnum> PreparedStatementCacheKey;
typedef boost::unordered_map<PreparedStatementCacheKey, sql::PreparedStatementRef> PreparedStatementCacheColl;

//---------------------------------------------------------------------------------------//
// class declaration Accessor
//---------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////////////////////////
/*! \brief  the class Accessor holds a list with the accessmethod objects.

	This is a final class.
	There is a difference between definition and instance.*/
/////////////////////////////////////////////////////////////////////////////////////////////////
class Accessor : public IAccessor
{
	friend class Connection;
public:
	//----------------------------------------------------------//
	// static section
	//----------------------------------------------------------//
	//! \brief static method creates an Accessor (Pattern: simple factory method). \n no-throw
	static IAccessorSharedPtr					create();

private:
	static const ULong32						s_UNINITIALIZED_TIME; //!< represents the invalid time 25:61:61:999

public:
	//----------------------------------------------------------//
	// object section
	//----------------------------------------------------------//
	/*! \brief	default constructor. \n no-throw */
	Accessor();

	/*! \brief d'tor	\n no-throw */
	virtual ~Accessor();
	
	/*! \brief The method creates an instance of a definition. \n no-throw */
	IAccessorSharedPtr							createInstance(	 const ConnectionRef     connRef, 
																 const CachingPolicyEnum eCachePolicy = ON_DEMAND_CACHING );

	/*! \brief The method returns a reference to a property description list 
	    \n     no-throw */
	basar::property::PropertyDescriptionListRef	getPropertyDescriptionList();

	/*! \brief The method returns a const reference to a property table/cacher. \n InvalidStateException */
	const AccessorPropertyTableRef				getPropertyTable() const ; 

	/*! \brief The method returns a reference to a property table/cacher. \n InvalidStateException */
	AccessorPropertyTableRef					getPropertyTable(); 

	/*! \brief The method returns the created reference to the accessmethod with this name, if it is a definition - accessor.
	    \n     ExistAccessorMethodException, UnknownAccessMethodException */
	AccessMethodRef								createAccessMethod( const AccessMethodName & Name );

	/*! \brief The method returns the reference to the accessmethod with this name.
	    \n     UnknownAccessMethodException */
	const AccessMethodRef						getAccessMethod( const AccessMethodName & Name ) const;

	/*! \brief The method returns a pointer to a list of all accessmethods of this accessor object
	    \n     no-throw */
	const AccessMethodListRef					getAccessMethodList() const;

	/*! \brief The method creates a sql-statement for each stringbuilder in the method and executes it on the given yIt.
	    \n     successive CICS statements with same accessor instance: preceding statement, resultset is automatically closed
	    \n     throw UnknownMethodNameException */
	ExecuteResultInfo							execute( const AccessMethodName          & name, 
													     AccessorPropertyTable_YIterator & yit,
														 bool                              clearBeforeSelect, 
														 bool                              flagExcept,
                                                         DatabaseHintEnum                  dbHint);
	/*! \brief The method creates a sql-statement for each stringbuilder in the method and executes it.
	    \n     successive CICS statements with same accessor instance: preceding statement, resultset is automatically closed
	    \n     throw UnknownMethodNameException */
	ExecuteResultInfo							execute( const AccessMethodName & name, 
														 bool                     clearBeforeSelect, 
														 bool                     flagExcept,
                                                         DatabaseHintEnum         dbHint);

	/*! \brief The method creates a sql-statement for each stringbuilder in the method and executes it on given range 
		 between yIt-begin and yIt-end.
	    \n     successive CICS statements with same accessor instance: preceding statement, resultset is automatically closed
	    \n     throw UnknownMethodNameException */
	ExecuteResultInfo						   execute( const AccessMethodName           & name, 
														 AccessorPropertyTable_YIterator & yBeg, 
														 AccessorPropertyTable_YIterator & yEnd,
														 bool                              clearBeforeSelect, 
														 bool                              flagExcept,
                                                         DatabaseHintEnum                  dbHint);

	/*! \brief The method creates a sql-aggregate-statement (count*, sum, max, min, avg) with one return value (decimal) 
				for each stringbuilder in method and executes it.
	    \n     successive CICS statements with same accessor instance: preceding statement, resultset is automatically closed
	    \n     throw InvalidStateException, UnknownMethodNameException */
	std::pair<bool, Decimal>					executeAggregate( const AccessMethodName & rName );

	/*! \brief The method creates a sql-aggregate-statement (count*, sum, max, min, avg) with one return value (decimal) 
				for each stringbuilder in method and executes it.
	    \n     successive CICS statements with same accessor instance: preceding statement, resultset is automatically closed
	    \n     throw InvalidStateException, UnknownMethodNameException */
	std::pair<bool, Decimal>					executeAggregate( const AccessMethodName        & rName,
																  AccessorPropertyTable_YIterator yit);



	/*! \brief The method sets default parameters for the sqlbuilders of the given method.
	    \n     UnknownMethodNameException */
	void					                    setDefaultParameter( cmnutil::ParameterList   paramList,
																	 const AccessMethodName & rName );
	
	/*! \brief The method sets default parameters for the sqlbuilders of the given method.
	    \n     no-throw */
	void					                    setDefaultParameter( const VarString        & rDefParams,
																	 const AccessMethodName & rName );
	
	/*! \brief The method sets default parameters for the sqlbuilders of the given method.
	    \n     no-throw */
	void				                     	setDefaultParameter( ConstString              defParams,
																	 const AccessMethodName & rName );

	/*! \brief The method returns the  default parameters that are set by setDefaultParameter(.., methodname)
	    \n     UnknownMethodNameException */
	cmnutil::ParameterList						getDefaultParameter(const AccessMethodName & rName) const;

	/*! \brief The method returns an iterator to the first element of the AccessMethod list. 
 		\n     no-throw */
	CollAccessorMethodRef::iterator				begin();

	/*! \brief The method returns an iterator to the last element of the AccessMethod list. 
 		\n     no-throw */
	CollAccessorMethodRef::iterator				end();

	/*! \brief The method returns the size of the AccessMethod list. 
 		\n     no-throw */
	AccessMethodListSize						size() const;

	/*! \brief The method checks if there are elements in the AccessMethod list. 
 		\n     no-throw */
	bool										empty() const;


	/*! \brief The method returns a shared pointer to this by casting weak -> shared
		\n no-throw */
	const IAccessorSharedPtr					getThis() const;

	/*! \brief The method returns the name of the accessor
		\n no-throw */
	const VarString &							getAccessorName() const;

	/*! \brief The method sets the accessor name. \n no-throw */
	void										setAccessorName( const VarString name );
	

	//-----------------------------------------------------------------------------------//
	// snapshots methods
	//-----------------------------------------------------------------------------------//
	/*! \brief	The method turns the flag on (true) or off (false) to decide if the internal snapshot must 
				be created or not. \n NullReferenceException */
	void										switchCreateInternalSnapshot( bool toCreate );

	/*! \brief	The method returns a frozen state and values of the current data to reset it later. 
	    \n      throw SnapshotNotAllowedToCreateException */
	const property::PropertyTableSnapshotRef	createSnapshot() const;
	
	/*! \brief	The method restores the states and values of data taken earlier. 
	    \n      throw SnapshotNotAllowedToSetException */
	void										resetToSnapshot( property::PropertyTableSnapshotRef snapShot );

	/*! \brief	The method clears the internal propertytablesnapshot. \n no-throw */
	void										clearInternalSnapshot();

	//-----------------------------------------------------------------------------------//
	// transaction methods
	//-----------------------------------------------------------------------------------//
	//! \brief	The method starts the transaction.		\n no-throw
	void										beginTransaction();
	//! \brief	The method realizes the transaction.	\n no-throw
	void										commitTransaction();
	//! \brief	The method cancels the transaction.		\n no-throw
	void										rollbackTransaction();


	//!	\brief The method indicates whether a transaction is active or not. \n throws NullReferenceException.
	bool										isInTransaction() const;

	//! \brief	The method returns the connectionref. \n no-throw.
	const ConnectionRef							getConnection() const;

 
    //-----------------------------------------------------------------------------------//
	// serial id methods
	//-----------------------------------------------------------------------------------//
    //! \brief	returns the last inserted Serial 
    Int64										getLastInsertedSerial	(Int32 nr //!< number within the serial list (0 to  getNumberOfLastInsertedSerials)
																		);

    //! \brief	are there Serials listed
    bool										hasLastInsertedSerial();

    //! \brief	returns the number of last inserted Serial 
    Int32										getNumberOfLastInsertedSerials();

    //! \brief	clears last inserted Serial  list
    void										clearLastInsertedSerials();

    //-----------------------------------------------------------------------------------//
	// AccessorExecuteListener methods
	//-----------------------------------------------------------------------------------//
    //! \brief	 sets the AccessorExecuteListener
     void										setAccessorExecuteListener		(const AccessorExecuteListener &);

    //! \brief	 removes  the AccessorExecuteListener
     void										removeAccessorExecuteListener	();

    //! \brief	 returns the AccessorExecuteListener
     AccessorExecuteListenerRef					getAccessorExecuteListener		() const;

private:
	//-----------------//
	// private methods
	//-----------------//	
	/*! \brief	The method checks if this object is a definition \n no-throw */
	bool									    isDefinition			() const;

	/*! \brief	if the client instructs a rollback (database action has failed), the internal data 
				from the first statement in the execute()-method, are backed up. \n no-throw */			
	void										resetInternalSnapshot	();

	/*! \brief The method sets the member weak pointer.\n no-throw */
	void										setWeakPtr				( AccessorWeakPtr weakPtr );		

    /*! \brief returns the ErrorCode 
     *  \return ExecuteErrorEnum indicating success or failure of execute method
     */
   ExecuteErrorEnum								getErrorCode			(const sql::ExecuteReturnInfo & info //!< the dbsql result
																		 );

   /*! \brief sets the ExecuteReturnInfo from  basar::db::sql::ExecuteReturnInfo*/
   void											extrFromSqlExecRetInfo (const sql::ExecuteReturnInfo & info,		//!< the basar::db::sql Result
																		aspect::ExecuteResultInfo    & oneresult,	//!< the result of the execute SQLBuilder
																		aspect::ExecuteResultInfo    & addedResult	//!< the result of the execute function
																	    );
    /*! \brief sets source values from yit to destination values in prepared statement
        \return reference to prepared statement
    */
    inline void                                 setPreparedStatementValues ( const AccessorPropertyTable_YIterator & yit,  //!< yit containing values to set in prepared statement
                                                                            const IndexTypeCollection& indexTypeColl,     //!< index type collection offering type of value to set and indexes in prepared statement and yit
                                                                            sql::PreparedStatementRef& psRef              //!< prepared statement ref to set values to - this is an out param
                                                                            );
    /*! \brief returns new or cached prepared statement 
        \return reference to prepared statement
    */
    const sql::PreparedStatementRef&            getPreparedStatementRef (const basar::VarString& rSql,   //!< sql statement to prepare
                                                                         sql::CursorTypeEnum eCursorType //!< cursor type to use
                                                                         );
    /*! \brief prepares prepared statement for execution (getting prep statement and setting values)
        \return reference to prepared statement
    */
    sql::PreparedStatementRef                   preparePreparedStatement (SQLStringBuilderList_Iterator itBuild,               //!< itBuild pointing to prepareable builder, offers statement to prepare and index type collection
                                                                          const AccessorPropertyTable_YIterator & yit,         //!< yit containing values to set in prepared statement
                                                                          sql::CursorTypeEnum eCursorType = sql::CURS_DEFAULT  //!< cursor type to use
                                                                          );
    /*! \brief prepares accessor for successive CICS statements by closing statement and result */
    void                                        prepareForSuccessiveCICSStatements();

	//-----------------//
	// private members
	//-----------------//
	//! \brief weak pointer to this, needed for common reference group 
	AccessorWeakPtr										m_WeakPtr;			
	
	//! \brief map to AccessMethod
	CollAccessorMethodRef								m_MethodList;	

	//! \brief copy of given ParameterLists, added for a stringent getDefaultParemeterList()
	std::map<AccessMethodName, cmnutil::ParameterList>	m_MethodDefaultParameter;	
	
	//! \brief caches prepared statements
	PreparedStatementCacheColl m_PreparedStatementCache;
	
	//! \brief is this accessor a definition ?
	bool												m_Definition;
		
	//! \brief description list of the propertytable
	property::PropertyDescriptionListRef				m_Descriptionlist;	
	
	//! \brief reference to cacher
	AccessorPropertyTableRef							m_Cacher;			
		
	//! \brief name of the accessor
	VarString											m_AccessorName;		
	
	//! \brief holds the one and only connection the accessor belongs to
	ConnectionRef										m_ConnRef;			
		
	/*! \brief	stores the time of the end of the transaction. 
				Is important for the check if an external snapshot is allowed to be set.
				The external snapshot is only allowed to be set if there is no open transaction 
				and the snapshot time is older than the end of the last transaction. */
	ULong32												m_TimeLastDatabaseAction;

    //! \brief the last Serials that were inserted  
    std::vector<Int64>									m_lastInsertedSerials;

    //! \brief the AccessorExecuteListener, that listens to the Execute Results  
    AccessorExecuteListenerRef							m_accessorExecuteListenerRef;
};

} // aspect
} // db
} // basar

#endif
