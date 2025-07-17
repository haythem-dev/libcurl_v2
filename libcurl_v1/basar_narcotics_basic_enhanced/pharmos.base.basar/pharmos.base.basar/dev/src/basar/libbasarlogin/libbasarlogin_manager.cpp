//----------------------------------------------------------------------------
/*! \file
 *  \brief  public functions login class (singleton)
 *  \author Thomas Hörath
 *  \date   12.12.2005
 */
//----------------------------------------------------------------------------

#include "libbasarlogin_manager.h"

#include "libbasarcmnutil_exceptions.h"
#include "loginappl.h"
#include "libbasarlogin_functions.h"

/*----------------------------------------------------------------------------*/
namespace basar
{

//------------------------------------------------------------------------------
    namespace cmnutil
    {

//------------------------------------------------------------------------------
        using login::LoginBase;

        /*! specialised method */
//! \return shared_ptr to LoginBase instance
        template <>
        typename Singleton<LoginBase>::InstancePtr & Singleton<LoginBase>::inst()
        {
            // avoid init order fiasco
            login::init();

            static InstancePtr s_Inst;
            return s_Inst;
        }

//------------------------------------------------------------------------------
    }

//----------------------------------------------------------------------------
    namespace login
    {

//----------------------------------------------------------------------------
        /*! \throw  NullReferenceException
            \return shared pointer to application object */
        const boost::shared_ptr<LoginAppl>& LoginBase::getAppl() const
        {
            if ( m_pLogin.get() == 0 )
            {
                ExceptInfo sInfo( "basar.login.LoginBase.getAppl", "Reference of LoginAppl is no pointer to appl-class", __FILE__, __LINE__ );
                throw NullReferenceException( sInfo );
            }
            return m_pLogin;
        }

//----------------------------------------------------------------------------
        /*! \throw  no-throw */
        LoginBase::LoginBase() : m_pLogin( LoginAppl::create() )
        {
        }

//---------------------------------------------------------------------------
        /*! \throw  no-throw
            \retval true if login was successful (username + password found)
            \return otherwise returns false */
        bool LoginBase::interactiveLogin( basar::I18nString& rApplName,
                                          const basar::I18nString& rWindowTitle,
                                          const basar::Int16 maxTrials )
        {
            return getAppl()->interactiveLogin( rApplName, rWindowTitle, maxTrials );
        }

//---------------------------------------------------------------------------
        /*! \throw  no-throw
            \retval true if login was successful (username + password found)
            \return otherwise returns false */
        bool LoginBase::nonInteractiveLogin( basar::I18nString& rApplName,
                                             const basar::I18nString& rUsername,
                                             const basar::I18nString& rCryptPW,
                                             const basar::Int32 areaID,
                                             basar::ConstString languageID )
        {
            return getAppl()->nonInteractiveLogin( rApplName, rUsername, rCryptPW, areaID, languageID );
        }
//---------------------------------------------------------------------------
        /*! \throw  no-throw
            \return applicationid */
        basar::Int32        LoginBase::getApplID        ()  const
        {
            return getAppl()->getApplID( );
        }

//---------------------------------------------------------------------------
        /*! \throw  no-throw
            \return applicationname */
        const basar::I18nString&    LoginBase::getApplName      ()  const
        {
            return getAppl()->getApplName( );
        }

//---------------------------------------------------------------------------
        /*! \throw  no-throw
            \return languageid */
        const basar::I18nString LoginBase::getLanguageID    ()  const
        {
            return getAppl()->getLanguageID( );
        }

//---------------------------------------------------------------------------
        /*! \throw  no-throw
            \return languagename */
        const basar::I18nString&    LoginBase::getLanguageName  ()  const
        {
            return getAppl()->getLanguageName( );
        }

//---------------------------------------------------------------------------
        /*! \throw  no-throw
            \return areaid */
        basar::Int16        LoginBase::getAreaID        ()  const
        {
            return getAppl()->getAreaID( );
        }

//---------------------------------------------------------------------------
        /*! \throw  no-throw
            \return areaid */
        basar::Int32        LoginBase::getAreaIDAD      ()  const
        {
            return getAppl()->getAreaIDAD( );
        }

//---------------------------------------------------------------------------
        /*! \throw  no-throw
            \return areaname */
        const basar::I18nString&    LoginBase::getAreaName      ()  const
        {
            return getAppl()->getAreaName( );
        }

//---------------------------------------------------------------------------
        /*! \throw  no-throw
            \return userid */
        basar::Int32        LoginBase::getUserID        ()  const
        {
            return getAppl()->getUserID( );
        }

//---------------------------------------------------------------------------
        /*! \throw  no-throw
            \return username */
        const basar::I18nString&    LoginBase::getUserName      ()  const
        {
            return getAppl()->getUserName( );
        }

//---------------------------------------------------------------------------
        /*! \throw  no-throw
            \return username */
        basar::Int32    LoginBase::getUserType      ()  const
        {
            return getAppl()->getUserType( );
        }

//---------------------------------------------------------------------------
        /*! \throw  no-throw
            \return description */
        const basar::I18nString&    LoginBase::getDescription   ()  const
        {
            return getAppl()->getDescription( );
        }

//---------------------------------------------------------------------------
        /*! \throw  no-throw
            \return surname */
        const basar::I18nString&    LoginBase::getSurName       ()  const
        {
            return getAppl()->getSurName( );
        }

//---------------------------------------------------------------------------
        /*! \throw  no-throw
            \return forename */
        const basar::I18nString&    LoginBase::getForeName      ()  const
        {
            return getAppl()->getForeName( );
        }

//---------------------------------------------------------------------------
        /*! \throw  no-throw
            \return mail */
        const basar::I18nString&    LoginBase::getMail          ()  const
        {
            return getAppl()->getMail( );
        }

//---------------------------------------------------------------------------
        /*! \throw  no-throw
            \return email */
        const basar::I18nString&    LoginBase::getEmail     ()  const
        {
            return getAppl()->getEmail( );
        }

//---------------------------------------------------------------------------
        /*! \throw  no-throw
            \return ntuser */
        const basar::I18nString&    LoginBase::getNtUser        ()  const
        {
            return getAppl()->getNtUser( );
        }

//---------------------------------------------------------------------------
        /*! \throw  no-throw
            \return crypted password */
        const basar::I18nString&    LoginBase::getCryptPW       ()  const
        {
            return getAppl()->getCryptPW( );
        }

//---------------------------------------------------------------------------
        /*! \throw  no-throw
            \retval true if right exists
            \return otherwise returns false */
        bool LoginBase::isLegitimated   ( const basar::Int32 moduleID,
                                          const basar::I18nString& rRight,
                                          const basar::Int32 areaID
                                        ) const
        {
            return getAppl()->isLegitimated( moduleID, rRight, areaID );
        }

//---------------------------------------------------------------------------
        /*! \throw  no-throw
            \retval true if right exists
            \return otherwise returns false */
        bool LoginBase::isLegitimated   ( const basar::Int32 moduleID,
                                          const basar::I18nString& rRight
                                        ) const
        {
            return getAppl()->isLegitimated( moduleID, rRight );
        }

//---------------------------------------------------------------------------
        /*! \throw  no-throw
            \retval true if parameter exists
            \return otherwise returns false */
        bool LoginBase::getParaValue    ( const basar::I18nString& rPara1,
                                          const basar::I18nString& rPara2,
                                          const basar::I18nString& rParaName,
                                          basar::I18nString& rParaValue
                                        ) const
        {
            return getAppl()->getParaValue( rPara1, rPara2, rParaName, rParaValue );
        }

//---------------------------------------------------------------------------
        /*! \throw  no-throw
            \retval true if parameter exists
            \return otherwise returns false */
        bool LoginBase::getParaValue    ( const basar::I18nString& rPara1,
                                          const basar::I18nString& rParaName,
                                          basar::I18nString& rParaValue
                                        ) const
        {
            return getAppl()->getParaValue( rPara1, rParaName, rParaValue );
        }

//---------------------------------------------------------------------------
        /*! \throw  no-throw
            \retval true if parameter exists
            \return otherwise returns false */
        bool LoginBase::getParaValue    ( const basar::I18nString& rParaName,
                                          basar::I18nString& rParaValue
                                        ) const
        {
            return getAppl()->getParaValue( rParaName, rParaValue );
        }

//---------------------------------------------------------------------------
        /*! \throw  no-throw
            \return collection with branch numbers */
        const CollBranchesWithRight LoginBase::getBranchesWithRight( const basar::Int16 moduleID, const basar::I18nString& rRight ) const
        {
            return getAppl()->getBranchesWithRight( moduleID, rRight );
        }

//---------------------------------------------------------------------------
        /*! \throw  no-throw
            \return collection with branch names */
        const CollBranchesWithRightOrderedByName LoginBase::getBranchesWithRightOrderedByName( const basar::Int16 moduleID, const basar::I18nString& rRight ) const
        {
            return getAppl()->getBranchesWithRightOrderedByName( moduleID, rRight );
        }

//---------------------------------------------------------------------------
        /*! \throw  no-throw
            \return collection with right names */
        const CollRightsForModAndArea LoginBase::getRightsByModuleAndArea(  const basar::Int16 moduleID, const basar::Int16 areaID  ) const
        {
            return getAppl()->getRightsByModuleAndArea( moduleID, areaID );
        }

//---------------------------------------------------------------------------

        bool LoginBase::hasUserAnyRights() const
        {
            return getAppl()->hasUserAnyRights();
        }

//---------------------------------------------------------------------------

        bool LoginBase::hasUserRightsForCurrentArea() const
        {
            return getAppl()->hasUserRightsForCurrentArea();
        }

//---------------------------------------------------------------------------
//! \throw      no-throw
        void LoginBase::setConnectionConfig     ( const basar::VarString& strHostMain,
                const basar::VarString& strHostBackup,
                const basar::VarString& strDbMain,
                const basar::VarString& strDbBackup )
        {
            return getAppl()->setConnectionConfig( strHostMain, strHostBackup, strDbMain, strDbBackup );
        }

//---------------------------------------------------------------------------
        /*! \throw      no-throw
            \return     main host name */
        const basar::VarString  LoginBase::getHostMain()        const
        {
            return getAppl()->getHostMain();
        }

//---------------------------------------------------------------------------
        /*! \throw      no-throw
            \return     backup host name */
        const basar::VarString  LoginBase::getHostBackup()      const
        {
            return getAppl()->getHostBackup();
        }

//---------------------------------------------------------------------------
        /*! \throw      no-throw
            \return     main database name */
        const basar::VarString  LoginBase::getDbMain()          const
        {
            return getAppl()->getDbMain();
        }

//---------------------------------------------------------------------------
        /*! \throw      no-throw
            \return     backup database name */
        const basar::VarString  LoginBase::getDbBackup()        const
        {
            return getAppl()->getDbBackup();
        }

//---------------------------------------------------------------------------
        /*! \throw      no-throw
        */
        void LoginBase::setLoginMode( LoginModeEnum loginMode )
        {
            getAppl()->setLoginMode( loginMode );
        }

//----------------------------------------------------------------------------
    }   // namespace login
}   // namespace basar

