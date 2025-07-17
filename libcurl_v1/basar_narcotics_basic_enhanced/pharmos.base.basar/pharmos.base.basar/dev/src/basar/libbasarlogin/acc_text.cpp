//----------------------------------------------------------------------------
/*! \file
 *  \brief  string builder definition for text-accessor + registration of text-accessor
 *  \author Thomas Hörath
 *  \date   15.12.2005
 */
//----------------------------------------------------------------------------

#include "acc_text.h"

#include "application_definitions.h"

#include "libbasarproperty_propertydescriptionlistref.h"
#include "libbasardbaspect_accessor.h"
#include "libbasardbaspect_manager.h"
#include "libbasardbaspect_accessmethod.h"

//----------------------------------------------------------------------------
namespace acc_text
{

//---------------------------------------------------------------------------
using basar::db::aspect::SQLStringBuilder;

//---------------------------------------------------------------------------
    BUILDER_DEFINITION( SelText )

    ENSURE_PROPERTIES_ARE_SET( SelText, "applicationid; moduleid; languageid; reftext;" )

    SQL_PATTERN( SelText,
                 "SELECT text "
                 "FROM pptext "
                 "WHERE applicationid=#applicationid# and moduleid=#moduleid# and languageid='#languageid#' and reftext='#reftext#'" )

//---------------------------------------------------------------------------
    BEGIN_ACCESSOR_DEFINITION( "Text" )

    PROPERTY_DESCRIPTION_LIST( "string text;" )

    ACCESS_METHOD( domMod::text::lit::SELECT_TEXT_BY_PATTERN )
    SQL_BUILDER_CREATE( SelText )
    SQL_BUILDER_PUSH_BACK( SelText )

    END_ACCESSOR_DEFINITION

//----------------------------------------------------------------------------
}//acc_para

