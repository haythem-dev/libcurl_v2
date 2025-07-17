 /** $Id$                                          
  *                                                  
  * @file                                            
  * @author Anke Klink                      
  * @date 2008                            
  */    
                                  
#ifndef  __BASAR_TEST_UNIT_DBASPECT_LOGGERPOOL_H__ 
#define  __BASAR_TEST_UNIT_DBASPECT_LOGGERPOOL_H__ 


namespace basar
{
	namespace test
	{
		namespace unit
		{
			namespace dbaspect
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
//#define SHOW_ON_CERR(X) std::cerr << X << std::endl; 

#define TEXTOUTPUT(X) \
	if (basar::test::unit::dbaspect::LoggerPool::loggerTest.isEnabledFor(log4cplus::TRACE_LOG_LEVEL)) \
	{  \
	std::ostringstream str; \
	str << X; \
	VarString msg(str.str()); \
	str.flush();  \
	basar::test::unit::dbaspect::LoggerPool::loggerTest.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__); \
	} \
	/* std::cout << X << std::endl; */  \

#define TEXTOUTPUT2(X) \
	if (basar::test::unit::dbaspect::LoggerPool::loggerTest.isEnabledFor(log4cplus::DEBUG_LOG_LEVEL)) \
	{  \
	std::ostringstream str; \
	str << X; \
	VarString msg(str.str()); \
	str.flush();  \
	basar::test::unit::dbaspect::LoggerPool::loggerTest.forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__); \
	} \
	/* std::cout << X << std::endl; */   \

#define TEXTOUTPUT3(X) \
	 /* std::cout << X << std::endl; */  \

#define TEXTOUTPUT4(X) \
	 /* std::cout << X << std::endl; */  \

#define TEXTOUTPUT5(X) \
	 /*std::cout << X << std::endl; */   \

#define TEXTOUTPUT_WARN(X)  \
	if (basar::test::unit::dbaspect::LoggerPool::loggerTest.isEnabledFor(log4cplus::DEBUG_LOG_LEVEL)) \
	{  \
	std::ostringstream str; \
	str << X; \
	VarString msg(str.str()); \
	str.flush();  \
	basar::test::unit::dbaspect::LoggerPool::loggerTest.forcedLog(log4cplus::WARN_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__); \
	} \
    std::cout << X << std::endl; \

 

#endif 

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
