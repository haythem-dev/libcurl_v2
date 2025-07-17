#include "adloginlanguageacc.h"

#include "application_definitions.h"

#include "libbasarproperty_propertydescriptionlistref.h"
#include "libbasardbaspect_accessor.h"
#include "libbasardbaspect_manager.h"
#include "libbasardbaspect_accessmethod.h"

//----------------------------------------------------------------------------
namespace acc_adloginlanguage
{

//---------------------------------------------------------------------------
    using basar::db::aspect::SQLStringBuilder;

//---------------------------------------------------------------------------
    BUILDER_DEFINITION( SelLanguage )

    ENSURE_PROPERTIES_ARE_SET( SelLanguage, "applicationid;" )

    SQL_PATTERN( SelLanguage,
                 "SELECT alm.languagemapid as languagemapid, lg.languagename as languagename, "
                 "lg.languageid as languageid "
                 "FROM language lg , applanguagemap alm "
                 "WHERE alm.applicationid =#applicationid# and lg.languagemapid=alm.languagemapid " )

//---------------------------------------------------------------------------
    BUILDER_DEFINITION( SelAppLanguage )

    ENSURE_PROPERTIES_ARE_SET( SelAppLanguage, "applicationid;languageid;" )

    SQL_PATTERN( SelAppLanguage,

                 "SELECT alm.languagemapid as languagemapid, lg.languagename as languagename, "
                 "lg.languageid as languageid "
                 "FROM language lg , applanguagemap alm "
                 "WHERE alm.applicationid =#applicationid# and lg.languagemapid=alm.languagemapid "
                 "and lg.languageid ='#languageid#'" )

//---------------------------------------------------------------------------
    BEGIN_ACCESSOR_DEFINITION( "ADLoginLanguage" )

    PROPERTY_DESCRIPTION_LIST( "string languageid; string languagename; int32 languagemapid;" )

    ACCESS_METHOD( domMod::language::lit::SELECT_LANGUAGE_BY_APPLICATION_ID )
    SQL_BUILDER_CREATE( SelLanguage )
    SQL_BUILDER_PUSH_BACK( SelLanguage )

    ACCESS_METHOD( domMod::language::lit::SELECT_APPLICATION_LANGUAGE )
    SQL_BUILDER_CREATE( SelAppLanguage )
    SQL_BUILDER_PUSH_BACK( SelAppLanguage )

    END_ACCESSOR_DEFINITION

//----------------------------------------------------------------------------
}//acc_language

