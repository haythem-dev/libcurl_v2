//-----------------------------------------------------------------------------------//
/*! \file
 *  \brief  all functions implementation Accessor 
 *  \author Roland Kiefert
 *  \date   18.07.2005
 */
//-----------------------------------------------------------------------------------//


//-----------------------------------------------------------------------------------//
// includes
//-----------------------------------------------------------------------------------//

#include "accessor.h"

#include "loggerpool.h"
#include "cachecontroller.h"
#include "accessmethod.h"

#include "libbasardbaspect_accessmethod.h"
#include "libbasardbaspect_exceptions.h"
#include "libbasardbaspect_sqlstringbuilder.h"
#include "libbasardbaspect_manager.h"
#include "libbasardbsql_exceptions.h"
#include "libbasardbsql_databaseinfo.h"
#include "libbasardbsql_preparedstatementref.h"
#include "../libbasarproperty/propertydescriptionlist.h"

#include "libbasardbaspect_stringbuildertypeindexes.h"

//-----------------------------------------------------------------------------------//
// namespaces
//-----------------------------------------------------------------------------------//
namespace basar {
namespace db {
namespace aspect {


//-----------------------------------------------------------------------------------//
// static section
//-----------------------------------------------------------------------------------//
// comment: s_UNINITIALIZED_TIME represents the invalid time 25:61:61:999.
//			The member 'm_TimeLastDatabaseAction' is initialized with it by default.
//			So it can be used in comparisons to find out if 'm_TimeLastDatabaseAction'
//			has changed.
const ULong32 Accessor::s_UNINITIALIZED_TIME = 256161999;

///////////////////////////////////////////////////////////////////////////////////////
//!	\return pointer to the new accessor itself 
//!	\throw  no-throw
///////////////////////////////////////////////////////////////////////////////////////
IAccessorSharedPtr Accessor::create()
{
	AccessorSharedPtr p( new Accessor );

	p->setWeakPtr( p );
	p->m_Definition = true;

	return p;
}
//-----------------------------------------------------------------------------------//
// object section
//-----------------------------------------------------------------------------------//
///////////////////////////////////////////////////////////////////////////////////////
//!	\throw  no-throw
///////////////////////////////////////////////////////////////////////////////////////
Accessor::Accessor() : m_TimeLastDatabaseAction( Accessor::s_UNINITIALIZED_TIME )
{
}

///////////////////////////////////////////////////////////////////////////////////////
//!	\throw  no-throw
///////////////////////////////////////////////////////////////////////////////////////
Accessor::~Accessor()
{
	try
	{
		if ( isDefinition() )
			return;

		if ( ! getPropertyTable().isNull() )
		{ 
			getPropertyTable().clear(); 
			getPropertyTable().reset(); 
		} 

        //check if we're registered
		if ( ! m_ConnRef.isNull() && m_ConnRef.isRegistered( *this ) )
		{
			m_ConnRef.unRegister( *this );
		}
	}
	catch (const basar::Exception &)
	{
		assert(0);
	}
	catch (...)
	{
		BLOG_ERROR(LoggerPool::accessor(), "unknown exception");
		assert(0);
	}
}

///////////////////////////////////////////////////////////////////////////////////////
//!	\return an accessor instance reference 
//!	\param connRef Connection for the accessor instance
//! \param eCachePolicy caching policy for created accessor, default ON_DEMAND_CACHING
//!	\throw  no-throw
///////////////////////////////////////////////////////////////////////////////////////
IAccessorSharedPtr Accessor::createInstance( const ConnectionRef     connRef, 
										    const CachingPolicyEnum eCachePolicy /* = ON_DEMAND_CACHING */ )
{
	static ConstString fun = "Accessor::createInstance()";

	if ( m_Descriptionlist.isNull() )
		throw InvalidStateException( 
				ExceptInfo ( fun, 
				             "No descriptionlist set!", 
							 __FILE__, __LINE__) );
		

	AccessorSharedPtr acc = AccessorSharedPtr( new Accessor );
	acc->setWeakPtr(acc);
	acc->m_ConnRef = connRef;

	BLOG_DEBUG(LoggerPool::accessor(), 
		       "createInstance() - New Accessor is created" );


	CollAccessorMethodRef::const_iterator itEnd = m_MethodList.end();

	for(CollAccessorMethodRef::iterator it = m_MethodList.begin(); itEnd != it; ++it )
	{
		acc->m_MethodList.insert(CollAccessorMethodRef::value_type(it->first, it->second));
	}
	
	BLOG_DEBUG(LoggerPool::accessor(), 
		       "createInstance() - copying of all accessor methods is done." );


	itEnd = acc->m_MethodList.end();

	for (CollAccessorMethodRef::iterator it = acc->m_MethodList.begin(); 
		 itEnd != it; 
		 ++it )
	{
		it->second = it->second.createInstance();
	}

	BLOG_DEBUG(LoggerPool::accessor(), 
		       "createInstance() - each accessor method created itself" );


	acc->m_Definition = false;
	acc->m_Cacher	  = AccessorPropertyTableRef( CacheController::create(m_Descriptionlist, 
			                                                              eCachePolicy)    );

	return acc;
}
///////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw 
	\return Reference to a PropertyDescriptionList */
///////////////////////////////////////////////////////////////////////////////////////
property::PropertyDescriptionListRef Accessor::getPropertyDescriptionList()
{
	if( isDefinition() )
	{
		if( m_Descriptionlist.isNull() )
		{
			m_Descriptionlist = PropertyDescriptionListRef(
				property::PropertyDescriptionList::create());
			return m_Descriptionlist;
		}

		return m_Descriptionlist;
	}
	else
	{
		return m_Cacher.getPropertyDescriptionList();
	}
}
///////////////////////////////////////////////////////////////////////////////////////
/*!	\throw InvalidStateException 
	\return const reference to a property table / cacher  */
///////////////////////////////////////////////////////////////////////////////////////
const AccessorPropertyTableRef Accessor::getPropertyTable() const
{
	if (isDefinition())
	{
		throw InvalidStateException(
					ExceptInfo( 
						"basar.db.aspect.Accessor.getPropertyTable()", 
						"This function is only executable if accessor is an instance!", 
						__FILE__, __LINE__) );
	}

	return m_Cacher;
}

///////////////////////////////////////////////////////////////////////////////////////
/*!	\throw InvalidStateException 
	\return reference to a property table / cacher  */
///////////////////////////////////////////////////////////////////////////////////////
AccessorPropertyTableRef Accessor::getPropertyTable()
{
	if (isDefinition())
	{
		throw InvalidStateException(
				ExceptInfo( 
					"basar.db.aspect.Accessor.getPropertyTable()", 
					"This function is only executable if accessor is an instance!", 
					__FILE__, __LINE__) );
	}

	return m_Cacher;
}

///////////////////////////////////////////////////////////////////////////////////////
/*!	\throw ExistAccessorMethodException, UnknownAccessMethodException 
	\return Reference to an AccessMethod 
	\param rName name of the accessmethod */
///////////////////////////////////////////////////////////////////////////////////////
AccessMethodRef Accessor::createAccessMethod( const AccessMethodName& rName ) 
{
	ConstString fun = "Accessor::createAccessMethod()";
	
	if( m_MethodList.find( rName ) != m_MethodList.end() )
	{
		VarString msg;
		msg.format("The method <%s> already exists.", rName.c_str());
		throw ExistAccessorMethodException( 
				ExceptInfo(fun, 
				           msg.c_str(), 
						   __FILE__, __LINE__) );
	}	

	if( !isDefinition() )
	{
		VarString msg;
		msg.format("Not possible to create method <%s> "
			       "in accessor state 'instance'.", 
				   rName.c_str());
		throw UnknownAccessMethodException( 
					ExceptInfo(fun, 
					           msg.c_str(), 
							   __FILE__, __LINE__ ) );
	}

	//! \todo if an instance is created, it mustn't create a new method. 
	AccessMethodRef t(AccessMethod::create());

	if ( !m_MethodList.insert(CollAccessorMethodRef::value_type(rName, t)).second)
	{
		VarString msg;
		msg.format("Inserting method <%s> to method list failed.", 
			       rName.c_str());
		throw UnknownAccessMethodException(
				ExceptInfo( fun, 
				            msg.c_str(), 
							__FILE__, __LINE__));
	}

	t.setAccessorMethodName(rName);

	return t;
}

///////////////////////////////////////////////////////////////////////////////////////
/*!	\throw UnknownAccessMethodException 
	\return Reference to an AccessMethod 
	\param rName name of the wanted accessmethod */
///////////////////////////////////////////////////////////////////////////////////////
const AccessMethodRef Accessor::getAccessMethod(const AccessMethodName& rName) const
{
	CollAccessorMethodRef::const_iterator It = m_MethodList.find(rName);

	if (It == m_MethodList.end())
	{
		VarString msg;
		msg.format("method <%s> not known in method list.", rName.c_str());
		ExceptInfo sInfo("basar.db.aspect.getAccessMethod()", 
			             msg.c_str(), 
						 __FILE__, __LINE__);
		throw UnknownAccessMethodException(sInfo);
	}

	return It->second;
}
///////////////////////////////////////////////////////////////////////////////////////
/*!	\throw  no-throw
	\return Reference to the AccessMethodList */
///////////////////////////////////////////////////////////////////////////////////////
const AccessMethodListRef Accessor::getAccessMethodList() const
{
	return AccessMethodListRef( getThis() );
}
///////////////////////////////////////////////////////////////////////////////////////
/*!	\throw  no-throw
	\retval true if accessor is a definition 
	\return otherwise returns false */
///////////////////////////////////////////////////////////////////////////////////////
bool Accessor::isDefinition() const
{
	return m_Definition;
}
///////////////////////////////////////////////////////////////////////////////////////
/*!	\throw  no-throw
	\return iterator to the first element of the list */
///////////////////////////////////////////////////////////////////////////////////////
CollAccessorMethodRef::iterator Accessor::begin()
{
	return m_MethodList.begin();
}
///////////////////////////////////////////////////////////////////////////////////////
/*!	\throw  no-throw
	\return iterator to the last element of the list */
///////////////////////////////////////////////////////////////////////////////////////
CollAccessorMethodRef::iterator Accessor::end()
{
	return m_MethodList.end();
}
///////////////////////////////////////////////////////////////////////////////////////
/*!	\throw  no-throw
	\return the size of the list */
///////////////////////////////////////////////////////////////////////////////////////
AccessMethodListSize Accessor::size() const
{
	return m_MethodList.size();
}
///////////////////////////////////////////////////////////////////////////////////////
/*!	\throw  no-throw
	\retval true, if there are no AccessMethods in the accessor
	\return otherwise returns false */
///////////////////////////////////////////////////////////////////////////////////////
bool Accessor::empty() const
{
	return m_MethodList.empty();
}
///////////////////////////////////////////////////////////////////////////////////////
/*! \return (only one group of) Accessor shared pointer 
	\throw  no-throw */
///////////////////////////////////////////////////////////////////////////////////////
const IAccessorSharedPtr Accessor::getThis() const
{
	return AccessorSharedPtr(m_WeakPtr);
}
///////////////////////////////////////////////////////////////////////////////////////
/*! \throw  no-throw
	\param weakPtr weak pointer to the class Accessor */
///////////////////////////////////////////////////////////////////////////////////////
void Accessor::setWeakPtr(AccessorWeakPtr weakPtr)
{
	m_WeakPtr = weakPtr;
}
///////////////////////////////////////////////////////////////////////////////////////
/*! \throw  no-throw
	\param name name of the accessor */
///////////////////////////////////////////////////////////////////////////////////////
void Accessor::setAccessorName( const VarString name )
{
	m_AccessorName = name;
}
///////////////////////////////////////////////////////////////////////////////////////
/*! \throw  no-throw
	\return name of the accessor */
///////////////////////////////////////////////////////////////////////////////////////
const VarString & Accessor::getAccessorName() const
{
	return m_AccessorName;
}
//-----------------------------------------------------------------------------------//
// execute methods
//-----------------------------------------------------------------------------------//
///////////////////////////////////////////////////////////////////////////////////////
//!	\retval	ExecuteResultInfo resultstruct Errornumber / AffectedRows
//!	\param	rName name of an accessmethod 
//!	\param	yit an iterator of the property table
//! \param  clearBeforeSelect if true then the propertytable will be cleared before the selection
//! \param  flagExcept if error: DB exception / return code
//! \param  dbHint Hint for the database engine (e.g. for optimisation)
//!	\throw	UnknownMethodNameException
///////////////////////////////////////////////////////////////////////////////////////
ExecuteResultInfo Accessor::execute(const AccessMethodName          & rName,
									AccessorPropertyTable_YIterator & yit,
									bool                              clearBeforeSelect,
									bool                              flagExcept, 
                                    DatabaseHintEnum                  dbHint )
{
	ConstString fun = "Accessor::execute()";

	m_TimeLastDatabaseAction = cmnutil::DateTime::getCurrent().getTime();

	CollAccessorMethodRef::iterator it = m_MethodList.find( rName );
	
	if( it == m_MethodList.end() )
	{
		VarString msg;
		msg.format("method <%s> not known in method list for executing.", 
			       rName.c_str());
		throw UnknownMethodNameException( 
				ExceptInfo(fun, 
				           msg.c_str(), 
						   __FILE__, __LINE__) );
	}

	if (LoggerPool::accessor().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format("method <%s> found for executing.", 
			       rName.c_str());
		LoggerPool::accessor().forcedLog(log4cplus::DEBUG_LOG_LEVEL, 
			                                 msg.c_str(), 
											 __FILE__, __LINE__);
	}

	if ( m_ConnRef.isInTransaction() )
    {
		//	It is possible that the client begins a transaction 
		//  via the connection object only and not via an accessor. 
		//  Therefore in the context of a connection it cannot be decided 
		//	which object a propertytablesnapshot has to be made for. 
		//  So here is the only place to create a snapshot for every 
		//  accessor and not in the method 'Accessor::transactionBegin()'. 
		//	But the snapshot is created only if the connection hasn't 
		//  registered the concerning accessor yet. So that the snapshot 
		//  is created only once, namely the first time the method 'execute()' 
		//  is called within a transaction. A new transaction will have 
		//  to register all belonging accessors again.
		if ( m_Cacher.createInternalSnapshot() )
		{
			// only register if an internal snapshot has been created.
			m_ConnRef.doRegister( m_WeakPtr ); 

			if (LoggerPool::accessor().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
			{
				VarString msg = "connection in transaction. snapshot "
					            "created before executing.";
				LoggerPool::accessor().forcedLog(log4cplus::DEBUG_LOG_LEVEL, 
					                                 msg.c_str(), 
													 __FILE__, __LINE__);
			}
		}
	}
	
	bool                         validSqlString	= false;
	bool                         manipDB	    = false;
	sql::StatementRef	         statement;
	sql::PreparedStatementRef    preparedStatement;
	sql::ResultsetRef	         resultSet;
	ExecuteResultInfo::RowNumber hits           = 0;
    
	
	sql::CursorTypeEnum eCursorType = sql::CURS_DEFAULT;
	
	if ( (dbHint & HOLD_RECORDSET) == HOLD_RECORDSET )
	{
	    eCursorType = sql::CURS_HOLD_OVER_COMMIT;
	}
    
    AccessorExecuteEvent event(rName, yit);
    ExecuteResultInfo    emptyExecuteResultInfo;
    ExecuteResultInfo    separateExecuteResultInfo; 
    ExecuteResultInfo    retval;

	SQLStringBuilderList_Iterator itBuildEnd = it->second.getBuilderList().end ();

	for (SQLStringBuilderList_Iterator itBuild  = it->second.getBuilderList().begin(); 
		 itBuild != itBuildEnd; 
		 ++itBuild ) 
	{
		statement = m_ConnRef.createStatement();
		BLOG_DEBUG(LoggerPool::accessor(), 
			       "execute() - statement created" );

		if( itBuild->build(yit) )
		{
			BLOG_DEBUG(LoggerPool::accessor(), 
				       "execute() - sqlstringbuilder is executable, sql-string valid!" );
			
			validSqlString = true;

			// only last resultset can be accessed 
			// (if more than one builder in accessmethod)
			if( itBuild->isQuery().first ) 
			{
				BLOG_DEBUG(LoggerPool::accessor(), 
					       "execute() - sqlstringbuilder is query." );
			    
			    if (itBuild->isPrepareable())
			    {
			        //handle prepareable query
				    preparedStatement = preparePreparedStatement(itBuild, yit, eCursorType);
			    }
			    
                prepareForSuccessiveCICSStatements();

				
				try
				{
					if( (SINGLE_QUERY    == itBuild->isQuery().second || 
						AGGREGATE_QUERY == itBuild->isQuery().second) )
					{
					    if (itBuild->isPrepareable())
					    {
				            resultSet = preparedStatement.executeSingleQuery();
				            BLOG_DEBUG(LoggerPool::accessor(), 
								   "execute() - single prepared query executed. ");					    
					    }
					    else
					    {
						    resultSet = statement.executeSingleQuery(itBuild->getSQLString());
						    BLOG_DEBUG(LoggerPool::accessor(), 
								    "execute() - single query executed." );
					    }
					}
					else 
					{
					    if	(itBuild->isPrepareable())
					    {
				            resultSet = preparedStatement.executeQuery();
                            BLOG_DEBUG(LoggerPool::accessor(), 
								   "execute() - multi prepared query executed. ");						    
					    }
					    else
					    {   
					        resultSet = statement.executeQuery(itBuild->getSQLString(), eCursorType);
						    BLOG_DEBUG(LoggerPool::accessor(), 
								    "execute() - multi query executed. ");
					    }
					}
				}
				catch (const sql::DbException &)
				{
					if (flagExcept)
						throw;

					retval.m_error = SQL_ERROR_NONSPECIFIC;

					if (LoggerPool::accessor().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
					{
						VarString msg;
						msg.format("execute() - no exception rethrown. "
							       "query failed: <%s>", 
								   (*itBuild).getSQLString().c_str());
						LoggerPool::accessor().forcedLog(log4cplus::DEBUG_LOG_LEVEL, 
							                                 msg, 
															 __FILE__, __LINE__);
					}
				}

                //now clearBeforeSelect is a misnomer, as m_Cacher's data is needed during execution of preparedQueries
                //from m_Cacher's perspective, it is cleared before newly selected values are set
				if (clearBeforeSelect)
				{
					m_Cacher.clear();
					BLOG_DEBUG(LoggerPool::accessor(), 
						       "execute() - cacher cleared before calling select." );
				}

				m_Cacher.setResultset( resultSet );
				// --> Index aufbauen
				m_Cacher.setStatement( statement );
				m_Cacher.begin();
                event.m_separateBuilderResults.push_back(emptyExecuteResultInfo);
			}
			else
			{
				// special handling with CICS connection
				if (CICS == m_ConnRef.getDbInfo().eConnectType)
				{
                    prepareForSuccessiveCICSStatements();
				}

				manipDB = true;
                sql::ExecuteReturnInfo sqlinfo;				

				if( itBuild->isPrepareable() )
			    {
			        //handle non-query (insert, update, delete) prepared statement
    			
				    BLOG_DEBUG(LoggerPool::accessor(), 
					        "execute() - sqlstringbuilder is prepareable and for "
						    "manipulating (no query)." );						    

				    preparedStatement = preparePreparedStatement(itBuild, yit, eCursorType);
    				
                    sqlinfo = preparedStatement.execute(flagExcept);

				    BLOG_DEBUG(LoggerPool::accessor(), 
					        "execute() - manipulating prepared statement executed." );				        
			    } 
			    else 
			    {
				    BLOG_DEBUG(LoggerPool::accessor(), 
					        "execute() - sqlstringbuilder is for "
						    "manipulating (no query)." );
						    
				    sqlinfo = statement.execute( itBuild->getSQLString(), flagExcept );  

				    BLOG_DEBUG(LoggerPool::accessor(), 
					        "execute() - manipulating statement executed." );
			    }

                extrFromSqlExecRetInfo(sqlinfo, separateExecuteResultInfo, retval);
				hits = retval.getAffectedRows();	// cumulated affected rows

                event.m_separateBuilderResults.push_back(separateExecuteResultInfo);
			    
			}
		}
		else
		{
			BLOG_DEBUG(LoggerPool::accessor(), 
				       "execute() - sqlstringbuilder is NOT executable!" );
		}
	}
	
	if ( validSqlString )
	{
		if( manipDB )
		{
            // Call the Listener Routine in m_accessorExecuteListenerRef
            if (!m_accessorExecuteListenerRef.isNull())
            {
				event.m_executeResult = retval;
				m_accessorExecuteListenerRef.executePerformed(event);
            }

			// executePerformed only 
			// if ( yit != aspect::Manager::getInstance().getAccessorNullYIterator() )
			if (!yit.isNull() && 
				0 != yit.getCacheControllerSharedPtr()->getPropertyDescriptionList().size())
            {
				yit = yit.getCacheControllerSharedPtr()->executePerformed( yit );
            }

			if (LoggerPool::accessor().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
			{
				VarString msg;
				msg.format("<%d> rows affected by executing manipulating sql statement. "
					       "Execute performed!",
						   hits);
				LoggerPool::accessor().forcedLog(log4cplus::DEBUG_LOG_LEVEL, 
													 msg, 
													 __FILE__, __LINE__);
			}
		}
	}
	else 
	{
		retval.m_error = NOT_EXECUTABLE;
	}

	return retval;
}
///////////////////////////////////////////////////////////////////////////////////////
/*!	\throw  UnknownMethodNameException
	\retval ExecuteResultInfo resultstruct Errornumber / AffectedRows
	\param  rName name of an accessmethod 
	\param  yBeg iterator of property table begin
	\param  yEnd iterator of property table end 
	\param  clearBeforeSelect if true then the propertytable will be cleared before the first selection 
	\param  flagExcept if error: DB exception / return code
	\param  dbHint Hint for the database engine (e.g. for optimisation)
*/
///////////////////////////////////////////////////////////////////////////////////////
ExecuteResultInfo Accessor::execute( const AccessMethodName          & rName, 
                                     AccessorPropertyTable_YIterator & yBeg, 
                                     AccessorPropertyTable_YIterator & yEnd,
                                     bool                              clearBeforeSelect, 
                                     bool                              flagExcept,
                                     DatabaseHintEnum                  dbHint )
{
    ExecuteResultInfo totalHits;

    if (yBeg != yEnd && !yBeg.isEnd())
    { 
        ExecuteResultInfo               singleHits;
        AccessorPropertyTable_YIterator yIt    = yBeg;
        AccessorPropertyTable_YIterator yItOld =  yIt;

        singleHits = execute(rName, 
			                 yIt, 
							 clearBeforeSelect, 
							 flagExcept, 
							 dbHint);

        if(singleHits.hasError() )
            totalHits.m_error = singleHits.getError();
        else
            totalHits.m_affectedRows += singleHits.getAffectedRows();

        // if there is a delete the ++ is already done
        if (yItOld == yIt)
			++yIt;
        
        while  (yIt != yEnd && !yIt.isEnd())
        {
            yItOld = yIt;
            singleHits = execute(rName, yIt, false, flagExcept, dbHint );

            if(singleHits.hasError() )
                totalHits.m_error = singleHits.getError();
            else
                totalHits.m_affectedRows += singleHits.getAffectedRows();

            // if there is a delete the ++ is already done
			if (yItOld == yIt)
	            ++yIt;
        }
    }
    else
	    totalHits.m_error = NOT_EXECUTABLE;

    return totalHits;
}

///////////////////////////////////////////////////////////////////////////////////////
//!	\retval		ExecuteResultInfo resultstruct Errornumber / AffectedRows
//!	\param	rName name of an accessmethod
//!	\param  clearBeforeSelect if true then the propertytable will be cleared before selection 
//!	\param  flagExcept if error: DB exception / return code
//!	\param  dbHint Hint for the database engine (e.g. for optimisation)
//!	\throw	UnknownMethodNameException
///////////////////////////////////////////////////////////////////////////////////////
ExecuteResultInfo Accessor::execute( const AccessMethodName & rName, 
									 bool                     clearBeforeSelect, 
									 bool                     flagExcept, 
                                     DatabaseHintEnum         dbHint )
{
	AccessorPropertyTable_YIterator yIt;
	return execute( rName, 
		            yIt, 
					clearBeforeSelect, 
					flagExcept, 
					dbHint );
}

///////////////////////////////////////////////////////////////////////////////////////
//!	\return	pair = <statement executable, result of sql-aggregate-select (value directly from database) as decimal>
//!	\param	rName name of an accessmethod
//!	\throw	UnknownMethodNameException
///////////////////////////////////////////////////////////////////////////////////////
std::pair<bool, Decimal> Accessor::executeAggregate( const AccessMethodName & rName )
{
	return executeAggregate(rName, 
		                    AccessorPropertyTable_YIterator());
}

///////////////////////////////////////////////////////////////////////////////////////
//!	\return	pair = <statement executable?, result of sql-aggregate-select (value directly from database) as decimal>
//!	\param	rName name of an accessmethod
//!	\param	yit an iterator of the property table
//!	\throw	UnknownMethodNameException, InvalidStateException
///////////////////////////////////////////////////////////////////////////////////////
std::pair<bool, Decimal> Accessor::executeAggregate(const AccessMethodName        & rName, 
													AccessorPropertyTable_YIterator yit )
{
	ConstString fun = "Accessor::executeAggregate()";
	
	m_TimeLastDatabaseAction = cmnutil::DateTime::getCurrent().getTime();

	CollAccessorMethodRef::iterator it = m_MethodList.find( rName );
	
	if( it == m_MethodList.end() )
	{
		VarString msg;
		msg.format("method <%s> not known in method list for executing.", rName.c_str());
		throw UnknownMethodNameException( ExceptInfo(fun, msg, __FILE__, __LINE__) );
	}

	if (LoggerPool::accessor().isEnabledFor(log4cplus::TRACE_LOG_LEVEL))
	{
		VarString msg;
		msg.format("method <%s> found for executing.", rName.c_str());
		LoggerPool::accessor().forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	sql::StatementRef             statement;
	sql::PreparedStatementRef     preparedStatement;
	
	SQLStringBuilderList_Iterator itBuildEnd = it->second.getBuilderList().end ();
	
	for (SQLStringBuilderList_Iterator itBuild = it->second.getBuilderList().begin(); 
		 itBuild != itBuildEnd; 
		 ++itBuild ) 
	{
	
		statement = m_ConnRef.createStatement();
		BLOG_DEBUG( LoggerPool::accessor(), "executeAggregate() - statement created." );

		// only first result is returned (if more than one builder in accessmethod)
		if( itBuild->build(yit) ) 
		{		
			BLOG_DEBUG(LoggerPool::accessor(), 
				       "executeAggregate() - sqlstringbuilder is executable, sql-string valid!" );
			
			if( itBuild->isQuery().first )
			{
				BLOG_DEBUG(LoggerPool::accessor(), 
					       "executeAggregate() - sqlstringbuilder is query." );
			
				if( AGGREGATE_QUERY == (*itBuild).isQuery().second ) 
				{
					// special handling with CICS connection
					if (CICS == m_ConnRef.getDbInfo().eConnectType)
					{
                        prepareForSuccessiveCICSStatements();
					}


					if (itBuild->isPrepareable())
					{
				        preparedStatement = preparePreparedStatement(itBuild, yit);
					    
					    BLOG_DEBUG(LoggerPool::accessor(), 
						        "executeAggregate() - aggregate prepared query executing." );
						        				        
				        return std::make_pair(true, preparedStatement.executeAggregate());
				        
					}
					else
					{				
					    BLOG_DEBUG(LoggerPool::accessor(), 
						        "executeAggregate() - aggregate query executing." );

					    return std::make_pair(true, statement.executeAggregate( (*itBuild).getSQLString() ));
					}
				}
				else 
				{
					VarString msg;
					msg.format("builder of accessmethod <%s> not marked for aggregate select.", 
						       rName.c_str());
					throw InvalidStateException(ExceptInfo(fun, msg, __FILE__, __LINE__)); 	
				}
			} 
			else 
			{
				VarString msg;
				msg.format("builder of accessmethod <%s> not marked as query builder.", 
					       rName.c_str());
				throw InvalidStateException(ExceptInfo(fun, msg.c_str(), __FILE__, __LINE__)); 	
			}
		}
		else
		{
			BLOG_DEBUG(LoggerPool::accessor(), 
				       "executeAggregate() - sqlstringbuilder is NOT executable!" );
		}
	}

	// if no stringbuilder was executable!
	return std::make_pair(false, Decimal() );
}

//-----------------------------------------------------------------------------------//
// default parameter
//-----------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//!	\param  paramList	list containing default values 
//!	\param  rName		name of access method for which all stringbuilders get this default params (default empty)
//!	\throw	UnknownMethodNameException
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void Accessor::setDefaultParameter( cmnutil::ParameterList paramList, const AccessMethodName & rName )
{
	std::map<VarString,AccessMethodRef>::iterator	itMethod; 
	SQLStringBuilderList_Iterator					itBuilder;
	SQLStringBuilderList_Iterator					itBuilderEnd;		

	if ( rName.empty() )
	{
		CollAccessorMethodRef::const_iterator itMethodEnd = m_MethodList.end(); 

		// all SQLBuilders of all methods of this accessor get these default parameters
		for ( itMethod = m_MethodList.begin(); itMethodEnd != itMethod; ++itMethod )
		{
			itBuilder    = itMethod->second.getBuilderList().begin();
			itBuilderEnd = itMethod->second.getBuilderList().end  ();

			while ( itBuilder != itBuilderEnd )
			{
				itBuilder->setParameterList(paramList);
				++itBuilder;
			}

			// all older default Parameter Sets are invalid:
			m_MethodDefaultParameter[itMethod->first] = paramList;
		}
	}
	else
	{
		itMethod = m_MethodList.find( rName );
	
		if( itMethod == m_MethodList.end() )
		{
			VarString msg;
			msg.format("The method name <%s> is unknown for setting default parameter to.", 
				       rName.c_str());
			
			throw UnknownMethodNameException(
					ExceptInfo("basar.db.aspect.Accessor.execute", 
					           msg, 
							   __FILE__, __LINE__) );
		}

		itBuilder    = itMethod->second.getBuilderList().begin();
		itBuilderEnd = itMethod->second.getBuilderList().end  ();

		while ( itBuilder != itBuilderEnd )
		{
			itBuilder->setParameterList(paramList);
			++itBuilder;
		}
	}

	// add here as default value, in order to return with getDefaultParameter
	// necessary for "" (allMethods) and if an SQLBuilder changes its DefaultParamter
	this->m_MethodDefaultParameter[rName] = paramList;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!  returns the given ParameterList, that is set by 
 *   Acessor::setDefaultParameterList and is
 *   stored in the map Acessor::m_MethodDefaultParameter
 *   That is neccessary as the DefaultParamter in the suservent
 *   SQLStringBuilder may be changed seperatetly and because it is 
 *   possible to give a DefaultParameterList for all Methods. 
 *   If there is no DefaultParameterList stored in the map for a
 *   given Method, the DefaultParamterList of the first 
 *   SQLStringBuilder (begin) within this Method is returned.
 *   \param  rName		name of access method for which all stringbuilders get this default params (default empty)
 *	 \throw	 UnknownMethodNameException
 *   \return the Accessor's ParameterList
 */
/////////////////////////////////////////////////////////////////////////////////////////////////////////
cmnutil::ParameterList Accessor::getDefaultParameter(const AccessMethodName & rName) const
{
	cmnutil::ParameterList retval;

	std::map<VarString, cmnutil::ParameterList>::const_iterator iterParamList; 

	iterParamList = m_MethodDefaultParameter.find( rName );

	if ( iterParamList == m_MethodDefaultParameter.end() )
	{
		if (rName.empty())
		{
			// means all methods, when not set
			return retval; // empty DefaultParameterList (default)
		}

		CollAccessorMethodRef::const_iterator iterMethod = m_MethodList.find( rName );

		if (iterMethod == m_MethodList.end() )
		{
			VarString msg;
			msg.format("The method name <%s> is unknown for getting default parameter from.", 
				       rName.c_str());
			throw UnknownMethodNameException( 
					ExceptInfo("basar.db.aspect.Accessor.execute", 
					           msg, 
							   __FILE__, __LINE__) );
		}
		else if (iterMethod->second.size() > 0)
		{
			// try to return separately set ParameterList.
			SQLStringBuilderRef builder = iterMethod->second.getBuilder(0);
	    	retval = builder.getParameterList();
		}
		// else return empty ParameterList (default)
	}
	else
		retval = iterParamList->second;

	return retval;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
//!	\param  rDefParams	string containing default values (separeted with ';')
//!	\param  rName		name of access method for which all stringbuilders get this default params (default empty)
//!	\throw	no-throw
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void Accessor::setDefaultParameter( const VarString & rDefParams, const AccessMethodName& rName )
{
	cmnutil::ParameterList pl(rDefParams);

	return setDefaultParameter(pl, rName);		
}	
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//!	\param  defParams	string containing default values (';'-separeted)
//!	\param  rName		name of access method for which all stringbuilders get this default params (default empty)
//!	\throw	no-throw
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void Accessor::setDefaultParameter( ConstString defParams, const AccessMethodName& rName )
{
	cmnutil::ParameterList pl(defParams);

	return setDefaultParameter(pl, rName);	
}

//-----------------------------------------------------------------------------------//
// transaction methods
//-----------------------------------------------------------------------------------//
///////////////////////////////////////////////////////////////////////////////////////
/*!	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////
void Accessor::beginTransaction()
{
	m_ConnRef.beginTransaction();
}
///////////////////////////////////////////////////////////////////////////////////////
/*!	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////
void Accessor::commitTransaction()
{
	m_TimeLastDatabaseAction= cmnutil::DateTime::getCurrent().getTime();
	m_ConnRef.commitTransaction();
}
///////////////////////////////////////////////////////////////////////////////////////
/*!	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////
void Accessor::rollbackTransaction()
{
	m_TimeLastDatabaseAction= cmnutil::DateTime::getCurrent().getTime();
	m_ConnRef.rollbackTransaction();
}
//////////////////////////////////////////////////////////////////////////////////////
//! \return reference to the created PropertyTableSnapshot
//!	\throw	SnapshotNotAllowedToCreateException 
//////////////////////////////////////////////////////////////////////////////////////
const property::PropertyTableSnapshotRef Accessor::createSnapshot() const
{
	if( true == m_ConnRef.isInTransaction() ){
		throw SnapshotNotAllowedToCreateException( 
				ExceptInfo( "basar.db.aspect.Accessor::createSnapshot()", 
							"Snapshot is not allowed to create, as a transaction is active!", 
							__FILE__, __LINE__) );
	}

	return m_Cacher.createSnapshot();
}
//////////////////////////////////////////////////////////////////////////////////////
//! \param  snapShot reference to snapshot-propertytable
//!	\throw	SnapshotNotAllowedToSetException
//////////////////////////////////////////////////////////////////////////////////////
void Accessor::resetToSnapshot( property::PropertyTableSnapshotRef snapShot )
{
	ConstString fun = "Accessor::resetToSnapshot()";

	//----------------------------//
	//	transaction is activ
	//----------------------------//
	//	--> it is not even allowed to reset to a snapshot as a transaction is going on
	if( m_ConnRef.isInTransaction() )
	{
		throw SnapshotNotAllowedToResetException(
				ExceptInfo(
					fun, 
					"Snapshot is not allowed to reset, if a transaction is active!", 
					__FILE__, __LINE__) );
	}
	
	//----------------------------//
	//	transaction is not active
	//----------------------------//
	//	--> it is allowed to reset to a snapshot:
	//	a) if the snapshot is created before a begin, commit, rollback and an execute and 
	//		should be reset before a begin, commit, rollback and an execute, 
	//		i.e. before any database has done so far.
	//	Or
	//	b) after any database action but before a new one is started.
	if( (m_TimeLastDatabaseAction!= s_UNINITIALIZED_TIME) && 
		(static_cast<ULong32>(snapShot.getSnapshotTime()) < m_TimeLastDatabaseAction) )
	{
			throw SnapshotNotAllowedToResetException(
					ExceptInfo(
						fun, 
						"Snapshot is created at a earlier time "
						"than the last database statement!", 
						__FILE__, __LINE__) );
	}

	m_Cacher.resetToSnapshot( snapShot );
}
//////////////////////////////////////////////////////////////////////////////////////
//!	\throw	no-throw
//////////////////////////////////////////////////////////////////////////////////////
void Accessor::resetInternalSnapshot()
{
	m_Cacher.resetInternalSnapshot();
}
//////////////////////////////////////////////////////////////////////////////////////
//! \return Reference to the Connection
//!	\throw	NullReferenceException, DifferentPropertyTableException
//////////////////////////////////////////////////////////////////////////////////////
const ConnectionRef Accessor::getConnection() const
{
	return m_ConnRef;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//!	\retval	true if a transaction is activ
//!	\return otherwise returns false
//!	\throw	no-throw
////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Accessor::isInTransaction() const
{
	return m_ConnRef.isInTransaction();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//!	\param	toCreate passed argument of type bool indicating if the internal snapshot has to be created.
//!	\throw	NullReferenceException
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Accessor::switchCreateInternalSnapshot( bool toCreate )
{
	m_Cacher.switchCreateInternalSnapshot( toCreate );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//!	\throw	no-throw
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Accessor::clearInternalSnapshot()
{
	m_Cacher.clearInternalSnapshot();
}

// -------------------------------------------------------------------------------------------
//! \return the last inserted Serial 
Int64 Accessor::getLastInsertedSerial(Int32 nr) 
{ 
    if (nr < 0 || nr >= static_cast<Int32>( m_lastInsertedSerials.size()))
    {
        ExceptInfo info( "db::aspect::Accessor::getLastInsertedSerial", 
						 "specified serial number not listed", 
						 __FILE__, __LINE__);
        throw InvalidParameterException(info);
    }

    return m_lastInsertedSerials[nr];
}
// -------------------------------------------------------------------------------------------

bool Accessor::hasLastInsertedSerial()
{ 
    return (m_lastInsertedSerials.size() > 0);
} 

// -------------------------------------------------------------------------------------------

Int32 Accessor::getNumberOfLastInsertedSerials()
{
    return static_cast<Int32>(m_lastInsertedSerials.size());
} 

// -------------------------------------------------------------------------------------------
void Accessor::clearLastInsertedSerials() 
{ 
    m_lastInsertedSerials.clear();
} 

// -------------------------------------------------------------------------------------------
ExecuteErrorEnum  Accessor::getErrorCode(const sql::ExecuteReturnInfo & info)
{
	using db::sql::DBErrorCode;

    switch (info.m_error)
    {
    case DBErrorCode::SUCCESS : 
        return SUCCESS;

    case DBErrorCode::UNKNOWN:  
        return SQL_ERROR_UNKNOWN;

    case DBErrorCode::INSERT_DUPL:    
        return SQL_ERROR_DUPL_INSERT;

    case DBErrorCode::NONSPECIFIC:  
        return SQL_ERROR_NONSPECIFIC;

    default:
        return SQL_ERROR_NONSPECIFIC;
    }
}

// -------------------------------------------------------------------------------------------
void Accessor::extrFromSqlExecRetInfo (const sql::ExecuteReturnInfo & sqlInfo   ,
									   aspect   ::ExecuteResultInfo & directInfo,
									   aspect   ::ExecuteResultInfo & addedInfo )
{
    directInfo.m_affectedRows = sqlInfo.m_affectedRows;

    if (!sqlInfo.hasError())
    {
        directInfo.m_error        = SUCCESS;
        addedInfo.m_affectedRows += directInfo.getAffectedRows();

        // Serial Ids:
        std::vector<Int64>::const_iterator iterEnd = sqlInfo.m_lastSerial.end();

        for (std::vector<Int64>::const_iterator iter = sqlInfo.m_lastSerial.begin(); iter != iterEnd; ++iter)
        {
            m_lastInsertedSerials.push_back(*iter);
        }      
    }
    else
    {
        directInfo.m_error = getErrorCode(sqlInfo); 

        if (SUCCESS != directInfo.m_error)
        {
	        addedInfo.m_error = directInfo.getError();
            addedInfo.m_DbError = sqlInfo.m_DBError;
        }
    }
}

// -------------------------------------------------------------------------------------------

inline void Accessor::setPreparedStatementValues( const AccessorPropertyTable_YIterator& yit, const IndexTypeCollection& indexTypeColl, sql::PreparedStatementRef& psRef)
{
    IndexTypeCollection::const_iterator itEnd = indexTypeColl.end();

    for (IndexTypeCollection::const_iterator it = indexTypeColl.begin(); itEnd != it; ++it)
	{
        basar::Int32 indexInYit = it->m_IndexYit;           
        basar::Int32 indexInPrepStatement = it->m_IndexPreparedStatement;
        
        switch(it->m_Type.getType())
        {
            case basar::INT64:
            {
                psRef.setInt64(indexInPrepStatement, yit.getInt64(indexInYit));
            }
            break;
            //fallthrough
            case basar::INT32:
            case basar::INTDATE:
            case basar::INTTIME:
            {
                psRef.setInt32(indexInPrepStatement, yit.getInt32(indexInYit));
            }
            break;
            case basar::INT16:
            {
                psRef.setInt16(indexInPrepStatement, yit.getInt16(indexInYit));
            }
            break;
            case basar::STRING:
            {
                //need const char* const anyway
                psRef.setString(indexInPrepStatement, yit.getConstString(indexInYit));
            }
            break;
            case basar::FLOAT64:
            {
                psRef.setFloat64(indexInPrepStatement, yit.getFloat64(indexInYit));
            }
            break;
            case basar::FLOAT32:
            {
                psRef.setFloat32(indexInPrepStatement, yit.getFloat32(indexInYit));
            }
            break;
            case basar::DECIMAL:
            {
                psRef.setDecimal(indexInPrepStatement, yit.getDecimal(indexInYit));
            }
            break;
            case basar::TIME:
            {
                psRef.setTime(indexInPrepStatement, yit.getTime(indexInYit));
            }
            break;
            case basar::DATE:
            {
                psRef.setDate(indexInPrepStatement, yit.getDate(indexInYit));
            }
            break;
            case basar::DATETIME:
            {
                psRef.setDateTime(indexInPrepStatement, yit.getDateTime(indexInYit));
            }
            break;
            case basar::TIMESPAN:
            {
                psRef.setTimeSpan(indexInPrepStatement, yit.getTimeSpan(indexInYit));
            }
            break;
            case basar::BINARY:
            {
                psRef.setBinary(indexInPrepStatement, yit.getBinary(indexInYit));
            }
            break;
            default:
            {
                VarString msg;
                msg.format("unknown type <%d> encountered", it->m_Type.getType());
                ExceptInfo info( "db::aspect::Accessor::setPreparedStatementValues", 
						    msg.c_str(), 
						    __FILE__, __LINE__);
                throw UnknownTypeException(info);                
            }
            break;
        }//end switch
    }//end for
}

// -------------------------------------------------------------------------------------------

const sql::PreparedStatementRef& Accessor::getPreparedStatementRef(const basar::VarString& rSql, sql::CursorTypeEnum eCursorType)
{
    PreparedStatementCacheKey key(rSql, eCursorType);
    
    PreparedStatementCacheColl::iterator it = m_PreparedStatementCache.find(key);
    
    if (m_PreparedStatementCache.end() == it)
    {
        BLOG_DEBUG_STREAM(LoggerPool::accessor(), "getPreparedStatementRef() - cache miss, no prepared statement found for sql >" << rSql << "<, cursortype >" << eCursorType << "<");
        //not found, create, store, return
        sql::PreparedStatementRef prepStmt = m_ConnRef.createPreparedStatement(rSql, eCursorType);
        std::pair<PreparedStatementCacheColl::iterator, bool> insRetVal = m_PreparedStatementCache.insert(PreparedStatementCacheColl::value_type(key, prepStmt));
        
        if (insRetVal.second)
        {
            it = insRetVal.first;
        }
        else
        {
            //done: exception, pointing out which element prevented insert
            std::ostringstream os;
            //insRetVal.first: get iterator to blocking element; ->first: get key part of map entry; .first: get first part of key, the sql statement; .second: get second part of key, the cursor type
            os << "store of prepared statement failed, store is blocked by prepared statement for sql >" << insRetVal.first->first.first << "<, cursortype >" << insRetVal.first->first.second << "<";
            ExceptInfo info( "db::aspect::Accessor::setPreparedStatementValues", 
						os.str().c_str(), 
						__FILE__, __LINE__);            
            throw StorePreparedStatementFailedException(info);
        }
    }
    
    return it->second;
}

// -------------------------------------------------------------------------------------------

sql::PreparedStatementRef Accessor::preparePreparedStatement (SQLStringBuilderList_Iterator itBuild, const AccessorPropertyTable_YIterator & yit, sql::CursorTypeEnum eCursorType /*= sql::CURS_DEFAULT*/)
{
    sql::PreparedStatementRef ps = getPreparedStatementRef(itBuild->getSQLString(), eCursorType);
    
    const IndexTypeCollection& indexTypeColl = itBuild->getPreparedParameterTypePositionCollection();

    setPreparedStatementValues(yit, indexTypeColl, ps);

    return ps;
}


// -------------------------------------------------------------------------------------------

void Accessor::prepareForSuccessiveCICSStatements()
{
	// reset (= close) resultset and statement: 
	// important for successive CICS statements; 
	// preceding statement must completely be processed and closed
	m_Cacher.setResultset(sql::ResultsetRef());
	m_Cacher.setStatement(sql::StatementRef());

	BLOG_DEBUG(LoggerPool::accessor(), "prepareForSuccessiveCICSStatements() - preceding CICS statement and resultset is closed" );    
}

// -------------------------------------------------------------------------------------------

void Accessor::setAccessorExecuteListener(const AccessorExecuteListener  & proto)
{
    m_accessorExecuteListenerRef = proto.create();
}

// -------------------------------------------------------------------------------------------

void Accessor::removeAccessorExecuteListener()
{
    m_accessorExecuteListenerRef.reset();
}

// -------------------------------------------------------------------------------------------
AccessorExecuteListenerRef  Accessor::getAccessorExecuteListener() const
{
    return m_accessorExecuteListenerRef;
}

// -------------------------------------------------------------------------------------------
} // aspect
} // db
} // basar
