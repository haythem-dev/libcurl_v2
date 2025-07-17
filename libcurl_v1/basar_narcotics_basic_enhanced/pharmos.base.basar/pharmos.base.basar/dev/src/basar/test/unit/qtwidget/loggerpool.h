 /*
  * @file                                            
  * @author Anke Klink                      
  * @date 2008                            
  */    
 
                                         
#ifndef  __BASAR_TEST_UNIT_QTWIDGET_LOGGERPOOL_H__ 
#define  __BASAR_TEST_UNIT_QTWIDGET_LOGGERPOOL_H__ 

namespace basar
{
	namespace test
	{
		namespace unit
		{
			namespace qtwidget
			{
				class LoggerPool
				{
				public:	
					//! LoggerInstance for tests
					static log4cplus::Logger loggerTest;
					 
				};
			}
		}
	}
}
#define TEXTOUTPUT(X) \
	if (basar::test::unit::qtwidget::LoggerPool::loggerTest.isEnabledFor(log4cplus::TRACE_LOG_LEVEL)) \
	{  \
	std::ostringstream str; \
	str << X; \
	basar::VarString msg(str.str()); \
	str.flush();  \
	basar::test::unit::qtwidget::LoggerPool::loggerTest.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__); \
	} \
	/* std::cout << "test: " << X << std::endl; \ */

#define TEXTOUTPUT2(X)  \
	if (basar::test::unit::qtwidget::LoggerPool::loggerTest.isEnabledFor(log4cplus::DEBUG_LOG_LEVEL)) \
	{  \
	std::ostringstream str; \
	str << X; \
	basar::VarString msg(str.str()); \
	str.flush();  \
	basar::test::unit::qtwidget::LoggerPool::loggerTest.forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__); \
	} \
	/* std::cout << "test: " << X << std::endl; */ \

#endif 

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
