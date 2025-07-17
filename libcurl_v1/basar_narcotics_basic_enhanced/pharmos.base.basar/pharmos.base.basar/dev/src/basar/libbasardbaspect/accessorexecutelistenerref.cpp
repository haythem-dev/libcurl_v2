/** $Id$                                          
*                                                  
* @file accessorexecutelistenerref.cpp                                                
* @brief ...                                       
* @author Anke Klink                      
* @date 2007                            
*/    
//--------------------------------------------------------------------------------------------------

#include "libbasardbaspect_accessorexecutelistener.h"

#include "libbasarcmnutil_exceptions.h"

//--------------------------------------------------------------------------------------------------
namespace basar   {
namespace db      {
namespace aspect  {

// ----------------------------------------------------------
// AccessorExecuteListenerRef
// ----------------------------------------------------------
AccessorExecuteListenerRef::AccessorExecuteListenerRef()
{
}

// ----------------------------------------------------------
AccessorExecuteListenerRef::AccessorExecuteListenerRef(boost::shared_ptr<db::aspect::AccessorExecuteListener> p)
                                                      : m_impl(p)
{
}

// ----------------------------------------------------------

AccessorExecuteListenerRef::~AccessorExecuteListenerRef()
{
}

// ----------------------------------------------------------
void AccessorExecuteListenerRef::executePerformed(const AccessorExecuteEvent & event)
{
    if (!isNull())
        m_impl->executePerformed(event);
}

// ----------------------------------------------------------
bool AccessorExecuteListenerRef::isNull()
{ 
    return (0 == m_impl.get());
}

// ----------------------------------------------------------

void AccessorExecuteListenerRef::reset()
{
    m_impl.reset();
}

// ----------------------------------------------------------
const boost::shared_ptr<AccessorExecuteListener> & AccessorExecuteListenerRef::getImpl() const
{
    if (m_impl.get() == 0)
    {
        ExceptInfo sInfo( "basar.db.aspect.AccessorExecuteListenerRef.getImpl()", 
                          "Reference of AccessorExecuteListener in AccessorExecuteListenerRef "
						  "- no pointer to impl-class", 
						  __FILE__, __LINE__);
        throw NullReferenceException(sInfo);
    }

    return m_impl;
}

//--------------------------------------------------------------------------------------------------
}
}
}

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
