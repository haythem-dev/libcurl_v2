//----------------------------------------------------------------------------
/*! \file
 *  \brief  exception class implementation to derive from
 *  \author Thomas Hörath
 *  \date   18.07.2005
 */
//----------------------------------------------------------------------------

#include "libbasarcmnutil_exceptions.h"
#include <log4cplus/logger.h>
#include "libbasarcmnutil_logging.h"

//----------------------------------------------------------------------------
namespace basar {
namespace cmnutil{

//----------------------------------------------------------------------------
/*!	\throw no-throw  
    \see   ASSERTION
*/
void assertion	(const bool        expr,		
				 const VarString & exprString,
				 const VarString & context,			
				 const VarString & message,			
				 const VarString & file,	
				 const Int32       line)
{
	if ( expr )
		return;		// ok

	VarString reason = message;
	reason.append( " ( " );
	reason.append( exprString );
	reason.append( " )" );

	throw BasarAssertionException( ExceptionInfoStruct( context, reason, file, line ));
}

//----------------------------------------------------------------------------
/*!	\throw no-throw  
	\param sInfo         struct with output-info 
	\param exceptionType type of exception which is thrown
	\param addReason     additional reason, extends m_InfoStruct.reason
*/
BasarException::BasarException ( const ExceptionInfoStruct & sInfo               ,
								 const VarString           & exceptionType       ,
								 const VarString           & addReason /* = "" */)
                               : m_InfoStruct (sInfo)
{
	m_InfoStruct.exceptionType = exceptionType;		// complete info struct

	if (!addReason.empty())
	{
		m_InfoStruct.reason += "\n";
		m_InfoStruct.reason += addReason;
	}

	log4cplus::Logger logger = Logging::getInstance(m_InfoStruct.context);

	if (logger.isEnabledFor(log4cplus::ERROR_LOG_LEVEL)) 
	{
		VarString msg = m_InfoStruct.exceptionType;
		msg += ": ";
		msg += m_InfoStruct.reason;

		logger.forcedLog(log4cplus::ERROR_LOG_LEVEL, 
			             msg         .c_str(), 
			             m_InfoStruct.file.c_str(), 
						 m_InfoStruct.line);
	}
}

//----------------------------------------------------------------------------
/*!	\throw no-throw  */
BasarException::~BasarException()
{
}

//----------------------------------------------------------------------------
/*!	\throw no-throw 
	\return exception string, representing error in basar */
const VarString & BasarException::what( ) const
{
	m_What.format("exception: %s\n"
		          "context  : %s\n" 
				  "reason   : %s\n"
				  "file     : %s\n"
				  "line     : %ld", 
		          m_InfoStruct.exceptionType.c_str(),
				  m_InfoStruct.context      .c_str(),
				  m_InfoStruct.reason       .c_str(),
	  			  m_InfoStruct.file         .c_str(),
		          m_InfoStruct.line);
	

	return m_What;
}

//----------------------------------------------------------------------------
/*!	\throw no-throw */
void BasarException::fillInStackTrace( 
	const StackTraceElement & elem
		)
{
	m_StackTrace.push_back( elem );
}

//----------------------------------------------------------------------------
/*!	\throw no-throw */
void BasarException::fillInStackTrace( 
	const VarString & context,
	const VarString & file /*= ""*/,
	const UInt32      line /*= 0*/
		)
{
	this->fillInStackTrace( StackTraceElement( context, file, line ) );
}

//----------------------------------------------------------------------------
/*!	\throw no-throw 
	\return stacktrace string */
const VarString BasarException::stackTrace() const
{
	VarString result;
	VarString temp;
	result.format( "exception: %s\n"
			       "context  : %s\n" 
				   "reason   : %s\n"
				   "file     : %s\n"
				   "line     : %ld\nstacktrace:\n", 
		           m_InfoStruct.exceptionType.c_str(),
				   m_InfoStruct.context.c_str(),
				   m_InfoStruct.reason.c_str(),
	  			   m_InfoStruct.file.c_str(),
		           m_InfoStruct.line 
						);
	Int32 count = 0;
	StackTrace::const_iterator iter = m_StackTrace.begin();
	while( iter != m_StackTrace.end() )
	{
		const StackTraceElement & elem = *iter;
		temp.format( "%s in file: %s, line: %ld\n",
					 elem.context.c_str(),
					 elem.file.c_str(),
					 elem.line 
						);
		result.append( temp );
		++iter;
		count++;
	}

	if( 0 == count )
	{
		result.append( "No stacktrace available! Use BasarException::fillInStackTrace() to create one.\n" );
	}

	return result;
}

//----------------------------------------------------------------------------
/*!	\throw no-throw 
	\return collection containing stacktrace infos */
const StackTrace & BasarException::getStackTrace( ) const
{
	return m_StackTrace;
}


//----------------------------------------------------------------------------
/*! \throw  no-throw 
	\param sInfo         struct with output-info */
BasarRuntimeException::BasarRuntimeException ( const ExceptionInfoStruct & sInfo)
											 : BasarException(sInfo, "RuntimeException")
{
}


//----------------------------------------------------------------------------
/*! \throw  no-throw 
	\param sInfo         struct with output-info */
BasarOutOfRangeIndexException::BasarOutOfRangeIndexException (const ExceptionInfoStruct & sInfo)
															 : BasarException(sInfo, "OutOfRangeIndexException")
{
}


//----------------------------------------------------------------------------
/*! \throw  no-throw 
	\param sInfo         struct with output-info */
BasarNotTerminatedException::BasarNotTerminatedException (const ExceptionInfoStruct & sInfo)
														 : BasarException(sInfo, "NotTerminatedException")
{
}

//----------------------------------------------------------------------------
/*! \throw  no-throw 
	\param sInfo struct with output-info */
BasarNullReferenceException::BasarNullReferenceException ( const ExceptionInfoStruct& sInfo )
														 : BasarException(sInfo, "NullReferenceException")
{
}

//----------------------------------------------------------------------------
/*! \throw  no-throw 
	\param sInfo struct with output-info */
BasarInvalidStateException::BasarInvalidStateException  ( const ExceptionInfoStruct& sInfo )
														: BasarException(sInfo, "InvalidStateException")
{
}

//----------------------------------------------------------------------------
/*! \throw  no-throw 
	\param sInfo struct with output-info */
BasarUnknownPropertyTypeException::BasarUnknownPropertyTypeException (const ExceptionInfoStruct& sInfo )
														             : BasarException(sInfo, "UnknownPropertyTypeException")
{
}

//----------------------------------------------------------------------------
/*! \throw  no-throw 
	\param sInfo struct with output-info */
BasarUnknownPropertyNameException::BasarUnknownPropertyNameException (const ExceptionInfoStruct& sInfo )
														             : BasarException(sInfo, "UnknownPropertyNameException")
{
}

//----------------------------------------------------------------------------
/*! \throw  no-throw 
	\param sInfo struct with output-info */
BasarInvalidParameterException::BasarInvalidParameterException (const ExceptionInfoStruct& sInfo )
														       : BasarException(sInfo, "InvalidParameterException")
{
}

//----------------------------------------------------------------------------
/*! \throw  no-throw 
	\param sInfo struct with output-info */
BasarIteratorException::BasarIteratorException (const ExceptionInfoStruct& sInfo )
											   : BasarException(sInfo, "IteratorException")
{
}

//----------------------------------------------------------------------------
/*! \throw  no-throw 
	\param exceptionType type of exception which is thrown
	\param sInfo         struct with output-info 
*/
BasarIteratorException::BasarIteratorException (const ExceptionInfoStruct & sInfo,
												const VarString           & exceptionType)
											   : BasarException(sInfo, exceptionType)
{
}

//----------------------------------------------------------------------------
/*! \throw  no-throw 
	\param sInfo struct with output-info */
BasarDifferentContainersIteratorException::BasarDifferentContainersIteratorException (const  ExceptionInfoStruct& sInfo )
                                                                                     : BasarIteratorException(sInfo, "DifferentContainersIteratorException")
{
}

//----------------------------------------------------------------------------
/*! \throw  no-throw 
	\param sInfo struct with output-info */
BasarInvalidIteratorException::BasarInvalidIteratorException (const ExceptionInfoStruct& sInfo )
                                                             : BasarIteratorException(sInfo, "InvalidIteratorException")
{
}

//----------------------------------------------------------------------------
/*! \throw  no-throw 
	\param sInfo struct with output-info */
BasarOutOfRangeIteratorException::BasarOutOfRangeIteratorException (const ExceptionInfoStruct& sInfo )
                                                                   : BasarIteratorException(sInfo, "OutOfRangeIteratorException")
{
}

//----------------------------------------------------------------------------
/*! \throw  no-throw 
	\param sInfo struct with output-info */
BasarAssertionException::BasarAssertionException (const ExceptionInfoStruct& sInfo )
                                                 : BasarException(sInfo, "AssertionException")
{
}

//----------------------------------------------------------------------------
/*! \throw  no-throw 
	\param sInfo struct with output-info */
BasarNotImplementedException::BasarNotImplementedException (const ExceptionInfoStruct& sInfo )
                                                 : BasarException(sInfo, "NotImplementedException")
{
}

//----------------------------------------------------------------------------
/*! \throw  no-throw 
	\param sInfo struct with output-info */
BasarTypeMismatchException::BasarTypeMismatchException (const ExceptionInfoStruct& sInfo )
                                                 : BasarException(sInfo, "TypeMismatchException")
{
}

//----------------------------------------------------------------------------
/*! \throw  no-throw 
	\param sInfo struct with output-info */
BasarIncompatibleRangeException::BasarIncompatibleRangeException (const ExceptionInfoStruct& sInfo )
                                                 : BasarException(sInfo, "IncompatibleRangeException")
{
}

//----------------------------------------------------------------------------
/*! \throw  no-throw 
	\param sInfo struct with output-info */
BasarFileNotAccessibleException::BasarFileNotAccessibleException (const ExceptionInfoStruct& sInfo )
                                                 : BasarException(sInfo, "BasarFileNotAccessibleException")
{
}

//----------------------------------------------------------------------------
}//cmnutil
}//basar
