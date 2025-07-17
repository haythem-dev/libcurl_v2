/*
* @file cicsdefinitions.cpp
* @brief ...
* @author Anke Klink
* @date 2007
*/
//------------------------------------------------------------------------------

#include "../loggerpool.h"
#include "libbasarcmnutil_logging.h"
#include "libbasarcmnutil_definitions.h"
#include "libbasardbsql_exceptions.h"
#include "cicsdefinitions.h"
#include <iomanip>

//------------------------------------------------------------------------------
namespace basar
{
namespace db
{
namespace sql
{
// ------------------------------------------------------------
// CicsConnectionInfo
// -----------------------------------------------------------
CicsConnectionInfo::CicsConnectionInfo (LONG slot) 
    : m_slot(slot)
{
}

// ------------------------------------------------------------
// CicsStatementInfo
// -----------------------------------------------------------
CicsStatementInfo::CicsStatementInfo (
    const CicsConnectionInfo & connInfo) 
	: m_connInfo (connInfo),
      m_handle   (0)
{
}

// ------------------------------------------------------------
CicsStatementInfo::CicsStatementInfo (
    const CicsConnectionInfo & connInfo,
	const VarString          & statement)
    : m_connInfo (connInfo),
	  m_statement(statement),
      m_handle   (0)
{
}

// ------------------------------------------------------------
// CicsResultsetInfo
// -----------------------------------------------------------
CicsResultsetInfo::CicsResultsetInfo() 
    : m_pDesc    (0),
	  m_currRow  (0),
	  m_totalRows(0),
	  m_totalCols(0),
	  m_endOfData(TRUE)
{
}

// ---------------------------------------------------------------------------------------
// CicsReturnValues
// ---------------------------------------------------------------------------------------
CicsReturnValues::CicsReturnValues() :
	m_basarInt32		(0),
	m_basarInt16		(0),
	m_basarFloat64		(0.0),
	m_basarFloat32		(0.0),
	m_basarDecimal		(0.0),
	m_basarDate			(Date    ::NULL_DATE),
	m_basarTime			(Time    ::NULL_DATE, Time    ::NULL_TIME),
	m_basarDateTime		(DateTime::NULL_DATE, DateTime::NULL_TIME),

	m_longBuffer        (0),
	m_shortBuffer       (0),
	m_doubleBuffer		(0.0),

	m_cicsValueInt32	(LINT    , &m_longBuffer       , sizeof(m_longBuffer    )),
	m_cicsValueInt16	(SINT    , &m_shortBuffer      , sizeof(m_shortBuffer   )),
	m_cicsValueFloat64	(FLOAT   , &m_doubleBuffer     , sizeof(m_doubleBuffer  )),
	m_cicsValueFloat32	(FLOAT   , &m_doubleBuffer     , sizeof(m_doubleBuffer  )),
	m_cicsValueDecimal	(DECIMAL , &m_doubleBuffer     , sizeof(m_doubleBuffer  )),
	m_cicsValueDate		(DATE    , &m_dateBuffer       , sizeof(m_dateBuffer    )),
	m_cicsValueTime		(TIME    , &m_timeBuffer       , sizeof(m_timeBuffer    )),
	m_cicsValueDateTime	(DATETIME, &m_datetimeBuffer   , sizeof(m_datetimeBuffer)),
	m_cicsValueString	(STRING  , m_stringBuffer.get(), 0                       )
{
}

// ------------------------------------------------------------
// CicsErrorInfo
// -----------------------------------------------------------
CicsErrorInfo::CicsErrorInfo() : m_errorCode(0)
{
}

// ------------------------------------------------------------
// CicsValue
// -----------------------------------------------------------
CicsValue::CicsValue (	
    CicsTypeEnum type  ,
    PVOID        pValue,
    ULONG        length) :
    m_maxlength  (length),
	m_type	     (type),
	m_pValue	 (pValue),
	m_valuelength(0)
{
}

// ------------------------------------------------------------
// CicsDefinitions
// -----------------------------------------------------------

// -----------------------------------------------------------
//! \return CicsErrorInfo containing formated error message
CicsErrorInfo CicsDefinitions::getErrorInfo(
    ULONG       code,
    BOOL        comm,
    ConstString msg)
{
    CicsErrorInfo retval;

    retval.m_errorCode = code;
	retval.m_errorMessage.format("%s error: %s",
					             comm ? "communication" 
                                      : "application",
								 msg);

    return retval;
}

// -----------------------------------------------------------
CicsErrorInfo CicsDefinitions::getLastCicsError()
{
    const Int32   sizeOfBuffer         = 1024;
    CHAR          buffer[sizeOfBuffer] = { 0 };
    BOOL          isCommError          = false;
    ULONG         errorCode            = 0;

    tcACCESS_GetLastError(&isCommError, &errorCode, buffer, sizeOfBuffer);
    buffer[sizeOfBuffer - 1] = '\0';

    return getErrorInfo(errorCode, isCommError, buffer);
}

// -----------------------------------------------------------

DBErrorCode::Code CicsDefinitions::getLastError()
{
    DBErrorCode::Code retval = DBErrorCode::NONSPECIFIC;
    CicsErrorInfo     info   = getLastCicsError();

    switch (info.m_errorCode)
    {
    case 0:
        retval = DBErrorCode::SUCCESS;          
        break;

    case 0x209A590E:
        retval = DBErrorCode::INSERT_DUPL;
        break;

    default:
        retval = DBErrorCode::NONSPECIFIC;
		break;
    }

    BLOG_DEBUG_STREAM(
        LoggerPool::cicsDefinitions(),
        "CicsDefinitions::getLastError() returns " 
        << retval 
        << " (CICS error: 0x"
        << std::setfill('0')
        << std::setw(8)
        << std::hex
        << info.m_errorCode 
        << "(" 
        << info.m_errorMessage 
        << "))");

    return retval;
}

// -----------------------------------------------------------
//! \throw DbExceptionCics
//!
void CicsDefinitions::checkError(
    BOOL              error,
	const VarString & file,
	const UInt32      line)
{
    if (FALSE == error)
	{
		CicsErrorInfo info = getLastCicsError();

        assert(info.m_errorCode != 0);

		throw DbExceptionCics(
            info.m_errorCode, 
			info.m_errorMessage, 
			file, 
			line);
	}
}

// -----------------------------------------------------------
//! \throw  no-throw
//! \retval true  session for given slot exists
//! \retval false session for given slot doesn't exist
//!
bool CicsDefinitions::isSession(const CicsConnectionInfo & conn)
{
	LONG status;
	BOOL ok = tcACCESS_GetSessionStatus(&status, conn.m_slot);
	checkError(ok, __FILE__, __LINE__);

	return (TCASS_SESSION_WITH_VM <= status);
}
			
// -----------------------------------------------------------
//! \retval true  communication handler loaded and no session
//! \retval false either communication handler not loaded or
//!               session is available
//!
bool CicsDefinitions::isHandled(const CicsConnectionInfo & conn)
{
	LONG status;
	BOOL ok = tcACCESS_GetSessionStatus(&status, conn.m_slot);
	checkError(ok, __FILE__, __LINE__);

	return (TCASS_COMMUNICATION_HANDLER_LOADED == status);
}

// -----------------------------------------------------------
//! \throw  no throw
//! \retval CICS name for opened session
//!
VarString CicsDefinitions::getSessionCicsName(
    const CicsConnectionInfo & conn)
{
	return TcaGetHostApplId(conn.m_slot);
}

// -----------------------------------------------------------------------
//! workaround needed: if you start a Windows VS debug (sic!) session,
//! sometimes after a statement abort a following (re-)connect causes an
//! error with code 0x0 (!).
//! So after such an issue a connection is immediately retried and a warning 
//! log message is sent.
//! For all other issues an exception is thrown.
//! \throw no-throw
//!
void CicsDefinitions::openSession (CicsConnectionInfo & conn)
{
    // --- prepare configuration

	PSZ  _user         = const_cast <PSZ> (conn.m_user  .c_str());
	PSZ  _pwd          = const_cast <PSZ> (conn.m_passwd.c_str());
    PCSZ configuration = conn.m_config.c_str();

	VarString	customConnect;

	// if values exist
	if ( ! conn.m_custconn.ipAddr       .empty() &&
		 ! conn.m_custconn.port         .empty() &&
		 ! conn.m_custconn.startTransact.empty()    )
	{
		customConnect.format(
            "%s,%s,%s", 
            conn.m_custconn.ipAddr       .c_str(),
			conn.m_custconn.port         .c_str(),
			conn.m_custconn.startTransact.c_str());
	}

    // --- call tca

	BOOL  commError	    = FALSE;
	ULONG errorCode		= 0;
	CHAR  errorMsg[1024] = { 0 };

    const Int32 MAXTRIES = 3;   // connect retries
    Int32       retried  = 0;   // counter

    while (retried < MAXTRIES)
    {
	    BOOL ok = TcaAllocateSession (
            &conn.m_slot,				// slot
			NULL,                       // parent
			_user,						// user ID
			_pwd,						// password
			NULL,						// password new
			ASF_FORCE_NO_LOGON_DIALOG | 
			ASF_ATTACH_SILENT         | 
			ASF_GET_HOST_APPLID,		// flags
			configuration,				// connect configuration
            customConnect.c_str(),		// custom connect: 
                                        //    IP address
                                        //    port number
                                        //    start transaction (VSE)
			&commError,					// communication error
			&errorCode,					// error code
			errorMsg,					// error message
			sizeof(errorMsg));			// buffer size of error message

        // connection successful
        if (FALSE != ok)
        {
            assert(0 == errorCode);
            return;
        }

        // serious failure: throw exception
        if (0 != errorCode)
        {
            CicsErrorInfo info = getErrorInfo(errorCode, commError, errorMsg);

            // tcACCESS message:
            // "PC Communication Handler => TCP/IP (connect): Verbindungsversuch 
            // wurde zurueckgewiesen"
            // -> port listener not available
            if (0x1037083d == errorCode)
            {
                throw DbExceptionCicsUnavail(
                        info.m_errorCode,
			            info.m_errorMessage,
			            __FILE__, __LINE__);
            }
            else
            {
                throw DbExceptionCics(
                        info.m_errorCode, 
			            info.m_errorMessage, 
			            __FILE__, __LINE__);
            }
        }

        ++retried;

        // errorCode == 0 : special issue -> retry
        BLOG_WARN_STREAM(
            LoggerPool::cicsDefinitions(),
            ""
            << retried 
            << ". try to connect to "
            << configuration
            << "/"
            << customConnect
            << "@"
            << conn.m_slot
            << " failed: error code "
            << errorCode
            << " -> retry connection ...");
    } // while

    // MAXTRIES reached
    BLOG_ERROR_STREAM(
        LoggerPool::cicsDefinitions(),
        "max retries "
        << MAXTRIES
        << " reached for connect to "
        << configuration
        << "/"
        << customConnect
        << "@"
        << conn.m_slot
        << ". So given up");

    throw DbExceptionCics(
            0, 
			"max tries for connect reached. Unknown reason", 
			__FILE__, __LINE__);
}

// -----------------------------------------------------------
//! \throw no-throw
//!
void CicsDefinitions::closeSession(const CicsConnectionInfo & conn)
{ 
	BOOL ok;
    
    if (isSession(conn))
    {
        ok = tcACCESS_DeallocateSession(conn.m_slot);
        checkError(ok,__FILE__, __LINE__);
    }

#ifdef _WIN32
    if (isHandled(conn))
    {
		ok = tcACCESS_CloseCommunicationHandler(TCACCH_ALWAYS, conn.m_slot);
		checkError(ok,__FILE__, __LINE__);
    }
#endif
}

// -----------------------------------------------------------
//! \throw no-throw
//!
void CicsDefinitions::start(CicsStatementInfo & stmt)
{
    BOOL ok = tcACCESS_HostSqlQueryAsync_Start(
        TCADBT_SQL_ENGINE,								// DB type
		stmt.m_statement.c_str(),						// SQL statement
		0,												// MaxCharFieldLength
		NULL, NULL, NULL,								// host parameter
		0,												// max records
		&stmt.m_handle,									// query handle
		stmt.m_connInfo.m_slot,							// slot
		0,												// options
		NULL,											// parent window
		0,	// tcACCESS_HostTransferProgressFunction,	// progress callback
		0, 0											// error display
		);

    BLOG_DEBUG_STREAM(
        LoggerPool::cicsDefinitions(),
        "executed tcACCESS_HostSqlQueryAsync_Start ("
        << stmt.m_statement
        << ") on slot("
        << stmt.m_connInfo.m_slot
        << ") with handle(0x"
        << std::hex
        << stmt.m_handle
        << ") results to "
        << std::dec
        << ok);

    checkError(ok, __FILE__, __LINE__);
}

// -----------------------------------------------------------
//! \throw no-throw
//!
void CicsDefinitions::startAndExec(CicsStatementInfo & stmt)
{
    start(stmt);
	exec (stmt, true);
}

// -----------------------------------------------------------
//! \throw no-throw
//! \return ExecuteReturnInfo indicating success or failure of executed statement
ExecuteReturnInfo CicsDefinitions::exec(
    const CicsStatementInfo & stmt, 
    bool                      flagExcept)
{
    PTCACCESS_SQL_DATA_DESCRIPTION	pDesc;
	BOOL							endOfData    = TRUE;
    ULONG							numberOfCols = 0;
    ULONG							numberOfRows = 0;

    BOOL ok = tcACCESS_HostSqlQueryAsync_FetchData(
        (PPVOID)&pDesc,			// result description
		&numberOfCols,
		&numberOfRows,
		stmt.m_handle,
		&endOfData);

	if (flagExcept)
		checkError(ok, __FILE__, __LINE__);

    ExecuteReturnInfo retval;

    if (ok)
    {
        retval.m_affectedRows = numberOfRows;
    }
    else
    {
        retval.m_error = getLastError();
    }

    return retval;
}

// -----------------------------------------------------------
//! \throw no-throw
//!
void CicsDefinitions::fetch(
    const CicsStatementInfo & stmt,	
	CicsResultsetInfo       & result)
{
	BOOL ok = tcACCESS_HostSqlQueryAsync_FetchData(
        (PPVOID)&result.m_pDesc,
		&result.m_totalCols,
		&result.m_totalRows,
		stmt.m_handle,
		&result.m_endOfData);

	checkError(ok, __FILE__, __LINE__);
}

// -----------------------------------------------------------
//! \throw no-throw
//!
void CicsDefinitions::retrieveValues(
    const CicsStatementInfo & stmt,
	ULONG                     row,
	ULONG                     col,
	CicsValue               & val)
{
	BOOL fNull;

	// starts internally with index = 1 rather than zero-based
	BOOL ok = tcACCESS_HostSqlQueryAsync_GetOutputFieldValue(
        col + 1,				// column (1 .. n)
		row + 1,				// row    (1 .. n)
		val.m_pValue,			// value buffer
		val.m_maxlength,		// value buffer length
		&val.m_valuelength,		// value length
		&fNull,					// null flag
		stmt.m_handle);			// query handle

    checkError(ok, __FILE__, __LINE__);
}

// -----------------------------------------------------------
//! \throw no-throw
//!
void CicsDefinitions::stop(CicsStatementInfo & stmt )
{
    if (0 == stmt.m_handle)
		return;

    BOOL ok  = tcACCESS_HostSqlQueryAsync_Stop(stmt.m_handle);
    checkError(ok , __FILE__, __LINE__);
    stmt.m_handle = 0;
}

// ---------------------------------------------------------------------
//! \throw no-throw
//! \return basar enum type for given column
//!
CicsTypeEnum CicsDefinitions::getDescType(
    const TCACCESS_SQL_DATA_DESCRIPTION * const pDesc,
	ULONG							            index)
{
	if (0 == pDesc)
		return UNKNOWN;

	USHORT type = GET_ALIGNED_U16(&(pDesc[index].usHostSqlType));

	return getType(type);
}

// ---------------------------------------------------------------------
//! \throw no-throw
//! \return tcACCESS type name for given column	
//!
VarString CicsDefinitions::getDescTypeName(
    const TCACCESS_SQL_DATA_DESCRIPTION * const pDesc,
    ULONG						               index)
{
	if (0 == pDesc)
		return "no tcACCESS SQL data description available";

	USHORT type = GET_ALIGNED_U16(&(pDesc[index].usHostSqlType));

	return getTypeName(type);
}

// ---------------------------------------------------------------------
//! \throw no-throw
//! \return tcACCESS column name for given column	
//!
VarString CicsDefinitions::getDescColName (
    const TCACCESS_SQL_DATA_DESCRIPTION * const pDesc,
    ULONG							              index)
{
	if (0 == pDesc)
		return "no tcACCESS SQL data description available";

	return pDesc[index].szName;
}

// ---------------------------------------------------------------------
//! \throw no-throw
//! \return max. data length given column
//!
USHORT CicsDefinitions::getDescMaxLen( 
    const TCACCESS_SQL_DATA_DESCRIPTION * const pDesc,
    ULONG						               index)
{
	if (0 == pDesc)
		return 0;

	return GET_ALIGNED_U16(&(pDesc[index].usMaxPcDataLength));
}

// ---------------------------------------------------------------------
//! \throw no-throw
//! \return decimal places given decimal column
//!
BYTE CicsDefinitions::getDescByteScale( 
    const TCACCESS_SQL_DATA_DESCRIPTION * const pDesc,
    ULONG							            index)
{
	if (0 == pDesc)
		return 0;

	return pDesc[index].byteScale;
}

// ---------------------------------------------------------------------
//! \throw no-throw
//! \return basar enum type for given tcACCESS numeric type
//!
CicsTypeEnum CicsDefinitions::getType( USHORT type )
{
    switch (type)
    {
    case TCAHSDT_DATE :
    case TCAHSDT_DATE_NULLABLE :
        return DATE;

    case TCAHSDT_TIME :
    case TCAHSDT_TIME_NULLABLE :				
        return TIME;

    case TCAHSDT_TIMESTAMP :
    case TCAHSDT_TIMESTAMP_NULLABLE :			
        return DATETIME;

    case TCAHSDT_VARCHAR :
    case TCAHSDT_VARCHAR_NULLABLE :
    case TCAHSDT_CHAR :
    case TCAHSDT_CHAR_NULLABLE :
    case TCAHSDT_LONGVARCHAR :
    case TCAHSDT_LONGVARCHAR_NULLABLE :			
        return STRING;

    case TCAHSDT_VARGRAPHIC :
    case TCAHSDT_VARGRAPHIC_NULLABLE :
    case TCAHSDT_GRAPHIC :
    case TCAHSDT_GRAPHIC_NULLABLE :
    case TCAHSDT_LONGVARGRAPHIC :
    case TCAHSDT_LONGVARGRAPHIC_NULLABLE :		
        return GRAPHIC;

    case TCAHSDT_FLOAT :
    case TCAHSDT_FLOAT_NULLABLE :				
        return FLOAT;

    case TCAHSDT_DECIMAL :
    case TCAHSDT_DECIMAL_NULLABLE :				
        return DECIMAL;

    case TCAHSDT_LONGINT :
    case TCAHSDT_LONGINT_NULLABLE :				
        return LINT;

    case TCAHSDT_SMALLINT :
    case TCAHSDT_SMALLINT_NULLABLE :			
        return SINT;

    default:
        return UNKNOWN;
    }
}

// ---------------------------------------------------------------------
//! \throw no-throw
//! \return tcACCESS type name for given tcACCESS numeric type
//!
VarString CicsDefinitions::getTypeName(USHORT type)
{
    switch (type)
    {
    case TCAHSDT_DATE :						
        return "TCAHSDT_DATE";
	case TCAHSDT_DATE_NULLABLE :			
        return "TCAHSDT_DATE_NULLABLE";
	case TCAHSDT_TIME :						
        return "TCAHSDT_TIME";
	case TCAHSDT_TIME_NULLABLE :			
        return "TCAHSDT_TIME_NULLABLE";
	case TCAHSDT_TIMESTAMP :				
        return "TCAHSDT_TIMESTAMP";
	case TCAHSDT_TIMESTAMP_NULLABLE :		
        return "TCAHSDT_TIMESTAMP_NULLABLE";
	case TCAHSDT_VARCHAR :					
        return "TCAHSDT_VARCHAR";
	case TCAHSDT_VARCHAR_NULLABLE :			
        return "TCAHSDT_VARCHAR_NULLABLE";
	case TCAHSDT_CHAR :						
        return "TCAHSDT_CHAR";
	case TCAHSDT_CHAR_NULLABLE :			
        return "TCAHSDT_CHAR_NULLABLE";
	case TCAHSDT_LONGVARCHAR :				
        return "TCAHSDT_LONGVARCHAR";
	case TCAHSDT_LONGVARCHAR_NULLABLE :		
        return "TCAHSDT_LONGVARCHAR_NULLABLE";
	case TCAHSDT_VARGRAPHIC :				
        return "TCAHSDT_VARGRAPHIC";
	case TCAHSDT_VARGRAPHIC_NULLABLE :		
        return "TCAHSDT_VARGRAPHIC_NULLABLE : ";
	case TCAHSDT_GRAPHIC :					
        return "TCAHSDT_GRAPHIC";
	case TCAHSDT_GRAPHIC_NULLABLE :			
        return "TCAHSDT_GRAPHIC_NULLABLE";
	case TCAHSDT_LONGVARGRAPHIC :			
        return "TCAHSDT_LONGVARGRAPHIC";
	case TCAHSDT_LONGVARGRAPHIC_NULLABLE :	
        return "TCAHSDT_LONGVARGRAPHIC_NULLABLE";
	case TCAHSDT_FLOAT :					
        return "TCAHSDT_FLOAT";
	case TCAHSDT_FLOAT_NULLABLE :			
        return "TCAHSDT_FLOAT_NULLABLE";
	case TCAHSDT_DECIMAL :					
        return "TCAHSDT_DECIMAL";
	case TCAHSDT_DECIMAL_NULLABLE :			
        return "TCAHSDT_DECIMAL_NULLABLE";
	case TCAHSDT_LONGINT :					
        return "TCAHSDT_LONGINT";
	case TCAHSDT_LONGINT_NULLABLE :			
        return "TCAHSDT_LONGINT_NULLABLE";
	case TCAHSDT_SMALLINT :					
        return "TCAHSDT_SMALLINT";
	case TCAHSDT_SMALLINT_NULLABLE :		
        return "TCAHSDT_SMALLINT_NULLABLE";
	default:								
        return "";
    }
}

// ------------------------------------------------------------------------------------------------
//! \throw no-throw
//! 
void CicsDefinitions::trCics2Basar(
    Date & val, 
    const TCACCESS_SQL_DATE & buffer )
{
	val.setDate(GET_ALIGNED_U16(&(buffer.sYear)), 
				buffer.byteMonth, 
				buffer.byteDay);
}

// ------------------------------------------------------------------------------------------------
//! \throw no-throw
//! 
void CicsDefinitions::trCics2Basar(
    Time & val, 
    const TCACCESS_SQL_TIME & buffer )
{
    val.setTime(buffer.byteHour, 
				buffer.byteMinute, 
				buffer.byteSecond);
}

// ------------------------------------------------------------------------------------------------
//! \throw no-throw
//! 
void CicsDefinitions::trCics2Basar(
    DateTime & val, 
    const TCACCESS_SQL_TIMESTAMP & buffer )
{
	val.setDate(GET_ALIGNED_U16(&(buffer.sYear)), 
				buffer.byteMonth, 
				buffer.byteDay);
    val.setTime(buffer.byteHour, 
				buffer.byteMinute, 
				buffer.byteSecond);
}

// ------------------------------------------------------------------------------------------------
//! \throw no-throw
//! 
void CicsDefinitions::trCics2Basar(Float32 & val, const DOUBLE & buffer)
{
    val = static_cast<Float32>(buffer);
}

// ------------------------------------------------------------------------------------------------
//! \throw no-throw
//! 
void CicsDefinitions::trCics2Basar(Float64 & val , const DOUBLE &buffer)
{
	val = buffer;
}

// ------------------------------------------------------------------------------------------------
//! \throw no-throw
//! 
void CicsDefinitions::trCics2Basar(Decimal & val, const DOUBLE &buffer)
{
    val = Decimal(buffer);
}

// ------------------------------------------------------------------------------------------------
//! \throw no-throw
//! 
void CicsDefinitions::trCics2Basar(Int32 & val , const DOUBLE & buffer)
{
    val = static_cast<Int32>(buffer);
}

// ------------------------------------------------------------------------------------------------
//! \throw no-throw
//! 
void CicsDefinitions::trCics2Basar (Int32 & val , const LONG & buffer)
{
    val = static_cast<Int32>(buffer);
}

// ------------------------------------------------------------------------------------------------
//! \throw InvalidParameterException
//! 
void CicsDefinitions::trCics2Basar (Int16 & val, const DOUBLE & buffer)
{
	Int32 tmp;
	trCics2Basar(tmp, buffer);
    trCics2Basar(val, (LONG)tmp);
}

// ------------------------------------------------------------------------------------------------
//! \throw InvalidParameterException
//! 
void CicsDefinitions::trCics2Basar (Int16 & val, const LONG & buffer)
{
    if(
           buffer > 32767
        || buffer < -32768
    )
	{
		VarString msg;
        msg.format(
            "CICS value %d(0x%08x) not "
            "in range[-32768, 32767]", 
            buffer, 
            buffer);
        throw InvalidParameterException(
                ExceptInfo(
                    "basar.db.sql.CicsDefinitions.transformValue", 
					msg, 
					__FILE__, 
					__LINE__) );
	}

    val = static_cast<Int16>(buffer);
}

// ------------------------------------------------------------------------------------------------            
//! \throw InvalidParameterException
//!
void CicsDefinitions::trCics2Basar (Int16 & val, const SHORT & buffer)
{
	Int32 tmp;
	trCics2Basar(tmp, static_cast<LONG>(buffer));

	if ( (tmp > 32767) || 
         (tmp < -32768) )
	{
		VarString msg;
		msg.format(
            "CICS value %d(0x%08x) not "
            "in range[-32768, 32767]", 
            tmp, 
            tmp);
		throw InvalidParameterException(
                ExceptInfo(
                    "basar.db.sql.CicsDefinitions.transformValue", 
					msg, 
					__FILE__, 
					__LINE__) );
	}

    val = static_cast<Int16>(tmp);
}

// ================================================================================================

#ifdef EXTENDED_API

// ------------------------------------------------------------------------------------------------
//! \throw no-throw
//! 
void CicsDefinitions::trBasar2Cics(
    TCACCESS_SQL_DATE & buffer, 
    const Date        & val)
{
	SET_ALIGNED_U16(&(buffer.sYear), val.getYear());

    buffer.byteMonth = (BYTE) val.getMonth();
    buffer.byteDay   = (BYTE) val.getDay();
}

// ------------------------------------------------------------------------------------------------
//! \throw no-throw
//! 
void CicsDefinitions::trBasar2Cics(
    TCACCESS_SQL_TIME & buffer, 
    const Time        & val)
{
    buffer.byteHour   = (BYTE)val.getHour();
    buffer.byteMinute = (BYTE)val.getMinute();
    buffer.byteSecond = (BYTE)val.getSec();
}

// ------------------------------------------------------------------------------------------------
//! \throw no-throw
//! 
void CicsDefinitions::trBasar2Cics(
    TCACCESS_SQL_TIMESTAMP & buffer, 
    const DateTime         & val)
{
	SET_ALIGNED_U16(&(buffer.sYear), val.getYear());

    buffer.byteMonth  = (BYTE)val.getMonth();
    buffer.byteDay    = (BYTE)val.getDay();
    buffer.byteHour   = (BYTE)val.getHour();
    buffer.byteMinute = (BYTE)val.getMinute();
    buffer.byteSecond = (BYTE)val.getSec();
}

// ------------------------------------------------------------------------------------------------
//! \throw no-throw
//! 
void CicsDefinitions::trBasar2Cics(DOUBLE & buffer, const Float32 & val )
{
    buffer = val;
}

// ------------------------------------------------------------------------------------------------
//! \throw no-throw
//! 
void CicsDefinitions::trBasar2Cics(DOUBLE & buffer, const Float64 & val )
{
    buffer = val;
}

// ------------------------------------------------------------------------------------------------
//! \throw no-throw
//! 
void CicsDefinitions::trBasar2Cics(DOUBLE & buffer, const Decimal & val )
{
    buffer = val.toFloat64();
}

// ----------------------------------------------------------------------------------------
USHORT CicsDefinitions::getDefaultCicsType(
    CicsTypeEnum type, 
    bool         nullable /* = true */)
{
    switch (type)
    {
    case SINT:		
        return nullable ? TCAHSDT_SMALLINT_NULLABLE	  : TCAHSDT_SMALLINT;
	case LINT:		
        return nullable ? TCAHSDT_LONGINT_NULLABLE	  : TCAHSDT_LONGINT;
	case FLOAT:		
        return nullable ? TCAHSDT_FLOAT_NULLABLE	  : TCAHSDT_FLOAT;
	case DECIMAL:	
        return nullable ? TCAHSDT_DECIMAL_NULLABLE	  : TCAHSDT_DECIMAL;
	case STRING:	
        return nullable ? TCAHSDT_CHAR_NULLABLE		  : TCAHSDT_CHAR;
	case DATE:		
        return nullable ? TCAHSDT_DATE_NULLABLE		  : TCAHSDT_DATE;
	case DATETIME:	
        return nullable ? TCAHSDT_TIMESTAMP_NULLABLE  : TCAHSDT_TIMESTAMP;
	case TIME:		
        return nullable ? TCAHSDT_TIME_NULLABLE		  : TCAHSDT_TIME;
	case GRAPHIC:	
        return nullable ? TCAHSDT_VARGRAPHIC_NULLABLE : TCAHSDT_VARGRAPHIC;
	default:		
        return nullable ? TCAHSDT_VARCHAR_NULLABLE	  : TCAHSDT_VARCHAR;
    }
}

// ----------------------------------------------------------------------------------------
BOOL CALLBACK tcACCESS_HostTransferProgressFunction(
    PCSZ  ,     // name of transfer source
	PCSZ  ,     // name of transfer target
	LONG  ,     // used connection slot
	PCSZ  ,     // actual transfer status
	ULONG ,     // transfer completion in percent
	ULONG ,     // compression ration in percent
    LONG ,     // number of records already transferred														
	ULONG ,     // number of bytes already transferred
	ULONG ,     // number of seconds while transferring data
	BOOL  ,     // indicates whether CRC check is on or off
	BOOL)		// indicates whether data compression is on or off
{
    std::cout	<< "CicsDefinitions:: sm_datasets = " <<  ulRecords
				<< "bytes " << ulBytes
				<< " at " << ulProgressPercent << "%" << std::endl;

    std::cout << "CicsDefinitions all:: "<< std::endl
    << "   pcszTransferSource    =" << pcszTransferSource << std::endl
    << "   pcszTransferTarget    =" << pcszTransferTarget << std::endl
    << "   lSlot                 =" << lSlot << std::endl
    << "   pcszStatus            =" << pcszStatus << std::endl
    << "   ulProgressPercent     =" << ulProgressPercent << std::endl
    << "   ulCompressionRatio    =" << ulCompressionRatio << std::endl
    << "   ulRecords             =" << ulRecords << std::endl
    << "   ulBytes               =" << ulBytes << std::endl
    << "   ulElapsedSeconds      =" << ulElapsedSeconds << std::endl
    << "   bCRC                  =" << bCRC << std::endl
    << "   bCompression          =" << bCompression << std::endl;

    if (100 == ulProgressPercent )
    {
		//ConstString statement = 
        "select RECORDSSENT from SYSTEM.SYSSTAT where HOSTFILENAME=? AND";
    }

	return true;
}

#endif // EXTENDED_API

// ---------------------------------------------------------------------------
}   // namespace
}
}
