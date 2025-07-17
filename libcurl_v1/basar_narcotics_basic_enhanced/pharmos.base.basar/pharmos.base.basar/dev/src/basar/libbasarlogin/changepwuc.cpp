//----------------------------------------------------------------------------
/*! \file
 *  \brief  check login implemtation
 *  \author	Thomas Hörath
 *  \date   19.12.2005
 */
//----------------------------------------------------------------------------

#include "changepwuc.h"

#include "loginappl.h"
#include "crypt.h"

#include <string.h>

//----------------------------------------------------------------------------
namespace basar {
namespace login {

//----------------------------------------------------------------------------
namespace useCase {

//----------------------------------------------------------------------------
/*!	\throw no-throw */
ChangePw::ChangePw()
{

}

//----------------------------------------------------------------------------
/*!	\throw no-throw */
ChangePw::~ChangePw()
{
}

//----------------------------------------------------------------------------
/*!	\throw no-throw 
	\retval true if password has been changed
	\return otherwise returns false */
bool ChangePw::run()
{
	m_Password = getAppl()->m_currentUser.getString("password");
	
	//crypt PW before writing to db!
	char cryptedPW [PASSWORD_MAX_LENGTH + 1];
	
	char unCryptedPW [PASSWORD_MAX_LENGTH + 1]; //uncrypted length has to be max-length!
	memset(unCryptedPW, ' ', PASSWORD_MAX_LENGTH);
	memcpy(unCryptedPW, m_Password.c_str(), m_Password.length() > PASSWORD_MAX_LENGTH ? PASSWORD_MAX_LENGTH : m_Password.length());
	unCryptedPW[PASSWORD_MAX_LENGTH] = '\0';
	util::encryptPW( unCryptedPW, cryptedPW, PASSWORD_MAX_LENGTH);
	getAppl()->m_currentUser.setString("password", cryptedPW);
	getAppl()->m_currentUser.setInt16("changepw", PW_NOT_TO_CHANGE);

	//check, if pw is one of the last six ones
	basar::db::aspect::ExecuteResultInfo	executeResult;		// result of save call
	getAppl()->m_currentUser.setInt32("datevalidfrom", basar::cmnutil::Date::getCurrent().getDate());
	executeResult = getAppl()->m_UserDM->savePasswordPrevious(getAppl()->m_currentUser); 

	if ( executeResult.hasError() && executeResult.getError() == basar::db::aspect::SQL_ERROR_DUPL_INSERT )
		return false;

	if ( getAppl()->m_UserDM->findNumOfPreviousPasswords(getAppl()->m_currentUser) > NUM_OF_PW_IN_HISTORY )
	{
		getAppl()->m_UserDM->deletePasswordPrev(getAppl()->m_currentUser); 
		getAppl()->m_currentUser.setInt32("datevalidfrom", basar::cmnutil::Date::getCurrent().getDate());
	}

	getAppl()->m_currentUser.setInt32("lastpwchange", basar::cmnutil::Date::getCurrent().getDate());
	getAppl()->m_currentUser.setPropertyState("password", basar::SS_UPDATE);
	getAppl()->m_currentUser.setPropertyState("changepw", basar::SS_UPDATE);

	return true;
}

//----------------------------------------------------------------------------
/*!	\throw no-throw 
	\retval true if password is complex enough
	\return otherwise returns false */
bool ChangePw::isPwComplex(basar::I18nString &newPw)
{
	unsigned int i = 0;
	bool digitFound = false;
	bool upperFound = false;

	while ( (digitFound == false || upperFound == false ) &&
			i < newPw.length() )
	{
		if ( digitFound == false && isdigit(static_cast<UInt8>(newPw.at(i))) != 0 )
			digitFound = true;

		if ( upperFound == false && isupper(static_cast<UInt8>(newPw.at(i))) != 0 )
			upperFound = true;

		++i;
	}
	
	return digitFound == true && upperFound == true;
}

//----------------------------------------------------------------------------
}//useCase
}//login
}//basar
