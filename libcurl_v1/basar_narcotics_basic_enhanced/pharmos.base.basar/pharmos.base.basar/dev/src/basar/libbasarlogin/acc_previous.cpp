//----------------------------------------------------------------------------
/*! \file
 *  \brief  string builder definition for previous-accessor + registration of previous-accessor
 *  \author Thomas Hörath
 *  \date   15.12.2005
 */
//----------------------------------------------------------------------------

#include "acc_previous.h"

#include "application_definitions.h"

#include "libbasarproperty_propertydescriptionlistref.h"
#include "libbasarproperty_propertystateset.h"
#include "libbasardbaspect_accessor.h"
#include "libbasardbaspect_accessorpropertytable_yiterator.h"
#include "libbasardbaspect_manager.h"
#include "libbasardbaspect_accessmethod.h"

//----------------------------------------------------------------------------
namespace acc_previous
{

//---------------------------------------------------------------------------
using basar::db::aspect::SQLStringBuilder;

//---------------------------------------------------------------------------
    BUILDER_DEFINITION( SelPrevious )

    ENSURE_PROPERTIES_ARE_SET( SelPrevious, "applicationid; globaluser;" )

    SQL_PATTERN( SelPrevious,
                 "SELECT applicationid, globaluser, userid, languageid, branchregionid "
                 "FROM pploginprevious "
                 "WHERE applicationid=#applicationid# and lower(globaluser)='#globaluser#'" )

//---------------------------------------------------------------------------
//! \return share_ptr to SQLStringBuilder
    const boost::shared_ptr<SQLStringBuilder> SavePrevious::create() const
    {
        return boost::shared_ptr<SQLStringBuilder>( new SavePrevious );
    }

//! \return bool indicating statement is executeable
    bool SavePrevious::isExecutable() const
    {
        basar::db::aspect::PropertyStateSet pss ( basar::SS_UPDATE );
        pss.insert( basar::SS_INSERT );
        return arePropertiesSetAndOneMustBeInState( "applicationid; globaluser; userid; languageid; branchregionid;", pss );
    }

//! \ingroup SQLSTMT
//! update-statement of SQLStringBuilder "SavePrevious".
    basar::ConstString SavePrevious::s_SqlStmt_Upd =
        "UPDATE pploginprevious SET userid=#userid#, languageid='#languageid#', branchregionid=#branchregionid# "
        "WHERE applicationid=#applicationid# and globaluser='#globaluser#'";

//! \ingroup SQLSTMT
//! insert-statement of SQLStringBuilder "SavePrevious".
    basar::ConstString SavePrevious::s_SqlStmt_Ins =
        "INSERT INTO pploginprevious (applicationid, globaluser, userid, languageid, branchregionid) "
        "VALUES (#applicationid#, '#globaluser#', #userid#, '#languageid#', #branchregionid#)";

    void SavePrevious::buildSQLString()
    {
        if ( getPropertyList().getState() == basar::SS_UPDATE )
        {
            resolve( s_SqlStmt_Upd );
        }
        else
        {
            resolve( s_SqlStmt_Ins );
        }
    }

//---------------------------------------------------------------------------
    BEGIN_ACCESSOR_DEFINITION( "Previous" )

    PROPERTY_DESCRIPTION_LIST( "int16 applicationid; string globaluser; int16 userid; string languageid; int16 branchregionid; "
                               "string languagename; string areaname;" )

    ACCESS_METHOD( domMod::previous::lit::SELECT_PREVIOUS_BY_PATTERN )
    SQL_BUILDER_CREATE( SelPrevious )
    SQL_BUILDER_PUSH_BACK( SelPrevious )

    ACCESS_METHOD( domMod::previous::lit::SAVE_PREVIOUS )
    SQL_BUILDER_CREATE( SavePrevious )
    SQL_BUILDER_PUSH_BACK( SavePrevious )

    END_ACCESSOR_DEFINITION

//----------------------------------------------------------------------------
}//acc_previous
