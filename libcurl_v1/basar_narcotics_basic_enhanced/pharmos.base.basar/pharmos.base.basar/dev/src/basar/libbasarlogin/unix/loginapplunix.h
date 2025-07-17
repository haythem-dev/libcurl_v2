//----------------------------------------------------------------------------
/*! \file
 *  \brief  login implementation class for unix (without dialogs)
 *  \author Thomas Hörath
 *  \date   26.02.2006
 */
//----------------------------------------------------------------------------

#ifndef GUARD_LOGINAPPLUNIX_H
#define GUARD_LOGINAPPLUNIX_H

//----------------------------------------------------------------------------
#include "loginappl.h"

//----------------------------------------------------------------------------
namespace basar	{
namespace login	{

//----------------------------------------------------------------------------
//! login implementation class \n final class
class LoginApplUnix : public LoginAppl
{
	friend class LoginAppl;    //!< for construction from LoginAppl::create()

public:
	/*! \brief destructor
		\n	   no-throw */
	virtual ~LoginApplUnix();

	/*! \brief	start login by calling login-dialog, which is created modal.
		\n		no-throw */
	virtual bool interactiveLogin(basar::I18nString& rApplName,	//!< unique applicationname to start login for
				 const basar::I18nString& rWindowTitle, //!< window title to show in login dialog
				 const basar::Int16 maxTrials			//!< number of login trials
				 );

	/*! \brief	handle gui-event ok button pressed in login dlg
		\n		no-throw */
	virtual void	handleLogin( bool loginWithADCredentials = false );

	/*! \brief	handle gui-event ok button pressed in change pw dlg
		\n		no-throw */
	virtual void	handlePwChanged();

	/*! \brief	handle gui-event cancel button pressed
		\n		no-throw */
	virtual void	handleCancel();

	/*! \brief	call change password dialog and manage input check
		\n		no-throw */
	virtual bool	managePwChanging();

	/*! \brief	return multilang-text from pptext to reftext
		\n		NoTextFoundException */
	virtual const basar::I18nString	getLabel(basar::I18nString refText //!< reference text to search in label / message
									);


private:
	LoginApplUnix				();								//!< forbidden constructor
	LoginApplUnix				(const LoginApplUnix & rCopy);	//!< forbidden copy constructor
	LoginApplUnix & operator =	(const LoginApplUnix & rCopy);	//!< forbidden assignment operator

	/*! \brief	initialize members
		\n		no-throw */
	virtual void	init		();

	/*! \brief	clear members
		\n		no-throw */
	virtual void	clear		();
};

//----------------------------------------------------------------------------
}	// namespace login
}	// namespace basar

#endif	// GUARD
