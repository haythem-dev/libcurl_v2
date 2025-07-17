//----------------------------------------------------------------------------
/*! \file
 *  \brief  string builder definition for parameter-accessor + registration of parameter-accessor
 *  \author Thomas Hörath
 *  \date   15.12.2005
 */
//----------------------------------------------------------------------------

#include "acc_para.h"

#include "application_definitions.h"

#include "libbasarproperty_propertydescriptionlistref.h"
#include "libbasardbaspect_accessor.h"
#include "libbasardbaspect_manager.h"
#include "libbasardbaspect_accessmethod.h"

//----------------------------------------------------------------------------
namespace acc_para
{

//---------------------------------------------------------------------------
using basar::db::aspect::SQLStringBuilder;

//---------------------------------------------------------------------------
    BUILDER_DEFINITION( SelPara )

    ENSURE_PROPERTIES_ARE_SET( SelPara, "applicationid;" )

    SQL_PATTERN( SelPara,
                 "SELECT applicationid, para1, para2, parabez, paravalue "
                 "FROM pplogpara "
                 "WHERE applicationid=#applicationid#" )

//---------------------------------------------------------------------------
    BEGIN_ACCESSOR_DEFINITION( "Para" )

    PROPERTY_DESCRIPTION_LIST( "int16 applicationid; string para1; string para2; string parabez; string paravalue;" )

    ACCESS_METHOD( domMod::parameter::lit::SELECT_PARAMETER_BY_APPLICATION_ID )
    SQL_BUILDER_CREATE( SelPara )
    SQL_BUILDER_PUSH_BACK( SelPara )

    END_ACCESSOR_DEFINITION

//----------------------------------------------------------------------------
}//acc_para
