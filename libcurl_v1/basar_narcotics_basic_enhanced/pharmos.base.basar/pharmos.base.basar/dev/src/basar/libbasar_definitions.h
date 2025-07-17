#ifndef GUARD_LIBBASAR_DEFINITIONS_H
#define GUARD_LIBBASAR_DEFINITIONS_H
//-----------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  namespace basar definitions
 *  \author Thomas Hörath
 *  \date   15.07.2005
 */
//-----------------------------------------------------------------------------------------------------//

/*! \ingroup DIFF_WIN_UNIX */ 
									// defines for 64 bit environment
#ifndef _WIN32						// Unix-Environment
	#include <sys/types.h>			// provides access to definition of _LP64 and _ILP32
	#ifdef _LP64					// 64 bit memory model: long, pointer
		#define BASAR_ENV_64BIT     //!< 64 bit environment for basar
	#endif // _LP64
#elif defined(_WIN64)
	#define BASAR_ENV_64BIT         //!< 64 bit environment for basar
#endif // _WIN64

#ifdef __GNUC__
#include <assert.h>
#endif // __GNUC__

#include <vector>
#include <stdint.h>

//-----------------------------------------------------------------------------------------------------//
// main namespace for all basar-components
//-----------------------------------------------------------------------------------------------------//
namespace basar
{

//----------------------------------------------------------------------------
// definition for doxygen, so that differences between WIN32 and UNIX are documented -> scanning for "DIFF_WIN_UNIX" 
//! \defgroup DIFF_WIN_UNIX Differences in BASAR between WIN32 and UNIX

//----------------------------------------------------------------------------
// definition for doxygen, so that differences between DEBUG and RELEASE are documented -> scanning for "DIFF_DBG_REL" 
//! \defgroup DIFF_DBG_REL Differences in BASAR between DEBUG and RELEASE

//----------------------------------------------------------------------------
// definition for doxygen, so that differences between 32 and 64 bit environment are documented -> scanning for "DIFF_64BIT" 
//! \defgroup DIFF_64BIT Differences in BASAR between 32 and 64 bit environment

// forward declaration of classes from cmnutil
namespace cmnutil
{
	struct	ExceptionInfoStruct;
	class	BasarException;
	class	BasarOutOfRangeIndexException;
	class	BasarRuntimeException;
	class	BasarNotTerminatedException;
	class	BasarNullReferenceException;
	class	BasarInvalidStateException;
	class	BasarUnknownPropertyTypeException;
	class	BasarUnknownPropertyNameException;
	class	BasarInvalidParameterException;
	class	BasarIteratorException;
	class	BasarDifferentContainersIteratorException;
	class	BasarInvalidIteratorException;
	class	BasarOutOfRangeIteratorException;
	class   BasarAssertionException;
    class   BasarNotImplementedException;
	class   BasarTypeMismatchException;
	class	BasarIncompatibleRangeException;

	class	BString;
	class	I18nString;
	class	Decimal;
	class	DateTimeTools;
	class	DateTime;
	class	Date;
    class   TimeSpan;
}

//-----------------------------------------------------------------------------------------------------//
// enum section for basar
//-----------------------------------------------------------------------------------------------------//
//! \brief type of database to create connection to
enum ConnectionTypeEnum
{
	CICS,					//!< connection to CICS tcACCESS
	INFORMIX,				//!< connection to Informix db
	ODBC					//!< connection to ODBC db
};

/*! \brief	flags to determine the type of a insert of a new row - 
			PropertyTable row (accessed via YIterator). 
			This type is deciding from which state to which state it has to be switched.
*/
enum InsertTypeEnum
{
    FOR_UNKNOWN,	//!< open
    FOR_CLEAN,		//!< open
	FOR_INSERT,		//!< open
	FOR_UPDATE,		//!< open
    FOR_DELETE		//!< open
};

//! enum SupportedStateEnum defines all possible states
enum SupportedStateEnum 
{	
	SS_UNKNOWN	= 0x01,		//!< state is not known
	SS_CLEAN	= 0x02,		//!< state is clean -> no db-action necessary		
	SS_UNSET	= 0x04,		//!< state is unset -> parameter not set yet
	SS_INSERT	= 0x08,		//!< state is insert
	SS_UPDATE	= 0x10,		//!< state is update
	SS_DELETE	= 0x20		//!< state is delete
};

//! enum SupportedTypeEnum defines all valid types
enum SupportedTypeEnum 
{
	UNKNOWN = 0,		//!< type unknown
	INT16      ,		//!< type short
	INT32      ,		//!< type int
    INT64      ,        //!< type int64_t
	FLOAT32    ,		//!< type float
	FLOAT64    ,		//!< type double
	DECIMAL    ,		//!< type decimal
	STRING     ,		//!< type string
	DATETIME   ,		//!< type date time
	TIMESPAN   ,		//!< type time span
	DATE       ,		//!< type date
	TIME       ,		//!< type time
	INTDATE    ,		//!< type intdate (date encoded in an int - typically YYYYMMDD)
	INTTIME    ,		//!< type inttime (time encoded in an int - typically HHMMSS or (with ms) HHMMSSTTT)
    BINARY              //!< type binary (std::vector<char>)
};  


//! \brief mode for accessing locked rows or tables
enum LockModeEnum
{
	NOTWAIT ,				//!< database server ends the operation immediately and returns an error code
	WAIT	,		        //!< database server suspends the process until the lock releases
	WAITSECS				//!< \brief database server suspends process until lock releases or until waiting 
							//!<        period ends. If lock remains after waiting period, database server 
							//!<		ends the operation and returns an error code
};

//! \brief transaction isolation level for transactions
enum IsolationLevelEnum
{
	ANSI_READUNCOMMITTED			,	//!< dirty reads, non-repeatable reads and phantom reads can occur
	ANSI_READCOMMITTED  			,	//!< dirty reads are prevented; non-repeatable reads and phantom reads can occur
	ANSI_REPEATABLEREAD 			,	//!< dirty reads and non-repeatable reads are prevented; phantom reads can occur
	ANSI_SERIALIZABLE				,	//!< dirty reads, non-repeatable reads and phantom reads are prevented

	INFX_DIRTYREAD      			,	//!< informix correspondent to ANSI_READUNCOMMITTED
	INFX_DIRTYREAD_WITHWARNING		,	//!< warning for uncommitted or phantom row	
	INFX_COMITTEDREAD				,	//!< informix correspondent to ANSI_READCOMMITTED
	INFX_COMITTEDREAD_LASTCOMMITTED	,	//!< return most recently committed version of row, no lock wait
	INFX_CURSORSTABILITY			,	//!< not ANSI: fetched row doesn't change, exclusive lock can't be placed on it
	INFX_REPEATABLEREAD					//!< informix correspondent to ANSI_REPEATABLEREAD
};

//-----------------------------------------------------------------------------------------------------//
// typedef section for basar
//-----------------------------------------------------------------------------------------------------//
typedef const char *        ConstBuffer;    /*!< \brief  constant buffer: needed for return type       */
typedef const char * const  ConstString;    /*!< \brief  constant string                               */
typedef cmnutil::BString    VarString;      /*!< \brief  string with variable length                   */
typedef cmnutil::I18nString I18nString;     /*!< \brief  internationalized string with variable length */

typedef int8_t              Int8;           /*!< \brief   8 bit signed Integer        */
typedef int16_t             Int16;          /*!< \brief  16 bit signed Integer        */
typedef int32_t             Int32;          /*!< \brief  32 bit signed Integer        */

typedef uint8_t             UInt8;          /*!< \brief   8 bit unsigned Integer      */
typedef uint16_t            UInt16;         /*!< \brief  16 bit unsigned Integer      */
typedef uint32_t            UInt32;         /*!< \brief  32 bit unsigned Integer      */

typedef uint32_t    		ULong32;		/*!< \brief  32 bit unsigned Long         */
typedef int32_t      		Long32;			/*!< \brief  32 bit signed Long           */
typedef uint64_t    		ULong64;		/*!< \brief  64 bit unsigned Long         */
typedef int64_t             Long64;			/*!< \brief  64 bit signed Long           */
typedef uint64_t			UInt64;			/*!< \brief  64 bit unsigned Int          */
typedef int64_t				Int64;			/*!< \brief  64 bit signed Int            */

#if __cplusplus >= 201103L
static_assert(sizeof(int16_t)==2, "Safety check: Unknown platform: int16_t is not 16-Bit!");
static_assert(sizeof(int32_t)==4, "Safety check: Unknown platform: int32_t is not 32-Bit!");
static_assert(sizeof(int64_t)==8, "Safety check: Unknown platform: int64_t is not 64-Bit!");

#ifdef _LP64
static_assert(sizeof(long int)==sizeof(long long int), "Safety check: 64-Bit AIX platform should have sizeof(long) == sizeof(long long)");
#endif //_LP64
#endif // __cplusplus >= 201103L

/*! \ingroup DIFF_WIN_UNIX */ 
/*! \brief signed size_t, does not exist on windows, so we define one corresponding to vcruntime.h */
#ifdef _WIN64
    typedef int64_t         ssize_t;
#elif defined(_WIN32)
    typedef int32_t         ssize_t;
#endif

typedef size_t				BULong;				/*!< \brief  compiler specific unsigned long */
typedef ssize_t				BLong;				/*!< \brief  compiler specific long          */

typedef double				Float64;			/*!< \brief  64 bit Floating-point number */
typedef float				Float32;			/*!< \brief  32 bit Floating-point number */
typedef cmnutil::Decimal	Decimal;			/*!< \brief  decimal type                 */
typedef cmnutil::DateTime	DateTime;			/*!< \brief  DateTime type                */
typedef cmnutil::Date		Date;				/*!< \brief  Date type                    */
typedef	cmnutil::DateTime	Time;				/*!< \brief  Time type                    */
typedef	cmnutil::TimeSpan	TimeSpan;			/*!< \brief  TimeSpan type                */

typedef std::vector<char>   Binary;


//--------------- EXCEPTIONS -------------------------------------------
/*! \brief  A typedef representing the common basar exception info structure to set exception information text.*/
typedef cmnutil::ExceptionInfoStruct							ExceptInfo;

/*! \brief  A typedef representing the common basar exception class.*/
typedef cmnutil::BasarException									Exception;

/*! \brief  A typedef representing the common basar exception class for indexes out of range.*/
typedef cmnutil::BasarOutOfRangeIndexException					OutOfRangeIndexException;

/*! \brief  common basar exception class for detecting errors during runtime */
typedef cmnutil::BasarRuntimeException							RuntimeException;

/*! \brief  A typedef representing the common basar exception class for not terminated strings.*/
typedef cmnutil::BasarNotTerminatedException					NotTerminatedException;

/*! \brief  A typedef representing the common basar exception class for null references.*/
typedef cmnutil::BasarNullReferenceException					NullReferenceException;

/*! \brief  A typedef representing the common basar exception class for an invalid state.*/
typedef cmnutil::BasarInvalidStateException						InvalidStateException;

/*! \brief  A typedef representing the common basar exception class unknown property types.*/
typedef cmnutil::BasarUnknownPropertyTypeException				UnknownPropertyTypeException;

/*! \brief  A typedef representing the common basar exception class for unknown property names.*/
typedef cmnutil::BasarUnknownPropertyNameException				UnknownPropertyNameException;

/*! \brief  A typedef representing the common basar exception class for invalid parameters.*/
typedef cmnutil::BasarInvalidParameterException					InvalidParameterException;

/*! \brief  A typedef representing the common basar iterator exception class.*/
typedef cmnutil::BasarIteratorException							IteratorException;

/*! \brief  A typedef representing the common basar exception class for different iterator containers.*/
typedef cmnutil::BasarDifferentContainersIteratorException		DifferentContainersIteratorException;

/*! \brief  A typedef representing the common basar exception class for invalid iterators.*/
typedef cmnutil::BasarInvalidIteratorException					InvalidIteratorException;

/*! \brief  A typedef representing the common basar exception class for iterators out of range.*/
typedef cmnutil::BasarOutOfRangeIteratorException				OutOfRangeIteratorException;

/*! \brief  A typedef representing the common basar exception class for iterators out of range.*/
typedef cmnutil::BasarAssertionException						AssertionException;

/*! \brief common basar exception class; thrown if functionality is not implemented */
typedef cmnutil::BasarNotImplementedException                   NotImplementedException;

/*! \brief common basar exception class; thrown if operation performed on incompatible type */
typedef cmnutil::BasarTypeMismatchException                     TypeMismatchException;

/*! \brief common basar exception class; thrown if operation performed DateTime object with incompatible Range, e.g. compairing DateTime with Year to Day with DateTime Hour to Second */
typedef cmnutil::BasarIncompatibleRangeException				IncompatibleRangeException;

} // namespace basar

#endif
