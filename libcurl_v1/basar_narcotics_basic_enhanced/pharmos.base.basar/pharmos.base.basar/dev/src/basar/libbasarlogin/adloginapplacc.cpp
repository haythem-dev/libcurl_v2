#include "adloginapplacc.h"

#include "application_definitions.h"

#include "libbasarproperty_propertydescriptionlistref.h"
#include "libbasardbaspect_accessor.h"
#include "libbasardbaspect_manager.h"
#include "libbasardbaspect_accessmethod.h"

//----------------------------------------------------------------------------
namespace acc_adLoginAppl
{

//---------------------------------------------------------------------------
    using basar::db::aspect::SQLStringBuilder;

//---------------------------------------------------------------------------
    BUILDER_DEFINITION( SelApplID )

    ENSURE_PROPERTIES_ARE_SET( SelApplID, "applicationname;" )

    SQL_PATTERN( SelApplID,
                 "SELECT applicationid, appname as applicationname, description "
                 "FROM application "
                 "WHERE appname='#applicationname#'" )

//---------------------------------------------------------------------------
    BEGIN_ACCESSOR_DEFINITION( "ADLoginAppl" )

    PROPERTY_DESCRIPTION_LIST( "int32 applicationid; string applicationname; string description;string languageid;" )

    ACCESS_METHOD( domMod::application::lit::SELECT_APPLICATION_ID_BY_PATTERN )
    SQL_BUILDER_CREATE( SelApplID )
    SQL_BUILDER_PUSH_BACK( SelApplID )

    END_ACCESSOR_DEFINITION

//----------------------------------------------------------------------------
}//acc_appl

