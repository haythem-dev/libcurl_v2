//----------------------------------------------------------------------------
/*! \file
 *  \brief  object holding all login-specific data
 *  \author Thomas Hörath
 *  \date   13.12.2005
 */
//----------------------------------------------------------------------------
#ifndef GUARD_LOGINDATA_H
#define GUARD_LOGINDATA_H

#include <map>
#include <set>
#include <boost/shared_ptr.hpp>
#include "libbasar_definitions.h"
#include "libbasarcmnutil_i18nstring.h"
#include "loginusertype.h"

//----------------------------------------------------------------------------
namespace basar {

	//-----------------------------------------------------------------------
	namespace db
	{
		namespace aspect
		{
			class AccessorPropertyTableRef;
			class AccessorPropertyTable_YIterator;
		}
	}

namespace login	{

// --- typedef section ---------------------------------------------------

//! \brief definition for collection of rights, containing all rights from ppappright for this appl + user
//! \n     key  : string in format moduleID.areaID.rightName
typedef std::set < basar::I18nString >	CollRights;

//! \brief definition for collection of parameter, containing all parameters for this appl
//! \n     key  : string in format para1_para2_parabez (para1 and para2 may be 0)
//! \n     value: string containing paravalue
typedef std::map < basar::I18nString, basar::I18nString >	CollParamter;

//! \brief definition for collection of branches, containing all branches which are stored to the application the login is running for
//! \n     key  : small int - areaid's
//! \n     value: string - areaname
typedef std::map < basar::Int32, basar::I18nString >	CollBranches;

//---------------------------------------------------------------------------
/*! \brief class containing all login data - filled after successful login, managed by LoginAppl (m_CollLoginData)
	\n final class 
	\n throws no exceptions */
class LoginData
{
friend class LoginAppl;	//!< one and only class to access private members.
friend class LoginApplWin;	//!< one and only class to access private members.
friend class LoginApplUnix;	//!< one and only class to access private members.

public:	
	/*! \brief std-destructor
		\n no-throw */
	virtual ~LoginData();

	/*! \brief create a LoginData object. 
 		\n     no-throw */
	static boost::shared_ptr< LoginData > create();

	/*! \brief write rights from propertytable in m_CollRights
 		\n     no-throw */
	void insertRights		(basar::db::aspect::AccessorPropertyTableRef applRights, //!< propertytable containing all rights of a user and an application
							 const bool ADLogin //!< todo
							);

	/*! \brief search for given right in m_CollRights
 		\n     no-throw */
	bool findRight	(const basar::Int32 moduleID,				//!< module where right is searched
					 const basar::I18nString& rRight,			//!< rightname to ask for
					 const basar::Int32 areaID					//!< areaid where right is searched
				    ) const;

	/*! \brief write parameters from propertytable in m_CollPara
 		\n     no-throw */
	void insertParameters	(basar::db::aspect::AccessorPropertyTableRef parameters //!< propertytable containing all parameters of an application
							);

	/*! \brief search for given parameter in m_CollPara
 		\n     no-throw */
	bool findParameter(	const basar::I18nString& rPara1,	//!< 1st condition of parameter
						const basar::I18nString& rPara2,	//!< 2nd condition of parameter
						const basar::I18nString& rParaName,//!< name of parameter to search for 
						      basar::I18nString& value			//!< value of parameter = result
					  ) const;
	
	/*! \brief write user data to member variables
 		\n     no-throw */
	void insertUserData		(const basar::db::aspect::AccessorPropertyTable_YIterator userData,	//!< iterator containing user data
							 const bool ADLogin //!< todo
							);

	/*! \brief write application data to member variables
 		\n     no-throw */
	void insertApplData		(const basar::db::aspect::AccessorPropertyTable_YIterator applData	//!< iterator containing application data
							);

	/*! \brief write language + area data to member variables
 		\n     no-throw */
	void insertLangAreaData (const basar::db::aspect::AccessorPropertyTable_YIterator languageAreaData, //!< iterator containing language + area data
							 const bool ADLogin //!< todo
							);

	/*! \brief write areas data to member variables
 		\n     no-throw */
	void insertBranches		(basar::db::aspect::AccessorPropertyTableRef branches,	//!< propertytable containing all areas of an application
							 const bool ADLogin //!< todo
							);

	/*! \brief return all areas for the current application
 		\n     no-throw */
	const CollBranches&		getAllAreas() const;

	/*! \brief return all rights for the current user
 		\n     no-throw */
	const CollRights&		getAllRights() const;

	/*! \brief format key for right collection
 		\n     no-throw */
	const basar::I18nString formatRightEntry	(const basar::Int32 moduleID,		//!< module in where right is searched
												 const basar::I18nString& rRight,	//!< rightname to ask for
												 const basar::Int32 areaID			//!< areaid where right is searched
												 ) const;

    /*! \brief returns true if current user has rights for current area
        \n no-throw */
    bool hasUserRightsForCurrentArea() const;

    /*! \brief returns true if current user any rights at all
        \n no-throw */
    bool hasUserAnyRights() const;

private:
	LoginData(); //!< forbidden constructor
	
	/*! \brief format key for parameter collection
 		\n     no-throw */
	const basar::I18nString formatParaKey		(const basar::I18nString& rPara1,	//!< 1st condition of parameter
												 const basar::I18nString& rPara2,	//!< 2nd condition of parameter
												 const basar::I18nString& rParaName	//!< name of parameter to search for 
												 ) const;
	
	basar::Int32		m_ApplID;		    //!< application id
	basar::I18nString	m_ApplName;		    //!< applicationname
	basar::I18nString	m_LanguageID;	    //!< language id
	basar::Int32		m_LanguageMapID;    //!< Variable for languagemapid
	basar::I18nString	m_LanguageName;	    //!< languagename
	basar::Int16		m_AreaID;		    //!< areaid
	basar::Int32		m_AreaIDAD;		    //!< areaid
	basar::I18nString	m_AreaName;		    //!< areaname
	basar::Int32		m_UserID;		    //!< userid
	basar::I18nString	m_UserName;		    //!< username
	basar::I18nString	m_SurName;		    //!< surname
	basar::I18nString	m_ForeName;		    //!< forename
	basar::I18nString	m_Mail;			    //!< mail
	basar::I18nString	m_Email;		    //!< email
	basar::I18nString	m_NtUser;		    //!< ntuser
	basar::I18nString	m_CryptPW;		    //!< crypted password
	basar::I18nString	m_Description;	    //!< description
	
	CollRights			m_CollRights;	    //!< collection of rights for user + application
	CollParamter		m_CollPara;		    //!< collection of parameters for applictaion
	CollBranches		m_CollBranches;	    //!< collection of branches for applictaion

	basar::login::UserTypeEnum		m_UserType;		//!< usertype

};

//----------------------------------------------------------------------------
} // login
} // basar

//----------------------------------------------------------------------------
#endif	// GUARD
