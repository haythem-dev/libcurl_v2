/** $Id$                                          
*                                                  
* @file accessorexecutelistener.cpp                                                                       
* @author Anke Klink                      
* @date 2007                            
*/    
#include <UnitTest++/UnitTest++.h>
#include "libbasardbaspect.h"

#include "testaccessorexecutelistener.h"
#include "testnames.h"

ACCESSOREXECUTELISTENER_FUNCTION(basar::test::unit::dbaspect::TestAccessorExecuteListenerStdOutLog)
{
    std::cout << "test TestAccessorExecuteListener1 - result  Errornumber = " << event.m_executeResult.getError() << std::endl;
}

ACCESSOREXECUTELISTENER_FUNCTION(basar::test::unit::dbaspect::TestAccessorExecuteListenerHoldLastEvent)
{ 
    if (m_lastEvent.get() != 0)
    { 
      m_lastEvent->m_times++;
      m_lastEvent->m_lastEvent = boost::shared_ptr<basar::db::aspect::AccessorExecuteEvent>(new basar::db::aspect::AccessorExecuteEvent(event));
    }
}

basar::test::unit::dbaspect::TestAccessorExecuteListenerHoldLastEvent::TestAccessorExecuteListenerHoldLastEvent()
{
    m_lastEvent = boost::shared_ptr<AccessorExecuteEventHolder>(new AccessorExecuteEventHolder);
    m_lastEvent->m_times = 0;
}

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
