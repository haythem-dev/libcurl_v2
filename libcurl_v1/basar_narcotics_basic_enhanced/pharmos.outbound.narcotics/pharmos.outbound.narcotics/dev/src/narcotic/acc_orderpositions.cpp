//----------------------------------------------------------------------------
/*! \file
 *  \brief  string builder definition for order accessor
 *  \author Harald Bayer
 *  \date   02.082012
 */
//-------------------------------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "acc_orderpositions.h"

#include "loggerpool.h"

//-------------------------------------------------------------------------------------------------------------------------//
// namespace section
//-------------------------------------------------------------------------------------------------------------------------//
namespace acc_orderpositions {

//---------------------------------------------------------------------------
BUILDER_DEFINITION(SelOrderPositionsByPattern)
	
ENSURE_PROPERTIES_ARE_SET(SelOrderPositionsByPattern,"branchno;transactiondate;purchaseorderno;customersupplierno")

void SelOrderPositionsByPattern::buildSQLString()
{
	resolve( 	
			" Select catalogid, articleno, branchno, transactiondate, transactiontime from narctransactioncatalog"
			" where branchno = #branchno#"
			" AND purchaseorderno = #purchaseorderno#"
			" AND transactiondate = #transactiondate#"
			" AND customersupplierno = #customersupplierno#"
			" AND deletedflag = '0' "
		);

	BLOG_TRACE( narcotics::LoggerPool::loggerDomModules, getSQLString() );
}

//---------------------------------------------------------------------------
BUILDER_DEFINITION(DelOrderPositionsBySelection)

ENSURE_PROPERTIES_ARE_SET(DelOrderPositionsBySelection, "catalogid")

void DelOrderPositionsBySelection::buildSQLString()
{
	
	resolve(
		"Update narctransactioncatalog set deletedflag = '1'"
		" where catalogid = #catalogid# "
		);
	BLOG_TRACE( narcotics::LoggerPool::loggerDomModules, getSQLString() );
}

//---------------------------------------------------------------------------
BEGIN_ACCESSOR_DEFINITION("NarcoticsOrdersPositions")

	PROPERTY_DESCRIPTION_LIST("int16 branchno; int32 articleno; int32 catalogid; int32 transactiondate; int32 transactiontime;"
								"int32 purchaseorderno; int32 supplierno; string suppliername; decimal qtyorder;"
								"int32 bgano; string unity; string selection; int32 contributionvoucherno; int32 customersupplierno;"
								)

	ACCESS_METHOD("SelOrderPositionsByPattern")
		SQL_BUILDER_CREATE(SelOrderPositionsByPattern)
		SQL_BUILDER_PUSH_BACK(SelOrderPositionsByPattern)

	ACCESS_METHOD("DelOrderPositionsBySelection")
		SQL_BUILDER_CREATE(DelOrderPositionsBySelection)
		SQL_BUILDER_PUSH_BACK(DelOrderPositionsBySelection)

END_ACCESSOR_DEFINITION
				
} //namespace acc_narcoticssupplier