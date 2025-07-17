//-----------------------------------------------------------------------------------------------//
/*! \file	
 *  \brief  macros for systemeventmanager
 *  \author Thomas Hörath
 *  \date   26.08.2006
 */
//-----------------------------------------------------------------------------------------------//
#ifndef GUARD_LIBBASARAPPL_EVENTMACROS_H
#define GUARD_LIBBASARAPPL_EVENTMACROS_H

/*! \ingroup DIFF_WIN_UNIX */ 
#ifdef _WIN32
#pragma warning(error: 4002) // set warning C4002 as error (not enough handles for selected HANDLE_ macro)
#pragma warning(error: 4003) // set warning C4003 as error (to many handles for selected HANDLE_ macro)
#endif

//-----------------------------------------------------------------------------------------------//
// namespaces
//-----------------------------------------------------------------------------------------------//
namespace basar {
namespace appl {

//-----------------------------------------------------------------------------------------------//
// declaration handler
//-----------------------------------------------------------------------------------------------//
//! macro to declare a handle-function (e.g. in UC-header)
#define SYSTEM_EVENT_HANDLER_DECLARATION(eventName) basar::appl::EventReturnType handle ## eventName \
		(basar::appl::IEventSource & rSource, /*!< source from which event was fired */ \
		basar::db::aspect::AccessorPropertyTable_YIterator yitFromEvSrc /*!< iterator containing data from event source */ \
		);

//-----------------------------------------------------------------------------------------------//
// definition handler
//----------------------------------------------------------------------------
//! macro to define a handle-function (e.g. in UC-cpp)
#define SYSTEM_EVENT_HANDLER_DEFINITION(ucc,eventName) basar::appl::EventReturnType ucc::handle ## eventName\
		(basar::appl::IEventSource & rSource, basar::db::aspect::AccessorPropertyTable_YIterator yitFromEvSrc)

//-----------------------------------------------------------------------------------------------//
// registration handler
//----------------------------------------------------------------------------
//! macro to register an use cases handle-function for a specific event sent by a specific source
#define SYSTEM_EVENT_HANDLER_REGISTRATION_WITH_SOURCE(ucc, eventName, source) \
	basar::appl::SystemEventManager::getInstance().registerExecuter( \
		# eventName, \
		basar::appl::FuncExecuter(new basar::appl::SysEventExecuter<ucc> \
								 (this,& ucc :: handle ## eventName, source)), \
		forSave);

//----------------------------------------------------------------------------
//! macro to register an use cases handle-function for a specific event
#define SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName) \
	basar::appl::SystemEventManager::getInstance().registerExecuter( \
		# eventName, \
		basar::appl::FuncExecuter(new basar::appl::SysEventExecuter<ucc> \
								 (this,& ucc :: handle ## eventName )), \
		forSave);

//----------------------------------------------------------------------------
//! macro to register an use cases handle-function for a specific event
#define BEGIN_HANDLERS_REGISTRATION(ucc) \
	void ucc::registerEventHandlers(const bool forSave) { 

//----------------------------------------------------------------------------
//! macro to register an use cases handle-function for a specific event
#define END_HANDLERS_REGISTRATION() \
	}

	
//-----------------------------------------------------------------------------------------------//
// all handlers at one point
//----------------------------------------------------------------------------
//! macro to define the registration / deregistration of 1 use case handle-function
#define HANDLE_1_SYSTEM_EVENT(ucc, eventName) \
	void ucc::registerEventHandlers(const bool forSave) { \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName) \
	}
	
//----------------------------------------------------------------------------
//! macro to define the registration / deregistration of 2 use case handle-functions
#define HANDLE_2_SYSTEM_EVENTS(ucc, eventName1, eventName2) \
	void ucc::registerEventHandlers(const bool forSave) { \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName1) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName2) \
	}

//----------------------------------------------------------------------------
//! macro to define the registration / deregistration of 3 use case handle-functions
#define HANDLE_3_SYSTEM_EVENTS(ucc, eventName1, eventName2, eventName3) \
	void ucc::registerEventHandlers(const bool forSave) { \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName1) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName2) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName3) \
	}

//----------------------------------------------------------------------------
//! macro to define the registration / deregistration of 4 use case handle-functions
#define HANDLE_4_SYSTEM_EVENTS(ucc, eventName1, eventName2, eventName3, eventName4) \
	void ucc::registerEventHandlers(const bool forSave) { \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName1) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName2) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName3) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName4) \
	}

//----------------------------------------------------------------------------
//! macro to define the registration / deregistration of 5 use case handle-functions
#define HANDLE_5_SYSTEM_EVENTS(ucc, eventName1, eventName2, eventName3, eventName4, eventName5) \
	void ucc::registerEventHandlers(const bool forSave) { \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName1) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName2) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName3) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName4) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName5) \
	}

//----------------------------------------------------------------------------
//! macro to define the registration / deregistration of 6 use case handle-functions
#define HANDLE_6_SYSTEM_EVENTS(ucc, eventName1, eventName2, eventName3, eventName4, eventName5, eventName6) \
	void ucc::registerEventHandlers(const bool forSave) { \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName1) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName2) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName3) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName4) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName5) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName6) \
	}

//----------------------------------------------------------------------------
//! macro to define the registration / deregistration of 7 use case handle-functions
#define HANDLE_7_SYSTEM_EVENTS(ucc, eventName1, eventName2, eventName3, eventName4, eventName5, \
									eventName6, eventName7) \
	void ucc::registerEventHandlers(const bool forSave) { \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName1) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName2) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName3) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName4) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName5) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName6) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName7) \
	}

//----------------------------------------------------------------------------
//! macro to define the registration / deregistration of 8 use case handle-functions
#define HANDLE_8_SYSTEM_EVENTS(ucc, eventName1, eventName2, eventName3, eventName4, eventName5, \
									eventName6, eventName7, eventName8) \
	void ucc::registerEventHandlers(const bool forSave) { \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName1) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName2) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName3) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName4) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName5) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName6) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName7) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName8) \
	}

//----------------------------------------------------------------------------
//! macro to define the registration / deregistration of 9 use case handle-functions
#define HANDLE_9_SYSTEM_EVENTS(ucc, eventName1, eventName2, eventName3, eventName4, eventName5, \
									eventName6, eventName7, eventName8, eventName9) \
	void ucc::registerEventHandlers(const bool forSave) { \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName1) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName2) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName3) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName4) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName5) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName6) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName7) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName8) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName9) \
	}

//----------------------------------------------------------------------------
//! macro to define the registration / deregistration of 10 use case handle-functions
#define HANDLE_10_SYSTEM_EVENTS(ucc, eventName1, eventName2, eventName3, eventName4, eventName5, \
									 eventName6, eventName7, eventName8, eventName9, eventName10) \
	void ucc::registerEventHandlers(const bool forSave) { \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName1) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName2) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName3) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName4) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName5) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName6) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName7) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName8) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName9) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName10) \
	}

//----------------------------------------------------------------------------
//! macro to define the registration / deregistration of 11 use case handle-functions
#define HANDLE_11_SYSTEM_EVENTS(ucc, eventName1, eventName2, eventName3, eventName4, eventName5, \
									 eventName6, eventName7, eventName8, eventName9, eventName10, \
									 eventName11) \
	void ucc::registerEventHandlers(const bool forSave) { \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName1) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName2) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName3) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName4) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName5) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName6) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName7) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName8) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName9) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName10) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName11) \
	}

//----------------------------------------------------------------------------
//! macro to define the registration / deregistration of 12 use case handle-functions
#define HANDLE_12_SYSTEM_EVENTS(ucc, eventName1, eventName2, eventName3, eventName4, eventName5, \
									 eventName6, eventName7, eventName8, eventName9, eventName10, \
									 eventName11, eventName12) \
	void ucc::registerEventHandlers(const bool forSave) { \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName1) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName2) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName3) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName4) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName5) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName6) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName7) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName8) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName9) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName10) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName11) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName12) \
	}

//----------------------------------------------------------------------------
//! macro to define the registration / deregistration of 13 use case handle-functions
#define HANDLE_13_SYSTEM_EVENTS(ucc, eventName1, eventName2, eventName3, eventName4, eventName5, \
									 eventName6, eventName7, eventName8, eventName9, eventName10, \
									 eventName11, eventName12, eventName13) \
	void ucc::registerEventHandlers(const bool forSave) { \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName1) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName2) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName3) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName4) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName5) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName6) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName7) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName8) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName9) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName10) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName11) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName12) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName13) \
	}

//----------------------------------------------------------------------------
//! macro to define the registration / deregistration of 14 use case handle-functions
#define HANDLE_14_SYSTEM_EVENTS(ucc, eventName1, eventName2, eventName3, eventName4, eventName5, \
									 eventName6, eventName7, eventName8, eventName9, eventName10, \
									 eventName11, eventName12, eventName13, eventName14) \
	void ucc::registerEventHandlers(const bool forSave) { \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName1) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName2) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName3) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName4) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName5) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName6) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName7) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName8) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName9) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName10) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName11) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName12) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName13) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName14) \
	}

//----------------------------------------------------------------------------
//! macro to define the registration / deregistration of 15 use case handle-functions
#define HANDLE_15_SYSTEM_EVENTS(ucc, eventName1, eventName2, eventName3, eventName4, eventName5, \
									 eventName6, eventName7, eventName8, eventName9, eventName10, \
									 eventName11, eventName12, eventName13, eventName14, eventName15) \
	void ucc::registerEventHandlers(const bool forSave) { \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName1) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName2) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName3) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName4) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName5) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName6) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName7) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName8) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName9) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName10) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName11) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName12) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName13) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName14) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName15) \
	}

//----------------------------------------------------------------------------
//! macro to define the registration / deregistration of 16 use case handle-functions
#define HANDLE_16_SYSTEM_EVENTS(ucc, eventName1, eventName2, eventName3, eventName4, eventName5, \
									 eventName6, eventName7, eventName8, eventName9, eventName10, \
									 eventName11, eventName12, eventName13, eventName14, eventName15, \
									 eventName16) \
	void ucc::registerEventHandlers(const bool forSave) { \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName1) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName2) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName3) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName4) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName5) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName6) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName7) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName8) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName9) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName10) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName11) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName12) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName13) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName14) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName15) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName16) \
	}

//----------------------------------------------------------------------------
//! macro to define the registration / deregistration of 17 use case handle-functions
#define HANDLE_17_SYSTEM_EVENTS(ucc, eventName1, eventName2, eventName3, eventName4, eventName5, \
									 eventName6, eventName7, eventName8, eventName9, eventName10, \
									 eventName11, eventName12, eventName13, eventName14, eventName15, \
									 eventName16, eventName17) \
	void ucc::registerEventHandlers(const bool forSave) { \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName1) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName2) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName3) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName4) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName5) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName6) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName7) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName8) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName9) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName10) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName11) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName12) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName13) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName14) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName15) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName16) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName17) \
	}

//----------------------------------------------------------------------------
//! macro to define the registration / deregistration of 18 use case handle-functions
#define HANDLE_18_SYSTEM_EVENTS(ucc, eventName1, eventName2, eventName3, eventName4, eventName5, \
									 eventName6, eventName7, eventName8, eventName9, eventName10, \
									 eventName11, eventName12, eventName13, eventName14, eventName15, \
									 eventName16, eventName17, eventName18) \
	void ucc::registerEventHandlers(const bool forSave) { \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName1) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName2) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName3) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName4) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName5) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName6) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName7) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName8) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName9) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName10) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName11) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName12) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName13) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName14) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName15) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName16) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName17) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName18) \
	}

//----------------------------------------------------------------------------
//! macro to define the registration / deregistration of 19 use case handle-functions
#define HANDLE_19_SYSTEM_EVENTS(ucc, eventName1, eventName2, eventName3, eventName4, eventName5, \
									 eventName6, eventName7, eventName8, eventName9, eventName10, \
									 eventName11, eventName12, eventName13, eventName14, eventName15, \
									 eventName16, eventName17, eventName18, eventName19) \
	void ucc::registerEventHandlers(const bool forSave) { \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName1) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName2) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName3) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName4) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName5) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName6) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName7) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName8) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName9) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName10) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName11) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName12) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName13) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName14) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName15) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName16) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName17) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName18) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName19) \
	}

//----------------------------------------------------------------------------
//! macro to define the registration / deregistration of 20 use case handle-functions
#define HANDLE_20_SYSTEM_EVENTS(ucc, eventName1, eventName2, eventName3, eventName4, eventName5, \
									 eventName6, eventName7, eventName8, eventName9, eventName10, \
									 eventName11, eventName12, eventName13, eventName14, eventName15, \
									 eventName16, eventName17, eventName18, eventName19, eventName20) \
	void ucc::registerEventHandlers(const bool forSave) { \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName1) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName2) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName3) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName4) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName5) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName6) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName7) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName8) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName9) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName10) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName11) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName12) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName13) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName14) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName15) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName16) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName17) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName18) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName19) \
		SYSTEM_EVENT_HANDLER_REGISTRATION(ucc, eventName20) \
	}

//----------------------------------------------------------------------------
} // appl
} // basar

#endif
