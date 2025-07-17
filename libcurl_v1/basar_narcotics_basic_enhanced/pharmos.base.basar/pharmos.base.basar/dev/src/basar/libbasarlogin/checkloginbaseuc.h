#ifndef GUARD_CHECKLOGINBASEUC_H
#define GUARD_CHECKLOGINBASEUC_H

//----------------------------------------------------------------------------
#include "usecasecontroller.h"

#include "configurationptr.h"

#include "libbasar_definitions.h"
#include "libbasarcmnutil_bstring.h"
//----------------------------------------------------------------------------
namespace basar
{
    namespace login
    {

//----------------------------------------------------------------------------
//! namespace for use cases
        namespace useCase
        {

//---------------------------------------------------------------------------
            /*! \brief class for use case check login
                \throws no exceptions
                \n final class */
            class CheckLoginBase : public UseCaseController
            {
                public:

                    CheckLoginBase( Configurationptr config );

                    /*! \brief std-destructor
                        \n no-throw */
                    virtual ~CheckLoginBase();

                    typedef boost::shared_ptr< CheckLoginBase >   CheckLoginBaseSharedPtr;  //!< shared pointer to LoginAppl

                    /*! \brief start checking login data / input
                        \n no-throw */
                    virtual bool run() = 0;

                    /*! \brief setting login mode / input
                        \n no-throw */
                    virtual void setLoginMode( bool singleSignOnRequested );

                    /*! \brief checking userid/password empty state / input
                        \n no-throw */
                    virtual bool isUserIdOrPasswordEmpty();

                    /*! \brief checking right availablity / input
                        \n no-throw */
                    virtual bool isRightAvailable();

                    void setPassword( const basar::VarString& password );

                protected:

                    Configurationptr getConfig() const;

                    const basar::VarString getPassword() const;

                private:

                    Configurationptr m_Config;

                    basar::VarString m_Password;
            };

//----------------------------------------------------------------------------
        } // useCase
    } // login
} // basar

//----------------------------------------------------------------------------
#endif  // GUARD
