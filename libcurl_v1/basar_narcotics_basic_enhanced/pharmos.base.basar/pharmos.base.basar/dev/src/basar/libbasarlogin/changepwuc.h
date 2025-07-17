//----------------------------------------------------------------------------
/*! \file
 *  \brief  use case declarations for changing password
 *  \author Thomas Hörath
 *  \date   15.01.2006
 */
//----------------------------------------------------------------------------

#ifndef GUARD_CHANGEPWUC_H
#define GUARD_CHANGEPWUC_H

//----------------------------------------------------------------------------
#include "libbasarcmnutil_i18nstring.h"
#include "usecasecontroller.h"

//----------------------------------------------------------------------------
namespace basar {
namespace login	{

//----------------------------------------------------------------------------
namespace useCase {

//---------------------------------------------------------------------------
/*! \brief class for use case change password 
	\throws no exceptions
	\n final class */
class ChangePw : public UseCaseController
{
public:	
	/*! \brief std-destructor
		\n no-throw */
	virtual ~ChangePw();
	
	/*! \brief std-constructor
		\n no-throw */
	ChangePw();

	/*! \brief start changing password actions
		\n no-throw */
	bool run();

	/*! \brief check if password matches guidelines of complexity
		\n no-throw */
	bool isPwComplex(basar::I18nString &newPw //!< new password, entered by user - should be stored afterwards
					);

private:
	basar::I18nString m_Password; //!< new (changed) password
};

//----------------------------------------------------------------------------
} // useCase
} // login
} // basar

//----------------------------------------------------------------------------
#endif	// GUARD
