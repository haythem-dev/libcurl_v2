//----------------------------------------------------------------------------
/*! \file
 *  \brief  system event manager class definitions
 *  \author Thomas Hörath
 *  \date   14.08.2006
 */
//----------------------------------------------------------------------------

#ifndef GUARD_LIBBASARAPPL_SYSTEMEVENTMANAGER_H
#define GUARD_LIBBASARAPPL_SYSTEMEVENTMANAGER_H

#ifndef LIBBASARAPPL_API
	#ifdef _WIN32
		#ifdef LIBBASARAPPL_EXPORTS
			#define LIBBASARAPPL_API __declspec(dllexport)		//!< dll exported
		#else
			#define LIBBASARAPPL_API __declspec(dllimport)		//!< dll imported
		#endif 
	#else 
		#define LIBBASARAPPL_API
	#endif
#endif

//----------------------------------------------------------------------------
#include "libbasarcmnutil_pattern.h"
#include "libbasarcmnutil_i18nstring.h"

#include <map>
#include <list>
#include <boost/shared_ptr.hpp>

namespace basar
{
	namespace db		
	{
		namespace aspect	
		{
			class AccessorPropertyTable_YIterator;
		}
	}
}

//----------------------------------------------------------------------------
namespace basar {
namespace appl	{

//----------------------------------------------------------------------------
// enum section
//----------------------------------------------------------------------------
//! \brief part of EventReturnStruct to represent error level of handler
enum EventReturnEnum
{
	HANDLER_NOT_FOUND	= -1,	//!< no listener registered for event thrown	
	HANDLER_OK			= 0,	//!< listener handled event without problems
	HANDLER_INFO,				//!< no error in listener, but message which should be shown
	HANDLER_ERROR				//!< error in listener -> fireMulti interrupted
};

//----------------------------------------------------------------------------
// struct section
//----------------------------------------------------------------------------
//! \brief type returned after event handled
struct EventReturnStruct
{
	//! \brief standard return type - no error, empty message
	EventReturnStruct() : message(""), ret(HANDLER_OK)
	{
	}

	//! \brief often used return type - given error, empty message
	EventReturnStruct(EventReturnEnum retEnum	//!< error level
						) : message(""), ret(retEnum)
	{
	}

	//! \brief often used return type - given error, given message
	EventReturnStruct(	EventReturnEnum retEnum,			//!< error level
						const basar::I18nString & text		//!< error message
					  ) : message(text), ret(retEnum)
	{
	}

	//! \brief returns true  at HANDLER_OK and HANDLER_INFO
	bool isOK() const
	{
		return (HANDLER_INFO == ret || HANDLER_OK == ret);
	}

	//!  \brief returns true, if HANDLER_OK is  set.
	bool isHandlerOK() const
	{
		return (HANDLER_OK == ret);
	}

	I18nString			message;	//!< message to return to event source
	EventReturnEnum		ret;		//!< error code of handler
};

//----------------------------------------------------------------------------
// typedef section
//----------------------------------------------------------------------------
typedef I18nString					EventName;			//!< name of event, representing handle
typedef appl::EventReturnStruct		EventReturnType;	//!< return type of handle's

//------------------------------------------------------------------------------
//! \brief simple interface for telling fire() from which source (VC) it is thrown
//! \n interface class
//! no exceptions thrown
class IEventSource 
{
public:
	//! \brief shut down											\n no-throw
	virtual ~IEventSource() {};
	
	//! \brief pure virtual operator ==	(for comparison)			\n no-throw
	//! \return bool indicating equality
	virtual bool operator == (const IEventSource& rhs //!< event source to compare with
								) const = 0;
};

//------------------------------------------------------------------------------
//! \brief simple class to receive a null event source (default if there is none at registrating or firing) 
//! \n final class
//! no exceptions thrown
class NullEventSourceImpl : public IEventSource
{
public:
	//! \brief shut down											\n no-throw
	~NullEventSourceImpl() {};

	//! \brief operator ==	(for comparison)						\n no-throw
	//! \return bool indicating equality
	LIBBASARAPPL_API bool operator == (const IEventSource& rhs	//!< source to compare with
										) const;
};

//------------------------------------------------------------------------------
//! \brief singleton wrapper for NullEventSourceImpl \n final class
//! \note  class is necessary for dll exporting Singleton\<NullEventSourceImpl\> and 
//!        instantiating Manager::m_Instance
//! no exceptions thrown
class LIBBASARAPPL_API NullEventSource : public cmnutil::Singleton<NullEventSourceImpl>
{
private:
							NullEventSource ();									//!< forbidden
							NullEventSource (const NullEventSource & rCopy);	//!< forbidden
	NullEventSource &		operator =		(const NullEventSource & rCopy);	//!< forbidden
};

//------------------------------------------------------------------------------
//! \brief pure virtual interface for system event executer classes / template 
//! \n interface class
//! no exceptions thrown
class ISysEventExecuter 
{
public:
	//! \brief shut down											\n no-throw
	virtual					~ISysEventExecuter	()					{};

	//! \brief pure virtual operator ==								\n no-throw
	//! \return bool indicating equality
	virtual bool			operator ==			(const ISysEventExecuter& rhs	//!< object to compare with
												 )	= 0;
	
	//! \brief pure virtual function to call registered handle		\n no-throw
	//! \return EventReturnType indicating executed event's success or failure
	virtual EventReturnType execute				(IEventSource                                & rSource,	//!< source from which event is started
												 db::aspect::AccessorPropertyTable_YIterator   yit		//!< parameter to send to handle
												)				= 0;

	//! \brief return source where event was started from			\n no-throw
	virtual const IEventSource & getEventSource() const = 0;
};

//------------------------------------------------------------------------------
//! \brief class for operating on SysEventExecuter via shared_ptr 
//! \n final class
//! no exceptions thrown
class FuncExecuter
{
	public:
		//! \brief construction								\n no-throw
		LIBBASARAPPL_API									FuncExecuter(ISysEventExecuter* executer //!< ptr to executer
																		);

		//! \brief operator ->								\n no-throw
		boost::shared_ptr<ISysEventExecuter>				operator -> ();
		
		//! \brief operator == for comparison				\n no-throw
		bool												operator == (const FuncExecuter& rhp	//!< object to compare with
																		) const;

	private:
		boost::shared_ptr<ISysEventExecuter>				m_Executer;	//!< shared ptr to executer
};

//------------------------------------------------------------------------------
//! \brief manager for system events 
//! \n final class
//! no exceptions thrown
class SystemEventManagerImpl
{
	friend class cmnutil::Singleton < SystemEventManagerImpl > ;	//!< creator class

public:
	//! \brief shut down					\n no-throw
									   ~SystemEventManagerImpl	();
	
	//! \brief register / deregister event and matching executer			\n no-throw
	LIBBASARAPPL_API void				registerExecuter	(EventName event,		//!< name of (gui-)event
															 FuncExecuter executer,	//!< executer containing (uc-) handle
															 const bool forSave		//!< if true -> register; else -> deregister
															);
	
	//! \brief check if an executer to given event and source exists		\n no-throw
	LIBBASARAPPL_API bool			    isHandlerAvailable	(EventName event		//!< name of (gui-)event
															) const;
	
	//! \brief called by event sender, delegates event to last registered valid receiver				\n no-throw 
	LIBBASARAPPL_API EventReturnType	fire				(EventName                                   event,		//!< name of (gui-)event
															 db::aspect::AccessorPropertyTable_YIterator yit,		//!< parameter to send to handle
															 IEventSource                              & rSource = appl::NullEventSource::getInstance() //!< ptr to event source sending event
															);

	//! \brief called by event sender, delegates event to receiver(for convieniance: without parameter)	\n no-throw 
	LIBBASARAPPL_API EventReturnType	fire				(EventName      event,											//!< name of (gui-)event
															 IEventSource & rSource	= appl::NullEventSource::getInstance()	//!< ptr to event source sending event
															);

	//! \brief called by event sender, delegates event to all registered valid receivers				\n no-throw
	LIBBASARAPPL_API EventReturnType	fireMulti			(EventName                                   event,		//!< name of (gui-)event
															 db::aspect::AccessorPropertyTable_YIterator yit,		//!< parameter to send to handle
															 IEventSource                              & rSource	= appl::NullEventSource::getInstance() //!< ptr to event source sending event
															);

	//! \brief called by event sender, delegates event to all registered valid receivers(for convieniance: without parameter)	\n no-throw
	LIBBASARAPPL_API EventReturnType	fireMulti			(EventName      event,											//!< name of (gui-)event
															 IEventSource & rSource	 = appl::NullEventSource::getInstance() //!< ptr to event source sending event
															);

	//! \brief called by event sender, delegates event to all valid receivers registered for given source(for convieniance: without parameter)	\n no-throw
	LIBBASARAPPL_API EventReturnType	fireSpecified		(EventName      event,		//!< name of (gui-)event
															 IEventSource & rSource		//!< source firing (e.g. ViewConnector)
															);

	//! \brief called by event sender, delegates event to all valid receivers registered for given source	\n no-throw
	LIBBASARAPPL_API EventReturnType	fireSpecified		(EventName                                   event,		//!< name of (gui-)event
															 db::aspect::AccessorPropertyTable_YIterator yit,		//!< parameter to send to handle
															 IEventSource                              & rSource	//!< source firing (e.g. ViewConnector)
															);


private:
	typedef std::list<FuncExecuter>					CollFuncExecuter;	//!< collection of function executers
	typedef std::map<EventName, CollFuncExecuter>	CollEvent;			//!< collection of events and matching collection of function executers
	
	SystemEventManagerImpl		            ();										//!< constructed by singleton \n no-throw
	SystemEventManagerImpl		            (const SystemEventManagerImpl & rCopy);	//!< forbidden
	SystemEventManagerImpl &	operator =	(const SystemEventManagerImpl & rCopy);	//!< forbidden

	//! \brief register event and matching executer, for redirecting event later to handle-function		\n no-throw
	void						registerExecuter	(EventName event,		//!< name of (gui-)event
													 FuncExecuter executer	//!< executer containing (uc-) handle
													);

	//! \brief deregister event and matching executer before executer (= UC) is shut down				\n no-throw 
	void						unregisterExecuter	(EventName event,		//!< name of (gui-)event
													 FuncExecuter executer	//!< executer containing (uc-) handle
													);

	
	CollEvent					m_SysEvents;	//!< collection of all registered events
};

//------------------------------------------------------------------------------
//! \brief singleton wrapper for SystemEventManagerImpl \n final class
//! \note  class is necessary for dll exporting Singleton\<SystemEventManagerImpl\> and 
//!        instantiating Manager::m_Instance
//! no exceptions thrown
class LIBBASARAPPL_API SystemEventManager : public cmnutil::Singleton<SystemEventManagerImpl>
{
private:
							SystemEventManager  ();									//!< forbidden
							SystemEventManager  (const SystemEventManager & rCopy);	//!< forbidden
	SystemEventManager &    operator =			(const SystemEventManager & rCopy);	//!< forbidden
};

//----------------------------------------------------------------------------
}	// namespace appl
}	// namespace basar

//----------------------------------------------------------------------------
#endif	// GUARD
