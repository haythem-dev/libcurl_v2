/*
 * @file cicsdefinitions.h
 * @brief ...
 * @author Anke Klink
 * @date 2007
 */

#ifndef  __GUARD_BASAR_LIBBASARDBSQL_CICS_CICSDEFINITIONS_H__ 
#define  __GUARD_BASAR_LIBBASARDBSQL_CICS_CICSDEFINITIONS_H__ 

//------------------------------------------------------------------------------
#include "libbasarcmnutil_bstring.h"
#include "libbasarcmnutil_decimal.h"
#include "libbasarcmnutil_datetime.h"
#include "libbasarcmnutil_date.h"
#include "libbasardbsql_definitions.h"
#include "libbasardbsql_databaseinfo.h"

#include <boost/shared_array.hpp>

#include <tcaapi.h>

//------------------------------------------------------------------------------
namespace basar
{
namespace db
{
namespace sql
{
// -----------------------------------------------------------
//! \brief  known max length of tcaccess statements
const size_t TCACCESS_STATEMENT_MAX_LENGTH = 32768;

// -----------------------------------------------------------
//! \brief  known and used CICS types
enum CicsTypeEnum
{
	UNKNOWN,			//!< undefined Value 
	SINT,				//!< 16bit Value corresponding to basar::Int16
	LINT,				//!< 32bit Value corresponding to basar::Int32
	FLOAT,				//!< float Value corresponding to basar::Float64 / basar::Float32
	DECIMAL,			//!< decimal Value corresponding to basar::Decimal
	STRING,				//!< String Value corresponding to basar::VarString  
	DATE,				//!< Date Value corresponding to basar::Date  
	DATETIME,			//!< DATETIME Value corresponding to basar::DateTime  
	TIME,				//!< Time Value corresponding to basar::DateTime  
	GRAPHIC				//!< GRAPHIC Value, has no corresponding basar Value, not used.
};

// -----------------------------------------------------------
//! information on CICS connection
struct CicsConnectionInfo
{
	LONG						m_slot;			//!< connection slot
	VarString					m_config;		//!< tcACCESS configuration
	DatabaseInfo::CicsCustom	m_custconn;		//!< tcACCESS custom connection (ip, port, start transaction)
	VarString					m_user;			//!< tcACCESS user
	VarString					m_passwd;		//!< tcACCESS password

	//! \brief c'tor for unopend connection 
	CicsConnectionInfo(LONG slot);
};

// -----------------------------------------------------------
//! information on statement for CICS
struct CicsStatementInfo
{
	CicsConnectionInfo	m_connInfo;		//!< connection info
	VarString			m_statement;    //!< SQL Statement
	TCAHANDLE   		m_handle;       //!< Handle on the statement, as it is executed

	//! \brief constructor, with known statement
	CicsStatementInfo(const CicsConnectionInfo & connInfo ,		//!< connection info
					  const VarString          & statement		//!< statement
					 );

	//! \brief constructor, without known statement
	CicsStatementInfo(const CicsConnectionInfo & connInfo 		//!< connection info
				     );
};

// -----------------------------------------------------------
//! information on resultset for CICS
struct CicsResultsetInfo
{
	PTCACCESS_SQL_DATA_DESCRIPTION	m_pDesc;			//!< tcACCESS result description
	ULONG							m_currRow;			//!< current row within tcACCESS buffer
	ULONG							m_totalRows;		//!< number of rows
	ULONG							m_totalCols;		//!< number of columns
	BOOL							m_endOfData;		//!< no further data from host

	//! \brief c'tor
	CicsResultsetInfo();
};

// -----------------------------------------------------------
//! \brief tcACCESS values in resultset
struct CicsValue
{
	ULONG						m_maxlength;			//!< Maximal length in byte of the Value
	CicsTypeEnum				m_type;					//!< Type of the Value
	PVOID						m_pValue;				//!< Pointer to the Value
	USHORT						m_valuelength;			//!< current value length

	//! Constructor
	CicsValue ( CicsTypeEnum type,		//!< Type of Data
				PVOID        pValue,	//!< Pointer to Data
				ULONG        length		//!< Length of Data in Byte
			  );
};

// -----------------------------------------------------------
//! CICS Return Values
struct CicsReturnValues
{
	Int32						m_basarInt32;			//!< to return reference to Int32 values
	Int16						m_basarInt16;			//!< to return reference to Int16 values
	Float64						m_basarFloat64;			//!< to return reference to Float64 values
	Float32						m_basarFloat32;			//!< to return reference to Float32 values
	Decimal						m_basarDecimal;			//!< to return reference to Decimal values
	Date						m_basarDate;			//!< to return reference to Date values
	Time						m_basarTime;			//!< to return reference to Time values
	DateTime					m_basarDateTime;		//!< to return reference to DateTime values

	LONG                        m_longBuffer;			//!< buffer to hold a long value
	SHORT                       m_shortBuffer;			//!< buffer to hold a short value
	DOUBLE						m_doubleBuffer;			//!< buffer to hold a decimal value
	TCACCESS_SQL_DATE			m_dateBuffer;			//!< buffer to hold a date value
	TCACCESS_SQL_TIME			m_timeBuffer;			//!< buffer to hold a time value
	TCACCESS_SQL_TIMESTAMP		m_datetimeBuffer;		//!< buffer to hold a tcaccess_sql_timestamp value
	boost::shared_array<CHAR>	m_stringBuffer;			//!< buffer to hold a string value

	CicsValue					m_cicsValueInt32;		//!< hold a Int32 Value
	CicsValue					m_cicsValueInt16;		//!< hold a Int16 Value
	CicsValue					m_cicsValueFloat64;		//!< hold a Float64 Value
	CicsValue					m_cicsValueFloat32;		//!< hold a Float32 Value
	CicsValue					m_cicsValueDecimal;		//!< hold a Decimal Value
	CicsValue					m_cicsValueDate;		//!< hold a Date Value
	CicsValue					m_cicsValueTime;		//!< hold a Time Value
	CicsValue					m_cicsValueDateTime;	//!< hold a DateTime Value
	CicsValue					m_cicsValueString;		//!< hold a String Value

	//! c'tor
	CicsReturnValues();
};	

// -----------------------------------------------------------
//! \brief Error Info from CICS
struct CicsErrorInfo
{
	ULONG		m_errorCode;		//!< cics Error Code
	VarString	m_errorMessage;		//!< cics Error Message

    //! \brief default c'tor
	CicsErrorInfo();
};

// -----------------------------------------------------------
//! \brief definition for CICS values and constants, used to transform CICS Values in basar values and vice versa
class CicsDefinitions
{
public:
	//! \brief returns the last Error as CicsErrorInfo
	static  CicsErrorInfo		getLastCicsError	();

	//! \brief returns the last Error as DBErrorCode::Code
	static DBErrorCode::Code	getLastError		();

	//! \brief get CICS name for opened session	\n no-throw
	static VarString			getSessionCicsName	(const CicsConnectionInfo & conn	//!< CICS connection info
													);

	//! \brief open tcACCESS session	\n throw DbExceptionCics
	static void					openSession			(CicsConnectionInfo & conn			//!< CICS connection info
													);
	//! \brief close tcACCESS session	\n no-throw
	static void					closeSession		(const CicsConnectionInfo & conn	//!< CICS connection info
													);

	//! \brief executes a statement	\n no-throw
	static void					start				(CicsStatementInfo & stmt			//!< SQL statement info
													);
	//! \brief executes a statement and checks returned data	\n no throw
	static void					startAndExec		(CicsStatementInfo & stmt			//!< SQL statement info
													);
	//! \brief executes a statement \n no throw
	static ExecuteReturnInfo	exec				(const CicsStatementInfo & stmt,	//!< SQL statement info
													 bool flagExcept					//!< if error: DB exception / return code
													);
	//! \brief fetch rows for executed statement	\n no-throw
	static void					fetch				(const CicsStatementInfo & stmt,	//!< SQL statement info
													 CicsResultsetInfo       & result	//!< resultset info
													);
	//! \brief retrieve fetched values	\n no-throw
	static void					retrieveValues		(const CicsStatementInfo & stmt,	//!< SQL statement info
													 ULONG                     row,		//!< resultset row    (0 .. n-1)
													 ULONG                     col,		//!< resultset column (0 .. n-1)		
													 CicsValue               & val		//!< resultset value struct
													);
	//! \brief sends stop command (used in closeStatement/ closeConnection) \n no-throw
	static void					stop				(CicsStatementInfo & stmt //!< SQL statement info
															);

	//! \brief get basar enum type for given column	\n no-throw
	static CicsTypeEnum			getDescType			(const TCACCESS_SQL_DATA_DESCRIPTION * const pDesc,	//!< tcACCESS result description
													 ULONG							             index	//!< column (0 .. n-1)
													);
	//! \brief get tcACCESS type name for given column	\n no-throw
	static VarString			getDescTypeName		(const TCACCESS_SQL_DATA_DESCRIPTION * const pDesc,	//!< tcACCESS result description
													 ULONG							             index	//!< column (0 .. n-1)
													);
	//! \brief get tcACCESS column name for given column	\n no-throw
	static VarString			getDescColName		(const TCACCESS_SQL_DATA_DESCRIPTION * const pDesc,	//!< tcACCESS result description
													 ULONG							             index	//!< column (0 .. n-1)
													);
	//! \brief get max. data length for given column	\n no-throw
	static USHORT    			getDescMaxLen		(const TCACCESS_SQL_DATA_DESCRIPTION * const pDesc,	//!< tcACCESS result description
													 ULONG							             index	//!< column (0 .. n-1)
													);
	//! \brief get decimal places for given decimal column	\n no-throw
	static BYTE    				getDescByteScale	(const TCACCESS_SQL_DATA_DESCRIPTION * const pDesc,	//!< tcACCESS result description
													 ULONG							             index	//!< column (0 .. n-1)
													);
	//! \brief translates tcACCESS type to basar enum type \n no-throw
	static CicsTypeEnum			getType				(USHORT type					                    //!< tcACCESS host type
													);
	//! \brief returns tcACCESS type name for given tcACCESS numeric type	\n no-throw
	static VarString			getTypeName			(USHORT type					                    //!< tcACCESS host type
													);

	//! \brief transforms CICS to basar value	\n no-throw
	static void					trCics2Basar		(Date                         & val,	//!< destination: basar value 
													 const TCACCESS_SQL_DATE      & buffer	//!< source: CICS value buffer  
													);
	//! \brief transforms CICS to basar value	\n no-throw
	static void					trCics2Basar		(Time                         & val,	//!< destination: basar value 
													 const TCACCESS_SQL_TIME      & buffer 	//!< source: CICS value buffer  
													);
	//! \brief transforms CICS to basar value	\n no-throw
	static void					trCics2Basar		(DateTime                     & val, 	//!< destination: basar value
													 const TCACCESS_SQL_TIMESTAMP & buffer 	//!< source: CICS value buffer  
													);
	//! \brief transforms CICS to basar value	\n no-throw
	static void					trCics2Basar		(Float32                      & val, 	//!< destination: basar value
													 const DOUBLE                 & buffer	//!< source: CICS value buffer  
													);
	//! \brief transforms CICS to basar value	\n no-throw
	static void					trCics2Basar		(Float64                      & val, 	//!< destination: basar value
													 const DOUBLE                 & buffer	//!< source: CICS value buffer  
													);
	//! \brief transforms CICS to basar value	\n no-throw
	static void					trCics2Basar		(Decimal                      & val, 	//!< destination: basar value
													 const DOUBLE                 & buffer	//!< source: CICS value buffer  
													);
	//! \brief transforms CICS to basar value	\n no-throw
	static void					trCics2Basar		(Int32                        & val,	//!< destination: basar value
													 const DOUBLE                 & buffer	//!< source: CICS value buffer  
													);
	//! \brief transforms CICS to basar value	\n no-throw
	static void					trCics2Basar		(Int32                        & val,	//!< destination: basar value
													 const LONG                   & buffer	//!< source: CICS value buffer  
													);
	//! \brief transforms CICS to basar value	\n throw InvalidParameterException
	static void					trCics2Basar		(Int16                        & val,	//!< destination: basar value
													 const DOUBLE                 & buffer	//!< source: CICS value buffer  
													);
	//! \brief transforms CICS to basar value	\n throw InvalidParameterException
	static void					trCics2Basar		(Int16                        & val,	//!< destination: basar value
													 const LONG                   & buffer	//!< source: CICS value buffer  
													);
	//! \brief transforms CICS to basar value	\n throw InvalidParameterException
	static void					trCics2Basar		(Int16                        & val,	//!< destination: basar value
													 const SHORT                  & buffer	//!< source: CICS value buffer  
													);
private:
	//! \brief does session already exist for given slot	\n no-throw
	static bool			 isSession	  (const CicsConnectionInfo & conn	//!< CICS connection info
									  );
	//! \brief is communication handler loaded for given slot
	static bool			 isHandled	  (const CicsConnectionInfo & conn	//!< CICS connection info
									  );

	//! \brief error handler \n throw DbExceptionCics
	static void			 checkError	  (BOOL              error, //!< return value from tcACCESS API
									   const VarString & file,	//!< filename where error occurred
									   const UInt32      line 	//!< fileline where error occurred
                                      );
    //! \brief formats error message
    static CicsErrorInfo getErrorInfo (ULONG             code, //!< code
                                       BOOL              comm, //!< communication/application type
                                       ConstString       msg   //!< message
                                      );
		};

#ifdef EXTENDED_API
	//! \brief translates a Enum Type value to the tcACCESS Type
	static USHORT				getDefaultCicsType	(TypeEnum type,						//!< Type
													 bool     nullable = true			//!< might the value be NULL
													);
	//! \brief transforms basar to CICS value	\n no-throw
	static void					trBasar2Cics		(TCACCESS_SQL_DATE      & buffer,	//!< destination: CICS value buffer 
													 const Date             & val		//!< basar value
													);
	//! \brief transforms basar to CICS value	\n no-throw
	static void					trBasar2Cics		(TCACCESS_SQL_TIME      & buffer,	//!< destination: CICS value buffer 
													 const Time             & val		//!< basar value
													);
	//! \brief transforms basar to CICS value	\n no-throw
	static void					trBasar2Cics		(TCACCESS_SQL_TIMESTAMP & buffer, 	//!< destination: CICS value buffer 
													 const DateTime         & val		//!< basar value
													);
	//! \brief transforms basar to CICS value	\n no-throw
	static void					trBasar2Cics		(DOUBLE                 & buffer, 	//!< destination: CICS value buffer 
													 const Float32          & val		//!< basar value
													);
	//! \brief transforms basar to CICS value	\n no-throw
	static void					trBasar2Cics		(DOUBLE                 & buffer, 	//!< destination: CICS value buffer 
													 const Float64          & val		//!< basar value
													);
	//! \brief transforms basar to CICS value	\n no-throw
	static void					trBasar2Cics		(DOUBLE                 & buffer, 	//!< destination: CICS value buffer 
													 const Decimal          & val		//!< basar value
													);

	//! Callback Function for Logging Purpose
	BOOL CALLBACK tcACCESS_HostTransferProgressFunction (
        PCSZ  pcszTransferSource,     // name of transfer source
		PCSZ  pcszTransferTarget,     // name of transfer target
		LONG  lSlot,                  // used connection slot
		PCSZ  pcszStatus,             // actual transfer status
		ULONG ulProgressPercent,      // transfer completion in percent
		ULONG ulCompressionRatio,     // compression ration in percent
		ULONG ulRecords,              // number of records already transferred
		ULONG ulBytes,                // number of bytes already transferred
		ULONG ulElapsedSeconds,       // number of seconds while transferring data
		BOOL  bCRC,                   // indicates whether CRC check is on or off
		BOOL  bCompression            // indicates whether data compression is on or off
	);
#endif // EXTENDED_API

}	// namespace
}
}

#endif 
