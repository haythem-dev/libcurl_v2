//----------------------------------------------------------------------------
/*! \file
 *  \brief  prepared statement informix implementation class
 *  \author Roland Kiefert
 *  \date   03.11.2005
 */
//----------------------------------------------------------------------------

#ifndef GUARD_PREPAREDSTATEMENTINFX_H
#define GUARD_PREPAREDSTATEMENTINFX_H

//----------------------------------------------------------------------------
#include <boost/scoped_array.hpp>
#include "preparedstatement.h"
#include "esqlc.h"
#include "libbasarcmnutil_bstring.h"

//----------------------------------------------------------------------------
struct sqlda;

//----------------------------------------------------------------------------
namespace basar	{
namespace db	{
namespace sql	{

//----------------------------------------------------------------------------
//! \brief prepared statement implementation class for informix\n final class
//!
class PreparedStatementInfx : public PreparedStatement
{
public:

	// --- method section ----------------------------------------------------

	//! create statement implementation object								\n no-throw
	static PreparedStatementSharedPtr	create			(ConnectionSharedPtr pConn,			//!< shared pointer to connection implementation object
														 const VarString   & rSql ,			//!< SQL statement
 														 CursorTypeEnum	     eCursorType	//!< cursor type
														);
	//! create statement implementation object for update/delete cursor 	\n no-throw
	static PreparedStatementSharedPtr	createRowStmt	(ConnectionSharedPtr           pConn,	//!< shared pointer to connection implementation object
														 const VarString             & rSql,	//!< SQL statement
 														 const PreparedStatementInfx & stmtSel	//!< select ... for update statement whose cursor should be used
														);

	//! close and destruct statement implementation object					\n no-throw
	virtual					   ~PreparedStatementInfx	();

	//! execute a SQL select statement, return a resultset					\n InvalidParameterException
	virtual ResultsetRef		executeQuery			();
	//! execute a SQL single select statement, return a one row resultset	\n InvalidParameterException
	virtual ResultsetRef		executeSingleQuery		();
	//! insert cursor statement												\n InvalidParameterException
	virtual InsertsetRef		executeInsert			();
	//! execute a given SQL statement (non-select)							\n InvalidParameterException
	virtual ExecuteReturnInfo	execute					(bool  flagExcept	//!< if error: DB exception / return code
														);

	//! \brief execute a SQL single select statement containing count(*) or sum, avg, min, max on numeric fields	\n InvalidParameterException
	//! \return result of aggregate select
	virtual basar::Decimal		executeAggregate		();

	//! release statement resources											\n no-throw
	virtual void				close					();

    /*! \brief set a parameter value as an Int64, the value is saved in the structure
    \n no-throw */
    virtual void                setInt64                (UInt32 pos,                //!< position of the parameter
                                                         const Int64 & rValue       //!< value of the parameter
                                                        );

	/*! \brief set a parameter value as an Int32, the value is saved in the structure
		\n no-throw */
	virtual void				setInt32				(UInt32        pos,			//!< position of the parameter
														 const Int32 & rValue		//!< value of the parameter
														);
	/*! \brief set a parameter value as an Int16, the value is saved in the structure
	//!	\n no-throw */
	virtual void				setInt16				(UInt32        pos,			//!< position of the parameter
														 const Int16 & rValue		//!< value of the parameter
														);
	/*! \brief set a parameter value as a string, the value is saved in the structure
	//!	\n no-throw */
	virtual void				setString				(UInt32      pos,			//!< position of the parameter
														 ConstString value			//!< value of the parameter
														);
    /*! \brief set a parameter value as a binary, the value is saved in the structure
    //!	\n no-throw */
    virtual void                setBinary               (UInt32 pos,                //!< position of the parameter
                                                        const Binary & value        //!< value of the parameter
                                                        );
	/*! \brief set a parameter value as a Float64, the value is saved in the structure
	//!	\n no-throw */
	virtual void				setFloat64				(UInt32          pos,		//!< position of the parameter
														 const Float64 & rValue		//!< value of the parameter
														);
	/*! \brief set a parameter value as a Float32, the value is saved in the structure
	//!	\n no-throw */
	virtual void				setFloat32				(UInt32          pos,		//!< position of the parameter
														 const Float32 & rValue		//!< value of the parameter
														);
	/*! \brief set a parameter value as a decimal, the value is saved in the structure
	//!	\n no-throw */
	virtual void				setDecimal				(UInt32	         pos,		//!< position of the parameter
														 const Decimal & rValue		//!< value of the parameter
														);
	/*! \brief set a parameter value as a Time, the value is saved in the structure
	//!	\n no-throw */
	virtual void				setTime					(UInt32           pos ,		//!< position of the parameter
														 const Time     & rValue	//!< value of the parameter
														);
	/*! \brief set a parameter value as a Date, the value is saved in the structure
	//!	\n no-throw */
	virtual void				setDate					(UInt32           pos ,		//!< position of the parameter
														 const Date     & rValue	//!< value of the parameter
														);
	/*! \brief set a parameter value as a DateTime, the value is saved in the structure
	//!	\n no-throw */
	virtual void				setDateTime				(UInt32           pos,		//!< position of the parameter
														 const DateTime & rValue	//!< value of parameter
														);
	/*! \brief set a parameter value as a TimeSpan, the value is saved in the structure
	//!	\n no-throw */
	virtual void				setTimeSpan				(UInt32           pos,		//!< position of the parameter
														 const TimeSpan & rValue	//!< value of parameter
														);

private:

    typedef boost::scoped_array < char  >	RowSmartPtr;		//!< definition for smart pointer to row buffer
    typedef boost::scoped_array < Int16 >	RowIndicatorPtr;	//!< definition for smart pointer to indicator variables (DB: NULL values)

	// --- method section ----------------------------------------------------

	//! cursor types, needed for calculation cursor id
	enum CursorIdEnum
	{
		CURSORID_RESULTSET,		//!< for resultset
		CURSORID_INSERTSET		//!< for insertset
	};

	//! for exceptions: get recommended method name for given SQL type
	static ConstBuffer 			getMethodName			(Int32 type					//!< simplified Informix SQL type (integral type because of hiding Informix type enum)
														);

	//! construct implementation object by connection reference			\n no-throw
								PreparedStatementInfx	(ConnectionSharedPtr pConn,				//!< shared pointer to connection implementation object
														 const VarString   & rSql ,				//!< SQL statement
														 CursorTypeEnum	     eCursorType		//!< cursor type
														);

	//! construct implementation object by connection reference			\n no-throw
								PreparedStatementInfx	(ConnectionSharedPtr           pConn,	//!< shared pointer to connection implementation object
														 const VarString             & rSql ,	//!< SQL statement
														 const PreparedStatementInfx & stmtSel	//!< select ... for update statement whose cursor should be used														
														);

								PreparedStatementInfx	();										//!< forbidden
								PreparedStatementInfx	(const PreparedStatementInfx & rCopy);	//!< forbidden
	PreparedStatementInfx &    	operator =				(const PreparedStatementInfx & rCopy);	//!< forbidden

	//! get cursor id for created resultset/insertset	\n no-throw
	VarString					getCursorId				(CursorIdEnum eCursorId		//!< cursor type
														) const;

	//! check if column type is compatible with called method	\n throw InvalidParameterException
	void						checkType				(UInt32          index,		//!< column index (0, ..., n-1)
														 InfxSqlTypeEnum cmpType,	//!< simplified Informix column type, compared with indexed column type
														 ConstString     func,		//!< calling method
														 ConstString     file,		//!< calling file
														 UInt32		     line		//!< calling file line
														) const;

	//! set connection to current	\n no-throw
	void						setCurrent				();

	//! called by c'tors \n no-throw
	void						construct				();

    typedef std::deque < bool >	CollHasBufferFlags;     //!< definition for Column Buffer Flags

	::sqlda *					m_pDescIn;				//!< sql descriptor area for input data -> parameterized values
	VarString					m_StmtId;				//!< application's unique statement ID (i.e. object address)
	CursorTypeEnum				m_eCursorType;			//!< cursor type
    VarString                   m_statement;            //!< statement
    RowSmartPtr                 m_pRow;             //!< smart pointer to row buffer memory
    RowIndicatorPtr				m_pIndicator;		//!< smart pointer to indicator variables (DB: NULL values)

    CollHasBufferFlags          m_CollHasBuffer;        //!< deque for storing information if Column in resultset has had added buffer memory from m_pRow
};

//----------------------------------------------------------------------------
}	// namespace sql
}	// namespace db
}	// namespace basar

#endif	// GUARD
