/*
 * @file resultsetcics.cpp
 * @brief ...
 * @author Anke Klink
 * @date 2007
 */
//----------------------------------------------------------------------------

#include "loggerpool.h"
#include "libbasarcmnutil_logging.h"
#include "libbasarcmnutil_i18nstring.h"
#include <string.h>
#include "resultsetcics.h"

//----------------------------------------------------------------------------
namespace basar
{
namespace db
{
namespace sql
{

// ---------------------------------------------------------------------------
//! \return recommended method name
//! \throw  no-throw
//!
ConstBuffer ResultsetCics::getMethodName(CicsTypeEnum type)
{
	static ConstString method[] =	// regard zero-based enum order!
	{
		s_METHOD_UNKNOWN,
		s_METHOD_GETINT16,	
		s_METHOD_GETINT32,	
		s_METHOD_GETFLOAT64,
		s_METHOD_GETDECIMAL,
		s_METHOD_GETSTRING,
		s_METHOD_GETDATE,	
		s_METHOD_GETDATETIME,
		s_METHOD_GETTIME,
		s_METHOD_INVALID
	};

	if ((Int32)(sizeof(method)/sizeof(*method)) <= type)
	{
		assert(0);
		return s_METHOD_INVALID;
	}

	return method[type];
}

// ---------------------------------------------------------------------------
//! \throw no-throw
//! \return shared_ptr to new cics resultset
ResultsetSharedPtr ResultsetCics::create(
    ConnectionSharedPtr       pConn, 
    const CicsStatementInfo & info)
{
	return ResultsetSharedPtr(new ResultsetCics(pConn, info));
}

// ---------------------------------------------------------------------------
//! \throw no-throw
//!
ResultsetCics::ResultsetCics ( ConnectionSharedPtr       pConn, 
							   const CicsStatementInfo & info)
							 : Resultset (pConn),
							   m_stmtInfo(info)
{
	assert(m_stmtInfo.m_handle != 0);

    describeStmt(); // describe statement, get column names

    m_eState = STATE_OPENED;
    m_ePos   = POS_BEFORE_FIRST;
}

// ---------------------------------------------------------------------------
//! \throw no-throw
//!
ResultsetCics::~ResultsetCics ()
{
	try
	{
		close();
	}
	catch (const basar::Exception &)
	{
		assert(0);
	}
	catch (...)
	{
		BLOG_ERROR(LoggerPool::resSetCics(), "unknown exception");
		assert(0);
	}
}

// ---------------------------------------------------------------------------
//! \throw no-throw
//!
void ResultsetCics::describeStmt()
{
	ConstString func = "basar.db.sql.ResultsetCics.describeStmt";

	CicsDefinitions::fetch(m_stmtInfo, m_resInfo);

	// get column names
	if (0 != m_resInfo.m_pDesc)
	{
		for (UInt32 i = 0; i < static_cast<UInt32>(m_resInfo.m_totalCols); ++i)
        {
			storeColNames(	
                CicsDefinitions::getDescColName(m_resInfo.m_pDesc, i).c_str(), 
			                i, 
							func, 
                __FILE__, __LINE__);

            storeColTypes( getBasarTypeForCicsType( CicsDefinitions::getDescType( m_resInfo.m_pDesc, i ) ) );

        }
	}
}

// ---------------------------------------------------------------------------
//! \throw no-throw
//! \retval true  if new current row is valid
//! \retval false if there are no more rows
//!
bool ResultsetCics::next ()
{
	if (POS_AFTER_LAST == m_ePos)			// new fetch not necessary
	{
        BLOG_DEBUG_STREAM(
            LoggerPool::resSetCicsNext(),
            "" 
            << m_FetchedRows 
            << " rows fetched: m_ePos == POS_AFTER_LAST("
            << m_ePos
            << ") -> m_ePos = POS_AFTER_LAST("
            << POS_AFTER_LAST
            << "), return false");

		return false;
	}

	if (m_resInfo.m_currRow >= m_resInfo.m_totalRows)	// buffer exceeded
	{
        // no further data, cannot request new buffer
		if (m_resInfo.m_endOfData)
			{
            BLOG_DEBUG_STREAM(
                LoggerPool::resSetCicsNext(),
                ""
                << m_FetchedRows
                << "rows fetched: m_resInfo.m_endOfData == true; "
                << "m_resInfo.m_currRow("
                << m_resInfo.m_currRow
                << ") >= m_resInfo.m_totalRows("
                << m_resInfo.m_totalRows
                << ")");

			m_ePos = POS_AFTER_LAST;
			return false;
		}

		// ask for next tcACCESS buffer
		CicsDefinitions::fetch(m_stmtInfo, m_resInfo);

        BLOG_DEBUG_STREAM(
            LoggerPool::resSetCicsNext(),
            ""
            << m_FetchedRows 
            << " rows fetched: asked for new tcACCESS buffer:\n"
            << "number of columns        "
            << m_resInfo.m_totalCols
            << "\n"
            << "m_resInfo.m_totalRows    "
            << m_resInfo.m_totalRows
            << "\n"
            << "m_stmtInfo.m_handle      0x"
            << std::hex 
            << m_stmtInfo.m_handle
            << "\n"
            << std::dec
            <<"m_resInfo.m_endOfData    "
            << m_resInfo.m_endOfData
            << "\n"
            << "m_resInfo.m_currRow      "
            << m_resInfo.m_currRow
            << "-> 0");

        // reset current index
		m_resInfo.m_currRow = 0;
	}

	if (m_resInfo.m_currRow < m_resInfo.m_totalRows )
	{
        BLOG_DEBUG_STREAM(
            LoggerPool::resSetCicsNext(),
            ""
            << m_FetchedRows
            << " rows fetched: current row("
            << m_resInfo.m_currRow
            << "->"
            << m_resInfo.m_currRow + 1
            << ") < total rows("
            << m_resInfo.m_totalRows
            << "->"
            << m_resInfo.m_totalRows + 1
            << ")");

		m_resInfo.m_currRow++;
		m_FetchedRows++;
		m_ePos = POS_VALID;
	}

	return true;
}

// ---------------------------------------------------------------------------
//! \throw no-throw
//! 
void ResultsetCics::close ()
{
	if ( isOpen() )
	{
		m_eState = STATE_INVALID;

        BLOG_DEBUG_STREAM(LoggerPool::resSetCics(),
            "stmt handle 0x"
            << std::hex 
            << m_stmtInfo.m_handle
            << " (slot "
            << std::dec
            << m_stmtInfo.m_connInfo.m_slot
            << ") - close cursor");

		m_FetchedRows       = 0;
		m_ePos				= POS_INVALID;
	}
}

// ---------------------------------------------------------------------------
//! \throw NotImplementedException
//! \return Int64
const Int64 & ResultsetCics::getInt64(UInt32) const
{
    throw NotImplementedException(ExceptInfo("basar.db.sql.ResultsetCics::getInt64", "Not implemented (yet?)", __FILE__, __LINE__));
}

// ---------------------------------------------------------------------------
//! \throw InvalidParameterException
//! \return the CicsDefinitions::Type of a column as position index
CicsTypeEnum ResultsetCics::getType(UInt32 index) const
{
	ConstString func = "basar::db::sql::ResultsetCics::getType" ;

	checkRange(index, func, __FILE__, __LINE__);

	if (0 == m_resInfo.m_pDesc)
	{
		VarString msg;
		msg.format(
            "index %ld, type \"unknown\" : "
            "no tcACCESS SQL data description available!", 
            index);
		throw InvalidParameterException(
                ExceptInfo(func, msg, __FILE__, __LINE__));
	}

	return CicsDefinitions::getDescType(m_resInfo.m_pDesc, index);
}

// ---------------------------------------------------------------------------
//! \throw InvalidParameterException
//!
void ResultsetCics::checkRangeAndType ( UInt32       index,
										CicsTypeEnum type ,
										ConstString  func ,
										ConstString  file ,
										UInt32       line ) const
{
	CicsTypeEnum eCmpType = getType(index);

	if (type != eCmpType)
	{
		VarString msg;
		msg.format(
            "index %ld, name \"%s\", type \"%s\" : use method %s!",
					index,
					CicsDefinitions::getDescColName (m_resInfo.m_pDesc, index).c_str(),
					CicsDefinitions::getDescTypeName(m_resInfo.m_pDesc, index).c_str(),
					getMethodName(eCmpType));

		throw InvalidParameterException(
                ExceptInfo (func, msg, file, line));
	}
}

// ---------------------------------------------------------------------------
//! \throw  OutOfRangeIndexException
//!
void ResultsetCics::getValueInternal(UInt32       index,
									 CicsValue  & value) const
{
	if ( (m_resInfo.m_currRow <= 0                    ) || 
         (m_resInfo.m_currRow >  m_resInfo.m_totalRows) )
	{
		VarString msg;
		msg.format("current index for tcACCESS resultset buffer "
                   "m_resInfo.m_currRow %d not in range [1, %d]", 
					m_resInfo.m_currRow, 
					m_resInfo.m_totalRows);

		throw OutOfRangeIndexException(
                ExceptInfo("basar.db.sql.ResultsetCics.getValueInternal", 
													msg, 
													__FILE__, __LINE__));
	}

	CicsDefinitions::retrieveValues(
        m_stmtInfo, 
        m_resInfo.m_currRow - 1, 
        index, 
        value);
}
      
// ---------------------------------------------------------------------------
//! \throw no-throw
//!
void ResultsetCics::getValue(UInt32        index,
							 CicsValue   & value,
							 ConstString   func,
							 ConstString   file,
							 UInt32        line) const
{
    checkNextCalled();
	checkRangeAndType(index, value.m_type, func, file, line);
	getValueInternal (index, value);
}

// ---------------------------------------------------------------------------
//! \throw no-throw
//!
void ResultsetCics::getStringValue (UInt32       index,
									ConstString  func,
									ConstString  file,
									UInt32       line) const
{
    checkNextCalled();

	CicsValue & value = m_resValues.m_cicsValueString;

	checkRangeAndType(index, value.m_type, func, file, line);

	USHORT length = CicsDefinitions::getDescMaxLen(m_resInfo.m_pDesc, index);

	if (length > value.m_maxlength || 
        NULL  == value.m_pValue      )
	{
		m_resValues.m_stringBuffer = 
            boost::shared_array<char>(new char[length + 1]);
		value.m_maxlength          = length;
		value.m_pValue             = m_resValues.m_stringBuffer.get();
	}

	m_resValues.m_stringBuffer[length] = '\0';

	getValueInternal(index, value);
}

// ---------------------------------------------------------------------------
//! \throw no-throw
//!
const Int32 & ResultsetCics::getInt32 (UInt32 index) const
{
	ConstString func = "basar::db::sql::ResultsetCics::getInt32";

    switch(getType(index))
	{
	case DECIMAL:
		{
			// decimal without scale are handled as integer (until SP18)
			if(0 == CicsDefinitions::getDescByteScale( m_resInfo.m_pDesc, index))
			{
				getValue(index, m_resValues.m_cicsValueDecimal, func, __FILE__, __LINE__);
				CicsDefinitions::trCics2Basar( m_resValues.m_basarInt32, m_resValues.m_doubleBuffer );
			}
			else
			{
				throw NotImplementedException(ExceptInfo(func, "Not implemented (yet?)", __FILE__, __LINE__));
			}
		} break;
	case LINT: // also DECIMAL with Scale 0 (since SP20)
		{
			getValue(index, m_resValues.m_cicsValueInt32, func, __FILE__, __LINE__);
			CicsDefinitions::trCics2Basar(m_resValues.m_basarInt32, m_resValues.m_longBuffer);
		} break;
	default:
		{
			throw NotImplementedException(ExceptInfo(func, "Not implemented (yet?)", __FILE__, __LINE__));
		} break;
	}

    BLOG_DEBUG_STREAM(
        LoggerPool::resSetCicsGetVal(),
        "<"
        << m_stmtInfo.m_statement
        << "> - "
        << func
        << "("
        << CicsDefinitions::getDescColName(m_resInfo.m_pDesc, index)
        << "["
        << index
        << "]): <"
        << m_resValues.m_basarInt32
        << ">");

	return m_resValues.m_basarInt32;
}

// ---------------------------------------------------------------------------
//! \throw no-throw
//!
const Int16 & ResultsetCics::getInt16 (UInt32 index) const
{
	ConstString func = "basar::db::sql::ResultsetCics::getInt16";

	switch(getType(index))
	{
	case DECIMAL:
		{
			// decimal without scale are handled as integer (until SP18)
			if(0 == CicsDefinitions::getDescByteScale( m_resInfo.m_pDesc, index))
			{
				getValue(index, m_resValues.m_cicsValueDecimal, func, __FILE__, __LINE__);
				CicsDefinitions::trCics2Basar( m_resValues.m_basarInt16, m_resValues.m_doubleBuffer );
			}
			else
			{
				throw NotImplementedException(ExceptInfo(func, "Not implemented (yet?)", __FILE__, __LINE__));
			}
		} break;
    case LINT: // also DECIMAL with Scale 0 (since SP20)
        {
			getValue(index, m_resValues.m_cicsValueInt32, func, __FILE__, __LINE__);
			CicsDefinitions::trCics2Basar(m_resValues.m_basarInt16, m_resValues.m_longBuffer);
        } break;
	case SINT:
		{
			getValue(index, m_resValues.m_cicsValueInt16, func, __FILE__, __LINE__);
			CicsDefinitions::trCics2Basar(m_resValues.m_basarInt16, m_resValues.m_shortBuffer);
		} break;
	default:
		{
			throw NotImplementedException(ExceptInfo(func, "Not implemented (yet?)", __FILE__, __LINE__));
		} break;
	}

    BLOG_DEBUG_STREAM(
        LoggerPool::resSetCicsGetVal(),
        "<"
        << m_stmtInfo.m_statement
        << "> - "
        << func
        << "("
        << CicsDefinitions::getDescColName(m_resInfo.m_pDesc, index)
        << "["
        << index
        << "]): <"
        << m_resValues.m_basarInt16
        << ">");

	return m_resValues.m_basarInt16;
}

// ---------------------------------------------------------------------------
//! \throw no-throw
//!
ConstBuffer ResultsetCics::getString (UInt32 index) const
{
	ConstString func = "basar::db::sql::ResultsetCics::getString";

	getStringValue(index, func,  __FILE__, __LINE__);

	// trim right 
	if ( ! isFetchModeRaw() )
	{
		char * const pBegin = m_resValues.m_stringBuffer.get();
		char *       pCur   = pBegin + ::strlen(pBegin) - 1;

		while ( (pBegin <= pCur) && 
                (::isspace(static_cast<UInt8>(*pCur))) )
		{
			*pCur = '\0';
			--pCur;
		}
	}

    BLOG_DEBUG_STREAM(
        LoggerPool::resSetCicsGetVal(),
        "<"
        << m_stmtInfo.m_statement
        << "> - "
        << func
        << "("
        << CicsDefinitions::getDescColName(m_resInfo.m_pDesc, index)
        << "["
        << index
        << "]): <"
        << m_resValues.m_stringBuffer.get()
        << ">");

	return m_resValues.m_stringBuffer.get();
}

// ---------------------------------------------------------------------------
//! \throw no-throw
//!
const Binary & ResultsetCics::getBinary(UInt32 /*index*/) const
{
    throw NotImplementedException(ExceptInfo("basar.db.sql.ResultsetCics::getBinary", "Not implemented (yet?)", __FILE__, __LINE__));
}

// ---------------------------------------------------------------------------
//! \throw no-throw
//!
const Float64 & ResultsetCics::getFloat64 (UInt32 index) const
{
	ConstString func = "basar::db::sql::ResultsetCics::getFloat64";

	switch(getType(index))
	{
	case FLOAT:
		{
			getValue(index, m_resValues.m_cicsValueFloat64, func, __FILE__, __LINE__);
			CicsDefinitions::trCics2Basar(m_resValues.m_basarFloat64, m_resValues.m_doubleBuffer);
		} break;
	default:
		{
			throw NotImplementedException(ExceptInfo(func, "Not implemented (yet?)", __FILE__, __LINE__));
		} break;
	}

    BLOG_DEBUG_STREAM(
        LoggerPool::resSetCicsGetVal(),
        "<"
        << m_stmtInfo.m_statement
        << "> - "
        << func
        << "("
        << CicsDefinitions::getDescColName(m_resInfo.m_pDesc, index)
        << "["
        << index
        << "]): <"
        << m_resValues.m_basarFloat64
        << ">");
	
	return m_resValues.m_basarFloat64;
}

// ---------------------------------------------------------------------------
//! \throw no-throw
//!
const Float32 & ResultsetCics::getFloat32 (UInt32 index) const
{
	ConstString func = "basar::db::sql::ResultsetCics::getFloat32";

	switch(getType(index))
	{
	case FLOAT:
		{
			getValue(index, m_resValues.m_cicsValueFloat32, func, __FILE__, __LINE__);
			CicsDefinitions::trCics2Basar(m_resValues.m_basarFloat32, m_resValues.m_doubleBuffer);
		} break;
	default:
		{
			throw NotImplementedException(ExceptInfo(func, "Not implemented (yet?)", __FILE__, __LINE__));
		} break;
	}

    BLOG_DEBUG_STREAM(
        LoggerPool::resSetCicsGetVal(),
        "<"
        << m_stmtInfo.m_statement
        << "> - "
        << func
        << "("
        << CicsDefinitions::getDescColName(m_resInfo.m_pDesc, index)
        << "["
        << index
        << "]): <"
        << m_resValues.m_basarFloat32
        << ">");

	return m_resValues.m_basarFloat32;
}

// ---------------------------------------------------------------------------
//! \throw no-throw
//!
const Decimal & ResultsetCics::getDecimal (UInt32 index) const
{
	ConstString func = "basar::db::sql::ResultsetCics::getDecimal";

    const CicsTypeEnum type = getType(index);
	switch(type)
	{
	case DECIMAL:
		{
			getValue( index, m_resValues.m_cicsValueDecimal, func, __FILE__, __LINE__ );

			// decimal without scale are handled as integer (until SP18)
			if( 0 == CicsDefinitions::getDescByteScale( m_resInfo.m_pDesc, index ) )
			{
				Int32 i;
				CicsDefinitions::trCics2Basar( i, m_resValues.m_doubleBuffer );
				m_resValues.m_basarDecimal = Decimal(i);
			}
			else
			{
				CicsDefinitions::trCics2Basar( m_resValues.m_basarDecimal, m_resValues.m_doubleBuffer );

				// round resulting decimal because tcACCESS sends imprecise double value
				m_resValues.m_basarDecimal.round( CicsDefinitions::getDescByteScale(m_resInfo.m_pDesc, index) );
			}
		} break;
    case LINT: // also DECIMAL with Scale 0 (since SP20)
        {
            Int32 i;
			getValue(index, m_resValues.m_cicsValueInt32, func, __FILE__, __LINE__);
			CicsDefinitions::trCics2Basar( i, m_resValues.m_longBuffer);
            m_resValues.m_basarDecimal = Decimal( i );
        } break;
	default:
		{
			throw NotImplementedException(ExceptInfo(func, "Not implemented (yet?)", __FILE__, __LINE__));
		} break;
	}

    BLOG_DEBUG_STREAM(
        LoggerPool::resSetCicsGetVal(),
        "<"
        << m_stmtInfo.m_statement
        << "> - "
        << func
        << "("
        << CicsDefinitions::getDescColName(m_resInfo.m_pDesc, index)
        << "["
        << index
        << "]): <"
        << m_resValues.m_basarDecimal.toString()
        << ">");

	return m_resValues.m_basarDecimal;
}

// ---------------------------------------------------------------------------
//! \throw no-throw
//!
const Date & ResultsetCics::getDate (UInt32 index) const
{
	ConstString func = "basar::db::sql::ResultsetCics::getDate";

	getValue(
											  index,
        m_resValues.m_cicsValueDate, 
        func,  
        __FILE__, __LINE__);

	CicsDefinitions::trCics2Basar( 
        m_resValues.m_basarDate, 
        m_resValues.m_dateBuffer);

    BLOG_DEBUG_STREAM(
        LoggerPool::resSetCicsGetVal(),
        "<"
        << m_stmtInfo.m_statement
        << "> - "
        << func
        << "("
        << CicsDefinitions::getDescColName(m_resInfo.m_pDesc, index)
        << "["
        << index
        << "]): <"
        << m_resValues.m_basarDate.toStrDate()
        << ">");

	return m_resValues.m_basarDate;
}

// ---------------------------------------------------------------------------
//! \throw no-throw
//!
const Time & ResultsetCics::getTime (UInt32 index) const
{
	ConstString func = "basar::db::sql::ResultsetCics::getTime";

	getValue(
											  index,
        m_resValues.m_cicsValueTime,
        func,  
        __FILE__, __LINE__);

	CicsDefinitions::trCics2Basar(
        m_resValues.m_basarTime, 
        m_resValues.m_timeBuffer);

    BLOG_DEBUG_STREAM(
        LoggerPool::resSetCicsGetVal(),
        "<"
        << m_stmtInfo.m_statement
        << "> - "
        << func
        << "("
        << CicsDefinitions::getDescColName(m_resInfo.m_pDesc, index)
        << "["
        << index
        << "]): <"
        << m_resValues.m_basarTime.toStrTime()
        << ">");

	return m_resValues.m_basarTime;
}

// ---------------------------------------------------------------------------
//! \throw no-throw
//!
const DateTime & ResultsetCics::getDateTime (UInt32 index) const
{
	ConstString func = "basar::db::sql::ResultsetCics::getDateTime";

	getValue(
												 index,
        m_resValues.m_cicsValueDateTime, 
        func,  
        __FILE__, __LINE__);

	CicsDefinitions::trCics2Basar( 
        m_resValues.m_basarDateTime, 
        m_resValues.m_datetimeBuffer);

    BLOG_DEBUG_STREAM(
        LoggerPool::resSetCicsGetVal(),
        "<"
        << m_stmtInfo.m_statement
        << "> - "
        << func
        << "("
        << CicsDefinitions::getDescColName(m_resInfo.m_pDesc, index)
        << "["
        << index
        << "]): <"
        << m_resValues.m_basarDateTime.toStrTime()
        << ">");

	return m_resValues.m_basarDateTime;
}

// ---------------------------------------------------------------------------
//! \throw NotImplementedException
//!
const TimeSpan& ResultsetCics::getTimeSpan (UInt32) const
{
    throw NotImplementedException(
            ExceptInfo(
                "basar.db.sql.ResultsetCics::getDateTime",
                "Not implemented",
                __FILE__, __LINE__));
}

// ---------------------------------------------------------------------------
//! \throw no throw
//! \return matching basar type to cics type
inline basar::SupportedTypeEnum ResultsetCics::getBasarTypeForCicsType( const CicsTypeEnum cicsType ) const
{
    switch ( cicsType )
    {
        case SINT:
            return basar::INT16;
            break;

        case LINT:
            return basar::INT32;
            break;

        case FLOAT:
            return basar::FLOAT32;
            break;

        case DECIMAL:
            return basar::DECIMAL;
            break;

        case STRING:
            return basar::STRING;
            break;

        case DATE:
            return basar::DATE;
            break;

        case DATETIME:
        case TIME:
            return basar::DATETIME;
            break;

        default:
            return basar::UNKNOWN;
            break;
    };
}

// ---------------------------------------------------------------------------
}	// namespace sql
}	// namespace db
}	// namespace basar
