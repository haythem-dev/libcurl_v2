/** $Id$                                          
*                                                  
* @file                                            
* @author Anke Klink                      
* @date 2009                            
*/    

#include "libbasarguitie_defaultexceptionhandler.h"
#include "libbasarguitie_exceptionhandlerref.h" 

//----------------------------------------------------------------------------
namespace basar
{
namespace gui
{
namespace tie
{
		
// ----------------------------------------------------------
ExceptionHandlerRef::ExceptionHandlerRef(boost::shared_ptr<ExceptionHandler> a)
{
	m_exceptionHandler = a;
}

// ----------------------------------------------------------
ExceptionHandlerRef::ExceptionHandlerRef()
{
	m_exceptionHandler = boost::shared_ptr<ExceptionHandler>(new DefaultExceptionHandler());
}

// ----------------------------------------------------------
ExceptionHandlerRef::~ExceptionHandlerRef( )
{
}

// ----------------------------------------------------------

bool ExceptionHandlerRef::handleException(const basar::Exception   & ex, 
										  const ExceptionFoundInfo & foundInfo, 
										  bool                       defretval) const
{ 
	bool retval = defretval; 
	if (m_exceptionHandler.get() != 0)
	{
		retval = m_exceptionHandler->handleException(ex, foundInfo,defretval ); 
	}
	return retval;
}

// ----------------------------------------------------------

bool ExceptionHandlerRef::handleException(const std::exception     & ex, 
										  const ExceptionFoundInfo & foundInfo, 
										  bool                       defretval) const
{
	bool retval = defretval; 
	if (m_exceptionHandler.get() != 0)
	{
		retval = m_exceptionHandler->handleException(ex, foundInfo,defretval); 
	}
	return retval;
}

// ----------------------------------------------------------

bool ExceptionHandlerRef::handleUnknownException(const ExceptionFoundInfo & foundInfo, 
												 bool                       defretval) const 
{
	bool retval = defretval; 
	if (m_exceptionHandler.get() != 0)
	{
		retval = m_exceptionHandler->handleUnknownException(foundInfo,defretval); 
	}
	return retval;
}

// ----------------------------------------------------------
}
}
}
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
