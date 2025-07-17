//-------------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  string builder definition for user-accessor + registration of user-accessor
 *  \author Roland Kiefert
 *  \date   17.02.2006
 */
//-------------------------------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------------------------------//
// includes
//-------------------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "acc_catalog.h"
#include "loggerpool.h"

//-------------------------------------------------------------------------------------------------------//
// namespace section
//-------------------------------------------------------------------------------------------------------//
namespace acc_catalog {

//-------------------------------------------------------------------------------------------------------//
//
//-------------------------------------------------------------------------------------------------------//
BUILDER_DEFINITION( CheckUniqueTimestampByBranchArticle )
	ENSURE_PROPERTIES_ARE_SET( CheckUniqueTimestampByBranchArticle, "branchno; articleno; transactiondate; transactiontime;" )

	void CheckUniqueTimestampByBranchArticle::buildSQLString()
	{
		resolve("SELECT COUNT(*) "
				"FROM narctransactioncatalog "
				"WHERE branchno=#branchno# AND "
					"articleno=#articleno# AND "
					"transactiondate = #transactiondate# AND "
                    "transactiontime = #transactiontime# ");
		BLOG_TRACE(LoggerPool::loggerDomModules, getSQLString());
	}

//-------------------------------------------------------------------------------------------------------//
//
//-------------------------------------------------------------------------------------------------------//
BUILDER_DEFINITION( SelQuickPrevQtyInStock )
	ENSURE_PROPERTIES_ARE_SET( SelQuickPrevQtyInStock, "branchno; articleno; transactiondate; transactiontime;" )

	void SelQuickPrevQtyInStock::buildSQLString()
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
		BLOG_TRACE(LoggerPool::loggerDomModules, getSQLString());
	}

//-------------------------------------------------------------------------------------------------------//
//
//-------------------------------------------------------------------------------------------------------//
BUILDER_DEFINITION( SelSuccQtyInStock )
	ENSURE_PROPERTIES_ARE_SET( SelSuccQtyInStock, "branchno; articleno; transactiondate; transactiontime;" )

	void SelSuccQtyInStock::buildSQLString()
	{
		resolve("SELECT catalogid, transactiontype, qtytransaction, qtyinstock "
				"FROM narctransactioncatalog "
				"WHERE branchno=#branchno# AND "
					"articleno=#articleno# AND "
					"processedflag= '1' AND "
					"deletedflag='0' AND "
					"((transactiondate > #transactiondate#) OR "
					"((transactiondate = #transactiondate#) AND "
					"(transactiontime >= #transactiontime#) ) ) "
				"ORDER BY transactiondate ASC, transactiontime ASC");
		BLOG_TRACE(LoggerPool::loggerDomModules, getSQLString());
	}

//-------------------------------------------------------------------------------------------------------//
//
//-------------------------------------------------------------------------------------------------------//
BUILDER_DEFINITION(SelUnprocessedPurchaseOrder)
ENSURE_PROPERTIES_ARE_SET(SelUnprocessedPurchaseOrder, "branchno; articleno; purchaseorderno;")
SQL_PATTERN(SelUnprocessedPurchaseOrder,
	"SELECT "
	"CatalogID, "
	"TransactionType, "
	"QtyTransaction, "
	"QtyInstock "
	"FROM NarcTransactionCatalog "
	"WHERE BranchNo      = #branchno# "
	"AND ArticleNo       = #articleno# "
	"AND ProcessedFlag   = '0' "
	"AND DeletedFlag     = '0' "
	"AND purchaseorderno = #purchaseorderno# ")

	//-------------------------------------------------------------------------------------------------------//
	//
	//-------------------------------------------------------------------------------------------------------//
	BUILDER_DEFINITION(InsCatalog)
	ENSURE_PROPERTIES_ARE_SET(InsCatalog,
		"branchno;articleno;transactiondate;transactiontime;customersupplierno;"
		"transactiontype;purchaseorderno;qtytransaction;qtyinstock;processedflag;"
		"customersuppliername;customersupplierstreet;customersuppliercity;customersuppliercip;narcgroup;narcgroup;concentration;"
		"packageunit;narcfactor;acknowledgementstatus;tourid;warehouseorderno;")

	void InsCatalog::buildSQLString()
	{
		resolve(
				"INSERT INTO NarcTransactionCatalog "
				"( BranchNo, "
				"ArticleNo, "
				"TransactionDate, "
				"TransactionTime, "
				"CustomerSupplierNo, "
				"TransactionType, "
				"PurchaseOrderNo, "
				"QtyTransaction, "
				"QtyInStock, "
				"DeletedFlag, "
				"ProcessedFlag, "
				"customersuppliername, "
                "customersupplierstreet, "
				"customersuppliercity, "
				"customersuppliercip, "
				"invoiceno, "
				"narcgroup, "
				"narcname, "
				"narcpotency, "
				"narcunit, "
				"narcfactor, "
				"acknowledgementstatus, "
				"tourid, "
				"warehouseorderno ) "
				"VALUES "
				"( #branchno#, "
				"#articleno#, "
				"#transactiondate#, "
				"#transactiontime#, "
				"#customersupplierno#, "
				"'#transactiontype#', "
				"#purchaseorderno#, "
				"#qtytransaction#, "
				"#qtyinstock#, "
				"'0', "
				"'#processedflag#', "
				"'#customersuppliername#', "
                "'#customersupplierstreet#', "
				"'#customersuppliercity#', "
				"'#customersuppliercip#', "
				"0, "
				"'#narcgroup#', "
				"'#narcname#', "
				"#concentration#, "
				"'#packageunit#', "
				"#narcfactor#, "
				"#acknowledgementstatus#, "
				"'#tourid#', "
				"#warehouseorderno# "
				")");
		BLOG_TRACE( LoggerPool::loggerDomModules, getSQLString() );
	}

//-------------------------------------------------------------------------------------------------------//
//
//-------------------------------------------------------------------------------------------------------//
BUILDER_DEFINITION(InsOriginal)
ENSURE_PROPERTIES_ARE_SET(InsOriginal,
						  "branchno; articleno; transactiondate; transactiontime; customersupplierno;"
						  "transactiontype; purchaseorderno; qtytransaction; qtyinstock; tourid; warehouseorderno;")

	void InsOriginal::buildSQLString()
	{
		resolve("INSERT INTO narctransactionoriginal "
				"(branchno, articleno, transactiondate, transactiontime, customersupplierno, "
				"transactiontype, purchaseorderno, qtytransaction, qtyinstock, tourid, warehouseorderno) "
				"VALUES "
				"(#branchno#, #articleno#, #transactiondate#, #transactiontime#, #customersupplierno#, "
			"'#transactiontype#', #purchaseorderno#, #qtytransaction#, #qtyinstock#, '#tourid#', #warehouseorderno#)");
		BLOG_TRACE(LoggerPool::loggerDomModules, getSQLString());
	}

//-------------------------------------------------------------------------------------------------------//
//
//-------------------------------------------------------------------------------------------------------//
BUILDER_DEFINITION( UpdQtyTransaction  )
	ENSURE_PROPERTIES_ARE_SET( UpdQtyTransaction, "qtyinstock; catalogid;" )
	void UpdQtyTransaction::buildSQLString()
	{
		resolve("UPDATE narctransactioncatalog "
				"SET qtyinstock=#qtyinstock#, qtytransaction=#qtytransaction# "
				"WHERE catalogid = #catalogid#");
		BLOG_TRACE(LoggerPool::loggerDomModules, getSQLString());
	}

//-------------------------------------------------------------------------------------------------------//
//
//-------------------------------------------------------------------------------------------------------//
BUILDER_DEFINITION( UpdAcknowledgementstatus  )
	ENSURE_PROPERTIES_ARE_SET( UpdAcknowledgementstatus, "acknowledgementstatus; branchno; transactiontype;" )
	void UpdAcknowledgementstatus::buildSQLString()
	{
		resolve("UPDATE narctransactioncatalog "
				"SET acknowledgementstatus='#acknowledgementstatus#' "
				"WHERE acknowledgementstatus = '0' "// status AS_INSERTED
				"AND transactiontype = '#transactiontype#' "
				"AND branchno = #branchno# "
				"AND purchaseorderno NOT IN "
					"(SELECT purchaseorderno FROM narctransactiontransfer "
					"WHERE branchno = narctransactioncatalog.branchno and "
						"transactiondate = narctransactioncatalog.transactiondate and "
						"customersupplierno = narctransactioncatalog.customersupplierno and " // zusätzlich wegen BNR = 0 auch JOIN auf customersupplierno !
						"transactiontype in ('a', 'u', 's', 'S'))");
		BLOG_TRACE(LoggerPool::loggerDomModules, getSQLString());
	}

//-------------------------------------------------------------------------------------------------------//
// ACCESSOR DEFINITION
//-------------------------------------------------------------------------------------------------------//
BEGIN_ACCESSOR_DEFINITION("Catalog")
PROPERTY_DESCRIPTION_LIST("int32 catalogid; "
						  "int16 branchno; "
						  "int32 articleno; "
						  "int32 transactiondate; "
						  "int32 transactiontime; "
						  "int32 customersupplierno; "
						  "string customersuppliername; "
						  "string customersupplierstreet; "
						  "string customersuppliercity; "
						  "string customersuppliercip; "
						  "string transactiontype; "
						  "int32 purchaseorderno; "
						  "int32 qtytransaction; "
						  "int32 qtyinstock; "
						  "string deletedflag; "
						  "string processedflag; "

						  "string narcgroup; "
						  "string narcname; "

						  "string packageunit; "
						  "decimal narcpotency; "
						  "string narcunit; "
						  "decimal narcfactor; "
						  "int32 contributionvoucherno; "
						  "int16 acknowledgementstatus; "
						  "string tourid; "
						  "int32 warehouseorderno;")
	//----------------------------------------------------------//
	// SELECTS
	//----------------------------------------------------------//
    ACCESS_METHOD( "SelQuickPrevQtyInStock" )
		SQL_BUILDER_CREATE( SelQuickPrevQtyInStock )
		SQL_BUILDER_PUSH_BACK( SelQuickPrevQtyInStock )

	ACCESS_METHOD( "SelSuccQtyInStock" )
		SQL_BUILDER_CREATE( SelSuccQtyInStock )
		SQL_BUILDER_PUSH_BACK( SelSuccQtyInStock )

    ACCESS_METHOD( "SelUnprocessedPurchaseOrder" )
		SQL_BUILDER_CREATE( SelUnprocessedPurchaseOrder )
		SQL_BUILDER_PUSH_BACK( SelUnprocessedPurchaseOrder )

	ACCESS_METHOD( "InsCatalog" )
		SQL_BUILDER_CREATE( InsCatalog )
		SQL_BUILDER_PUSH_BACK( InsCatalog )

	ACCESS_METHOD( "InsOriginal" )
		SQL_BUILDER_CREATE( InsOriginal )
		SQL_BUILDER_PUSH_BACK( InsOriginal )

	ACCESS_METHOD( "UpdQtyTransaction" )
		SQL_BUILDER_CREATE( UpdQtyTransaction )
		SQL_BUILDER_PUSH_BACK( UpdQtyTransaction )

	ACCESS_METHOD( "UpdAcknowledgementstatus" )
		SQL_BUILDER_CREATE( UpdAcknowledgementstatus )
		SQL_BUILDER_PUSH_BACK( UpdAcknowledgementstatus )

	ACCESS_METHOD( "CheckUniqueTimestampByBranchArticle" )
		SQL_BUILDER_CREATE( CheckUniqueTimestampByBranchArticle )
		SQL_BUILDER_PUSH_BACK( CheckUniqueTimestampByBranchArticle )

END_ACCESSOR_DEFINITION

} //namespace acc_catalog