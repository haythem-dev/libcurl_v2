//---------------------------------------------------------------------------//
/*! \file exceptionfixture.cpp
 *  \brief definition of test fixture for test of exception classes
 *  \author Dirk Kapusta
 *  \date 18.03.2009
 *  \version 00.00.00.01 first version
 */ //-----------------------------------------------------------------------//

#include "libbasarcmnutil.h"
#include "exceptionfixture.h"

using basar::VarString;
using basar::UInt32;
using basar::Int16;

using basar::cmnutil::BasarException;
using basar::cmnutil::BasarRuntimeException;
using basar::cmnutil::ExceptionInfoStruct;

namespace cmnutil_unit_test
{
ExceptionFixture::ExceptionFixture()
{
}

ExceptionFixture::~ExceptionFixture()
{
} 

void ExceptionFixture::throwBasarException( 
	const VarString & exceptionType,
	const VarString & context,
	const VarString & reason,
	const VarString & file,
	const UInt32 line,
	const Int16 depth /*= 1*/
		)
{
	try
	{
		if( 1 < depth )
		{
			// recursion
			this->throwBasarException( exceptionType, context, reason, file, line, depth - 1 );
		}

		throw BasarException( ExceptionInfoStruct( context, reason, file, line ), exceptionType );
	}
	catch( BasarException & e )
	{
		VarString newContext;
		newContext.format( "%ld new context", depth );
		e.fillInStackTrace( newContext, file, line );
		throw;
	}
}

void ExceptionFixture::throwBasarExceptionWithoutStacktrace( 
	const VarString & exceptionType,
	const VarString & context,
	const VarString & reason,
	const VarString & file,
	const UInt32 line
		)
{
	throw BasarException( ExceptionInfoStruct( context, reason, file, line ), exceptionType );
}

void ExceptionFixture::throwBasarRuntimeException( 
	const VarString & context,
	const VarString & reason,
	const VarString & file,
	const UInt32 line,
	const Int16 depth /*= 1*/
		)
{
	try
	{
		if( 1 < depth )
		{
			// recursion
			this->throwBasarRuntimeException( context, reason, file, line, depth - 1 );
		}

		throw BasarRuntimeException( ExceptionInfoStruct( context, reason, file, line ) );
	}
	catch( BasarRuntimeException & e )
	{
		VarString newContext;
		newContext.format( "%ld new context", depth );
		e.fillInStackTrace( newContext, file, line );
		throw;
	}
}

void ExceptionFixture::throwBasarRuntimeExceptionWithoutStacktrace( 
	const VarString & context,
	const VarString & reason,
	const VarString & file,
	const UInt32 line
		)
{
	throw BasarRuntimeException( ExceptionInfoStruct( context, reason, file, line ) );
}

} // namespace cmnutil_unit_test
