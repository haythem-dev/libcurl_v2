/** $Id$
*
* @file
* @author Anke Klink
* @date 2009
*/


#include <UnitTest++/UnitTest++.h>
#include "libbasarguitie.h"

#include "testexceptionthrower.h"

namespace basar
{
	namespace test
	{
		namespace unit
		{
			namespace guitie
			{
				struct NoException
				{
				};
				// -------------------------------------------------------

				void TestExceptionThrower::throwBasarException(bool a)
				{
					LIBBASARGUITIE_EXCEPTIONHANDLERMACRO_BEGIN
						throw basar::Exception(basar::ExceptInfo("test","test",  __FILE__, __LINE__), "test"); 
	
					LIBBASARGUITIE_EXCEPTIONHANDLERMACRO_END_WITH_VALUE("TestExceptionThrower::throwBasarException",a,a,a)
				}
				// -------------------------------------------------------
				void TestExceptionThrower::throwSTDException(bool a)
				{
					LIBBASARGUITIE_EXCEPTIONHANDLERMACRO_BEGIN
						throw std::exception();
					LIBBASARGUITIE_EXCEPTIONHANDLERMACRO_END_WITH_VALUE("TestExceptionThrower::throwSTDException",a,a,a)
				}
				// -------------------------------------------------------
				void TestExceptionThrower::throwUnknownException(bool a)
				{
					LIBBASARGUITIE_EXCEPTIONHANDLERMACRO_BEGIN
						throw NoException();
					LIBBASARGUITIE_EXCEPTIONHANDLERMACRO_END_WITH_VALUE("TestExceptionThrower::throwUnknownException",a,a,a)
				}
				// -------------------------------------------------------

				void TestExceptionThrower::throwBasarException( )
				{
					LIBBASARGUITIE_EXCEPTIONHANDLERMACRO_BEGIN
						throw basar::Exception(basar::ExceptInfo("test","test",  __FILE__, __LINE__), "test"); 
					LIBBASARGUITIE_EXCEPTIONHANDLERMACRO_END("TestExceptionThrower::throwBasarException")
				}
				// -------------------------------------------------------
				void TestExceptionThrower::throwSTDException( )
				{
					LIBBASARGUITIE_EXCEPTIONHANDLERMACRO_BEGIN
						throw std::exception();
					LIBBASARGUITIE_EXCEPTIONHANDLERMACRO_END("TestExceptionThrower::throwSTDException")
				}
				// -------------------------------------------------------
				void TestExceptionThrower::throwUnknownException()
				{
					LIBBASARGUITIE_EXCEPTIONHANDLERMACRO_BEGIN
						throw NoException();
					LIBBASARGUITIE_EXCEPTIONHANDLERMACRO_END("TestExceptionThrower::throwUnknownException")
				}
				// -------------------------------------------------------

			}
		}
	}
}
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
