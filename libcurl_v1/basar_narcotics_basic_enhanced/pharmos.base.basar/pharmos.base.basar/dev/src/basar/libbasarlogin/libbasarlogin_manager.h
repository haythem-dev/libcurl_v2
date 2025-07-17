//----------------------------------------------------------------------------
/*! \file
 *  \brief  public interface for login-class (singleton)
 *  \author Thomas Hörath
 *  \date   15.07.2005
 */
//----------------------------------------------------------------------------

#ifndef GUARD_LIBBASARLOGIN_MANAGER_H
#define GUARD_LIBBASARLOGIN_MANAGER_H

//----------------------------------------------------------------------------
#ifndef LIBBASARLOGIN_API
	#ifdef _WIN32
		#ifdef LIBBASARLOGIN_EXPORTS
			#define LIBBASARLOGIN_API __declspec(dllexport)	//!< dll exported
		#else
			#define LIBBASARLOGIN_API __declspec(dllimport)	//!< dll imported
		#endif 
	#else 
		#define LIBBASARLOGIN_API
	#endif
#endif

//----------------------------------------------------------------------------
#include <boost/shared_ptr.hpp>
#include "libbasar_definitions.h"
#include "libbasarcmnutil_pattern.h"
#include "libbasarcmnutil_i18nstring.h"
#include "libbasarlogin_definitions.h"

//----------------------------------------------------------------------------
namespace basar
{
namespace login
{

//---------------------------------------------------------------------------------------//
// forward declaration
//---------------------------------------------------------------------------------------//
class LoginAppl;
class Manager;

//---------------------------------------------------------------------------------------//
/*! \brief  One and only instance for using login functions.

	\n This is a final class. Singleton 
	\n throws NullReferenceException */
class LoginBase
{
	friend class basar::cmnutil::Singleton<LoginBase>;	//!< friend for calling create-methods

	
public:
	/*! \brief	start login by calling login-dialog, which is created modal. (only for WIN32!)
		\n		no-throw */
    LIBBASARLOGIN_API bool interactiveLogin(	basar::I18nString & rApplName,			//!< unique applicationname to start login for
										const basar::I18nString & rWindowTitle = "",	//!< window title to show in login dialog, default empty
										const basar::Int16        maxTrials    = 3		//!< number of login trials, default 3
									);

	/*! \brief	start login without calling login-dialog
		\n		no-throw */
	LIBBASARLOGIN_API bool nonInteractiveLogin(	basar::I18nString & rApplName,			//!< unique applicationname to start login for
										const basar::I18nString & rUsername,			//!< name of user to login
										const basar::I18nString & rCryptPW,				//!< according password to username
										const basar::Int32        areaID,				//!< areaid to login
										basar::ConstString        languageID = ""		//!< language can be set (e.g. "en" / "de") because no selection without login dialog
									 );

	/*! \brief	return applicationid of current login
		\n		no-throw */
	LIBBASARLOGIN_API basar::Int32		        getApplID		()	const;
	
	/*! \brief	return applicationname of current login
		\n		no-throw */
	LIBBASARLOGIN_API const basar::I18nString&	getApplName		()	const;
	
	/*! \brief	return languageid of current login in UPPER case
		\n		no-throw */
	LIBBASARLOGIN_API const basar::I18nString	getLanguageID	()	const;
	
	/*! \brief	return languagename of current login
		\n		no-throw */
	LIBBASARLOGIN_API const basar::I18nString&	getLanguageName	()	const;
	
	/*! \brief	return areaid of current login
		\n		no-throw */
	LIBBASARLOGIN_API basar::Int16		        getAreaID		()	const;

	/*! \brief	return areaid of current login
		\n		no-throw */
	LIBBASARLOGIN_API basar::Int32		        getAreaIDAD		()	const;
	
	/*! \brief	return areaname of current login
		\n		no-throw */
	LIBBASARLOGIN_API const basar::I18nString&	getAreaName		()	const;
	
	/*! \brief	return userid of current login
		\n		no-throw */
	LIBBASARLOGIN_API basar::Int32		        getUserID		()	const;
	
	/*! \brief	return username of current login
		\n		no-throw */
	LIBBASARLOGIN_API const basar::I18nString&	getUserName		()	const; 

	/*! \brief	return usertype of current login
		\n		no-throw */
	LIBBASARLOGIN_API basar::Int32				getUserType		()	const; 
	
	/*! \brief	return description of current login
		\n		no-throw */
	LIBBASARLOGIN_API const basar::I18nString&	getDescription	()	const;
	
	/*! \brief	return surname of current login
		\n		no-throw */
	LIBBASARLOGIN_API const basar::I18nString&	getSurName		()	const;
	
	/*! \brief	return forename of current login
		\n		no-throw */
	LIBBASARLOGIN_API const basar::I18nString&	getForeName		()	const;
	
	/*! \brief	return mail of current login
		\n		no-throw */
	LIBBASARLOGIN_API const basar::I18nString&	getMail			()	const;
	
	/*! \brief	return email of current login
		\n		no-throw */
	LIBBASARLOGIN_API const basar::I18nString&	getEmail		()	const;
	
	/*! \brief	return ntuser of current login
		\n		no-throw */
	LIBBASARLOGIN_API const basar::I18nString&	getNtUser		()	const; 
	
	/*! \brief	return crypted password of current login
		\n		no-throw */
	LIBBASARLOGIN_API const basar::I18nString&	getCryptPW		()	const;
	
	

	/*! \brief	check if right exists for current login
		\n		no-throw */
	LIBBASARLOGIN_API bool isLegitimated (	const basar::Int32        moduleID,		//!< module in which right is asked for
											const basar::I18nString & rRight,		//!< rightname to ask for
											const basar::Int32        areaID		//!< areaid in which right is asked for (only necessary, if not equal to area selected at login)
										 ) const;

	/*! \brief	check if right exists for current login
		\n		no-throw */
	LIBBASARLOGIN_API bool isLegitimated (	const basar::Int32        moduleID,		//!< module in which right is asked for
											const basar::I18nString & rRight		//!< rightname to ask for
										 ) const;


	/*! \brief	ask for parameter of current login
		\n		no-throw */
	LIBBASARLOGIN_API bool	getParaValue (	const basar::I18nString & rPara1,		//!< 1st condition of parameter (not necessary if none exists)
											const basar::I18nString & rPara2,		//!< 2nd condition of parameter (not necessary if none exists)
											const basar::I18nString & rParaName,	//!< name of parameter to search for
											basar::I18nString       & rParaValue	//!< value of parameter = result
										 ) const;

	/*! \brief	ask for parameter of current login
		\n		no-throw */
	LIBBASARLOGIN_API bool	getParaValue (	const basar::I18nString & rPara1,		//!< 1st condition of parameter (not necessary if none exists)
											const basar::I18nString & rParaName,	//!< name of parameter to search for
											basar::I18nString       & rParaValue	//!< value of parameter = result
										 ) const;

	/*! \brief	ask for parameter of current login
		\n		no-throw */
	LIBBASARLOGIN_API bool	getParaValue (	const basar::I18nString & rParaName,	//!< name of parameter to search for
											basar::I18nString       & rParaValue	//!< value of parameter = result
										 ) const;

	/*! \brief	ask for collection containing all branches (branchno) in which the given user right is granted.
		\n		no-throw */
	LIBBASARLOGIN_API const CollBranchesWithRight getBranchesWithRight( const basar::Int16		  moduleID,	//!< module in which right is asked for,
																		const basar::I18nString & rRight	//!< rightname to ask for 
																	  ) const;

	
	/*! \brief	ask for collection containing all branches (ordered by name) in which the given user right is granted.
		\n		no-throw */
	LIBBASARLOGIN_API const CollBranchesWithRightOrderedByName getBranchesWithRightOrderedByName( const basar::Int16		  moduleID,	//!< module in which right is asked for,
																		const basar::I18nString & rRight	//!< rightname to ask for 
																	  ) const;

	/*! \brief	ask for collection containing all rightnames of given module and area in which are granted to the current user.
		\n		no-throw */
	LIBBASARLOGIN_API const CollRightsForModAndArea getRightsByModuleAndArea( const basar::Int16 moduleID,	//!< module in which right is asked for
																			  const basar::Int16 areaID		//!< area in which right is asked for
																			) const;

    //! \brief ask whether or not user has any rights for current application
    //! \return true if user has any rights for current application, false otherwise
    LIBBASARLOGIN_API bool hasUserAnyRights() const;

    LIBBASARLOGIN_API bool hasUserRightsForCurrentArea() const;

	/*! \brief return the pointer to the login implementation class. 
 		\n     NullReferenceException */
	const boost::shared_ptr<LoginAppl>&	getAppl() const;

	/*! \brief set connection configuration for login, if not default values should be used. 
 		\n     no-throw */
	LIBBASARLOGIN_API void setConnectionConfig		   ( const basar::VarString& strHostMain,	//!< main host name
														 const basar::VarString& strHostBackup,	//!< backup host name
														 const basar::VarString& strDbMain,		//!< main database name
														 const basar::VarString& strDbBackup	//!< backup database name
													   );

	/*! \brief get main host name, to which login is connected. 
 		\n     no-throw */
	LIBBASARLOGIN_API const basar::VarString	getHostMain()		const;
	
	/*! \brief get backup host name, to which login is connected. 
 		\n     no-throw */
	LIBBASARLOGIN_API const basar::VarString	getHostBackup()		const;
	
	/*! \brief get main database name, to which login is connected. 
 		\n     no-throw */
	LIBBASARLOGIN_API const basar::VarString	getDbMain()			const;
	
	/*! \brief get backup database name, to which login is connected. 
 		\n     no-throw */
	LIBBASARLOGIN_API const basar::VarString	getDbBackup()		const;
	
		/*! \brief set requested login mode. note: login mode defaults to known (old) behaviour 
 		\n     no-throw */
	LIBBASARLOGIN_API void setLoginMode(LoginModeEnum loginMode //!< requested login mode
	                                    );

private:
	/*! \brief forbidden std-constructor 
 		\n     no-throw */
	LIBBASARLOGIN_API					LoginBase();
	
	boost::shared_ptr<LoginAppl>		m_pLogin;			//!< pointer to login application controller
};



//---------------------------------------------------------------------------------------//
/*! \brief  derived class from LoginBase.

	This is a final class. Singleton 
 *  \note  class is necessary for dll exporting Singleton\<LoginBase\> and 
 *         instantiating Login::m_Instance */
class LIBBASARLOGIN_API Manager : public basar::cmnutil::Singleton<LoginBase>
{
private:
									/*! \brief standard constructor 
 										\n     no-throw */
									Manager    		();
									/*! \brief copy constructor 
 										\n     no-throw */
									Manager    		(const Manager     & r);
									/*! \brief assign operator 
 									\n     no-throw */
	Manager &    					operator =		(const Manager     & r);
};

//----------------------------------------------------------------------------
}	// namespace login
}	// namespace basar

//---------------------------------------------------------------------------------------//
#endif	// GUARD
