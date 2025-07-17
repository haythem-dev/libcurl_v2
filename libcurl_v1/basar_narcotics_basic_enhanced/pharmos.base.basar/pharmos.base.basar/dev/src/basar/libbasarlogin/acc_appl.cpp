//----------------------------------------------------------------------------
/*! \file
 *  \brief  string builder definition for appl-accessor + registration of appl-accessor
 *  \author Thomas Hörath
 *  \date   15.12.2005
 */
//----------------------------------------------------------------------------

#include "acc_appl.h"

#include "application_definitions.h"

#include "libbasarproperty_propertydescriptionlistref.h"
#include "libbasardbaspect_accessor.h"
#include "libbasardbaspect_manager.h"
#include "libbasardbaspect_accessmethod.h"

//----------------------------------------------------------------------------
namespace acc_appl
{

//---------------------------------------------------------------------------
using basar::db::aspect::SQLStringBuilder;

//---------------------------------------------------------------------------
    BUILDER_DEFINITION( SelApplID )

    ENSURE_PROPERTIES_ARE_SET( SelApplID, "applicationname;" )

    SQL_PATTERN( SelApplID,
                 "SELECT applicationid, name as applicationname, description "
                 "FROM ppapplication "
                 "WHERE name='#applicationname#'" )

//---------------------------------------------------------------------------
    BEGIN_ACCESSOR_DEFINITION( "Appl" )

    PROPERTY_DESCRIPTION_LIST( "int32 applicationid; string applicationname; string description;" )

    ACCESS_METHOD( domMod::application::lit::SELECT_APPLICATION_ID_BY_PATTERN )
    SQL_BUILDER_CREATE( SelApplID )
    SQL_BUILDER_PUSH_BACK( SelApplID )

    END_ACCESSOR_DEFINITION

//----------------------------------------------------------------------------
}//acc_appl 

