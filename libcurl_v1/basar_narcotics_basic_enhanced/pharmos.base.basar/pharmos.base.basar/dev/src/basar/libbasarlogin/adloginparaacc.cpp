#include "adloginparaacc.h"

#include "application_definitions.h"

#include "libbasarproperty_propertydescriptionlistref.h"
#include "libbasardbaspect_accessor.h"
#include "libbasardbaspect_manager.h"
#include "libbasardbaspect_accessmethod.h"

//----------------------------------------------------------------------------
namespace acc_adloginpara
{
    //---------------------------------------------------------------------------
    using basar::db::aspect::SQLStringBuilder;

    //---------------------------------------------------------------------------
    BUILDER_DEFINITION( SelPara )

    ENSURE_PROPERTIES_ARE_SET( SelPara, "applicationid;" )

    SQL_PATTERN( SelPara,
                 "SELECT applicationid, firstcondition as para1, secondcondition as para2, "
                 "parametername as parabez,parametervalue as paravalue "
                 "FROM applicationparameter "
                 "WHERE applicationid=#applicationid#" )

    BUILDER_DEFINITION( SelParaByName )

    ENSURE_PROPERTIES_ARE_SET( SelParaByName, "applicationname;" )

    SQL_PATTERN( SelParaByName,
                 "SELECT A.applicationid, AP.firstcondition as para1, AP.secondcondition as para2, "
                 "AP.parametername as parabez, AP.parametervalue as paravalue "
                 "FROM applicationparameter as AP, application as A "
                 "WHERE A.appname = '#applicationname#' AND AP.applicationid = A.applicationid " )

    //---------------------------------------------------------------------------
    BEGIN_ACCESSOR_DEFINITION( "ADLoginPara" )

    PROPERTY_DESCRIPTION_LIST( "int32 applicationid; string para1; string para2; string parabez; string paravalue;" )

    ACCESS_METHOD( domMod::parameter::lit::SELECT_PARAMETER_BY_APPLICATION_ID )
    SQL_BUILDER_CREATE( SelPara )
    SQL_BUILDER_PUSH_BACK( SelPara )

    ACCESS_METHOD( domMod::parameter::lit::SELECT_PARAMETER_BY_APPLICATION_NAME )
    SQL_BUILDER_CREATE( SelParaByName )
    SQL_BUILDER_PUSH_BACK( SelParaByName )
    END_ACCESSOR_DEFINITION
    //----------------------------------------------------------------------------
}//acc_para