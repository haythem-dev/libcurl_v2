/** $Id$                                          
*                                                  
* @file                                            
* @author Anke Klink                      
* @date 2009                            
*/    


#include <UnitTest++/UnitTest++.h>
#include "libbasarguitie.h"

#include "testexceptionhandler.h"

namespace basar
{
	namespace test
	{
		namespace unit
		{
			namespace guitie
			{
				// -------------------------------------------------------------------------------------------------------------------
				TestExceptionHandler::TestExceptionHandler(bool show)
				{
					m_timesBasarException = 0;
					m_timesSTDException = 0;
					m_timesUnknownException = 0;
					m_showException = show;
				}

				// -------------------------------------------------------------------------------------------------------------------
				TestExceptionHandler::~TestExceptionHandler()
				{
				} 


				// -------------------------------------------------------------------------------------------------------------------
				bool TestExceptionHandler::handleException(
					const basar::Exception & ex , 
					const  basar::gui::tie::ExceptionFoundInfo & found , 
					bool defretval) 
				{
					m_timesBasarException++;
					if (m_showException)
					{
						std::cerr << "Basar Exception in TestExceptionHandler found: " << ex.what() << std::endl
							<< "found in " << found.file << ":" << found.line << " (" << found.context << " )" << std::endl;
					}
					return defretval;
				}
				// -------------------------------------------------------------------------------------------------------------------
				bool TestExceptionHandler::handleException(
					const std::exception & ex, 
					const  basar::gui::tie::ExceptionFoundInfo & found, 
					bool defretval)
				{
					m_timesSTDException++;
					if (m_showException)
					{
						std::cerr << "std Exception in TestExceptionHandler found: " << ex.what() << std::endl
							<< "found in " << found.file << ":" << found.line << " (" << found.context << " )" << std::endl;
					}
					return defretval;

				}

				// -------------------------------------------------------------------------------------------------------------------

				bool TestExceptionHandler::handleUnknownException(
					const  basar::gui::tie::ExceptionFoundInfo & found, 
					bool defretval)  
				{
					m_timesUnknownException++;
					if (m_showException)
					{
						std::cerr << "Unknown Exception in TestExceptionHandler found "<< std::endl
							<< "found in " << found.file << ":" << found.line << " (" << found.context << " )" << std::endl;
					}
					return defretval;

				}

				// -------------------------------------------------------------------------------------------------------------------
					bool TestExceptionHandler::checkNoExceptionOccurred()
					{
						return (
							(0 == m_timesBasarException ) &&
							(0 == m_timesSTDException ) &&
							(0 == m_timesUnknownException )); 
					}


				// -------------------------------------------------------------------------------------------------------------------
			}
		}
	}
}
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
