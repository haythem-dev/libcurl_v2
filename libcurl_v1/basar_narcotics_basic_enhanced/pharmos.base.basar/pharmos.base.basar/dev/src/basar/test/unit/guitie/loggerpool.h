 /** $Id$                                          
  *                                                  
  * @file                                            
  * @author Anke Klink                      
  * @date 2008                            
  */    

                                         
#ifndef  __BASAR_TEST_UNIT_GUITIE_LOGGERPOOL_H__ 
#define  __BASAR_TEST_UNIT_GUITIE_LOGGERPOOL_H__ 

namespace basar
{
	namespace test
	{
		namespace unit
		{
			namespace guitie
			{
				class LoggerPool
				{
				public:	
					//! LoggerInstance for tests
					static log4cplus::Logger loggerTest;
					static log4cplus::Logger logPerf;			// measure performance
				};
			}
		}
	}
}
#define TEXTOUTPUT(X) \
	if (basar::test::unit::guitie::LoggerPool::loggerTest.isEnabledFor(log4cplus::TRACE_LOG_LEVEL)) \
	{  \
	std::ostringstream str; \
	str << X; \
	VarString msg(str.str()); \
	str.flush();  \
	basar::test::unit::guitie::LoggerPool::loggerTest.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__); \
	} \


#define TEXTOUTPUT2(X)  \
	if (basar::test::unit::guitie::LoggerPool::loggerTest.isEnabledFor(log4cplus::DEBUG_LOG_LEVEL)) \
	{  \
	std::ostringstream str; \
	str << X; \
	VarString msg(str.str()); \
	str.flush();  \
	basar::test::unit::guitie::LoggerPool::loggerTest.forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__); \
	} \

#endif 

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
