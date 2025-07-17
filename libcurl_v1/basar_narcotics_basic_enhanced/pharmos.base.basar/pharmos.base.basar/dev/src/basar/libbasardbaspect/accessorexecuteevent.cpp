/*!                                                  
* \file accessorexecuteevent.cpp                                               
* \brief ...                                       
* \author Anke Klink
*/                                                  
//--------------------------------------------------------------------------------------------------

#include "libbasardbaspect_accessorexecutelistener.h"

//--------------------------------------------------------------------------------------------------
namespace basar  {
namespace db     {
namespace aspect {

//--------------------------------------------------------------------------------------------------
AccessorExecuteEvent::AccessorExecuteEvent(const AccessMethodName                & name, 
										   const AccessorPropertyTable_YIterator & yit)
										  : m_methodName(name),
										    m_yIterator(yit)
{
}

//--------------------------------------------------------------------------------------------------
}
}
}

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
