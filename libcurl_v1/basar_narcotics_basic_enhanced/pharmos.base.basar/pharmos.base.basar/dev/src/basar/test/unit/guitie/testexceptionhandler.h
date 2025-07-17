/** $Id$                                          
*                                                  
* @file                                            
* @author Anke Klink                      
* @date 2009                            
*/    

#ifndef  __BASAR_TEST_UNIT_GUITIE_TESTEXCEPTIONHANDLER_H__ 
#define  __BASAR_TEST_UNIT_GUITIE_TESTEXCEPTIONHANDLER_H__ 

#include "libbasarguitie_exceptionhandler.h"

namespace basar
{
	namespace test
	{
		namespace unit
		{
			namespace guitie
			{
				/**
				* Class: testexceptionhandler 
				*  
				*/ 
				class TestExceptionHandler : public basar::gui::tie::ExceptionHandler
				{      
				public:
					int m_timesBasarException;
					int m_timesSTDException;
					int m_timesUnknownException;
					bool m_showException;

					TestExceptionHandler(bool show = false);
					virtual ~TestExceptionHandler();


					bool handleException(const basar::Exception & ex, const  basar::gui::tie::ExceptionFoundInfo & found, bool defretval) ;
					bool handleException(const std::exception & ex, const  basar::gui::tie::ExceptionFoundInfo & found, bool defretval) ;
					bool handleUnknownException(const  basar::gui::tie::ExceptionFoundInfo & found, bool defretval)  ;

					bool checkNoExceptionOccurred();
				}; 

		

			}
		}
	}
}
#endif 

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
