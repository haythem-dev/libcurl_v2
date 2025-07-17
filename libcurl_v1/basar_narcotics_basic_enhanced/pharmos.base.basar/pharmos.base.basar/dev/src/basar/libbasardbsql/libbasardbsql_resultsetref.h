//------------------------------------------------------------------------------
/*! \file
 *  \brief  public library interface: reference class for resultset
 *  \author Michael Eichenlaub
 *  \date   14.08.2005
 */
//------------------------------------------------------------------------------

#ifndef GUARD_LIBBASARDBSQL_RESULTSETREF_H
#define GUARD_LIBBASARDBSQL_RESULTSETREF_H

//------------------------------------------------------------------------------
#include "libbasardbsql_api.h"
#include "libbasardbsql_definitions.h"

//------------------------------------------------------------------------------
namespace basar {
namespace db	{
namespace sql	{

//------------------------------------------------------------------------------
//! \brief reference class representing database result set 
//! \n     final class 
//!
//! column names are stored in lower case, must be in sync with Property Description List!
//! When Resultset implementation object is constructed, an internal open() is called.\n
//! i.e.: in resultset with multiple rows cursor will immediately be opened;
//! so statements with "order by" clause can last some time. But this results in 
//! avoiding time penalties at calling next(), because an opened cursor can be assumed
//! (no need for opened cursor checking).
class ResultsetRef
{
	friend class StatementInfx;			//!< creator class
	friend class StatementOdbc;			//!< creator class
	friend class StatementCics;			//!< creator class
	friend class PreparedStatementInfx;	//!< creator class
	friend class PreparedStatementOdbc;	//!< creator class
	friend class PreparedStatementCics;	//!< creator class

public:

	// --- method section - reference handling -------------------------------

	//! construct reference object and initialise shared pointer to implementation object to null \n no-throw
	LIBBASARDBSQL_API						ResultsetRef		();

	//! destruct reference object																	\n no-throw
	LIBBASARDBSQL_API					   ~ResultsetRef		();

	LIBBASARDBSQL_API void					reset				() ;		//!< set pointer to implementation object to null		\n no-throw
	LIBBASARDBSQL_API bool					isNull				() const;	//!< check if shared pointer to implementation class is null	\n no-throw


	// --- method section - impl object state handling -----------------------

	//! check if resultset is open (i.e. cursor opened)							\n no-throw
	LIBBASARDBSQL_API bool					isOpen				() const;
	//! release resultset resources (also database resources: cursor, handles)! 
	//! So avoid calling close() in loops with PreparedStatementRef::executeXXX() 
	//! due to performance penalties.
	//! \n no-throw
	LIBBASARDBSQL_API void					close				();
	//! reopen previously opened resultset										\n no-throw
	LIBBASARDBSQL_API void					reopen				();


	// --- method section - row handling -------------------------------------

	//! delete/update on the current row								\n no-throw
	LIBBASARDBSQL_API Int32					execCurrentRow		(const VarString & rSql			//!< statement for delete/update
																);

	//! return number of already fetched rows					\n no-throw
	LIBBASARDBSQL_API UInt32				getFetchedRows		() const;

    //! \return column names of executed query, same order as in statement     \n no-throw
    LIBBASARDBSQL_API const CollColNames&   getColumnNames      () const;

    LIBBASARDBSQL_API const CollColTypes&   getColumnTypes      () const;

	//! move to the next row in the result set					\n no-throw
	LIBBASARDBSQL_API bool					next				();
	//! check if cursor is before first row in result set		\n no-throw
	LIBBASARDBSQL_API bool					isBeforeFirst		() const;
	//! check if cursor is after last row in result set			\n no-throw
	LIBBASARDBSQL_API bool					isAfterLast			() const;


	// --- method section - column handling  ---------------------------------

	//! return number of selected columns						\n no-throw
	LIBBASARDBSQL_API UInt32				getSelectedCols		() const;
	//! check if column name exists in resultset					\n no-throw
	LIBBASARDBSQL_API bool					isExist				(const VarString & rColName		//!< column name
																) const;
	//! return index (0, ..., n-1) of column name in resultset	\n no-throw
	LIBBASARDBSQL_API UInt32				getIndex			(const VarString & rColName		//!< column name
																) const;

    //! get a column's value as an Int64						\n no-throw
    LIBBASARDBSQL_API const Int64 &         getInt64            (UInt32            index       //!< column index (0, ..., n-1)
                                                                ) const;
    //! get a column's value as an Int64						\n no-throw
    LIBBASARDBSQL_API const Int64 &         getInt64            (const VarString&  rColName    //!< column name
                                                                ) const;
	//! get a column's value as an Int32						\n no-throw
	LIBBASARDBSQL_API const Int32 &			getInt32			(UInt32            index		//!< column index (0, ..., n-1)
																) const;
	//! get a column's value as an Int32						\n no-throw
	LIBBASARDBSQL_API const Int32	&		getInt32			(const VarString & rColName		//!< column name
																) const;
	//! get a column's value as an Int16						\n no-throw
	LIBBASARDBSQL_API const Int16 &			getInt16			(UInt32            index		//!< column index (0, ..., n-1)
																) const;
	//! get a column's value as an Int16						\n no-throw
	LIBBASARDBSQL_API const Int16	&		getInt16			(const VarString & rColName		//!< column name
																) const;
	//! get a column's value as a string						\n no-throw
	LIBBASARDBSQL_API ConstBuffer			getString			(UInt32            index		//!< column index (0, ..., n-1)
																) const;
	//! get a column's value as a string						\n no-throw
	LIBBASARDBSQL_API ConstBuffer			getString			(const VarString & rColName		//!< column name
																) const;
    //! get a column's value as a binary						\n no-throw
    LIBBASARDBSQL_API const Binary &        getBinary           (UInt32            index        //!< column index (0, ..., n-1)
                                                                ) const;
    //! get a column's value as a binary						\n no-throw
    LIBBASARDBSQL_API const Binary &        getBinary           (const VarString&  rColName     //!< column name
                                                                ) const;
	//! get a column's value as a Float64						\n no-throw
	LIBBASARDBSQL_API const Float64 &		getFloat64			(UInt32            index		//!< column index (0, ..., n-1)
																) const;
	//! get a column's value as a Float64						\n no-throw
	LIBBASARDBSQL_API const Float64 &		getFloat64			(const VarString & rColName		//!< column name
																) const;
	//! get a column's value as a Float32						\n no-throw
	LIBBASARDBSQL_API const Float32 &		getFloat32			(UInt32            index		//!< column index (0, ..., n-1)
																) const;
	//! get a column's value as a Float32						\n no-throw
	LIBBASARDBSQL_API const Float32 &		getFloat32			(const VarString & rColName		//!< column name
																) const;
	//! get a column's value as a decimal						\n no-throw
	LIBBASARDBSQL_API const Decimal &		getDecimal			(UInt32            index		//!< column index (0, ..., n-1)
																) const;
	//! get a column's value as a decimal						\n no-throw
	LIBBASARDBSQL_API const Decimal &		getDecimal			(const VarString & rColName		//!< column name
																) const;
	//! get a column's value as a Date							\n no-throw
	LIBBASARDBSQL_API const Date &			getDate				(UInt32            index		//!< column index (0, ..., n-1)
																) const;
	//! get a column's value as a Date							\n no-throw
	LIBBASARDBSQL_API const Date &			getDate				(const VarString & rColName		//!< column name
																) const;
	//! get a column's value as a Time							\n no-throw
	LIBBASARDBSQL_API const Time &			getTime				(UInt32            index		//!< column index (0, ..., n-1)
																) const;
	//! get a column's value as a Time							\n no-throw
	LIBBASARDBSQL_API const Time &			getTime				(const VarString & rColName		//!< column name
																) const;
		//! get a column's value as a DateTime					\n no-throw
	LIBBASARDBSQL_API const DateTime &		getDateTime			(UInt32            index		//!< column index (0, ..., n-1)
																) const;
	//! get a column's value as a DateTime						\n no-throw
	LIBBASARDBSQL_API const DateTime &		getDateTime			(const VarString & rColName		//!< column name
																) const;
	//! get a column's value as a TimeSpan					\n no-throw
	LIBBASARDBSQL_API const TimeSpan &		getTimeSpan			(UInt32            index		//!< column index (0, ..., n-1)
																) const;
	//! get a column's value as a TimeSpan						\n no-throw
	LIBBASARDBSQL_API const TimeSpan &		getTimeSpan			(const VarString & rColName		//!< column name
																) const;


private:
	//! construct resultset reference object by shared pointer to implementation object \n no-throw
											ResultsetRef		(ResultsetSharedPtr pImpl		//!< shared pointer to implementation object
																);
	
	//! check and return shared pointer to implementation object	\n throw NullReferenceException
	inline const ResultsetSharedPtr &				getImpl				() const;


	ResultsetSharedPtr						m_pImpl;	//!< shared pointer to implementation object
};

//------------------------------------------------------------------------------
}	// namespace sql
}	// namespace db
}	// namespace basar

//------------------------------------------------------------------------------
#endif	// GUARD
