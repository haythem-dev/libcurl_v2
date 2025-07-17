 /** $Id$                                          
  *                                                  
  * @file                                            
  * @author Anke Klink                      
  * @date 2008                            
  */    

#include <UnitTest++/UnitTest++.h>

#include "libbasarcmnutil.h"
#include "libbasarqtwidget.h"
#include "loggerpool.h"


namespace basar		{
namespace test		{
namespace unit		{
namespace qtwidget	{
	
	log4cplus::Logger LoggerPool::loggerTest = basar::cmnutil::Logging::getInstance("utqtwidget");

}
}
}
}

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
