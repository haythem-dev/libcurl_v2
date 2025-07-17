 /** $Id$                                          
  *                                                  
  * @file                                            
  * @author Anke Klink                      
  * @date 2008                            
  */    


#include <UnitTest++/UnitTest++.h>

#include "libbasarguitie.h"
#include "loggerpool.h"


namespace basar
{
namespace test
{
namespace unit
{
namespace guitie
{
	log4cplus::Logger LoggerPool::loggerTest = basar::cmnutil::Logging::getInstance("utguitie");
	log4cplus::Logger LoggerPool::logPerf    = basar::cmnutil::Logging::getInstance("utguitieperf");
}

}
}
}
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
