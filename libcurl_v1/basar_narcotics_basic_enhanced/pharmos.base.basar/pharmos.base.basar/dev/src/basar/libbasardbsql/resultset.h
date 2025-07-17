//----------------------------------------------------------------------------
/*! \file
 *  \brief  resultset implementation class
 *  \author Michael Eichenlaub
 *  \date   27.07.2005
 */
//----------------------------------------------------------------------------

#ifndef GUARD_RESULTSET_H
#define GUARD_RESULTSET_H

//----------------------------------------------------------------------------
#include "sqloperative.h"

//----------------------------------------------------------------------------
namespace basar	{
namespace db	{
namespace sql	{

//----------------------------------------------------------------------------
//! resultset implementation class \n base class
//!
//! column names are stored in lower case, must be in sync with Property Description List!
class Resultset : public SqlOperative
{
public:

	// --- method section - object state handling ----------------------------

	//! close and destruct resultset implementation object					\n no-throw
	virtual					   ~Resultset			();

	//! is resultset open (i.e. cursor opened)?								\n no-throw
	bool						isOpen				() const;
	//! release resultset resources											\n no-throw
	virtual void				close				() = 0;
	//! reopen previously opened resultset									\n throw InvalidStateException
	virtual void				reopen				();


	// --- method section - row handling -------------------------------------

	//! delete/update on the current row									\n throw InvalidStateException
	virtual Int32				execCurrentRow		(const VarString & rSql //!< statement for delete/update
													);

	//! move to the first/next row in the result set						\n no-throw
	virtual bool				next				() = 0;

	//! return number of already fetched rows								\n no-throw
	UInt32						getFetchedRows		() const;

    //! \return column names of executed query, same order as in statement  \n no-throw
    const CollColNames&         getColumnNames      () const;

    const CollColTypes&         getColumnTypes      () const;

	//! check if cursor is before first row in result set					\n no-throw
	bool						isBeforeFirst		() const;
	//! check if cursor is after last row in result set						\n no-throw
	bool						isAfterLast			() const;


	// --- method section - column handling  ---------------------------------

	//! return number of selected columns									\n no-throw
	UInt32						getSelectedCols		() const;
	//! does column name exist in resultset?								\n no-throw
	bool						isExist				(const VarString & rColName		//!< column name
													) const;
	//! return index (0, ..., n-1) of column name in resultset				\n throw OutOfRangeIndexException
	UInt32						getIndex			(const VarString & rColName		//!< column name
													) const;
    //! get a column's value as an Int64									\n no-throw
    //! \return column value as Int64
    virtual const Int64 &        getInt64           (UInt32            index        //!< column index (0, ..., n-1)
                                                    ) const = 0;
    //! get a column's value as an Int64									\n no-throw
    //! \return column value as Int64
    const Int64 &                getInt64ByName     (const VarString&  rColName     //!< column name
                                                    ) const;
	//! get a column's value as an Int32									\n no-throw
	//! \return column value as Int32
	virtual const Int32 &		getInt32			(UInt32            index		//!< column index (0, ..., n-1)
													) const = 0;
	//! get a column's value as an Int32									\n no-throw
	const Int32 &				getInt32ByName		(const VarString & rColName		//!< column name
													) const;
	//! get a column's value as an Int16									\n no-throw
	//! \return column value as Int16
	virtual const Int16 &		getInt16			(UInt32            index		//!< column index (0, ..., n-1)
													) const = 0;
	//! get a column's value as an Int16									\n no-throw
	const Int16 &				getInt16ByName		(const VarString & rColName		//!< column name
													) const;
	//! get a column's value as a string									\n no-throw
	//! \return column value as C-String
	virtual ConstBuffer 		getString			(UInt32            index		//!< column index (0, ..., n-1)
													) const = 0;
	//! get a column's value as a string									\n no-throw
	ConstBuffer 				getStringByName		(const VarString & rColName		//!< column name
													) const;
    //! get a column's value as a binary									\n no-throw
    //! \return column value as binary
    virtual const Binary &      getBinary           (UInt32            index       //!< column index (0, ..., n-1)
                                                    ) const = 0;
    //! get a column's value as a binary									\n no-throw
    //! \return column value as binary
    const Binary &              getBinaryByName     (const VarString & rColName    //!< column name
                                                    ) const;
	//! get a column's value as a Float64									\n no-throw
	//! \return column value as Float64
	virtual const Float64 &		getFloat64			(UInt32            index		//!< column index (0, ..., n-1)
													) const = 0;
	//! get a column's value as a Float64									\n no-throw
	const Float64 &				getFloat64ByName	(const VarString & rColName		//!< column name
													) const;
	//! get a column's value as a Float32									\n no-throw
	//! \return column value as Float32
	virtual const Float32 &		getFloat32			(UInt32            index		//!< column index (0, ..., n-1)
													) const = 0;
	//! get a column's value as a Float32									\n no-throw
	const Float32 &				getFloat32ByName	(const VarString & rColName		//!< column name
													) const;
	//! get a column's value as a decimal									\n no-throw
	//! \return column value as Decimal
	virtual const Decimal &		getDecimal			(UInt32            index		//!< column index (0, ..., n-1)
													) const = 0;
	//! get a column's value as a decimal									\n no-throw
	const Decimal &				getDecimalByName	(const VarString & rColName		//!< column name
													) const;
	//! get a column's value as a Date										\n no-throw
	//! \return column value as Date
	virtual const Date &		getDate				(UInt32            index		//!< column index (0, ..., n-1)
													) const = 0;
	//! get a column's value as a Date										\n no-throw
	const Date &				getDateByName		(const VarString & rColName		//!< column name
													) const;
	//! get a column's value as a Time										\n no-throw
	//! \return column value as Time
	virtual const Time &		getTime				(UInt32            index		//!< column index (0, ..., n-1)
													) const = 0;
	//! get a column's value as a Time										\n no-throw
	const Time &				getTimeByName		(const VarString & rColName		//!< column name
													) const;
	//! get a column's value as a DateTime									\n no-throw
	//! \return column value as DateTime
	virtual const DateTime &	getDateTime			(UInt32            index		//!< column index (0, ..., n-1)
													) const = 0;

    //! get a column's value as a DateTime									\n no-throw
	const DateTime &			getDateTimeByName	(const VarString & rColName		//!< column name
													) const;

    //! get a column's value as a TimeSpan									\n no-throw
    //! \return column value as TimeSpan
	virtual const TimeSpan &	getTimeSpan			(UInt32            index		//!< column index (0, ..., n-1)
													) const = 0;
	
    //! get a column's value as a DateTime									\n no-throw
	const TimeSpan &			getTimeSpanByName	(const VarString & rColName		//!< column name
													) const;

protected:

	// --- enum section ------------------------------------------------------

	enum StateEnum				//! resultset state (important order!)
	{
		STATE_INVALID,			//!< object invalid				(cursor not yet declared/opened)
		STATE_OPENED			//!< object opened				(cursor declared & opened)
	};

	enum PositionEnum			//! current position in resultset
	{
		POS_INVALID,			//!< (cursor) position invalid (resultset not created)
		POS_BEFORE_FIRST,		//!< (cursor) before first row 
		POS_VALID,				//!< (cursor) on a valid row
		POS_AFTER_LAST			//!< (cursor) after last row
	};

	// --- method section ----------------------------------------------------

	//! constructs implementation object by connection reference			\n no-throw
								Resultset			(ConnectionSharedPtr pConn		//!< shared pointer to connection implementation object
													);

	//! check connection state                        						\n no-throw
	virtual void				checkConnState		(const VarString & rWhat,	//!< text description 
													 const VarString & rFile,	//!< file where exception is thrown
													 UInt32			   line		//!< line of file where exception is thrown 
													) const;

	//! check if index is within correct range		\n throw OutOfRangeIndexException
	void						checkRange			(UInt32      index,		//!< column index (0, ..., n-1)
													 ConstString func,		//!< calling method
													 ConstString file,		//!< calling file
													 UInt32		 line		//!< calling file line
													) const;

	//! is raw flag for fetching set?				\n no-throw
	bool						isFetchModeRaw		() const;

	//! store column names (retrieved by statement description) into collection; names are standardised (lower/upper case)	
	//  \n throw InvalidParameterException
	void						storeColNames		(ConstString name,		//!< column name
													 UInt32      index,		//!< column index
													 ConstString func,		//!< calling method
													 ConstString file,		//!< calling file
													 UInt32		 line		//!< calling file line
													);

	//! store column types (retrieved by statement description) into collection
	//  \n throw InvalidParameterException
    void                        storeColTypes       (basar::SupportedTypeEnum   type   //!< column type
                                                    );
    //! checks if next has been called at least once
    void                        checkNextCalled() const;

    // --- typedef section ----------------------------------------------------

    typedef std::map < VarString, UInt32 >          CollColItems;               //!< definition for map with selected column names/indices
	// --- member section ----------------------------------------------------

	static ConstString			s_METHOD_INVALID;		//!< invalid SQL type
	static ConstString			s_METHOD_GETINT32;		//!< method name for getInt32   ()
	static ConstString			s_METHOD_GETINT16;		//!< method name for getInt16   ()
	static ConstString			s_METHOD_GETSTRING;		//!< method name for getString  ()
	static ConstString			s_METHOD_GETFLOAT64;	//!< method name for getFloat64 ()
	static ConstString			s_METHOD_GETFLOAT32;	//!< method name for getFloat32 ()
	static ConstString			s_METHOD_GETDECIMAL;	//!< method name for getDecimal ()
	static ConstString			s_METHOD_GETTIME;		//!< method name for getTime    ()
	static ConstString			s_METHOD_GETDATE;		//!< method name for getDate    ()
	static ConstString			s_METHOD_GETDATETIME;	//!< method name for getDateTime()
	static ConstString			s_METHOD_GETTIMESPAN;	//!< method name for getTimeSpan()
    static ConstString          s_METHOD_GETBYTE;       //!< method name for getBinary  ()
    static ConstString          s_METHOD_GETINT64;      //!< method name for getInt64   ()
	static ConstString			s_METHOD_UNKNOWN;		//!< unknown SQL type

	StateEnum					m_eState;				//!< cursor state
	PositionEnum				m_ePos;					//!< position attribute
	UInt32						m_FetchedRows;			//!< number of fetched rows
	CollColItems				m_CollColItems;			//!< collection with selected column names/indices
    CollColNames                m_CollColNames;
    CollColTypes                m_CollColTypes;


	ConnectionSharedPtr			m_pConn;				//!< shared pointer to connection implementation object


private:

	// --- method section ----------------------------------------------------

								Resultset			();							//!< forbidden
								Resultset			(const Resultset & rCopy);	//!< forbidden
	Resultset &    				operator =			(const Resultset & rCopy);	//!< forbidden
};

//----------------------------------------------------------------------------
}	// namespace sql
}	// namespace db
}	// namespace basar

#endif	// GUARD
