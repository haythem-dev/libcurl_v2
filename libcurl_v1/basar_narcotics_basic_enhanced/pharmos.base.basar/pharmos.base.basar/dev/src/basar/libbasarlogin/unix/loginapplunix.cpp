//----------------------------------------------------------------------------
/*! \file
 *  \brief  all win-specific functions implemented of LoginAppl
 *  \author Thomas Hörath
 *  \date   26.02.2006
 */
//----------------------------------------------------------------------------

#include "loginapplunix.h"
#include "logindata.h"

//----------------------------------------------------------------------------
namespace basar {
namespace login {

//----------------------------------------------------------------------------
/*!	\throw  no-throw */
LoginApplUnix::LoginApplUnix() 
{
}

//----------------------------------------------------------------------------
/*!	\throw  no-throw */
LoginApplUnix::~LoginApplUnix()
{
}

//----------------------------------------------------------------------------
/*!	\throw  no-throw */
void LoginApplUnix::init ()
{
	LoginAppl::init();
}

//----------------------------------------------------------------------------
/*!	\throw  no-throw */
void LoginApplUnix::clear ()
{
	LoginAppl::clear();
}

//---------------------------------------------------------------------------
/*!	\throw  no-throw 
	\retval true if login was successful
	\return otherwise returns false */
bool LoginApplUnix::interactiveLogin( basar::I18nString& , const basar::I18nString& , 
															const basar::Int16 )
{
	if (true)
	{
		ConstString func   = "basar.login.LoginApplUnix.interactiveLogin";
		ConstString reason = "no login dialog on unix!";
		throw InvalidStateException(ExceptInfo(func, reason, __FILE__, __LINE__));
	}

	return false;
}

void LoginApplUnix::handleLogin( bool loginWithADCredentials )
{
	if( !loginWithADCredentials) {
		loginWithADCredentials = false;
	}

	ConstString func   = "basar.login.LoginApplUnix.handleLogin";
	ConstString reason = "no login dialog on unix!";
	throw InvalidStateException(ExceptInfo(func, reason, __FILE__, __LINE__));
}

//----------------------------------------------------------------------------
/*!	\throw  no-throw */
void LoginApplUnix::handlePwChanged()
{
	ConstString func   = "basar.login.LoginApplUnix.handlePwChanged";
	ConstString reason = "no password changing on unix!";
	throw InvalidStateException(ExceptInfo(func, reason, __FILE__, __LINE__));
}

//----------------------------------------------------------------------------
/*!	\throw  no-throw */
void LoginApplUnix::handleCancel()
{
	ConstString func   = "basar.login.LoginApplUnix.handleCancel";
	ConstString reason = "no login dialog on unix!";
	throw InvalidStateException(ExceptInfo(func, reason, __FILE__, __LINE__));
}

//---------------------------------------------------------------------------
/*!	\throw  InvalidStateException
    \return false (always)
*/
bool LoginApplUnix::managePwChanging()
{
	if (true)
	{
		ConstString func   = "basar.login.LoginApplUnix.managePwChanging";
		ConstString reason = "no password changing on unix!";
		throw InvalidStateException(ExceptInfo(func, reason, __FILE__, __LINE__));
	}

	return false;
}

//---------------------------------------------------------------------------
/*!	\throw  NoTextFoundException 
	\return multilang text for labels / messages */
const basar::I18nString	LoginApplUnix::getLabel(basar::I18nString refText)
{
	if (true)
	{
		ConstString func   = "basar.login.LoginApplUnix.getLabel";
		ConstString reason = "no labels on unix!";
		throw InvalidStateException(ExceptInfo(func, reason, __FILE__, __LINE__));
	}

	refText = refText;

	return I18nString();
}

//---------------------------------------------------------------------------
} // login
} // basar

