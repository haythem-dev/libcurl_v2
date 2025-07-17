 /** $Id$                                          
  *                                                  
  * @file                                            
  * @author Anke Klink                      
  * @date 2008                            
  */    
#include <UnitTest++/UnitTest++.h>

#include "libbasarcmnutil.h"
#include "libbasarqtwidget.h"

#include "libbasarqtwidget_integersignalemitter.h"
//------------------------------------------------------------------------------
SUITE(test_utqtwidget_integersignalemitter)
{
  
    //// ----------------------------------------------------------

    TEST (test_utqtwidget_integersignalemitter1)
    {
       int number = 1;
       IntegerSignalEmitter emitter(number,0);
       CHECK_EQUAL(number, emitter.getNumber());
       number = -3214124;
       emitter.setNumber(number);
       CHECK_EQUAL(number, emitter.getNumber());
    }
}
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
