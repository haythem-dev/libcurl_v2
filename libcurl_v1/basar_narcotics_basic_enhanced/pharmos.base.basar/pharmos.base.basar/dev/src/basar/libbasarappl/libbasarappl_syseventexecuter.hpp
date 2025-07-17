//----------------------------------------------------------------------------
/*! \file
 *  \brief  system event executer template
 *  \author Thomas Hörath
 *  \date   01.09.2006
 */
//----------------------------------------------------------------------------

#ifndef GUARD_LIBBASARAPPL_SYSEVENTEXECUTER_HPP
#define GUARD_LIBBASARAPPL_SYSEVENTEXECUTER_HPP

#ifndef LIBBASARAPPL_API
	#ifdef _WIN32
		#ifdef LIBBASARAPPL_EXPORTS
			#define LIBBASARAPPL_API __declspec(dllexport)	//!< dll exported
		#else
			#define LIBBASARAPPL_API __declspec(dllimport)	//!< dll imported
		#endif 
	#else 
		#define LIBBASARAPPL_API
	#endif
#endif

#include "libbasardbaspect_accessorpropertytable_yiterator.h"

//----------------------------------------------------------------------------
namespace basar {
namespace appl	{

//------------------------------------------------------------------------------
//! \brief template for interface of use case controllers for using SystemEventManager
//! \n template class
//! no exceptions thrown
template <class Handler> class SysEventExecuter : public ISysEventExecuter
{
public:

	typedef EventReturnType (Handler::*PtrToMemberFunc)(IEventSource &, basar::db::aspect::AccessorPropertyTable_YIterator); //!< representing handle

	//! \brief construction											\n no-throw
	SysEventExecuter(Handler* pHandler,		//!< pointer to use case class where handles are defined
					 PtrToMemberFunc pmf,	//!< pointer to handle-function which is called after fire
					 IEventSource & rSource	= NullEventSource::getInstance() //!< ref to event source sending event
					 ) : m_pHandler (pHandler), m_PtrToMemberFunc (pmf), m_rEventSource(rSource)
	{
	}

	//! \brief called by fire of SystemEventManager, delegates to registered handle		\n no-throw
	//! \return EventReturnType indication success or failure of event's execution
	virtual EventReturnType execute(IEventSource & rSource,									//!< source where event is started
									basar::db::aspect::AccessorPropertyTable_YIterator yit	//!< parameter to send to called handle-function
									)
	{
		Handler& handler = *m_pHandler;
		// Method dispatching for the given object:
		return (handler.*m_PtrToMemberFunc)(rSource, yit);
	}
	
	//! \brief compare two SysEventExecuters		\n no-throw
	//! \return bool indication equality
	virtual bool operator == (const ISysEventExecuter& rhs	//!< object to compare with
							 )
	{
		SysEventExecuter ucc_rhs = static_cast<const SysEventExecuter&> (rhs);
		return (m_pHandler == ucc_rhs.m_pHandler) && (m_PtrToMemberFunc == ucc_rhs.m_PtrToMemberFunc);
	}

	//! \brief return source where event was started from			\n no-throw
	//! \return event's source
	virtual const IEventSource & getEventSource() const
	{
		return m_rEventSource;
	}
	
	//! \brief operator =			\n no-throw
	SysEventExecuter& operator=(const SysEventExecuter& rCopy)
	{
		m_pHandler = rCopy.m_pHandler;
		m_PtrToMemberFunc = rCopy.m_PtrToMemberFunc;
		m_rEventSource = rCopy.m_rEventSource;

		return *this;
	}

private:
	Handler *			m_pHandler;			//!< pointer to use case class
	PtrToMemberFunc		m_PtrToMemberFunc;	//!< pointer to handle-function
	IEventSource &		m_rEventSource;		//!< ref to event source sending event
};

//----------------------------------------------------------------------------
}	// namespace appl
}	// namespace basar

//----------------------------------------------------------------------------
#endif	// GUARD
