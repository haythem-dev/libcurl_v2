//-------------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  string builder definition for narcotic booking-accessor
 *  \author Bjoern Bischof
 *  \date   12.07.2006
 */
//-------------------------------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------------------------------//
// includes
//-------------------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "acc_bookingscontrol.h"
#include "loggerpool.h"

//-------------------------------------------------------------------------------------------------------//
// namespace section
//-------------------------------------------------------------------------------------------------------//
namespace acc_bookingscontrol
{

//-------------------------------------------------------------------------------------------------------//
// ACCESSOR DEFINITION
//-------------------------------------------------------------------------------------------------------//
BEGIN_ACCESSOR_DEFINITION( "BookingsControl" )
	PROPERTY_DESCRIPTION_LIST(
		"int32 catalogid; "
		"int16 branchno; "
		"int32 articleno; "
		"string articlecode; "
		"string articlename; "
		"string pharmaform; "
		"string packageunit; "
		"int32 transactiondate; "
		"int32 transactiontime; "
		"int32 customersupplierno; "
		"string customersuppliername; "
		"string customersupplierlocation; "
		"string transactiontype; "
		"int32 purchaseorderno; "
		"int32 qtytransaction; "
		"int32 qtyinstock; "
		"string deletedflag; "
		"string processedflag; "
		"string telno; "
		"string quickdialing;" )

	//----------------------------------------------------------//
	// SELECTS
	//----------------------------------------------------------//
	ACCESS_METHOD( "SelNarcBookingsForInquiry" )
		SQL_BUILDER_CREATE( SelNarcBookingsForInquiry )
		SQL_BUILDER_PUSH_BACK( SelNarcBookingsForInquiry )


END_ACCESSOR_DEFINITION

BUILDER_DEFINITION( SelNarcBookingsForInquiry )
	ENSURE_PROPERTIES_ARE_SET( SelNarcBookingsForInquiry,"branchno;" )
	void SelNarcBookingsForInquiry::buildSQLString()
	{
		resolve(
			"SELECT "
				"n.catalogid, "
				"n.branchno, "
				"n.articleno, "
				"c.article_code AS articlecode, "
				"z.artikel_name AS articlename, "
				"z.darreichform AS pharmaform, "
				"z.einheit AS packageunit, "
				"n.transactiondate, "
				"n.transactiontime, "
				"n.customersupplierno, "
				"n.customersuppliername, "
				"n.customersuppliercity as customersupplierlocation, "
				"n.transactiontype, "
				"n.purchaseorderno, "
				"n.qtytransaction, "
				"n.qtyinstock, "
				"n.deletedflag, "
				"n.processedflag, " 
				"DECODE(n.transactiontype, "
					"'a', (NVL((SELECT telnr FROM kunde WHERE kunde.vertriebszentrumnr = branchno AND kunde.kundennr = customersupplierno ), ' ')), "
					"'c', (NVL((SELECT telnr FROM kunde WHERE kunde.vertriebszentrumnr = branchno AND kunde.kundennr = customersupplierno ), ' ')), "
					"' ') AS telno, "
				"DECODE( transactiontype, "
					"'a', (NVL((SELECT telkurzwahl FROM kunde WHERE kunde.vertriebszentrumnr = branchno AND kunde.kundennr = customersupplierno), ' ')), "
					"'c', (NVL((SELECT telkurzwahl FROM kunde WHERE kunde.vertriebszentrumnr = branchno AND kunde.kundennr = customersupplierno), ' ')), "
					"' ') AS quickdialing "
			"FROM "
				"narctransactioncatalog n, "
				"artikelzentral z, "
				"articlecodes c "
			"WHERE "
				"n.branchno=#branchno# AND "
				"n.articleno=z.artikel_nr AND "
				"n.articleno=c.articleno AND c.preferred_flag=1 AND "
				"n.deletedflag='0' AND "
				"n.processedflag='1' AND "
				"n.transactiontype!='u' " );
		if (getPropertyList().contains(constants::ARTICLENO) && getPropertyList().getState(constants::ARTICLENO) != basar::SS_UNSET)
			resolve("AND n.articleno=#articleno# ");

		if (getPropertyList().contains(constants::CUSTOMERSUPPLIERNO) && getPropertyList().getState(constants::CUSTOMERSUPPLIERNO) != basar::SS_UNSET)
			resolve("AND n.customersupplierno=#customersupplierno# ");
		
		//-----------------------------------------------------------------------------------------//
		// ToDo: the following misuse
		// attention:	here the attribute 'customersuppliername' of the search iterator is misused, 
		//				because more than one transactiontype has to used in the sql clause.	
		if (getPropertyList().contains(constants::TRANSACTIONTYPE) && getPropertyList().contains(constants::CUSTOMER_SUPPLIER_NAME)	&&
			!(getPropertyList().getState(constants::TRANSACTIONTYPE) == basar::SS_UNSET || getPropertyList().getState(constants::CUSTOMER_SUPPLIER_NAME) == basar::SS_UNSET))
		{
            resolve("AND n.transactiontype in ('#transactiontype#', '#customersuppliername#') ");
		}
		//-----------------------------------------------------------------------------------------//

		if (getPropertyList().contains(constants::TODATE) && getPropertyList().getState(constants::TODATE) != basar::SS_UNSET)
			resolve("AND transactiondate <= #todate# ");
		if (getPropertyList().contains(constants::FROMDATE) && getPropertyList().getState(constants::FROMDATE) != basar::SS_UNSET)
			resolve("AND transactiondate >= #fromdate# ");

		if (getPropertyList().contains(constants::TRANSACTIONTYPE) && getPropertyList().getState(constants::TRANSACTIONTYPE) != basar::SS_UNSET)
			resolve("AND transactiontype = '#transactiontype#' ");

		resolve(" ORDER BY transactiondate, transactiontime ");

		BLOG_TRACE( narcotics::LoggerPool::loggerDomModules, getSQLString() );
	}

}//acc_bookingcatalog