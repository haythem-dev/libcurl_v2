#ifndef GUARD_CHECKADLOGINUC_H
#define GUARD_CHECKADLOGINUC_H

//----------------------------------------------------------------------------
#include "libbasarcmnutil_i18nstring.h"
#include "usecasecontroller.h"
#include "checkloginbaseuc.h"
#include "iaduserchecker.h"

//----------------------------------------------------------------------------
namespace basar
{
    namespace login
    {

        class IADUserChecker;

//----------------------------------------------------------------------------
//! namespace for use cases
        namespace useCase
        {

//---------------------------------------------------------------------------
            /*! \brief class for use case check login
                \throws no exceptions
                \n final class */
            class CheckADLogin : public CheckLoginBase
            {

                public:

                    /*! \brief std-constructor
                        \n no-throw */
                    CheckADLogin( Configurationptr config );

                    /*! \brief std-destructor
                        \n no-throw */
                    virtual ~CheckADLogin();

                    /*! \brief start checking login data / input
                        \n no-throw */
                    virtual bool run();

                    /*! \brief setting login mode / input
                        \n no-throw */
                    virtual void setLoginMode( bool singleSignOnRequested );

                    /*! \brief checking password empty / input
                        \n no-throw */
                    virtual bool isUserIdOrPasswordEmpty();

                    /*! \brief checking right availablity / input
                        \n no-throw */
                    virtual bool isRightAvailable();


                private:

                    /*! \brief checking login user right / input
                        \n no-throw */
                    bool hasUserRight();

                    /*! \retval basar::VarString containing users logon name */
                    basar::VarString getUserLogonName() const;

                    bool m_SingleSignOnRequested;   //! < keeping login selection
                    bool m_UserIdOrPasswordEmpty;   //! < storing password filled state
                    bool m_hasRightsForSeletedArea; //! < contains true if user has right for Selected area

                    //! AD Login Checker
                    boost::shared_ptr<basar::login::IADUserChecker>     m_ADUserChecker;

            };

//----------------------------------------------------------------------------
        } // useCase
    } // login
} // basar

//----------------------------------------------------------------------------
#endif  // GUARD
