 //----------------------------------------------------------------------------
/*! \file
 *  \brief  resultset Informix implementation class
 *  \author Michael Eichenlaub
 *  \date   21.07.2005
 */
//----------------------------------------------------------------------------

#include "resultsetinfx.h"

#include "libbasarcmnutil_decimal.h"
#include "libbasarcmnutil_datetime.h"
#include "libbasarcmnutil_timespan.h"
#include "libbasarcmnutil_date.h"
#include "libbasarcmnutil_i18nstring.h"
#include "loggerpool.h"
#include "libbasarcmnutil_logging.h"
#include "connectioninfx.h"

#include <string.h>

#include "esqlc.h"

//----------------------------------------------------------------------------
namespace basar	{
namespace db	{
namespace sql	{

//! array with custom SQL type size => used by infxPrepDescOut()
extern "C" InfxCustomTypeSize customTypeSize = 
{
    Decimal::getInfxDescSize(),
       Date::getInfxDescSize(),
   DateTime::getInfxDescSize(),
   TimeSpan::getInfxDescSize()
};

//------------------------------------------------------------------------------
//! \return recommended method name
//! \throw  no-throw
ConstBuffer ResultsetInfx::getMethodName(Int32 type)
{
	static ConstString method[] =	// regard enum InfxSqlTypeEnum order in esqlc.h !
	{
		s_METHOD_INVALID,
		s_METHOD_GETSTRING,
		s_METHOD_GETFLOAT64,
		s_METHOD_GETDECIMAL,
		s_METHOD_GETFLOAT32,
		s_METHOD_GETINT32,	
		s_METHOD_GETINT16,	
		s_METHOD_GETDATE,	
		s_METHOD_GETDATETIME,
        s_METHOD_GETTIMESPAN,
        s_METHOD_GETSTRING,
        s_METHOD_GETBYTE,
        s_METHOD_GETINT64,
		s_METHOD_UNKNOWN
	};

	if ((Int32)(sizeof(method)/sizeof(*method)) <= type)
	{
		assert(0);
		return s_METHOD_INVALID;
	}

	return method[type];
}

//------------------------------------------------------------------------------
//! \throw no-throw
//!
ResultsetInfx::ResultsetInfx ( ConnectionSharedPtr pConn  ,
					           const VarString &   rStmtId)
							 : Resultset (pConn  ),
							   m_StmtId  (rStmtId),
							   m_pDescOut(0      ),
                               m_RetValDateTime(),
                               m_RetValDate(),
                               m_RetValTime(),
                               m_RetValTimeSpan(),
                               m_RetValDecimal()

{
	describeStmt();	// describe statement, get column names, allocate data buffer
}

//------------------------------------------------------------------------------
//! \throw no-throw
//!
ResultsetInfx::~ResultsetInfx()
{  

    try
	{
		close();
		freeStmtDescription();
	}
	catch (const basar::Exception &)
	{
		assert(0);
	}
	catch (...)
	{
		BLOG_ERROR(LoggerPool::resSetInfx(), "unknown exception");
		assert(0);
	}
}

//------------------------------------------------------------------------------
void ResultsetInfx::freeStmtDescription()
{
	if (0 == m_pDescOut)
		return;

	if (LoggerPool::resSetInfx().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format("%s - close with m_pDescOut == 0x%0*x: number of columns %d",
					m_StmtId.c_str(),
					sizeof(ULong32 *) * 2, m_pDescOut,
					infxGetDescCount(m_pDescOut));

		LoggerPool::resSetInfx().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	infxFreeImplicitDesc(m_pDescOut, FREE_LOC_BUFFER);
	m_pDescOut = 0;
}

//------------------------------------------------------------------------------
//! \throw no-throw
//!
void ResultsetInfx::close()
{
}

//------------------------------------------------------------------------------
//! \throw InvalidParameterException
//!
void ResultsetInfx::describeStmt()
{
	ConstString func = "basar.db.sql.ResultsetInfx.describeStmt";

	try
	{
		m_pDescOut = infxDescribeStmtOut(m_StmtId.c_str());
		assert(m_pDescOut != 0);

		// get number of descriptors
		Int32 cols = infxGetDescCount(m_pDescOut);

		if (LoggerPool::resSetInfx().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
		{
			VarString msg;
			msg.format("%s - describe statement with m_pDescOut == 0x%0*x: number of columns %d",
						m_StmtId   .c_str(),
						sizeof(ULong32 *) * 2, m_pDescOut,
						cols);

			LoggerPool::resSetInfx().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);

			for (Int32 i = 0; i < cols; ++i)		
			{
				msg.format("%s - describe statement: col[%3d] == <%s>", 
       						m_StmtId   .c_str(),
							i,
							infxGetDescColName(m_pDescOut, i));
				LoggerPool::resSetInfx().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
			}
		}

		// retrieve column names
		for (Int32 i = 0; i < cols; ++i)		
		{
			storeColNames(infxGetDescColName(m_pDescOut, i), 
						i, 
						func, 
						__FILE__, 
						__LINE__);

            InfxSqlTypeEnum infxType = infxGetDescSimpleType(m_pDescOut, i);

            storeColTypes( getBasarTypeForInfxType( infxType ) );

		}


		// get row size and prepare descriptors (length, offsets)
		Int32 size = infxPrepDescOut(m_pDescOut, isFetchModeRaw());

		// allocate and init row buffer
		m_pRow.reset(new char [size]);
		memset(m_pRow.get(), 0, sizeof(char) * size);

		// allocate and init indicator variable
		m_pIndicator.reset(new Int16[cols]);
		memset(m_pIndicator.get(), 0, sizeof(Int16) * cols);

		// adjust descriptors with row buffer and indicator variables address
		infxAdjustDesc(m_pDescOut, m_pRow.get(), m_pIndicator.get());

		if (LoggerPool::resSetInfx().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
		{
			VarString msg;
			msg.format("%s - describe statement: row size %d",
						m_StmtId   .c_str(),
						size);

			LoggerPool::resSetInfx().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
		}
    }
    catch (const Exception & )
    {
		close();

        if (LoggerPool::resSetInfx().isEnabledFor(log4cplus::ERROR_LOG_LEVEL))
        {
            VarString msg;
            msg.format("%s: describing statement", m_StmtId.c_str());
            LoggerPool::resSetInfx().forcedLog(log4cplus::ERROR_LOG_LEVEL, msg, __FILE__, __LINE__);
        }

        throw;
    }
}

//------------------------------------------------------------------------------
//! \throw  InvalidParameterException
//!
void ResultsetInfx::checkType (UInt32          index,
							   InfxSqlTypeEnum cmpType,
							   ConstString     func,
							   ConstString     file,
							   UInt32		   line) const
{
	InfxSqlTypeEnum eType = infxGetDescSimpleType(m_pDescOut, index);

	if (cmpType != eType)
	{
		VarString val;
		val.format("index %ld, name \"%s\", type \"%s\" : use method %s!", 
					index,
					infxGetDescColName (m_pDescOut, index),
					infxGetDescTypeName(m_pDescOut, index),
					getMethodName      (eType         ));

		throw InvalidParameterException(ExceptInfo (func, val, file, line));
	}
}

//----------------------------------------------------------------------------
inline basar::SupportedTypeEnum ResultsetInfx::getBasarTypeForInfxType( const InfxSqlTypeEnum infxType ) const
{
    switch ( infxType )
    {
        case TYPE_TEXT:
        case TYPE_STRING:
        {
            return basar::STRING;
        }
        break;

        case TYPE_DOUBLE:
        {
            return basar::FLOAT64;
        }
        break;

        case TYPE_DECIMAL:
        {
            return basar::DECIMAL;
        }
        break;

        case TYPE_FLOAT:
        {
            return basar::FLOAT32;
        }
        break;

        case TYPE_INT:
        {
            return basar::INT32;
        }
        break;

        case TYPE_SHORT:
        {
            return basar::INT16;
        }
        break;

        case TYPE_DATE:
        {
            return basar::DATE;
        }
        break;

        case TYPE_DATETIME:
        {
            return basar::DATETIME;
        }
        break;

        case TYPE_INTERVAL:
        {
            return basar::TIMESPAN;
        }
        break;

        case TYPE_BYTE:
        {
            return basar::BINARY;
        }

        case TYPE_INT8:
        {
            return basar::INT64;
        }
        break;

        case TYPE_UNKNOWN:
        case TYPE_INVALID:
        default:
        {
            return basar::UNKNOWN;
        }
        break;
    };
}

//----------------------------------------------------------------------------
//! \return column value
//! \throw  no-throw
const Int64 & ResultsetInfx::getInt64(UInt32 index) const
{
    ConstString func = "basar.db.sql.ResultsetInfx.getLong64";

    checkRange(index, func, __FILE__, __LINE__);
    checkType(index, TYPE_INT8, func, __FILE__, __LINE__);
    checkNextCalled();

    const Long64 * pVal = infxGetDescInt64(m_pDescOut, index);

    assert(0 != pVal);

    if (LoggerPool::resSetInfxGetVal().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
    {
        VarString msg;
        msg.format("%s - getLong64(%s[%d]): <%d>",
            m_StmtId.c_str(),
            infxGetDescColName(m_pDescOut, index),
            index,
            *pVal);
        LoggerPool::resSetInfxGetVal().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
    }

    return *pVal;
}

//----------------------------------------------------------------------------
//! \return column value
//! \throw  no-throw
const Int32 & ResultsetInfx::getInt32(UInt32 index) const
{
	ConstString func = "basar.db.sql.ResultsetInfx.getInt32";

	checkRange(index, func, __FILE__, __LINE__);
	checkType (index, TYPE_INT, func, __FILE__, __LINE__);
    checkNextCalled();

	const Int32 * pVal = infxGetDescInt(m_pDescOut, index); 

	assert(0 != pVal);
	
	if (LoggerPool::resSetInfxGetVal().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format("%s - getInt32(%s[%d]): <%d>",
					m_StmtId.c_str(),
					infxGetDescColName (m_pDescOut, index),
					index,
					*pVal);
		LoggerPool::resSetInfxGetVal().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	return *pVal;
}

//----------------------------------------------------------------------------
//! \return column value
//! \throw  no-throw
const Int16 & ResultsetInfx::getInt16(UInt32 index) const
{
	ConstString func = "basar.db.sql.ResultsetInfx.getInt16";

	checkRange(index, func, __FILE__, __LINE__);
	checkType (index, TYPE_SHORT, func, __FILE__, __LINE__);
    checkNextCalled();

	const Int16 * pVal = infxGetDescShort(m_pDescOut, index); 

	assert(0 != pVal);
	
	if (LoggerPool::resSetInfxGetVal().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format("%s - getInt16(%s[%d]): <%d>",
					m_StmtId.c_str(),
					infxGetDescColName (m_pDescOut, index),
					index,
					*pVal);
		LoggerPool::resSetInfxGetVal().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	return *pVal;
}

//----------------------------------------------------------------------------
//! \return column value
//! \throw  no-throw
ConstBuffer ResultsetInfx::getString(UInt32 index) const
{
	ConstString func = "basar.db.sql.ResultsetInfx.getString";

	checkRange(index, func, __FILE__, __LINE__);
	checkType (index, TYPE_STRING, func, __FILE__, __LINE__);
    checkNextCalled();

	ConstString	rVal = infxGetDescString(m_pDescOut, index); 
	assert(0 != rVal);

	if (LoggerPool::resSetInfxGetVal().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format("%s - getString(%s[%d]): <%s>",
					m_StmtId.c_str(),
					infxGetDescColName (m_pDescOut, index),
					index,
					rVal);
		LoggerPool::resSetInfxGetVal().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	return rVal;
}

//----------------------------------------------------------------------------
//! \return column value
//! \throw  no-throw
const Binary & ResultsetInfx::getBinary(UInt32 index) const
{
    ConstString func = "basar.db.sql.ResultsetInfx.getBinary";

    checkRange(index, func, __FILE__, __LINE__);
    checkType(index, TYPE_BYTE, func, __FILE__, __LINE__);
    checkNextCalled();

    int size = infxGetDescColLen(m_pDescOut, index);

    ConstString	rVal = infxGetDescString(m_pDescOut, index);
    assert(0 != rVal);

    m_RetValBinary.reserve(size);
    m_RetValBinary.assign(rVal, rVal + size);

    if (LoggerPool::resSetInfxGetVal().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
    {
        VarString msg;
        msg.format("%s - getString(%s[%d]): <%s>",
            m_StmtId.c_str(),
            infxGetDescColName(m_pDescOut, index),
            index,
            rVal);
        LoggerPool::resSetInfxGetVal().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
    }

    return m_RetValBinary;
}

//----------------------------------------------------------------------------
//! \return column value
//! \throw  no-throw
const Float64 &	ResultsetInfx::getFloat64(UInt32 index) const
{
	ConstString func = "basar.db.sql.ResultsetInfx.getFloat64";

	checkRange(index, func, __FILE__, __LINE__);
	checkType (index, TYPE_DOUBLE, func, __FILE__, __LINE__);
    checkNextCalled();

	const Float64 * pVal = infxGetDescDouble(m_pDescOut, index);
	assert(0 != pVal);

	if (LoggerPool::resSetInfxGetVal().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format("%s - getFloat64(%s[%d]): <%f>",
					m_StmtId.c_str(),
					infxGetDescColName (m_pDescOut, index),
					index,
					*pVal);
		LoggerPool::resSetInfxGetVal().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	return *pVal;
}

//----------------------------------------------------------------------------
//! \return column value
//! \throw  no-throw
const Float32 &	ResultsetInfx::getFloat32 (UInt32 index) const
{
	ConstString func = "basar.db.sql.ResultsetInfx.getFloat32";

	checkRange(index, func, __FILE__, __LINE__);
	checkType (index, TYPE_FLOAT, func, __FILE__, __LINE__);
    checkNextCalled();

	const Float32 * pVal = infxGetDescFloat(m_pDescOut, index); 
	assert(0 != pVal);

	if (LoggerPool::resSetInfxGetVal().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format("%s - getFloat32(%s[%d]): <%f>",
					m_StmtId.c_str(),
					infxGetDescColName (m_pDescOut, index),
					index,
					*pVal);
		LoggerPool::resSetInfxGetVal().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	return *pVal;
}

//----------------------------------------------------------------------------
//! \return column value
//! \throw  no-throw
const Decimal & ResultsetInfx::getDecimal (UInt32 index) const
{
	ConstString func = "basar.db.sql.ResultsetInfx.getDecimal";

	checkRange(index, func, __FILE__, __LINE__);
	checkType (index, TYPE_DECIMAL, func, __FILE__, __LINE__);
    checkNextCalled();

    m_RetValDecimal = cmnutil::Decimal( infxGetDescDecimal(m_pDescOut, index) );

	//assert(0 != pDec);

	if (LoggerPool::resSetInfxGetVal().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format("%s - getDecimal(%s[%d]): <%s>",
					m_StmtId.c_str(),
					infxGetDescColName (m_pDescOut, index),
					index,
					m_RetValDecimal.toString().c_str());
		LoggerPool::resSetInfxGetVal().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	return m_RetValDecimal;
}

//----------------------------------------------------------------------------
//! \return column value
//! \throw  no-throw
const Date& ResultsetInfx::getDate(UInt32 index) const
{
	ConstString func = "basar.db.sql.ResultsetInfx.getDate";

	checkRange(index, func, __FILE__, __LINE__);
	checkType (index, TYPE_DATE, func, __FILE__, __LINE__);
    checkNextCalled();

    m_RetValDate = cmnutil::Date( infxGetDescDate(m_pDescOut, index) );
	
	if (LoggerPool::resSetInfxGetVal().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format("%s - getDate(%s[%d]): <%s>",
					m_StmtId.c_str(),
					infxGetDescColName (m_pDescOut, index),
					index,
					m_RetValDate.toStrDate().c_str());
		LoggerPool::resSetInfxGetVal().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	return m_RetValDate;
}

//----------------------------------------------------------------------------
//! Informix SQL DateTime is used for Time
//! \return column value
//! \throw  no-throw
const Time & ResultsetInfx::getTime(basar::UInt32 index) const
{
	ConstString func = "basar.db.sql.ResultsetInfx.getTime";

	checkRange(index, func, __FILE__, __LINE__);
	checkType (index, TYPE_DATETIME, func, __FILE__, __LINE__);		// Infx SQL DateTime is used for Time
    checkNextCalled();

    m_RetValTime = cmnutil::DateTime(infxGetDescDateTime(m_pDescOut, index) );

	if (LoggerPool::resSetInfxGetVal().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format("%s - getTime(%s[%d]): <%s>",
					m_StmtId.c_str(),
					infxGetDescColName (m_pDescOut, index),
					index,
					m_RetValTime.toStrTime().c_str());
		LoggerPool::resSetInfxGetVal().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	return m_RetValTime;
}

//----------------------------------------------------------------------------
//! \return column value
//! \throw  no-throw
const DateTime & ResultsetInfx::getDateTime(UInt32 index) const
{
	ConstString func = "basar.db.sql.ResultsetInfx.getDateTime";

	checkRange(index, func, __FILE__, __LINE__);
	checkType (index, TYPE_DATETIME, func, __FILE__, __LINE__);
    checkNextCalled();

    m_RetValDateTime = cmnutil::DateTime(infxGetDescDateTime(m_pDescOut, index) );

	if (LoggerPool::resSetInfxGetVal().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format("%s - getDateTime(%s[%d]): <%s, %s>",
					m_StmtId.c_str(),
					infxGetDescColName (m_pDescOut, index),
					index,
					m_RetValDateTime.toStrDate().c_str(), m_RetValDateTime.toStrTime().c_str() );
		LoggerPool::resSetInfxGetVal().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	return m_RetValDateTime;
}

//----------------------------------------------------------------------------
//! \return column value
//! \throw  no-throw
const TimeSpan & ResultsetInfx::getTimeSpan(UInt32 index) const
{
	ConstString func = "basar.db.sql.ResultsetInfx.getTimeSpan";

	checkRange(index, func, __FILE__, __LINE__);
	checkType (index, TYPE_INTERVAL, func, __FILE__, __LINE__);
    checkNextCalled();

    m_RetValTimeSpan = cmnutil::TimeSpan(infxGetDescTimeSpan(m_pDescOut, index) );

	if (LoggerPool::resSetInfxGetVal().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format("%s - getTimeSpan(%s[%d]): <%s, %s>",
					m_StmtId.c_str(),
					infxGetDescColName (m_pDescOut, index),
					index,
					m_RetValTimeSpan.toNormalizedString().c_str() );
		LoggerPool::resSetInfxGetVal().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	return m_RetValTimeSpan;
}

//----------------------------------------------------------------------------
//! \return column type
//! \throw  no-throw
InfxSqlTypeEnum ResultsetInfx::getTypeByName(const VarString & rColName	) const
{
	return getType(getIndex(rColName));
}

//----------------------------------------------------------------------------
//! \return column type
//! \throw  no-throw
InfxSqlTypeEnum ResultsetInfx::getType		(const UInt32 index	) const
{
	return infxGetDescSimpleType(m_pDescOut, index);
}

//------------------------------------------------------------------------------
//! \throw no-throw
//!
void ResultsetInfx::setCurrent()
{	
	static_cast<ConnectionInfx *>(m_pConn.get())->setCurrent();
}

//----------------------------------------------------------------------------
}	// namespace sql
}	// namespace db
}	// namespace basar
