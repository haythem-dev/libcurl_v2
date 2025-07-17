//----------------------------------------------------------------------------
/*! \file
 *  \brief  string builder definition for language-accessor + registration of language-accessor
 *  \author Thomas Hörath
 *  \date   15.12.2005
 */
//----------------------------------------------------------------------------

#include "acc_language.h"

#include "application_definitions.h"

#include "libbasarproperty_propertydescriptionlistref.h"
#include "libbasardbaspect_accessor.h"
#include "libbasardbaspect_manager.h"
#include "libbasardbaspect_accessmethod.h"

//----------------------------------------------------------------------------
namespace acc_language
{

//---------------------------------------------------------------------------
using basar::db::aspect::SQLStringBuilder;

//---------------------------------------------------------------------------
    BUILDER_DEFINITION( SelLanguage )

    ENSURE_PROPERTIES_ARE_SET( SelLanguage, "applicationid;" )

    SQL_PATTERN( SelLanguage,
                 "SELECT l.languageid as languageid, l.name as languagename "
                 "FROM pploginlanguage l, ppcorapplang c "
                 "WHERE c.applicationid=#applicationid# and c.languageid=l.languageid" )


//---------------------------------------------------------------------------
    BEGIN_ACCESSOR_DEFINITION( "Language" )

    PROPERTY_DESCRIPTION_LIST( "string languageid; string languagename;" )

    ACCESS_METHOD( domMod::language::lit::SELECT_LANGUAGE_BY_APPLICATION_ID )
    SQL_BUILDER_CREATE( SelLanguage )
    SQL_BUILDER_PUSH_BACK( SelLanguage )

    END_ACCESSOR_DEFINITION

//----------------------------------------------------------------------------
}//acc_language

