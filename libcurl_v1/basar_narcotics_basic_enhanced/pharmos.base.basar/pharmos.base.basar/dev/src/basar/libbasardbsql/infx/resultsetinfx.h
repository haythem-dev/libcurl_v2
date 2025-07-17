//----------------------------------------------------------------------------
/*! \file
 *  \brief  resultset Informix implementation class
 *  \author Michael Eichenlaub
 *  \date   27.07.2005
 */
//----------------------------------------------------------------------------

#ifndef GUARD_RESULTSETINFX_H
#define GUARD_RESULTSETINFX_H

//----------------------------------------------------------------------------
#include <boost/scoped_array.hpp>
#include "esqlc.h"
#include "resultset.h"
#include "libbasarcmnutil_bstring.h"
#include "libbasarcmnutil_decimal.h"
#include "libbasarcmnutil_datetime.h"
#include "libbasarcmnutil_date.h"
#include "libbasarcmnutil_timespan.h"

//----------------------------------------------------------------------------
struct sqlda;

//----------------------------------------------------------------------------
namespace basar	{
namespace db	{
namespace sql	{

typedef std::map<basar::UInt32, basar::cmnutil::DateTime* > DateTimeCacheType;      //!< typedef for DateTimeCacheType

typedef std::map<basar::UInt32, basar::cmnutil::TimeSpan* > TimeSpanCacheType;      //!< typedef for TimeSpanCacheType

typedef std::map<basar::UInt32, basar::cmnutil::Decimal* > DecimalCacheType;        //!< typedef for DecimalCacheType

typedef std::map<basar::UInt32, basar::cmnutil::Date * > DateCacheType;             //!< typedef for DateCacheType

//----------------------------------------------------------------------------
//! resultset implementation class \n base class, should not be instantiated
class ResultsetInfx : public Resultset
{
public:

	// --- method section - object state handling ----------------------------

	//! close and destruct resultset implementation object					\n no-throw

	virtual					   ~ResultsetInfx		();

	//! release resultset resources											\n no-throw
	virtual void				close				();

	// --- method section - column handling  ---------------------------------

    //! get a column's value as an Int64									\n no-throw
    virtual const Int64 &       getInt64            (UInt32 index       //!< column index (0, ..., n-1)
                                                    ) const;

	//! get a column's value as an Int32									\n no-throw
	virtual const Int32 &		getInt32			(UInt32 index		//!< column index (0, ..., n-1)
													) const;
	//! get a column's value as an Int16									\n no-throw
	virtual const Int16 &		getInt16			(UInt32 index		//!< column index (0, ..., n-1)
													) const;
	//! get a column's value as a string									\n no-throw
	virtual ConstBuffer 		getString			(UInt32 index		//!< column index (0, ..., n-1)
													) const;
    //! get a column's value as a binary									\n no-throw
    virtual const Binary &      getBinary           (UInt32 index       //!< column index (0, ..., n-1)
                                                    ) const;
	//! get a column's value as a Float64									\n no-throw
	virtual const Float64 &		getFloat64			(UInt32 index		//!< column index (0, ..., n-1)
													) const;
	//! get a column's value as a Float32									\n no-throw
	virtual const Float32 &		getFloat32			(UInt32 index		//!< column index (0, ..., n-1)
													) const;
	//! get a column's value as a decimal									\n no-throw
	virtual const Decimal &		getDecimal			(UInt32 index		//!< column index (0, ..., n-1)
													) const;
	//! get a column's value as a Date										\n no-throw
	virtual const Date &		getDate				(UInt32 index		//!< column index (0, ..., n-1)
													) const;
	//! get a column's value as a Time										\n no-throw
    virtual const Time &		getTime				(basar::UInt32 index		//!< column index (0, ..., n-1)
													) const;
	//! get a column's value as a DateTime									\n no-throw
	virtual const DateTime &	getDateTime			(UInt32 index		//!< column index (0, ..., n-1)
													) const;

	//! get a column's value as a TimeSpan									\n no-throw
	virtual const TimeSpan &	getTimeSpan			(UInt32 index		//!< column index (0, ..., n-1)
													) const;

	//! get a column's return type											\n no-throw
	InfxSqlTypeEnum		        getType				(UInt32            index		//!< column index (0, ..., n-1)
													) const;
	//! get a column's return type											\n no-throw
	InfxSqlTypeEnum		        getTypeByName		(const VarString & rColName		//!< column name
													) const;

protected:

	// --- method section ----------------------------------------------------

	//! constructs implementation object by connection reference			\n no-throw
								ResultsetInfx		(ConnectionSharedPtr pConn,		//!< shared pointer to connection implementation object
													 const VarString &   rStmtId	//!< statement ID
													);
	//! describe statement and create row buffer							\n throw InvalidParameterException
	void						describeStmt		();
	//! free statement description 
	void                        freeStmtDescription ();

	//! set connection to current								\n no-throw
	void						setCurrent			();

	// --- member section ----------------------------------------------------

	VarString					m_StmtId;			//!< application's unique statement ID  (i.e. statement object address)
	::sqlda *					m_pDescOut;			//!< sql descriptor area for output data -> selected values



	mutable DateTime			m_RetValDateTime;	//!< to return reference to DateTime values
	mutable Date				m_RetValDate;		//!< to return reference to Date values
	mutable Time				m_RetValTime;		//!< to return reference to Time values
	mutable TimeSpan			m_RetValTimeSpan;   //!< to return reference to TimeSpan values
	mutable Decimal				m_RetValDecimal;	//!< to return reference to Decimal values
    mutable Binary              m_RetValBinary;     //!< to return reference to Byte values

private:

	typedef boost::scoped_array < char  >	RowSmartPtr;		//!< definition for smart pointer to row buffer
	typedef boost::scoped_array < Int16 >	RowIndicatorPtr;	//!< definition for smart pointer to indicator variables (DB: NULL values)

	// --- method section ----------------------------------------------------

	//! for exceptions: get recommended method name for give SQL type
	static ConstBuffer 			getMethodName		(Int32 type					//!< simplified Informix SQL type (integral type because of hiding Informix type enum)
													);

								ResultsetInfx		();								//!< forbidden
								ResultsetInfx		(const ResultsetInfx & rCopy);	//!< forbidden
	ResultsetInfx &    			operator =			(const ResultsetInfx & rCopy);	//!< forbidden

	//! check if column type is compatible with called method	\n throw InvalidParameterException
	void						checkType			(UInt32          index,		//!< column index (0, ..., n-1)
													 InfxSqlTypeEnum cmpType,	//!< simplified Informix column type, compared with indexed column type
													 ConstString     func,		//!< calling method
													 ConstString     file,		//!< calling file
													 UInt32		     line		//!< calling file line
													) const;

    inline basar::SupportedTypeEnum    getBasarTypeForInfxType( const InfxSqlTypeEnum infxType ) const;

	// --- member section ----------------------------------------------------

	RowSmartPtr					m_pRow;				//!< smart pointer to row buffer
	RowIndicatorPtr				m_pIndicator;		//!< smart pointer to indicator variables (DB: NULL values)
};

//----------------------------------------------------------------------------
}	// namespace sql
}	// namespace db
}	// namespace basar

#endif	// GUARD
