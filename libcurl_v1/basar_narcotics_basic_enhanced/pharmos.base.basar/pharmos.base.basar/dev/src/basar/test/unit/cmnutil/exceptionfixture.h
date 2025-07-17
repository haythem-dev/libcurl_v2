//---------------------------------------------------------------------------//
/*! \file exceptionfixture.h
 *  \brief declaration of test fixture for test of exception classes
 *  \author Dirk Kapusta
 *  \date 13.05.2009
 *  \version 00.00.00.01 first version
 */ //-----------------------------------------------------------------------//

#ifndef GUARD_EXCEPTIONFIXTURE_H
#define GUARD_EXCEPTIONFIXTURE_H

#include "testfixturebase.h"

namespace cmnutil_unit_test
{
class ExceptionFixture : public TestFixtureBase
{
public:
	/*! \brief std constructor \n no-throw */
	ExceptionFixture();
	/*! \brief virtual destructor \n no-throw */
	virtual	~ExceptionFixture();

protected:
	/*! \brief throws a BasarException \n \throw BasarException */
	void throwBasarException( 
		const basar::VarString & exceptionType,	//!< type of exception
		const basar::VarString & context,			//!< namespace
		const basar::VarString & reason,			//!< reason
		const basar::VarString & file,				//!< source code file 
		const basar::UInt32      line,				//!< line of source code
		const basar::Int16       depth = 1			//!< recursion depth
			);
	/*! \brief throws a BasarException without a stacktrace \n \throw BasarException */
	void throwBasarExceptionWithoutStacktrace( 
		const basar::VarString & exceptionType,	//!< type of exception
		const basar::VarString & context,			//!< namespace
		const basar::VarString & reason,			//!< reason
		const basar::VarString & file,				//!< source code file 
		const basar::UInt32      line				//!< line of source code
			);
	/*! \brief throws a BasarRuntimeException \n \throw BasarRuntimeException */
	void throwBasarRuntimeException( 
		const basar::VarString & context,			//!< namespace
		const basar::VarString & reason,			//!< reason
		const basar::VarString & file,				//!< source code file 
		const basar::UInt32      line,				//!< line of source code
		const basar::Int16       depth = 1			//!< recursion depth
			);
	/*! \brief throws a BasarRuntimeException without a stacktrace \n \throw BasarRuntimeException */
	void throwBasarRuntimeExceptionWithoutStacktrace( 
		const basar::VarString & context,			//!< namespace
		const basar::VarString & reason,			//!< reason
		const basar::VarString & file,				//!< source code file 
		const basar::UInt32      line				//!< line of source code
			);

private:
	/*! \brief copy constructor \n no-throw */
	ExceptionFixture ( const ExceptionFixture & r );
	/*! \brief assign operator \n no-throw */
	ExceptionFixture operator = ( const ExceptionFixture & r );
};

} // end namespace cmnutil_unit_test

#endif // GUARD_EXCEPTIONFIXTURE_H
