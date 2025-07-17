#ifndef GUARD_LIBBASARCMNUTIL_REMOTETASKEXCEPTION_H
#define GUARD_LIBBASARCMNUTIL_REMOTETASKEXCEPTION_H


//------------------------------------------------------------------------------------------------------//
// dll import and export macros
//------------------------------------------------------------------------------------------------------//
#ifndef LIBBASARCMNUTIL_API
	//#ifdef _WIN32
	//	#ifdef LIBBASARCMNUTIL_EXPORTS
	//		#define LIBBASARCMNUTIL_API __declspec(dllexport)	//!< dll exported
	//	#else
	//		#define LIBBASARCMNUTIL_API __declspec(dllimport)	//!< dll imported
	//	#endif 
	//#else 
		#define LIBBASARCMNUTIL_API
	//#endif
#endif

//------------------------------------------------------------------------------------------------------//
// include section
//------------------------------------------------------------------------------------------------------//
#include "libbasarcmnutil_exceptions.h"

//------------------------------------------------------------------------------------------------------//
// namespace section
//------------------------------------------------------------------------------------------------------//
namespace basar {
namespace cmnutil {

//------------------------------------------------------------------------------
//! thrown if remote task errors occur \n base class
class RemoteTaskException : public BasarException
{
public:
	//! constructs exception object and initialises text description string
	RemoteTaskException	(const ExceptInfo & sInfo	                      ,		//!< exception info struct
						 const VarString  & exceptionType = "RemoteTaskException" ,  //!< type of exception which is thrown
					     const VarString  & addReason     = ""				//!< additional reason, extends m_InfoStruct.reason
						 ) : basar::Exception(sInfo, exceptionType, addReason)
	{};
	//! destructor
	virtual	~RemoteTaskException	(){};

protected:
	RemoteTaskException	();					//!< forbidden
};

//------------------------------------------------------------------------------------------------------//
//! \brief special exception class for failed remote connections
//! \n final class
//------------------------------------------------------------------------------------------------------//
class RemoteConnectionFailedException : public RemoteTaskException
{
public:
	//! constructs exception object and initialises text description string
	LIBBASARCMNUTIL_API RemoteConnectionFailedException	(const ExceptInfo & sInfo) 
											: RemoteTaskException(sInfo, "RemoteConnectionFailedException")
											{};
private:
	RemoteConnectionFailedException	();							//!< forbidden

};

//------------------------------------------------------------------------------------------------------//
//! \brief special exception class for failed remote connections, thrown if remote endpoints address is empty
//! \n when it's queried by RemoteTask
//! \n final class
//------------------------------------------------------------------------------------------------------//
class RemoteEndpointAddressEmptyException : public RemoteTaskException
{
public:
	//! constructs exception object and initialises text description string
	LIBBASARCMNUTIL_API RemoteEndpointAddressEmptyException	(const ExceptInfo & sInfo) 
											: RemoteTaskException(sInfo, "RemoteEndpointAddressEmptyException")
											{};
private:
	RemoteEndpointAddressEmptyException	();							//!< forbidden

};

//------------------------------------------------------------------------------------------------------//
//! \brief special exception class for failed remote connections, thrown if remote task's command is empty
//! \n when it's queried by RemoteTask
//! \n final class
//------------------------------------------------------------------------------------------------------//
class RemoteTaskCommandEmptyException : public RemoteTaskException
{
public:
	//! constructs exception object and initialises text description string
	LIBBASARCMNUTIL_API RemoteTaskCommandEmptyException	(const ExceptInfo & sInfo) 
											: RemoteTaskException(sInfo, "RemoteTaskCommandEmptyException")
											{};
private:
	RemoteTaskCommandEmptyException	();							//!< forbidden

};

//------------------------------------------------------------------------------------------------------//
//! \brief special exception class for remote connections, thrown if SshSession is created with a non-open socket
//! \n final class
//------------------------------------------------------------------------------------------------------//
class SshSessionSocketNotOpenException : public RemoteTaskException
{
public:
	//! constructs exception object and initialises text description string
	LIBBASARCMNUTIL_API SshSessionSocketNotOpenException	(const ExceptInfo & sInfo) 
											: RemoteTaskException(sInfo, "SshSessionSocketNotOpenException")
											{};
private:
						SshSessionSocketNotOpenException	();							//!< forbidden

};

//------------------------------------------------------------------------------------------------------//
//! \brief special exception class for remote connections, thrown if Initialization of the SshSession fails
//! \n final class
//------------------------------------------------------------------------------------------------------//
class SshSessionInitFailedException : public RemoteTaskException
{
public:
	//! constructs exception object and initialises text description string
	LIBBASARCMNUTIL_API SshSessionInitFailedException	(const ExceptInfo & sInfo) 
											: RemoteTaskException(sInfo, "SshSessionInitFailedException")
											{};
private:
						SshSessionInitFailedException	();							//!< forbidden

};

//------------------------------------------------------------------------------------------------------//
//! \brief special exception class for remote connections, thrown if ssh handshake fails
//! \n final class
//------------------------------------------------------------------------------------------------------//
class SshHandshakeFailedException : public RemoteTaskException
{
public:
	//! constructs exception object and initialises text description string
	LIBBASARCMNUTIL_API SshHandshakeFailedException	(const ExceptInfo & sInfo) 
											: RemoteTaskException(sInfo, "SshHandshakeFailedException")
											{};
private:
						SshHandshakeFailedException	();							//!< forbidden

};

//------------------------------------------------------------------------------------------------------//
//! \brief special exception class for remote connections, thrown if login using username and password fails
//! \n final class
//------------------------------------------------------------------------------------------------------//
class SshLoginUsernamePasswordFailedException : public RemoteTaskException
{
public:
	//! constructs exception object and initialises text description string
	LIBBASARCMNUTIL_API SshLoginUsernamePasswordFailedException	(const ExceptInfo & sInfo) 
											: RemoteTaskException(sInfo, "SshLoginUsernamePasswordFailedException")
											{};
private:
						SshLoginUsernamePasswordFailedException	();							//!< forbidden

};

//------------------------------------------------------------------------------------------------------//
//! \brief special exception class for remote connections, thrown if opening of ssh channel fails
//! \n final class
//------------------------------------------------------------------------------------------------------//
class SshOpenChannelFailedException : public RemoteTaskException
{
public:
	//! constructs exception object and initialises text description string
	LIBBASARCMNUTIL_API SshOpenChannelFailedException	(const ExceptInfo & sInfo) 
											: RemoteTaskException(sInfo, "SshOpenChannelFailedException")
											{};
private:
						SshOpenChannelFailedException	();							//!< forbidden

};

//------------------------------------------------------------------------------------------------------//
//! \brief special exception class for remote connections, thrown if remote exec fails (the ssh command, not the task)
//! \n final class
//------------------------------------------------------------------------------------------------------//
class SshExecFailedException : public RemoteTaskException
{
public:
	//! constructs exception object and initialises text description string
	LIBBASARCMNUTIL_API SshExecFailedException	(const ExceptInfo & sInfo) 
											: RemoteTaskException(sInfo, "SshExecFailedException")
											{};
private:
						SshExecFailedException	();							//!< forbidden

};


//------------------------------------------------------------------------------------------------------//
//! \brief special exception class for remote connections, thrown if send during ssh session fails
//! \n final class
//------------------------------------------------------------------------------------------------------//
class SshSocketSendFailedException : public RemoteTaskException
{
public:
	//! constructs exception object and initialises text description string
	LIBBASARCMNUTIL_API SshSocketSendFailedException	(const ExceptInfo & sInfo) 
											: RemoteTaskException(sInfo, "SshSocketSendFailedException")
											{};
private:
						SshSocketSendFailedException	();							//!< forbidden

};

//------------------------------------------------------------------------------------------------------//
//! \brief special exception class for remote connections, thrown if other side closed the channel
//! \n final class
//------------------------------------------------------------------------------------------------------//
class SshChannelClosedException : public RemoteTaskException
{
public:
	//! constructs exception object and initialises text description string
	LIBBASARCMNUTIL_API SshChannelClosedException	(const ExceptInfo & sInfo) 
											: RemoteTaskException(sInfo, "SshChannelClosedException")
											{};
private:
						SshChannelClosedException	();							//!< forbidden

};

//------------------------------------------------------------------------------------------------------//
//! \brief special exception class for remote connections, thrown if passed ssh session in ssh channel is
//! \n not initialized
//! \n final class
//------------------------------------------------------------------------------------------------------//
class SshSessionNotInitializedException : public RemoteTaskException
{
public:
	//! constructs exception object and initialises text description string
	LIBBASARCMNUTIL_API SshSessionNotInitializedException	(const ExceptInfo & sInfo) 
											: RemoteTaskException(sInfo, "SshSessionNotInitializedException")
											{};
private:
						SshSessionNotInitializedException	();							//!< forbidden

};

//----------------------------------------------------------------------------
}	// namespace cmnutil
}	// namespace basar


#endif //GUARD_LIBBASARCMNUTIL_REMOTETASKEXCEPTION_H
