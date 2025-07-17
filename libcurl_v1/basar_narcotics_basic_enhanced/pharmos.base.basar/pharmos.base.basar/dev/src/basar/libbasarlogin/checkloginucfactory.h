#ifndef GUARD_CHECKLOGINUCFACTORY_H
#define GUARD_CHECKLOGINUCFACTORY_H

//----------------------------------------------------------------------------
#include <boost/shared_ptr.hpp>
#include "checkloginbaseuc.h"
#include "configurationptr.h"
#include "configuration.h"

//----------------------------------------------------------------------------
namespace basar
{
    namespace login
    {

//---------------------------------------------------------------------------
        /*! \brief class for use case configuration; handling standard and backup connect
            \n throws BasarException
            \n final class */
        class CheckLoginUCFactory
        {
            public:

                typedef boost::shared_ptr< CheckLoginUCFactory >   CheckLoginUCFactoryPtr;

                /*! \brief std-constructor
                    \n no-throw */
                CheckLoginUCFactory( basar::login::Configurationptr );

                /*! \brief std-destructor
                    \n no-throw */
                ~CheckLoginUCFactory();


                useCase::CheckLoginBase::CheckLoginBaseSharedPtr            getCheckLoginUC();

                void                                                        reset();

            private:

                basar::login::Configurationptr              m_ConfigurationPtr;

                useCase::CheckLoginBase::CheckLoginBaseSharedPtr            m_CheckLoginUCPtr;
        };

    }
} // basar

//----------------------------------------------------------------------------
#endif  // GUARD
