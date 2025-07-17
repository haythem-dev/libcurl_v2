/*
 * @file resultsetcics.h
 * @brief ...
 * @author Anke Klink
 * @date 2007
 */

#ifndef __GUARD_BASAR_LIBBASARDBSQL_CICS_RESULTSETCICS_H__ 
#define __GUARD_BASAR_LIBBASARDBSQL_CICS_RESULTSETCICS_H__ 

//----------------------------------------------------------------------------
#include "resultset.h"
#include "cicsdefinitions.h"

//----------------------------------------------------------------------------
namespace basar {
namespace db    {
namespace sql   {

//----------------------------------------------------------------------------
//! result set of StatementCics
class ResultsetCics : public Resultset
{
public:

	//! create resultset implementation object \n no-throw
	static ResultsetSharedPtr		create				(ConnectionSharedPtr       pConn,	//!< connection
														 const CicsStatementInfo & info		//!< statementInfo
														);

	//! c'tor
									ResultsetCics		(ConnectionSharedPtr pConn,         //!< connection
										                 const CicsStatementInfo & info     //!< statementInfo
														);

	//! d'tor \n no-throw
	virtual						   ~ResultsetCics		();

	//! move to the next row in the result set \n no-throw
	virtual bool					next				();
	//! \brief release resultset resources \n no-throw
	virtual void					close				();

    //! get a column's value as an Int64 - not implemented \n NotImplementedException
    //! \return column value as Int64
    virtual const Int64 &          getInt64             (UInt32 index   //!< column index (0, ..., n-1)
                                                        ) const;

	//! get a column's value as an Int32 \n no-throw
	//! \return column value as Int32
	virtual const Int32 &			getInt32			(UInt32 index	//!< column index (0, ..., n-1)
														) const;
	//! get a column's value as an Int16 \n no-throw
	//! \return column value as Int16
	virtual const Int16 &			getInt16			(UInt32 index	//!< column index (0, ..., n-1)
														) const;
	//! get a column's value as a string \n no-throw
	//! \return column value as C-String
	virtual ConstBuffer				getString			(UInt32 index	//!< column index (0, ..., n-1)
														) const;
    //! get a column's value as a binary - not implemented \n NotImplementedException
    //! \return column value as binary
    virtual const Binary &          getBinary           (UInt32 index   //!< column index (0, ..., n-1)
                                                        ) const;
	//! get a column's value as a Float64 \n no-throw
	//! \return column value as Float64
	virtual const Float64 &			getFloat64			(UInt32 index	//!< column index (0, ..., n-1)
														) const;
	//! get a column's value as a Float32 \n no-throw
	//! \return column value as Float32
	virtual const Float32 &			getFloat32			(UInt32 index	//!< column index (0, ..., n-1)
														) const;
	//! get a column's value as a decimal \n no-throw
	//! \return column value as Decimal
	virtual const Decimal &			getDecimal			(UInt32 index	//!< column index (0, ..., n-1)
														) const;
	//! get a column's value as a Date \n no-throw
	//! \return column value as Date
	virtual const Date &			getDate				(UInt32 index	//!< column index (0, ..., n-1)
														) const;
	//! get a column's value as a Time \n no-throw
	//! \return column value as Time
	virtual const Time &			getTime				(UInt32 index	//!< column index (0, ..., n-1)
														) const;
	//! get a column's value as a DateTime \n no-throw
	//! \return column value as DateTime
	virtual const DateTime &		getDateTime			(UInt32 index	//!< column index (0, ..., n-1)
														) const;
	
    //! get a column's value as a TimeSpan \n throw NotImplementedException
    //! \return column value as TimeSpan
	virtual const TimeSpan &		getTimeSpan			(UInt32 index	//!< column index (0, ..., n-1)
														) const;
	
    //! returns the CicsDefinitions::Type of a column 
    //! \n throw InvalidParameterException
	CicsTypeEnum					getType				(UInt32 index	//!< column index (0, ..., n-1)
														) const;

protected:

	//! describe statement and create map with column names		
    //! \n throw InvalidParameterException
	void							describeStmt		();

	//! checks the Range and Type of a column	
    //! \n throw InvalidParameterException
	void							checkRangeAndType	(UInt32		   index,		//!< column
														 CicsTypeEnum  type,		//!< Type checked
														 ConstString   func,		//!< Functionname of the function that calles this Function
														 ConstString   file,		//!< Filename, where the Error ocurred
														 const UInt32  line			//!< Fileline, where the Error ocurred
														) const;
	//! \brief writes requested host value to clientside CicsValue	
    //! \n throw OutOfRangeIndexException
	void							getValueInternal	(UInt32        index,		//!< column
														 CicsValue   & value		//!< ReturnValue
														) const;
	//! writes the Value to CicsValue
	void							getValue			(UInt32		   index,		//!< column
														 CicsValue   & value,		//!< ReturnValue
														 ConstString   func,		//!< Functionname of the function that calles this Function
														 ConstString   file,		//!< Filename, where the Error ocurred
														 const UInt32  line			//!< Fileline, where the Error ocurred
														) const;
	//! writes a StringValue
	void							getStringValue		(UInt32        index,		//!< column
														 ConstString   func,		//!< Functionname of the function that calles this Function
														 ConstString   file,		//!< Filename, where the Error ocurred
														 const UInt32  line			//!< Fileline, where the Error ocurred
														) const;

private:	

	//! for exceptions: get recommended method name for give SQL type
	static ConstBuffer 				getMethodName		(CicsTypeEnum type			//!< corresponding basar types for CICS types
														);
    //! get basar type to cics type
    inline basar::SupportedTypeEnum    getBasarTypeForCicsType( const CicsTypeEnum cicsType //!< column type
        ) const;

	const CicsStatementInfo		&	m_stmtInfo;	 //!< statement stuff
	CicsResultsetInfo				m_resInfo;				//!< resultset stuff

	mutable CicsReturnValues		m_resValues;			//!< resultset values
};	// class

//----------------------------------------------------------------------------
}	// namespace
}	// namespace
}	// namespace 

//----------------------------------------------------------------------------
#endif 
