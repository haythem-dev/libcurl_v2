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
#include "acc_order.h"
#include "loggerpool.h"

//-------------------------------------------------------------------------------------------------------------------------//
// namespace section
//-------------------------------------------------------------------------------------------------------------------------//
namespace acc_order 
{

//---------------------------------------------------------------------------
BUILDER_DEFINITION(DelOrderByPattern)
	
ENSURE_PROPERTIES_ARE_SET(DelOrderByPattern,"branchno;transactiondate;purchaseorderno;contributionvoucherno")

void DelOrderByPattern::buildSQLString()
{
	resolve( 	
			" Update narctransactionCatalog set deletedflag = '1'"
			" where branchno = #branchno#"
			" AND purchaseorderno = #purchaseorderno#"
			" AND transactiondate = #transactiondate#"
			" AND contributionvoucherno = #contributionvoucherno# "
		);

	BLOG_TRACE( narcotics::LoggerPool::loggerDomModules, getSQLString() );
}

//---------------------------------------------------------------------------
BUILDER_DEFINITION(UpdateOrderAckStatusByPatternSetToForPrint)

ENSURE_PROPERTIES_ARE_SET(UpdateOrderAckStatusByPatternSetToForPrint, "branchno;transactiondate;purchaseorderno")

void UpdateOrderAckStatusByPatternSetToForPrint::buildSQLString()
{
	resolve(
		"Update narctransactioncatalog set acknowledgementstatus = 1"
			" where branchno = #branchno#"
			" AND purchaseorderno = #purchaseorderno#"
			" AND transactiondate = #transactiondate#"
		);
	BLOG_TRACE( narcotics::LoggerPool::loggerDomModules, getSQLString() );
}

//-------------------------------------------------------------------------------------------------------//
BUILDER_DEFINITION( SelQuickPreviousBooking )
	ENSURE_PROPERTIES_ARE_SET( SelQuickPreviousBooking, "branchno; articleno; transactiondate; transactiontime;" )

	void SelQuickPreviousBooking::buildSQLString()
	{
		resolve("SELECT FIRST 1 qtyinstock "
				"FROM narctransactioncatalog, (select max(transactiondate) as maxdate from narctransactioncatalog "
                                               "where branchno = #branchno# and articleno = #articleno# AND processedflag='1' AND deletedflag='0' "
                                               "and ((transactiondate = #transactiondate# and transactiontime < #transactiontime#) or transactiondate < #transactiondate#) ) as t "
				"WHERE branchno=#branchno# AND "
					"articleno=#articleno# AND "
					"processedflag='1' AND "
					"deletedflag='0' AND "
                    "((transactiondate = t.maxdate and t.maxdate < #transactiondate#) OR " 
                    "(transactiondate = t.maxdate and t.maxdate = #transactiondate# and transactiontime < #transactiontime#)) "
				"ORDER BY transactiondate DESC, transactiontime DESC, qtyinstock ASC");
		BLOG_TRACE(narcotics::LoggerPool::loggerDomModules, getSQLString());
	}

	//-------------------------------------------------------------------------------------------------------//
BUILDER_DEFINITION(SelFollowingBookings)
	ENSURE_PROPERTIES_ARE_SET( SelFollowingBookings,"branchno; articleno; transactiondate; transactiontime;" )
	SQL_PATTERN(SelFollowingBookings,	"SELECT		catalogid, qtyinstock, qtytransaction, transactiontype, transactiondate, transactiontime "
										"FROM		narctransactioncatalog "
										"WHERE		processedflag='1' AND deletedflag='0' "
										"AND		branchno=#branchno# AND articleno=#articleno# "
										"AND		(	(transactiondate=#transactiondate# AND transactiontime>=#transactiontime#) "
										"OR			(transactiondate>#transactiondate#)	) "
										"ORDER BY	transactiondate ASC, transactiontime ASC")

//-------------------------------------------------------------------------------------------------------//
BUILDER_DEFINITION(UpdQtyInStock)
	ENSURE_PROPERTIES_ARE_SET( UpdQtyInStock,"catalogid; qtyinstock;" )
	SQL_PATTERN( UpdQtyInStock,	"UPDATE narctransactioncatalog "
								"SET	qtyinstock=#qtyinstock# "
								"WHERE	catalogid=#catalogid#" )

								//---------------------------------------------------------------------------
BEGIN_ACCESSOR_DEFINITION("NarcoticsOrders")

	PROPERTY_DESCRIPTION_LIST("int16 branchno; int32 articleno; int32 catalogid; int32 transactiondate;"
								"int32 purchaseorderno; int32 supplierno; string suppliername; decimal qtyorder;"
								"int32 bgano; string unity; int32 qtyinstock;  int32 qtytransaction; int32 transactiontime; string transactiontype;"
								)

	ACCESS_METHOD("UpdateOrderAckStatusByPatternSetToForPrint")
		SQL_BUILDER_CREATE(UpdateOrderAckStatusByPatternSetToForPrint)
		SQL_BUILDER_PUSH_BACK(UpdateOrderAckStatusByPatternSetToForPrint)

	ACCESS_METHOD("DelOrderByPattern")
		SQL_BUILDER_CREATE(DelOrderByPattern)
		SQL_BUILDER_PUSH_BACK(DelOrderByPattern)

	ACCESS_METHOD( "SelQuickPrevForUpdQtyInStock" )
		SQL_BUILDER_CREATE(SelQuickPreviousBooking)
		SQL_BUILDER_PUSH_BACK(SelQuickPreviousBooking)

	ACCESS_METHOD( "SelFollowForUpdQtyInStock" )
		SQL_BUILDER_CREATE(SelFollowingBookings)
		SQL_BUILDER_PUSH_BACK(SelFollowingBookings)
	
	ACCESS_METHOD( "UpdateQtyInStock" )
		SQL_BUILDER_CREATE(UpdQtyInStock)
		SQL_BUILDER_PUSH_BACK(UpdQtyInStock)

END_ACCESSOR_DEFINITION
				
} //acc_narcoticssupplier