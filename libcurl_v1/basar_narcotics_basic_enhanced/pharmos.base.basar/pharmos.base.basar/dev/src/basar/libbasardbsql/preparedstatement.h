//----------------------------------------------------------------------------
/*! \file
 *  \brief  prepared statement implementation class
 *  \author Roland Kiefert
 *  \date   02.11.2005
 */
//----------------------------------------------------------------------------

#ifndef GUARD_PREPAREDSTATEMENT_H
#define GUARD_PREPAREDSTATEMENT_H

//----------------------------------------------------------------------------
#include "sqloperative.h"
#include "libbasardbsql_resultsetref.h"
#include "libbasardbsql_insertsetref.h"

#include <deque>

//----------------------------------------------------------------------------
namespace basar	{
namespace db	{
namespace sql	{

//----------------------------------------------------------------------------
//! \brief statement implementation class \n base class
//!
class PreparedStatement : public SqlOperative
{
public:

	// --- method section ----------------------------------------------------

	//! close and destruct statement implementation object					\n no-throw
	virtual						   ~PreparedStatement		();


	//! execute a SQL select statement, return a resultset					\n InvalidParameterException
	virtual ResultsetRef			executeQuery			() = 0;
	//! execute a SQL single select statement, return a one row resultset	\n InvalidParameterException
	virtual ResultsetRef			executeSingleQuery		() = 0;
	//! insert cursor statement												\n InvalidStateException
	virtual InsertsetRef			executeInsert			();
	//! execute a given SQL statement (non-select)							\n InvalidParameterException
	//! \return ExecuteReturnInfo containing result of PreparedStatement
	virtual ExecuteReturnInfo	    execute					(bool  flagExcept	//!< if error: DB exception / return code
															) = 0;

	//! execute a SQL single select statement containing count(*) or sum, avg, min, max on numeric fields	\n InvalidParameterException
	//! \return result of aggregate select
	virtual basar::Decimal		    executeAggregate		() = 0;    

	//! release statement resources											\n no-throw
	virtual void					close					();
	//! is statement open?													\n no-throw
	bool							isOpen					() const;	

    //! set a parameter value as an Int64
    virtual void                    setInt64                (UInt32 pos,                //!< position of the parameter
                                                             const Int64 & rValue       //!< value of the parameter
                                                            ) = 0;

	//! set a parameter value as an Int32									
	virtual void					setInt32				(UInt32        pos,			//!< position of the parameter
															 const Int32 & rValue		//!< value of the parameter
															) = 0;
	//! set a parameter value as an Int16									
	virtual void					setInt16				(UInt32        pos,			//!< position of the parameter
															 const Int16 & rValue		//!< value of the parameter
															) = 0;
	//! set a parameter value as a string									
	virtual void					setString				(UInt32      pos,			//!< position of the parameter
															 ConstString value			//!< value of the parameter
															) = 0;
    //! set a parameter value as a binary
    virtual void                    setBinary               (UInt32 pos,                //!< position of the parameter
                                                             const Binary& value        //!< value of the parameter
                                                            ) = 0;
	//! set a parameter value as a Float64									
	virtual void					setFloat64				(UInt32          pos,		//!< position of the parameter
															 const Float64 & rValue		//!< value of the parameter
															) = 0;
	//! set a parameter value as a Float32									
	virtual void					setFloat32				(UInt32          pos,		//!< position of the parameter
															 const Float32 & rValue		//!< value of the parameter
														    ) = 0;
	//! set a parameter value as a decimal									
	virtual void					setDecimal				(UInt32          pos,		//!< position of the parameter
														     const Decimal & rValue		//!< value of the parameter
														    ) = 0;
	//! set a parameter value as a time										
	virtual void					setTime					(UInt32           pos,		//!< position of the parameter
														     const Time     & rValue	//!< value of the parameter
														    ) = 0;
	//! set a parameter value as a date										
	virtual void					setDate					(UInt32           pos,		//!< position of the parameter
														     const Date     & rValue	//!< value of the parameter
														    ) = 0;
	//! set a parameter value as a datetime									
	virtual void					setDateTime				(UInt32           pos,		//!< position of the parameter
															 const DateTime & rValue	//!< value of parameter
															) = 0;

    //! set a parameter value as a timespan									
	virtual void					setTimeSpan				(UInt32           pos,		//!< position of the parameter
															 const TimeSpan & rValue	//!< value of parameter
															) = 0;

protected:

	// --- method section ----------------------------------------------------

	//! check statement for character '?'									\n no-throw
	static UInt32					getCountParameter		(const VarString & rStmt	//! SQL statement to be checked
															);			

	//! check if all parameters of the statment are set;					\n no-throw
	bool							isParamsSet				();

	//! constructs implementation object by connection reference			\n no-throw
									PreparedStatement		(ConnectionSharedPtr pConn,		//!< shared pointer to connection implementation object
															 const VarString   & rSql		//!< SQL statement
														    );

									PreparedStatement		();									//!< forbidden
									PreparedStatement		(const PreparedStatement & rCopy);	//!< forbidden
	PreparedStatement & 			operator =				(const PreparedStatement & rCopy);	//!< forbidden

	//! check connection state                        						\n no-throw
	virtual void					checkConnState			(const VarString & rWhat,	//!< text description 
														     const VarString & rFile,	//!< file where exception is thrown
														     UInt32			   line		//!< line of file where exception is thrown 
														    ) const;

	//! check if index is within correct range		\n throw OutOfRangeIndexException
	void							checkRange				(UInt32      index,		//!< column index (0, ..., n-1)
															 ConstString func,		//!< calling method
															 ConstString file,		//!< calling file
															 UInt32		 line		//!< calling file line
															) const;

	//! is raw flag for fetching set?				\n no-throw
	bool							isFetchModeRaw			() const;


	// --- typedef section ---------------------------------------------------
	
	typedef std::deque < bool >	CollParamFlags;		//!< definition for collection with parameter flags
	
	// --- member section ----------------------------------------------------

	static ConstString				s_METHOD_INVALID;		//!< invalid/unknown SQL type
	static ConstString				s_METHOD_SETINT32;		//!< method name for setInt32   ()
	static ConstString				s_METHOD_SETINT16;		//!< method name for setInt16   ()
	static ConstString				s_METHOD_SETSTRING;		//!< method name for setString  ()
	static ConstString				s_METHOD_SETFLOAT64;	//!< method name for setFloat64 ()
	static ConstString				s_METHOD_SETFLOAT32;	//!< method name for setFloat32 ()
	static ConstString				s_METHOD_SETDECIMAL;	//!< method name for setDecimal ()
	static ConstString				s_METHOD_SETTIME;		//!< method name for setTime    ()
	static ConstString				s_METHOD_SETDATE;		//!< method name for setDate    ()
	static ConstString				s_METHOD_SETDATETIME;	//!< method name for setDateTime()
    static ConstString				s_METHOD_SETTIMESPAN;	//!< method name for setTimeSpan()
    static ConstString              s_METHOD_SETBINARY;     //!< method name for setBinary  ()
    static ConstString              s_METHOD_SETINT64;      //!< method name for setInt64   ()
	static ConstString				s_METHOD_UNKNOWN;		//!< unknown SQL type


	ResultsetRef					m_ResultsetRef;		//!< resultset reference object
	InsertsetRef					m_InsertsetRef;		//!< insertset reference object
	ConnectionSharedPtr				m_pConn;			//!< shared pointer to connection implementation object
	bool							m_Open;				//!< statement state: is statement prepared?

	UInt32							m_CountParameter;	//!< number of prepared parameters in statement
	CollParamFlags				    m_CollParamFlags;	//!< collection of flags to manage which parameters in statement are set/unset
};

//----------------------------------------------------------------------------
}	// namespace sql
}	// namespace db
}	// namespace basar

#endif	// GUARD
