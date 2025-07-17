//------------------------------------------------------------------------------
/*! \file
 *  \brief  public library interface: reference class for SQL prepared statements
 *  \author Roland Kiefert
 *  \date   02.11.2005
 */
//------------------------------------------------------------------------------

#ifndef GUARD_LIBBASARDBSQL_PREPAREDSTATEMENTREF_H
#define GUARD_LIBBASARDBSQL_PREPAREDSTATEMENTREF_H

//------------------------------------------------------------------------------
#include "libbasar_definitions.h"
#include "libbasardbsql_api.h"
#include "libbasardbsql_definitions.h"

//------------------------------------------------------------------------------
namespace basar {
namespace db	{
namespace sql	{

//------------------------------------------------------------------------------
// forward
class  ResultsetRef;
class  InsertsetRef;
struct ExecuteReturnInfo;

//------------------------------------------------------------------------------
//! reference class representing SQL parameter statement \n final class 
class PreparedStatementRef
{
	friend class ConnectionInfx;	//!< creator class
	friend class ConnectionOdbc;	//!< creator class
	friend class ConnectionCics;	//!< creator class

public:
	//! \brief construct reference object and initialise shared pointer to implementation object to null \n no-throw
	LIBBASARDBSQL_API					PreparedStatementRef();

	//! \brief destruct reference object																	\n no-throw
	LIBBASARDBSQL_API				   ~PreparedStatementRef();

	LIBBASARDBSQL_API void				reset				() ;		//!< set pointer to implementation object to null		\n no-throw
	LIBBASARDBSQL_API bool				isNull				() const;	//!< is shared pointer to implementation class null?	\n no-throw

	//! execute a SQL select statement, return a resultset					\n no-throw
	LIBBASARDBSQL_API ResultsetRef		executeQuery		();
	//! execute a SQL single select statement, return a one row resultset	\n no-throw
	LIBBASARDBSQL_API ResultsetRef		executeSingleQuery	();
	//! execute insert cursor statement										\n no-throw
	LIBBASARDBSQL_API InsertsetRef		executeInsert		();
	//! execute a given SQL statement										\n no-throw
	LIBBASARDBSQL_API ExecuteReturnInfo	execute				(bool flagExcept = true	//!< if error: DB exception / return code
															);

	//! execute a SQL single select statement containing count(*) or sum, avg, min, max on numeric fields	\n no-throw
	LIBBASARDBSQL_API basar::Decimal    executeAggregate	();

	//! release statement resources											\n no-throw
	LIBBASARDBSQL_API void				close				();
	//! is statement open (i.e. prepared)?									\n no-throw
	LIBBASARDBSQL_API bool				isOpen				() const;

    //! \brief set a parameter value as an Int64									\n no-throw									
    LIBBASARDBSQL_API void              setInt64            (UInt32 pos,                //!< position of parameter
                                                             const Int64 & rValue       //!< value of parameter
                                                            );
	//! \brief set a parameter value as an Int32									\n no-throw									
	LIBBASARDBSQL_API void				setInt32			(UInt32        pos,			//!< position of parameter
															 const Int32 & rValue		//!< value of parameter
															);
	//! \brief set a parameter value as an Int16									\n no-throw										
	LIBBASARDBSQL_API void				setInt16			(UInt32        pos,			//!< position of parameter
															 const Int16 & rValue		//!< value of parameter
															);
	//! \brief set a parameter value as a string									\n no-throw										
	LIBBASARDBSQL_API void	 			setString			(UInt32      pos,			//!< position of parameter
															 ConstString value			//!< value of parameter
															);
    //! \brief set a parameter value as a binary									\n no-throw										
    LIBBASARDBSQL_API void              setBinary           (UInt32 pos,                //!< position of parameter
                                                             const Binary& value        //!< value of parameter
                                                            );
	//! \brief set a parameter value as a Float64									\n no-throw										
	LIBBASARDBSQL_API void				setFloat64			(UInt32          pos,		//!< position of parameter
															 const Float64 & rValue		//!< value of parameter
															);
	//! \brief set a parameter value as a Float32									\n no-throw										
	LIBBASARDBSQL_API void				setFloat32			(UInt32          pos,		//!< position of parameter
															 const Float32 & rValue		//!< value of parameter
															);
	//! \brief set a parameter value as a decimal									\n no-throw										
	LIBBASARDBSQL_API void				setDecimal			(UInt32          pos,		//!< position of parameter
															 const Decimal & rValue		//!< value of parameter
															);
	//! \brief set a parameter value as a Date										\n no-throw											
	LIBBASARDBSQL_API void				setDate				(UInt32           pos,		//!< position of parameter
															 const Date     & rValue	//!< value of parameter
															);
	//! \brief set a parameter value as a Time										\n no-throw											
	LIBBASARDBSQL_API void				setTime				(UInt32           pos,		//!< position of parameter
															 const Time     & rValue	//!< value of parameter
															);
	//! \brief set a parameter value as a DateTime									\n no-throw										
	LIBBASARDBSQL_API void				setDateTime			(UInt32		      pos,		//!< position of parameter
															 const DateTime & rValue	//!< value of parameter
															);
	//! \brief set a parameter value as a TimeSpan									\n no-throw										
	LIBBASARDBSQL_API void				setTimeSpan			(UInt32		      pos,		//!< position of parameter
															 const TimeSpan & rValue	//!< value of parameter
															);


private:
	//! construct statement reference object by shared pointer to implementation object \n no-throw
										PreparedStatementRef(PreparedStatementSharedPtr   pImpl); 
	
	//! check and return shared pointer to implementation object \n throw NullReferenceException
	const PreparedStatementSharedPtr &	getImpl				() const;

	PreparedStatementSharedPtr			m_pImpl;	//!< shared pointer to implementation object
};

//------------------------------------------------------------------------------
}	// namespace sql
}	// namespace db
}	// namespace basar

//------------------------------------------------------------------------------
#endif	// GUARD
