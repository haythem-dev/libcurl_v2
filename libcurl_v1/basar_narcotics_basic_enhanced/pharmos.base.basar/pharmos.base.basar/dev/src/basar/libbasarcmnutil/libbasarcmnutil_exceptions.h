//----------------------------------------------------------------------------
/*! \file
 *  \brief  main exception class definitions
 *  \author Thomas Hörath
 *  \date   18.07.2005
 */
//----------------------------------------------------------------------------

#ifndef GUARD_LIBBASARCMNUTIL_EXCEPTIONS_H
#define GUARD_LIBBASARCMNUTIL_EXCEPTIONS_H

#ifndef LIBBASARCMNUTIL_API
	#ifdef _WIN32
		#ifdef LIBBASARCMNUTIL_EXPORTS
			#define LIBBASARCMNUTIL_API __declspec(dllexport)	//!< dll exported
		#else
			#define LIBBASARCMNUTIL_API __declspec(dllimport)	//!< dll imported
		#endif 
	#else 
		#define LIBBASARCMNUTIL_API
	#endif
#endif

//----------------------------------------------------------------------------
#include "libbasar_definitions.h"
#include "libbasarcmnutil_bstring.h"

//----------------------------------------------------------------------------
namespace basar	  {
namespace cmnutil {

//----------------------------------------------------------------------------
/*! \ingroup DIFF_DBG_REL */ 
#ifndef NDEBUG
//! \brief macro for throwing an exception 
//! \see assertion
#define ASSERTION(expr,context,msg) basar::cmnutil::assertion( (expr)   , \
															   #expr    , \
															   (context), \
															   (msg)    , \
															   __FILE__ , \
															   __LINE__ )
#else
#define ASSERTION(expr,context,msg) ((void)0)
#endif

//----------------------------------------------------------------------------
/*! \brief throw BasarAssertionException dependent on evaluated expression */
LIBBASARCMNUTIL_API void assertion (const bool        expr,			//!< evaluated expression that is ensured
									const VarString & exprString,	//!< expression as string
									const VarString & context,		//!< hierarchical context where exception is thrown
									const VarString & message,		//!< reason why exception is thrown
									const VarString & file,			//!< source code file where assertion is evaluated
									const Int32       line			//!< source code line assertion is evaluated
								   );

//----------------------------------------------------------------------------
/*! \brief structure to set output-info for exceptions */
struct ExceptionInfoStruct
{
	/*! \brief standard construction of info-struct 
	\n	   no-throw */
	ExceptionInfoStruct() : exceptionType(""), 
		                    context      (""), 
							reason       (""), 
							file         (""), 
							line         (0)
	{
	}
	
	/*! \brief inline construction of info-struct 
	\n	   no-throw */
	LIBBASARCMNUTIL_API ExceptionInfoStruct ( const VarString & ctx			//!< hierarchical context where exception is thrown
											, const VarString & rea			//!< reason why exception is thrown
											, const VarString & fi   = ""   //!< source code file where exception is thrown
											, const UInt32      li   = 0	//!< line of source code file where exception is thrown, default 0 if not set
											) : context(ctx), 
											    reason (rea),
												file   (fi ),
												line   (li ) 
	{
	}
	
	VarString	exceptionType;	//!< type of exception which is thrown
	VarString	context;		//!< namespace.class.method : hierarchical context where exception is thrown
	VarString	reason;			//!< reason why exception is thrown
	VarString 	file;			//!< source code file where exception is thrown
	UInt32		line;			//!< line of source code file where exception is thrown 
};


//----------------------------------------------------------------------------
/*! \brief structure to set info for stacktraces within exceptions */
struct StackTraceElement
{
	/*! \brief standard construction of info-struct 
		\throw no-throw */
	StackTraceElement() 
		:	context( "" ), 
			file   ( "" ), 
			line   ( 0 )
	{
	}
	
	/*! \brief inline construction of info-struct 
		\throw no-throw */
	LIBBASARCMNUTIL_API StackTraceElement( 
		const VarString & ctx,			//!< hierarchical context where exception is re-thrown
		const VarString & fi = "",		//!< source code file where exception is re-thrown
		const UInt32      li = 0		//!< line of source code file where exception is re-thrown, default 0 if not set
			) 
		:	context( ctx ), 
			file   ( fi ),
			line   ( li ) 
	{
	}
	
	VarString	context;		//!< hierarchical context where exception is re-thrown
	VarString 	file;			//!< source code file where exception is re-thrown
	UInt32		line;			//!< line of source code file where exception is re-thrown, default 0 if not set 
};

/*! \brief vector to store stacktrace infos */
typedef std::vector<StackTraceElement> StackTrace;

//----------------------------------------------------------------------------
/*! \brief The main exception class for basar.

	Basic class. All exception classes in basar should be derived from this common exception class!	
	So all basar-exceptions can be catched externally by catching this main basar exception class! */
class BasarException
{

public:
    //! construct exception object, initialize text description string and extend reason
	LIBBASARCMNUTIL_API								BasarException	(const ExceptionInfoStruct & sInfo,
																	 const VarString           & exceptionType,
																	 const VarString           & addReason = "");
	
	/*! \brief This destructor supports inheritance. 
		\n	   no-throw */
	LIBBASARCMNUTIL_API virtual					   ~BasarException	( );
	
	/*! \brief Method returning exception-string. 
		\n	   no-throw */
	LIBBASARCMNUTIL_API virtual const VarString &	what			( ) const;

	/*! \brief Method to fill in stacktrace message. \n
		see also \ref stackTrace, \ref getStackTrace \n
		\throw no-throw \n
		\code
		// how to use
		void MyClass::MyFunction()
		{
			try
			{
				// do something silly
				...
			}
			catch( basar::BasarException & e )
			{
				e.fillInStackTrace( "MyClass::MyFunction()", __FILE__, __LINE__ );
				throw;
			}
		}	

		// anywhere else
		try
		{
			MyClass::MyFunction();
		}
		catch( basar::BasarException & e )
		{
			BLOG_ERROR( logger, e.stackTrace() );
		}
		\endcode
		*/
	LIBBASARCMNUTIL_API	              void			fillInStackTrace( const VarString & context,	//!< context to indentify stacktrace position
																	  const VarString & file = "",	//!< source code file where exception is thrown
																	  const UInt32      line = 0	//!< line of source code file where exception is thrown, default 0 if not set
																		  );

	/*! \brief Method to fill in stacktrace message. \n
		see also \ref stackTrace, \ref getStackTrace \n
		\throw no-throw */
	LIBBASARCMNUTIL_API	              void			fillInStackTrace( const StackTraceElement & elem	//!< context to indentify stack trace position
																		  );

	/*! \brief Method returning stacktrace-string. \n
		see also \ref fillInStackTrace, \ref getStackTrace \n
		\throw no-throw */
	LIBBASARCMNUTIL_API	        const VarString		stackTrace		( ) const;

	/*! \brief Method returning stacktrace. \n
		see also \ref fillInStackTrace, \ref stackTrace \n
		\throw no-throw */
	LIBBASARCMNUTIL_API         const StackTrace &	getStackTrace	( ) const;

protected:
	ExceptionInfoStruct						m_InfoStruct;	//!< information struct, converted to m_What when what() is called

private:
	//! constructs exception object; text description string is set on default
													BasarException	( );

	mutable VarString						m_What;			//!< text description of exception
	StackTrace								m_StackTrace;	//!< contains stacktrace entries
};

//----------------------------------------------------------------------------
/*! \brief common basar exception class; to throw if any index is out of range

	This is a final class. */
class BasarOutOfRangeIndexException : public BasarException
{
public:
	//! constructs exception object and initialises text description string
	LIBBASARCMNUTIL_API	BasarOutOfRangeIndexException		(const ExceptionInfoStruct& sInfo);
private:
						BasarOutOfRangeIndexException		();	//!< forbidden
};

//----------------------------------------------------------------------------
/*! \brief common basar exception class; thrown to report errors presumably detectable only when the program executes.

	final class. */
class BasarRuntimeException : public BasarException
{
public:
	//! constructs exception object and initialises text description string
	LIBBASARCMNUTIL_API	BasarRuntimeException		(const ExceptionInfoStruct& sInfo);

private:
						BasarRuntimeException		();	//!< forbidden
};

//----------------------------------------------------------------------------
/*! \brief common basar exception class; to throw if a string is not set properly

	This is a final class. */
class BasarNotTerminatedException : public BasarException
{
public:
	//! constructs exception object and initialises text description string
	LIBBASARCMNUTIL_API	BasarNotTerminatedException		(const ExceptionInfoStruct& sInfo);
private:
						BasarNotTerminatedException		();	//!< forbidden
};

//----------------------------------------------------------------------------
/*! \brief common basar exception class; to throw if any null reference occurs

	This is a final class. */
class BasarNullReferenceException : public BasarException
{
public:
	//! constructs exception object and initialises text description string
	LIBBASARCMNUTIL_API	BasarNullReferenceException		(const ExceptionInfoStruct& sInfo);
private:
						BasarNullReferenceException		();	//!< forbidden
};

//----------------------------------------------------------------------------
/*! \brief common basar exception class; to throw if any invalid state occurs

	This is a final class. */
class BasarInvalidStateException : public BasarException
{
public:
	//! constructs exception object and initialises text description string
	LIBBASARCMNUTIL_API	BasarInvalidStateException		(const ExceptionInfoStruct& sInfo);
private:
						BasarInvalidStateException		();	//!< forbidden
};

//----------------------------------------------------------------------------
/*! \brief common basar exception class; to throw if any property type is unknown

	This is a final class. */
class BasarUnknownPropertyTypeException : public BasarException
{
public:
	//! constructs exception object and initialises text description string
	LIBBASARCMNUTIL_API	BasarUnknownPropertyTypeException	(const ExceptionInfoStruct& sInfo);
private:
						BasarUnknownPropertyTypeException	();	//!< forbidden
};

//----------------------------------------------------------------------------
/*! \brief common basar exception class; to throw if any property name is unknown

	This is a final class. */
class BasarUnknownPropertyNameException : public BasarException
{
public:
	//! constructs exception object and initialises text description string
	LIBBASARCMNUTIL_API	BasarUnknownPropertyNameException	(const ExceptionInfoStruct& sInfo);
private:
						BasarUnknownPropertyNameException	();	//!< forbidden
};

//----------------------------------------------------------------------------
/*! \brief common basar exception class; to throw if any parameter is invalid

	This is a final class. */
class BasarInvalidParameterException : public BasarException
{
public:
	//! constructs exception object and initialises text description string
	LIBBASARCMNUTIL_API	BasarInvalidParameterException	(const ExceptionInfoStruct& sInfo);
private:
						BasarInvalidParameterException	();	//!< forbidden
};

//----------------------------------------------------------------------------
/*! \brief common basar exception class for iterator exceptions

	Basic class. All exception classes with iterator context in basar should be derived from this!	*/
class BasarIteratorException : public BasarException
{
public:
	//! constructs exception object and initialises text description string
	LIBBASARCMNUTIL_API	BasarIteratorException	(const ExceptionInfoStruct & sInfo);
	LIBBASARCMNUTIL_API	BasarIteratorException	(const ExceptionInfoStruct & sInfo,
												 const VarString           & exceptionType);
private:
						BasarIteratorException	();	//!< forbidden
};

//----------------------------------------------------------------------------
/*! \brief common basar exception class; to throw if iterator's containers don't match

	This is a final class. */
class BasarDifferentContainersIteratorException : public BasarIteratorException
{
public:
	//! constructs exception object and initialises text description string
	LIBBASARCMNUTIL_API	BasarDifferentContainersIteratorException	(const ExceptionInfoStruct& sInfo);
private:
						BasarDifferentContainersIteratorException	();	//!< forbidden
};

//----------------------------------------------------------------------------
/*! \brief common basar exception class; to throw if any iterator is invalid

	This is a final class. */
class BasarInvalidIteratorException : public BasarIteratorException
{
public:
	//! constructs exception object and initialises text description string
	LIBBASARCMNUTIL_API	BasarInvalidIteratorException	(const ExceptionInfoStruct& sInfo);
private:
						BasarInvalidIteratorException	();	//!< forbidden
};

//----------------------------------------------------------------------------
/*! \brief common basar exception class; to throw if any iterator is out of range

	This is a final class. */
class BasarOutOfRangeIteratorException : public BasarIteratorException
{
public:
	//! constructs exception object and initialises text description string
	LIBBASARCMNUTIL_API	BasarOutOfRangeIteratorException	(const ExceptionInfoStruct& sInfo);
private:
						BasarOutOfRangeIteratorException	();	//!< forbidden
};

//----------------------------------------------------------------------------
/*! \brief exception used for development purposes. Thrown in context of assertion. 
    \see   assertion */
class BasarAssertionException : public BasarException
{
public:
	/*! \brief c'tor */
	LIBBASARCMNUTIL_API BasarAssertionException	(const ExceptionInfoStruct & sInfo);

private:
						BasarAssertionException	();	//!< forbidden				
};

//----------------------------------------------------------------------------
/*! \brief common basar exception class; thrown if functionality is not implemented */
class BasarNotImplementedException : public BasarException
{
public:
	/*! \brief c'tor */
	LIBBASARCMNUTIL_API BasarNotImplementedException	(const ExceptionInfoStruct & sInfo);

private:
						BasarNotImplementedException	();	//!< forbidden				
};

//----------------------------------------------------------------------------
/*! \brief common basar exception class; thrown if operation performed on incompatible type */
class BasarTypeMismatchException : public BasarException
{
public:
	/*! \brief c'tor */
	LIBBASARCMNUTIL_API BasarTypeMismatchException	(const ExceptionInfoStruct & sInfo);

private:
						BasarTypeMismatchException	();	//!< forbidden				
};

//----------------------------------------------------------------------------
/*! \brief common basar exception class; thrown if operation performed DateTime object with incompatible Range, e.g. compairing DateTime with Year to Day with DateTime Hour to Second */
class BasarIncompatibleRangeException : public BasarException
{
public:
	/*! \brief c'tor */
	LIBBASARCMNUTIL_API BasarIncompatibleRangeException	(const ExceptionInfoStruct & sInfo);

private:
						BasarIncompatibleRangeException	();	//!< forbidden				
};

//----------------------------------------------------------------------------
/*! \brief common basar exception class; thrown if requested file was found to be not accessable, e.g. not present, not readable, not writeable etc. */
class BasarFileNotAccessibleException : public BasarException
{
public:
	/*! \brief c'tor */
	LIBBASARCMNUTIL_API BasarFileNotAccessibleException	(const ExceptionInfoStruct & sInfo);

private:
						BasarFileNotAccessibleException	();	//!< forbidden				
};
//----------------------------------------------------------------------------
}	// namespace cmnutil
}	// namespace basar

//----------------------------------------------------------------------------
#endif	// GUARD
