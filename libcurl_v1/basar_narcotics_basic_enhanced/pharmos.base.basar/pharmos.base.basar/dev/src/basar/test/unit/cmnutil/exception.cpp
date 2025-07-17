//---------------------------------------------------------------------------//
/*! \file exception.cpp
 *  \brief test suite for Exception classes
 *  \author Dirk Kapusta
 *  \date 13.05.2009
 *  \version 00.00.00.01 first version
 */ //-----------------------------------------------------------------------//

#include <UnitTest++/UnitTest++.h>
#include "libbasarcmnutil.h"
#include "exceptionfixture.h"

//----------------------------------------------------------------------------
using basar::ConstString;
using basar::cmnutil::BasarException;
using basar::cmnutil::BasarRuntimeException;
using basar::cmnutil::ExceptionInfoStruct;
using basar::cmnutil::StackTrace;
using basar::cmnutil::StackTraceElement;
using basar::cmnutil::BasarAssertionException;
using basar::cmnutil::assertion;

//----------------------------------------------------------------------------
namespace cmnutil_unit_test
{

using basar::VarString;
using basar::UInt32;
using basar::Int16;

//----------------------------------------------------------------------------
SUITE( Exception )
{

//----------------------------------------------------------------------------
TEST_FIXTURE( ExceptionFixture, construction )
{
	VarString exceptionType( "BasarExceptionTest" );
	VarString context( "ExceptionFixture::stacktrace" );
	VarString reason( "test the construction" );
	VarString file( __FILE__ );
	UInt32 line = __LINE__;

	VarString compare;
	compare.format( 
		"exception: %s\n"
		"context  : %s\n" 
		"reason   : %s\n"
		"file     : %s\n"
		"line     : %ld", 
		exceptionType.c_str(),
		context.c_str(),
		reason.c_str(),
		file.c_str(),
		line 
			);

	CHECK_THROW( this->throwBasarExceptionWithoutStacktrace( exceptionType, context, reason, file, line ), BasarException );

	try
	{
		this->throwBasarExceptionWithoutStacktrace( exceptionType, context, reason, file, line );
	}
	catch( BasarException & e )
	{
		VarString temp( compare );
		CHECK_EQUAL( temp.c_str(), e.what().c_str() );

		temp.append( "\nstacktrace:\nNo stacktrace available! Use BasarException::fillInStackTrace() to create one.\n" );
		CHECK_EQUAL( temp.c_str(), e.stackTrace().c_str() );
		
		const StackTrace & st = e.getStackTrace();
		CHECK( st.empty() );
	}
}

//----------------------------------------------------------------------------
TEST_FIXTURE( ExceptionFixture, stacktrace )
{
	VarString exceptionType( "BasarExceptionTest" );
	VarString context( "ExceptionFixture::stacktrace" );
	VarString reason( "test the stacktrace" );
	VarString file( __FILE__ );
	UInt32 line = __LINE__;
	Int16 depth = 1;

	VarString compare;
	compare.format( 
		"exception: %s\n"
		"context  : %s\n" 
		"reason   : %s\n"
		"file     : %s\n"
		"line     : %ld\nstacktrace:\n", 
		exceptionType.c_str(),
		context.c_str(),
		reason.c_str(),
		file.c_str(),
		line 
			);

	try
	{
		this->throwBasarException( exceptionType, context, reason, file, line, depth );
	}
	catch( BasarException & e )
	{
		VarString temp( compare );
		VarString stack;
		stack.format( "%ld new context in file: %s, line: %ld\n", depth, file.c_str(), line );
		temp.append( stack );

		CHECK_EQUAL( temp.c_str(), e.stackTrace().c_str() );
	}

	depth = 3;
	try
	{
		this->throwBasarException( exceptionType, context, reason, file, line, depth );
	}
	catch( BasarException & e )
	{
		VarString temp( compare );
		VarString stack;
		Int16 i;
		for( i = 0; i < depth; i++ )
		{
			stack.format( "%ld new context in file: %s, line: %ld\n", i + 1, file.c_str(), line );
			temp.append( stack );
		}

		CHECK_EQUAL( temp.c_str(), e.stackTrace().c_str() );

		i = 0;
		const StackTrace & st = e.getStackTrace();
		basar::cmnutil::StackTrace::const_iterator iter = st.begin();
		while( iter != st.end() )
		{
			const StackTraceElement & elem = *iter;
			stack.format( "%ld new context", ++i );
			CHECK_EQUAL( stack.c_str(),		 elem.context.c_str() );
			CHECK_EQUAL( file.c_str(),		 elem.file.c_str() );
			CHECK_EQUAL( line,				 elem.line );
			++iter;
		}
	
		CHECK_EQUAL( depth, i );
	}
}

//----------------------------------------------------------------------------
TEST_FIXTURE( ExceptionFixture, derived )
{
	VarString exceptionType( "RuntimeException" );
	VarString context( "ExceptionFixture::stacktrace" );
	VarString reason( "test the derived classes" );
	VarString file( __FILE__ );
	UInt32 line = __LINE__;

	VarString compare;
	compare.format( 
		"exception: %s\n"
		"context  : %s\n" 
		"reason   : %s\n"
		"file     : %s\n"
		"line     : %ld", 
		exceptionType.c_str(),
		context.c_str(),
		reason.c_str(),
		file.c_str(),
		line 
			);

	try
	{
		this->throwBasarRuntimeExceptionWithoutStacktrace( context, reason, file, line );
	}
	catch( BasarRuntimeException & e )
	{
		VarString temp( compare );
		CHECK_EQUAL( temp.c_str(), e.what().c_str() );

		temp.append( "\nstacktrace:\nNo stacktrace available! Use BasarException::fillInStackTrace() to create one.\n" );
		CHECK_EQUAL( temp.c_str(), e.stackTrace().c_str() );
		
		const StackTrace & st = e.getStackTrace();
		CHECK( st.empty() );
	}

	try
	{
		this->throwBasarRuntimeExceptionWithoutStacktrace( context, reason, file, line );
	}
	catch( BasarException & e )
	{
		VarString temp( compare );
		CHECK_EQUAL( temp.c_str(), e.what().c_str() );

		temp.append( "\nstacktrace:\nNo stacktrace available! Use BasarException::fillInStackTrace() to create one.\n" );
		CHECK_EQUAL( temp.c_str(), e.stackTrace().c_str() );
		
		const StackTrace & st = e.getStackTrace();
		CHECK( st.empty() );
	}

	Int16 depth = 3;
	try
	{
		this->throwBasarRuntimeException( context, reason, file, line, depth );
	}
	catch( BasarRuntimeException & e )
	{
		VarString temp( compare );
		temp.append( "\nstacktrace:\n" );
		VarString stack;
		Int16 i;
		for( i = 0; i < depth; i++ )
		{
			stack.format( "%ld new context in file: %s, line: %ld\n", i + 1, file.c_str(), line );
			temp.append( stack );
		}

		CHECK_EQUAL( temp.c_str(), e.stackTrace().c_str() );

		i = 0;
		const StackTrace & st = e.getStackTrace();
		basar::cmnutil::StackTrace::const_iterator iter = st.begin();
		while( iter != st.end() )
		{
			const StackTraceElement & elem = *iter;
			stack.format( "%ld new context", ++i );
			CHECK_EQUAL( stack.c_str(),		 elem.context.c_str() );
			CHECK_EQUAL( file.c_str(),		 elem.file.c_str() );
			CHECK_EQUAL( line,				 elem.line );
			++iter;
		}
	
		CHECK_EQUAL( depth, i );
	}
}

//----------------------------------------------------------------------------
TEST(BasarAssertionException)
{
	ExceptionInfoStruct     info("context", "reason", "file", 333);

	ConstString expected = 
		"exception: AssertionException\n"
		"context  : context\n"
		"reason   : reason\n"
		"file     : file\n"
		"line     : 333";

	{
		BasarAssertionException ex(info);
		CHECK_EQUAL(expected, ex.what().c_str());
	}

	{
		basar::AssertionException ex(info);
		CHECK_EQUAL(expected, ex.what().c_str());
	}
}

//----------------------------------------------------------------------------
TEST(assertion)
{
	assertion(true , "trueString" , "context", "message", "file", 333);

	CHECK_THROW(assertion(false, "falseString", "context", "message", "file", 333), 
		        BasarAssertionException);


	bool caught = false;

	try
	{
		assertion(false, "falseString", "context", "message", "file", 333);
	}
	catch (const BasarAssertionException & except)
	{
		caught = true;

		ConstString expected = 
			"exception: AssertionException\n"
			"context  : context\n"
			"reason   : message ( falseString )\n"
			"file     : file\n"
			"line     : 333";
		CHECK_EQUAL(expected, except.what().c_str());
	}

	CHECK(caught);
}

//----------------------------------------------------------------------------
TEST(ASSERTION_MACRO)
{
	ASSERTION(true, "context", "message");

#ifndef NDEBUG
	CHECK_THROW(ASSERTION(false, "context", "message"), BasarAssertionException);
#endif


	bool caught = false;

	try
	{
		ASSERTION(false, "context", "message");
	}
	catch (const BasarAssertionException & except)
	{
		caught = true;

		VarString expected;
		expected.format(
			"exception: AssertionException\n"
			"context  : context\n"
			"reason   : message ( false )\n"
			"file     : %s\n"
			"line     : %d",
			__FILE__,
			__LINE__ - 14);

		CHECK_EQUAL(expected.c_str(), except.what().c_str());
	}

#ifndef NDEBUG
	CHECK(caught);
#else
	CHECK(!caught);
#endif
}

//----------------------------------------------------------------------------
}	// end SUITE

//----------------------------------------------------------------------------
} // end namespace cmnutil_unit_test
