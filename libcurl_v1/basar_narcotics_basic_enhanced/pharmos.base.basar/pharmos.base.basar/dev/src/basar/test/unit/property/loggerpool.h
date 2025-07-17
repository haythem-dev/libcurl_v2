 /** $Id$                                          
  *                                                  
  * @file                                            
  * @author Anke Klink                      
  * @date 2008                            
  */    
                                  
#ifndef  __BASAR_TEST_UNIT_PROPERTY_LOGGERPOOL_H__ 
#define  __BASAR_TEST_UNIT_PROPERTY_LOGGERPOOL_H__ 

#include "libbasarcmnutil_definitions.h"

namespace basar
{
	namespace test
	{
		namespace unit
		{
			namespace property
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
	if (basar::test::unit::property::LoggerPool::loggerTest.isEnabledFor(log4cplus::TRACE_LOG_LEVEL)) \
	{  \
	std::ostringstream _str; \
	_str << m_details.testName << ": " << X; \
	basar::VarString _msg(_str.str()); \
	_str.flush();  \
	basar::test::unit::property::LoggerPool::loggerTest.forcedLog(log4cplus::TRACE_LOG_LEVEL, _msg, __FILE__, __LINE__); \
	} 

#define TEXTOUTPUT_INFO(X) \
	if (basar::test::unit::property::LoggerPool::loggerTest.isEnabledFor(log4cplus::INFO_LOG_LEVEL)) \
	{  \
		std::ostringstream _str; \
		_str << m_details.testName << ": " << X; \
		basar::VarString _msg(_str.str()); \
		_str.flush();  \
		basar::test::unit::property::LoggerPool::loggerTest.forcedLog(log4cplus::INFO_LOG_LEVEL, _msg, __FILE__, __LINE__); \
	}


#define TEXTOUTPUT_WARN(X)  \
	if (basar::test::unit::property::LoggerPool::loggerTest.isEnabledFor(log4cplus::WARN_LOG_LEVEL)) \
	{  \
		std::ostringstream _str; \
		_str << m_details.testName << ": " << X; \
		basar::VarString _msg(_str.str()); \
		_str.flush();  \
		basar::test::unit::property::LoggerPool::loggerTest.forcedLog(log4cplus::WARN_LOG_LEVEL, _msg, __FILE__, __LINE__); \
	} \
    std::cerr << X << std::endl; \

 

#endif 

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
