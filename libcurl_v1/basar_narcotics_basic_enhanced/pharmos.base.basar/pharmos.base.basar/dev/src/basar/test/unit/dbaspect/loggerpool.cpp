 /** $Id$                                          
  *                                                  
  * @file                                            
  * @author Anke Klink                      
  * @date 2008                            
  */    

#include <UnitTest++/UnitTest++.h>
#include "libbasardbaspect.h"

#include "loggerpool.h"


 namespace basar
  {
   namespace test
    {
     namespace unit
      {
       namespace dbaspect
        {
			log4cplus::Logger LoggerPool::loggerTest = basar::cmnutil::Logging::getInstance("utdbaspect");
		}
      }
    }
  }
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
