//----------------------------------------------------------------------------
/*! \file
 *  \brief  build SQL - statements implementation
 *  \author Roland Kiefert
 *  \date   14.07.2005
 */
//----------------------------------------------------------------------------

#include "sqlstringbuilderimpl.h"
#include "libbasarcmnutil_bstring.h"
/*--------------------------------------------------------------------------------------------------*/
namespace basar
{
    namespace db
    {
        namespace aspect
        {

            SQLStringBuilderImpl::~SQLStringBuilderImpl()
            {
            }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
            /*! \throw UnknownPropertyNameException
                \param rPattern string which is interpreted in this function */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
            void SQLStringBuilderImpl::resolve( const VarString& rPattern )
            {
                VarString own = rPattern;
                VarString::size_type beginPos = 0;
                VarString::size_type endPos = 0;
                VarString help;

                while ( endPos != VarString::npos && beginPos < own.size() )
                {
                    help = SQLStringBuilderBaseImpl::getSubstringToNextMaskString( SQLStringBuilderBaseImpl::scm_maskStartString, own, beginPos, endPos );
                    SQLStringBuilderBaseImpl::literal( help );

                    if ( endPos != VarString::npos )
                    {
                        beginPos = endPos + SQLStringBuilderBaseImpl::scm_maskStartString.length();

                        if ( beginPos < own.length() )
                        {
                            help = SQLStringBuilderBaseImpl::getSubstringToNextMaskString( SQLStringBuilderBaseImpl::scm_maskEndString, own, beginPos, endPos );
                            value( help );

                            if ( endPos != VarString::npos )
                            {
                                beginPos = endPos + SQLStringBuilderBaseImpl::scm_maskEndString.length();
                            }
                        }
                    }
                }
            }

        }//aspect
    }//db
}//basar
