//--------------------------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  string builder definition for user-accessor + registration of user-accessor
 *  \author Bjoern Bischof
 *  \date   09.05.2006
 */
//--------------------------------------------------------------------------------------------------------------------//

//--------------------------------------------------------------------------------------------------------------------//
// include
//--------------------------------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "acc_locknarcbooking.h"

//--------------------------------------------------------------------------------------------------------------------//
// namespace section
//--------------------------------------------------------------------------------------------------------------------//
namespace acc_locknarcbooking
{

//--------------------------------------------------------------------------------------------------------------------//
// builder definitions
//--------------------------------------------------------------------------------------------------------------------//
BUILDER_DEFINITION( SelLock )
	ENSURE_PROPERTIES_ARE_SET( SelLock, "branchno;idbusinessobject;businesstype;" )
	SQL_PATTERN( SelLock,	"SELECT count(*) AS cntlocks "
							"FROM   narclock "
							"WHERE  branchno  =#branchno# "
							"AND	businesstype ='#businesstype#' "
							"AND	idbusinessobject =#idbusinessobject# ")

//--------------------------------------------------------------------------------------------------------------------//
// accessor defintion
//--------------------------------------------------------------------------------------------------------------------//
BEGIN_ACCESSOR_DEFINITION( "Locks" )

	PROPERTY_DESCRIPTION_LIST( "int16 branchno; int32 idbusinessobject; string username; int32 lockdate; int32 locktime; decimal cntlocks; string businesstype" )
	ACCESS_METHOD( "SelLock" )
		SQL_BUILDER_CREATE( SelLock )
		SQL_BUILDER_PUSH_BACK( SelLock )


END_ACCESSOR_DEFINITION
											
} //namespace acc_locknarcbooking