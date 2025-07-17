//----------------------------------------------------------------------------
/*! \file
 *  \brief  use case declarations for checking login data
 *  \author Thomas Hörath
 *  \date   19.12.2005
 */
//----------------------------------------------------------------------------

#ifndef GUARD_CHECKLOGINUC_H
#define GUARD_CHECKLOGINUC_H

//----------------------------------------------------------------------------
#include "libbasarcmnutil_i18nstring.h"
#include "usecasecontroller.h"
#include "checkloginbaseuc.h"
#include "configurationptr.h"
//----------------------------------------------------------------------------
namespace basar {
namespace login	{

//----------------------------------------------------------------------------
//! namespace for use cases
namespace useCase {

//---------------------------------------------------------------------------
            /*! \brief class for use case check login
                \throws no exceptions
                \n final class */
            class CheckLogin : public CheckLoginBase
            {
                public:

                    /*! \brief std-constructor
                        \n no-throw */
                    CheckLogin( Configurationptr config );

                    /*! \brief std-destructor
                        \n no-throw */
                    virtual ~CheckLogin();


                    /*! \brief start checking login data / input
                        \n no-throw */
                    virtual bool run();

};

//----------------------------------------------------------------------------
} // useCase
} // login
} // basar

//----------------------------------------------------------------------------
#endif	// GUARD
