//-------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  Accessor for matching contribution voucher entries
 *  \author Benjamin Endlicher
 *  \date   03.05.2010
 */
//-------------------------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------------------------//
// includes
//-------------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "acc_contributionvoucher.h"
#include "loggerpool.h"

//-------------------------------------------------------------------------------------------------//
// namespaces
//-------------------------------------------------------------------------------------------------//
namespace acc_contributionvoucher
{

//------------------------------------------------------------------------------------------------------------//
BUILDER_DEFINITION( SelOrderUnion )
ENSURE_PROPERTIES_ARE_SET(SelOrderUnion, "branchno; ")
void SelOrderUnion::buildSQLString()
{
	resolve(
        "SELECT distinct "
        "n.customersupplierno, n.transactiontype, n.branchno, n.purchaseorderno, n.processedflag, n.deletedflag, "
        "n.customersuppliername, n.customersuppliercity AS customersupplierlocation, n.transactiondate, "
        "n.contributionvoucherno, n.acknowledgementstatus, n.username, n.printeddate, n.printedtime, n.tourid, "
        "decode(n.transactiontype, "
		    "'a', (NVL((select k.telnr AS telno from kunde k where n.branchno=k.vertriebszentrumnr AND n.customersupplierno= k.kundennr), ' ')), "
		    "'d', (NVL((select concat(trim(l.vorwahl), trim(l.telefon)) AS telno from pusupplier l WHERE n.customersupplierno=l.bga_nr AND l.besla_nr=l.liefer_besla_nr), ' ')), "
		    "' ') as telno, "
        "decode(n.transactiontype, "
		    "'a', (NVL((select k.telkurzwahl AS quickdialing from kunde k where n.branchno=k.vertriebszentrumnr AND n.customersupplierno= k.kundennr), ' ')), "
		    "'d', (NVL((select '' AS quickdialing from pusupplier l WHERE n.customersupplierno=l.bga_nr AND l.besla_nr=l.liefer_besla_nr), ' ')), "
		    "' ')	as quickdialing, "
        "decode(n.transactiontype, "
		    "'a', (NVL((select k.faxnr AS faxno from kunde k where n.branchno=k.vertriebszentrumnr AND n.customersupplierno= k.kundennr), ' ')), "
		    "'d', (NVL((select concat(trim(l.vorwahl_fax), trim(l.telefax)) AS faxno  from pusupplier l WHERE n.customersupplierno=l.bga_nr AND l.besla_nr=l.liefer_besla_nr), ' ')), "
		    "' ') as faxno "
        "FROM narctransactioncatalog n WHERE "
		"n.branchno=#branchno# AND "
			"n.deletedflag='0' AND "
            " n.acknowledgementstatus between 2 and 5 ");

    if( isContainedAndSet(constants::TRANSACTIONDATE) ){
        resolve(" AND n.transactiondate=#transactiondate# ");
    }

    if( isContainedAndSet(constants::FROMDATE) && isContainedAndSet(constants::TODATE) )
	{
	    if( isContainedAndSet(constants::FROMTIME) && isContainedAndSet(constants::TOTIME) )
		{
			if (getPropertyList().getInt32(constants::FROMDATE) == getPropertyList().getInt32(constants::TODATE))
			{
				resolve(" AND n.transactiondate=#fromdate# AND n.transactiontime BETWEEN #fromtime# AND #totime# ");	// timespan only during one day
			}
			else
			{
				resolve(" AND (n.transactiondate = #fromdate# AND n.transactiontime >= #fromtime# ");	// first day of timespan
				resolve(" OR n.transactiondate=#todate# AND n.transactiontime <= #totime# ");			// last day of timespan
				resolve(" OR n.transactiondate > #fromdate# AND n.transactiondate < #todate#) ");		// days inbetween
			}
		}
		else
		{
	        resolve(" AND n.transactiondate between #fromdate# and #todate# ");
		}
    }

    if( isContainedAndSet(constants::ACKNOWLEDGEMENTSTATUS) ){
        resolve(" and n.acknowledgementstatus = #acknowledgementstatus# ");
    }

	if( isContainedAndSet(constants::CONTRIBUTIONVOUCHERNO) ){
		resolve(" AND n.contributionvoucherno=#contributionvoucherno# ");
	}

	if( isContainedAndSet(constants::PURCHASEORDERNO) ){
		resolve(" AND n.purchaseorderno=#purchaseorderno# ");
	}

	if( isContainedAndSet(constants::TOURID) ){
		resolve(" AND n.tourid like '#tourid#%' ");
	}

	if( isContainedAndSet(constants::ORDERBYTOUR) && getPropertyList().getInt16(constants::ORDERBYTOUR) == 2)
		resolve ("ORDER BY 15, 9 ASC, 4 ");// tourid leading
	else
	{
		resolve ("ORDER BY 9 ASC, "); // transactiondate

		if( isContainedAndSet(constants::ORDERBYTOUR) && getPropertyList().getInt16(constants::ORDERBYTOUR) == 1)
			resolve(" 15, 4 "); // tourid
		else
			resolve (" 4 "); // purchaseorderno
	}

    BLOG_TRACE( narcotics::LoggerPool::loggerDomModules, getSQLString() );
}

//------------------------------------------------------------------------------------------------------------//
BUILDER_DEFINITION( SelOrderPositions )
ENSURE_PROPERTIES_ARE_SET(SelOrderPositions, "branchno; purchaseorderno; transactiondate;")

void SelOrderPositions::buildSQLString()
{
	resolve(
			"SELECT n.catalogid, n.branchno, n.articleno, ac.article_code as articlecode, "
				"TRIM(z.artikel_name) AS articlename, TRIM(z.darreichform) AS pharmaform, "
				"TRIM(z.einheit) AS packageunit, "
				"n.customersupplierno, n.transactiontype, n.purchaseorderno, "
				"n.qtytransaction, n.qtytransaction AS originalqtytransaction, n.qtyinstock, n.processedflag, n.deletedflag, n.customersuppliername, "
				"n.customersuppliercity AS customersupplierlocation, n.customersupplierstreet, n.transactiondate, n.transactiontime, "
				"n.contributionvoucherno, n.acknowledgementstatus, n.tourid "
			"FROM "
				"narctransactioncatalog n JOIN "
				"artikelzentral z ON n.articleno=z.artikel_nr JOIN "
				"articlecodes ac on n.articleno=ac.articleno "
			"WHERE n.articleno=z.artikel_nr AND "
				"n.branchno=#branchno# AND "
				"n.purchaseorderno=#purchaseorderno# AND "
				"n.transactiondate=#transactiondate# AND "
				"n.deletedflag = '0' AND "
				"ac.preferred_flag = 1 ");

			if( isContainedAndSet("contributionvoucherno") ){
				resolve(" AND n.contributionvoucherno = #contributionvoucherno# ");
			}

            if( isContainedAndSet("acknowledgementstatus") ){
                resolve(" AND n.acknowledgementstatus = #acknowledgementstatus# ");
            }

			resolve ("ORDER BY n.transactiontime ASC");

	BLOG_TRACE( narcotics::LoggerPool::loggerDomModules, getSQLString() );
}

//------------------------------------------------------------------------------------------------------------//
BUILDER_DEFINITION( SelNewInsertedOrder )
ENSURE_PROPERTIES_ARE_SET(SelNewInsertedOrder, "branchno;purchaseorderno;transactiondate;acknowledgementstatus;customersupplierno;")
//only select orders from customers
void SelNewInsertedOrder::buildSQLString()
{
//	SELECT n.catalogid, n.branchno, n.articleno, ac.article_code, TRIM(z.artikel_name) AS articlename, TRIM(z.darreichform) AS pharmaform, TRIM(z.einheit) AS packageunit, n.customersupplierno, n.transactiontype, n.purchaseorderno,
//n.qtytransaction, n.qtyinstock, n.processedflag, n.deletedflag, n.customersuppliername, n.customersuppliercity AS customersupplierlocation, n.transactiondate, n.transactiontime, n.contributionvoucherno,
//n.acknowledgementstatus, n.tourid
//FROM narctransactioncatalog n join artikelzentral z on n.articleno = z.artikel_nr join articlecodes ac on ac.articleno = n.articleno
//WHERE n.branchno=5 AND n.acknowledgementstatus=6 AND n.purchaseorderno=1660
//AND n.customersupplierno=3093870 AND n.transactiondate=20121029  AND (n.transactiontype = 'a' OR n.transactiontype = 'd') AND n.deletedflag = '0'
//AND n.customersupplierno<5000000 AND ac.preferred_flag = '1' ORDER BY n.purchaseorderno, n.transactiondate DESC

	resolve(
		"SELECT n.catalogid, n.branchno, n.articleno, ac.article_code AS articlecode, "
			"TRIM(z.artikel_name) AS articlename, TRIM(z.darreichform) AS pharmaform, "
			"TRIM(z.einheit) AS packageunit, "
			"n.customersupplierno, n.transactiontype, n.purchaseorderno, "
			"n.qtytransaction, n.qtyinstock, n.processedflag, n.deletedflag, n.customersuppliername, "
			"n.customersuppliercity AS customersupplierlocation, n.transactiondate, n.transactiontime, "
			"n.contributionvoucherno, n.acknowledgementstatus, n.tourid, n.username, n.printeddate, n.printedtime "
		"FROM narctransactioncatalog n join artikelzentral z on n.articleno = z.artikel_nr join articlecodes ac on ac.articleno = n.articleno "
		"WHERE n.articleno=z.artikel_nr AND "
			"n.branchno=#branchno# AND "
            "n.acknowledgementstatus=#acknowledgementstatus# AND "
            "n.purchaseorderno=#purchaseorderno# AND "
            "n.customersupplierno=#customersupplierno# AND "
            "n.transactiondate=#transactiondate# "
			" AND (n.transactiontype = 'a' OR n.transactiontype = 'd')"
			" AND n.deletedflag = '0' "
            " AND ac.preferred_flag = 1 ");
	if( isContainedAndSet("internalaccounts") ){
		resolve(" AND n.customersupplierno<#internalaccounts# ");
	}
	resolve("ORDER BY n.purchaseorderno, n.transactiondate DESC");
	BLOG_TRACE( narcotics::LoggerPool::loggerDomModules, getSQLString() );
}

//------------------------------------------------------------------------------------------------------------//
BUILDER_DEFINITION( SelAllNewInsertedBookings )
ENSURE_PROPERTIES_ARE_SET(SelAllNewInsertedBookings, "branchno;")
//only select orders from customers
void SelAllNewInsertedBookings::buildSQLString()
{
	resolve(
		"SELECT DISTINCT n.customersupplierno, n.purchaseorderno, n.transactiondate, n.warehouseorderno "
		"FROM narctransactioncatalog n "
		"WHERE n.branchno=#branchno# AND "
			"n.acknowledgementstatus=1 "
			" AND (n.transactiontype = 'a' OR n.transactiontype = 'd')"
			" AND n.deletedflag = '0' " );

	if( isContainedAndSet("internalaccounts") ){
		resolve(" AND n.customersupplierno<#internalaccounts# ");
	}
	resolve("ORDER BY n.warehouseorderno, n.transactiondate DESC");
	BLOG_TRACE( narcotics::LoggerPool::loggerDomModules, getSQLString() );
}

//------------------------------------------------------------------------------------------------------------//
BUILDER_DEFINITION ( SelectNotPrintedBookingsIntoTemp )
void SelectNotPrintedBookingsIntoTemp::buildSQLString()
{
    resolve( "select catalogid "
             "from narctransactioncatalog nc1 "
             "where nc1.acknowledgementstatus = #formeracknowledgementstatus# and nc1.branchno = #branchno# and nc1.contributionvoucherno = 0 "
             "and not exists ("
                "select catalogid "
                "from narctransactioncatalog nc2 "
                "where nc2.contributionvoucherno <> 0 and nc1.purchaseorderno = nc2.purchaseorderno "
                "and nc1.transactiondate=nc2.transactiondate and nc1.branchno = nc2.branchno "
                "and nc1.customersupplierno = nc2.customersupplierno "
				"and nc2.acknowledgementstatus = #formeracknowledgementstatus# "
				") into temp noyetprintedtemptlb; "
            );
	BLOG_TRACE( narcotics::LoggerPool::loggerDomModules, getSQLString() );
}

//------------------------------------------------------------------------------------------------------------//
bool SelectNotPrintedBookingsIntoTemp::isExecutable() const
{
	static basar::ConstString fun = "SelectReturnHeaderForMerging::isExecutable()"; fun;
	static log4cplus::Logger& logger = narcotics::LoggerPool::loggerDomModules;

	const bool executable = arePropertiesSet( "branchno;formeracknowledgementstatus;" );

	return executable;
}

//------------------------------------------------------------------------------------------------------------//
BUILDER_DEFINITION ( UpdateNotPrintedBookingsUsingTemp )
void UpdateNotPrintedBookingsUsingTemp::buildSQLString()
{
    resolve( "update narctransactioncatalog set acknowledgementstatus = #acknowledgementstatus# where catalogid in (select catalogid from noyetprintedtemptlb); "
            );
	BLOG_TRACE( narcotics::LoggerPool::loggerDomModules, getSQLString() );
}

//------------------------------------------------------------------------------------------------------------//
bool UpdateNotPrintedBookingsUsingTemp::isExecutable() const
{
	static basar::ConstString fun = "UpdateNotPrintedBookingsUsingTemp::isExecutable()"; fun;
	static log4cplus::Logger& logger = narcotics::LoggerPool::loggerDomModules;

	const bool executable = arePropertiesSet( "branchno;formeracknowledgementstatus;" );

	return executable;
}

//------------------------------------------------------------------------------------------------------------//
BUILDER_DEFINITION ( DropNotPrintedBookingsTempTbl )
void DropNotPrintedBookingsTempTbl::buildSQLString()
{
    resolve(
                "drop table noyetprintedtemptlb; "
            );
	BLOG_TRACE( narcotics::LoggerPool::loggerDomModules, getSQLString() );
}

//------------------------------------------------------------------------------------------------------------//
bool DropNotPrintedBookingsTempTbl::isExecutable() const
{
	static basar::ConstString fun = "DropNotPrintedBookingsTempTbl::isExecutable()"; fun;
	static log4cplus::Logger& logger = narcotics::LoggerPool::loggerDomModules;

	const bool executable = arePropertiesSet( "branchno;formeracknowledgementstatus;" );

	return executable;
}

//------------------------------------------------------------------------------------------------------------//
BUILDER_DEFINITION ( CheckForInPrintBookings )
ENSURE_PROPERTIES_ARE_SET(CheckForInPrintBookings, "branchno;acknowledgementstatus;")
void CheckForInPrintBookings::buildSQLString()
{
    resolve( "select count(*) as numberofinprintbookings from narctransactioncatalog where branchno = #branchno# and acknowledgementstatus = #acknowledgementstatus# and deletedflag = '0' " );

    if( isContainedAndSet("contributionvoucherno") ){
        resolve(" and contributionvoucherno = #contributionvoucherno# ");
    }

    BLOG_TRACE( narcotics::LoggerPool::loggerDomModules, getSQLString() );
}

//------------------------------------------------------------------------------------------------------------//
BUILDER_DEFINITION ( FindPrintLockByBranch )
ENSURE_PROPERTIES_ARE_SET(FindPrintLockByBranch, "branchno;idbusinessobject;businesstype")
void FindPrintLockByBranch::buildSQLString()
{
    resolve( "select username from narclock where branchno = #branchno# and idbusinessobject = #idbusinessobject# and businesstype = '#businesstype#'" );
	BLOG_TRACE( narcotics::LoggerPool::loggerDomModules, getSQLString() );
}

//------------------------------------------------------------------------------------------------------------//
BUILDER_DEFINITION ( FindBookingsInPrint )
ENSURE_PROPERTIES_ARE_SET(FindBookingsInPrint, "branchno;acknowledgementstatus;")
void FindBookingsInPrint::buildSQLString()
{
    resolve( "select distinct customersuppliername, customersupplierno, purchaseorderno, contributionvoucherno, transactiondate, printeddate "
			 "from narctransactioncatalog where branchno = #branchno# and acknowledgementstatus = #acknowledgementstatus# and deletedflag = '0' and transactiontype in ('a', 'd') and contributionvoucherno <> 0" );
	BLOG_TRACE( narcotics::LoggerPool::loggerDomModules, getSQLString() );
}

//------------------------------------------------------------------------------------------------------------//
BUILDER_DEFINITION( SelAcknowledgementStatus )
ENSURE_PROPERTIES_ARE_SET(SelAcknowledgementStatus, "branchno; contributionvoucherno; transactiondate;")

void SelAcknowledgementStatus::buildSQLString()
{
	resolve(
		"SELECT acknowledgementstatus "
		"FROM narctransactioncatalog "
		"WHERE contributionvoucherno = #contributionvoucherno# "
        "AND branchno = #branchno# "
        "AND transactiondate = #transactiondate# and deletedflag = '0' and processedflag = '1'" );
	BLOG_TRACE( narcotics::LoggerPool::loggerDomModules, getSQLString() );
}

//------------------------------------------------------------------------------------------------------------//
BUILDER_DEFINITION(UpdAcknowledgementstatus)
ENSURE_PROPERTIES_ARE_SET( UpdAcknowledgementstatus, "acknowledgementstatus; catalogid;" )

void UpdAcknowledgementstatus::buildSQLString()
{
	resolve(
		"UPDATE narctransactioncatalog "
		"SET acknowledgementstatus=#acknowledgementstatus# "
		"WHERE catalogid=#catalogid#");
	BLOG_TRACE( narcotics::LoggerPool::loggerDomModules, getSQLString() );
}

//------------------------------------------------------------------------------------------------------------//
BUILDER_DEFINITION(UpdAcknowledgementstatusOfOrder)
ENSURE_PROPERTIES_ARE_SET( UpdAcknowledgementstatusOfOrder, "acknowledgementstatus;branchno;customersupplierno;purchaseorderno;transactiondate;" )

void UpdAcknowledgementstatusOfOrder::buildSQLString()
{
	resolve(
		"UPDATE narctransactioncatalog "
		"SET acknowledgementstatus=#acknowledgementstatus# ");
    if( isContainedAndSet(constants::USERNAME) )
    {
        resolve(", username='#username#' ");
    }

    if( isContainedAndSet(constants::PRINTED_DATE) )
    {
        resolve(", printeddate=#printeddate# ");
    }
    if( isContainedAndSet(constants::PRINTED_TIME) )
    {
        resolve(", printedtime=#printedtime# ");
    }

    resolve("WHERE branchno=#branchno# AND customersupplierno=#customersupplierno# AND purchaseorderno=#purchaseorderno# AND transactiondate=#transactiondate# and deletedflag = '0'");

    if( isContainedAndSet(constants::FORMER_ACKNOWLEDGEMENTSTATUS) )
    {
        resolve (" and acknowledgementstatus = #formeracknowledgementstatus# ");
    }

	if( isContainedAndSet(constants::CONTRIBUTIONVOUCHERNO) )
	{
		resolve(" AND contributionvoucherno=#contributionvoucherno#");
	}

	BLOG_TRACE( narcotics::LoggerPool::loggerDomModules, getSQLString() );
}

//------------------------------------------------------------------------------------------------------------//
BUILDER_DEFINITION(UpdAcknowledgementstatusOfOrderMark)
ENSURE_PROPERTIES_ARE_SET( UpdAcknowledgementstatusOfOrderMark, "acknowledgementstatus;formeracknowledgementstatus;branchno;transactiondate;contributionvoucherno" )

void UpdAcknowledgementstatusOfOrderMark::buildSQLString()
{
	resolve(
		"UPDATE narctransactioncatalog "
		"SET acknowledgementstatus=#acknowledgementstatus# ");

    resolve("WHERE branchno=#branchno# AND transactiondate=#transactiondate# and deletedflag = '0'");

    resolve (" and acknowledgementstatus = #formeracknowledgementstatus# ");

    resolve(" AND contributionvoucherno=#contributionvoucherno#");

	BLOG_TRACE( narcotics::LoggerPool::loggerDomModules, getSQLString() );
}

//------------------------------------------------------------------------------------------------------------//
BUILDER_DEFINITION( UpdContributionVoucherNo  )
ENSURE_PROPERTIES_ARE_SET( UpdContributionVoucherNo, "contributionvoucherno; catalogid;" )

void UpdContributionVoucherNo::buildSQLString()
{
	resolve(
		"UPDATE narctransactioncatalog "
        "SET contributionvoucherno=#contributionvoucherno# ");

    if( isContainedAndSet(constants::USERNAME) )
    {
        resolve (", username='#username#' ");
    }

    if( isContainedAndSet(constants::PRINTED_DATE) )
    {
        resolve (", printeddate=#printeddate# ");
    }
    if( isContainedAndSet(constants::PRINTED_TIME) )
    {
        resolve(", printedtime=#printedtime# ");
    }

    resolve ("WHERE catalogid=#catalogid#");

	BLOG_TRACE( narcotics::LoggerPool::loggerDomModules, getSQLString() );
}

//------------------------------------------------------------------------------------------------------------//
// Accessor Definition ACC_CONTRIBUTIONVOUCHER
//------------------------------------------------------------------------------------------------------------//
BEGIN_ACCESSOR_DEFINITION( "ContributionVoucher" )

	PROPERTY_DESCRIPTION_LIST(	"int32 catalogid; int16 branchno; int32 articleno; string articlename; string articlecode; "
								"string pharmaform; string packageunit;"
                                "int32 transactiondate; int32 transactiontime; "
								"int32 customersupplierno; string customersuppliername; string customersupplierlocation; string customersupplierstreet; "
								"string customersuppliercip; string transactiontype; int32 purchaseorderno; int32 qtytransaction; int32 originalqtytransaction; int32 qtyinstock; "
								"string deletedflag; string processedflag; string storagelocation; int32 balance; "
								"decimal countofbookings; string narcunit; string narcgroup; string narcname; decimal narcpotency; "
								"decimal narcfactor; int32 contributionvoucherno; int16 acknowledgementstatus; string telno; "
								"string quickdialing; string faxno; "
								"int16 rectificationreason; string rectificationtext; string rectificationcomment; string selection; "
								"int32 internalaccounts; string tourid; int16 formeracknowledgementstatus; decimal numberofinprintbookings; string username; "
								"int32 printeddate; int32 printedtime; "
								"string newtransactiontype; "
                                "string ScannedContributionVoucherNo; int32 fromdate; int32 todate; int32 fromtime; int32 totime; int16 orderbytour;"
                                )

	//----------------------------------------------------------//
	// SELECTS
	//----------------------------------------------------------//
	ACCESS_METHOD( "SelAcknowledgementStatus" )
		SQL_BUILDER_CREATE(SelAcknowledgementStatus)
		SQL_BUILDER_PUSH_BACK(SelAcknowledgementStatus)

	ACCESS_METHOD( "SelOrderPositions" )
		SQL_BUILDER_CREATE(SelOrderPositions)
		SQL_BUILDER_PUSH_BACK(SelOrderPositions)

	ACCESS_METHOD( "SelOrderUnion" )
		SQL_BUILDER_CREATE(SelOrderUnion)
		SQL_BUILDER_PUSH_BACK(SelOrderUnion)

   	ACCESS_METHOD( "SelAllNewInsertedBookings" )
		SQL_BUILDER_CREATE( SelAllNewInsertedBookings )
		SQL_BUILDER_PUSH_BACK( SelAllNewInsertedBookings )

	ACCESS_METHOD( "SetToInPrintAndSelNewInsertedBookingsEx" )
		SQL_BUILDER_CREATE( UpdAcknowledgementstatusOfOrder )
		SQL_BUILDER_PUSH_BACK( UpdAcknowledgementstatusOfOrder )
		SQL_BUILDER_CREATE( SelNewInsertedOrder )
		SQL_BUILDER_PUSH_BACK( SelNewInsertedOrder )

	ACCESS_METHOD( "UpdAcknowledgementstatus" )
		SQL_BUILDER_CREATE( UpdAcknowledgementstatus )
		SQL_BUILDER_PUSH_BACK( UpdAcknowledgementstatus )

	ACCESS_METHOD( "UpdAcknowledgementstatusOfOrder" )
		SQL_BUILDER_PUSH_BACK( UpdAcknowledgementstatusOfOrder )
        SQL_BUILDER_CREATE( UpdAcknowledgementstatusOfOrderMark )
        SQL_BUILDER_PUSH_BACK( UpdAcknowledgementstatusOfOrderMark )

	ACCESS_METHOD( "UpdContributionVoucherNo" )
		SQL_BUILDER_CREATE( UpdContributionVoucherNo )
		SQL_BUILDER_PUSH_BACK( UpdContributionVoucherNo )

    ACCESS_METHOD ("ResetNotYetPrintedBooking")
        SQL_BUILDER_CREATE(SelectNotPrintedBookingsIntoTemp)
        SQL_BUILDER_PUSH_BACK(SelectNotPrintedBookingsIntoTemp)
        SQL_BUILDER_CREATE(UpdateNotPrintedBookingsUsingTemp)
        SQL_BUILDER_PUSH_BACK(UpdateNotPrintedBookingsUsingTemp)
        SQL_BUILDER_CREATE(DropNotPrintedBookingsTempTbl)
        SQL_BUILDER_PUSH_BACK(DropNotPrintedBookingsTempTbl)

    //called by isContributionVoucherStillInPrint
    //and areBookingsInPrint
    //TODO: clean up
    ACCESS_METHOD ("CheckForInPrintBookings")
        SQL_BUILDER_CREATE(CheckForInPrintBookings)
        SQL_BUILDER_PUSH_BACK(CheckForInPrintBookings)

    ACCESS_METHOD ("FindBookingsInPrint")
        SQL_BUILDER_CREATE(FindBookingsInPrint)
        SQL_BUILDER_PUSH_BACK(FindBookingsInPrint)

    ACCESS_METHOD ("FindPrintLockByBranch")
        SQL_BUILDER_CREATE(FindPrintLockByBranch)
        SQL_BUILDER_PUSH_BACK(FindPrintLockByBranch)
END_ACCESSOR_DEFINITION

} //namespace ACC_CONTRIBUTIONVOUCHER