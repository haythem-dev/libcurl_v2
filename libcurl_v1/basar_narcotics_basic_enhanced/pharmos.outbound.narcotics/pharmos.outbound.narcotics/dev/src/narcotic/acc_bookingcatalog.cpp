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
#include "acc_bookingcatalog.h"
#include "loggerpool.h"

//-------------------------------------------------------------------------------------------------------//
// namespace section
//-------------------------------------------------------------------------------------------------------//
namespace acc_bookingcatalog
{

//-------------------------------------------------------------------------------------------------------//
//
//-------------------------------------------------------------------------------------------------------//
BUILDER_DEFINITION(SelCat)
	ENSURE_PROPERTIES_ARE_SET(SelCat,"branchno;")
	void SelCat::buildSQLString()
	{
		resolve( "SELECT catalogid, branchno, n.articleno, c1.article_code AS articlecode, artikel_name AS articlename, darreichform AS pharmaform, "
						"einheit AS packageunit, customersupplierno, transactiontype, "
						"purchaseorderno, qtytransaction, qtyinstock, processedflag, deletedflag, customersuppliername, "
						"customersupplierstreet, customersuppliercip, "
						"customersuppliercity AS customersupplierlocation, "
						"transactiondate AS transactiondate,  transactiontime AS transactiontime, "
						"contributionvoucherno, acknowledgementstatus, '0' as selection, username, printeddate, printedtime "
				 "FROM	narctransactioncatalog n, artikelzentral, articlecodes c1 ");
		if (getPropertyList().contains(constants::ARTICLECODE) &&  !(getPropertyList().getState(constants::ARTICLECODE) == basar::SS_UNSET))
		{
			resolve(", articlecodes c2 ");
		}
		resolve( "WHERE branchno=#branchno# "
				 "AND	n.articleno=artikelzentral.artikel_nr "
				 "AND   n.articleno=c1.articleno AND c1.preferred_flag=1 ");
		if (getPropertyList().contains(constants::ARTICLECODE) &&  !(getPropertyList().getState(constants::ARTICLECODE) == basar::SS_UNSET))
		{
			resolve("AND   n.articleno=c2.articleno ");
		}
		resolve( "AND	transactiontype != 'u' ");
		if( getPropertyList().contains( constants::CATALOGID ) &&
			!(getPropertyList().getState( constants::CATALOGID ) == basar::SS_UNSET))
		{
			basar::VarString res;
			res.format( "AND catalogid = #%s# ", constants::CATALOGID );
			resolve( res );
		}
		if( getPropertyList().contains(constants::ARTICLENO) &&
			!(getPropertyList().getState(constants::ARTICLENO) == basar::SS_UNSET))
		{
			basar::VarString res;
			res.format("AND n.articleno = #%s# ", constants::ARTICLENO);
			resolve(res);
		}
		if( getPropertyList().contains(constants::ARTICLECODE) &&
			!(getPropertyList().getState(constants::ARTICLECODE) == basar::SS_UNSET))
		{
			basar::VarString res;
			res.format("AND c2.article_code = '#%s#' ", constants::ARTICLECODE);
			resolve(res);
		}
		if( getPropertyList().contains(constants::PURCHASEORDERNO) &&
			!(getPropertyList().getState(constants::PURCHASEORDERNO) == basar::SS_UNSET))
		{
			basar::VarString res;
			res.format("AND purchaseorderno = #%s# ", constants::PURCHASEORDERNO);
			resolve(res);
		}
		if( getPropertyList().contains(constants::TRANSACTIONDATE) &&
			!(getPropertyList().getState(constants::TRANSACTIONDATE) == basar::SS_UNSET))
		{
			basar::VarString res;
			res.format("AND transactiondate = #%s# ", constants::TRANSACTIONDATE);
			resolve(res);
		}
		if( getPropertyList().contains(constants::TODATE)	&&
			getPropertyList().contains(constants::FROMDATE)	&&
			!(getPropertyList().getState(constants::TODATE) == basar::SS_UNSET	||
			getPropertyList().getState(constants::FROMDATE) == basar::SS_UNSET)	)
		{
			basar::VarString res;
			res.format("AND transactiondate BETWEEN #%s# AND #%s# ", constants::FROMDATE, constants::TODATE);
			resolve(res);
		} else {
			if (getPropertyList().contains(constants::TODATE) &&
				!(getPropertyList().getState(constants::TODATE) == basar::SS_UNSET))
			{
				basar::VarString res;
				res.format("AND transactiondate <= #%s# ", constants::TODATE);
				resolve(res);
			}
			else if (getPropertyList().contains(constants::FROMDATE) &&
				!(getPropertyList().getState(constants::FROMDATE) == basar::SS_UNSET))
			{
				basar::VarString res;
				res.format("AND transactiondate >= #%s# ", constants::FROMDATE);
				resolve(res);
			}
		}

		if( getPropertyList().contains(constants::TRANSACTIONTYPE) && !(getPropertyList().getState(constants::TRANSACTIONTYPE) == basar::SS_UNSET))
		{
			basar::VarString res;
			res.format("AND transactiontype = '#%s#' ", constants::TRANSACTIONTYPE);
			resolve(res);
		}
		if( getPropertyList().contains(constants::DELETEDFLAG) && !(getPropertyList().getState(constants::DELETEDFLAG) == basar::SS_UNSET))
		{
			basar::VarString res;
			res.format("AND deletedflag = '#%s#' ", constants::DELETEDFLAG);
			resolve(res);
		}
		if( getPropertyList().contains(constants::PROCESSEDFLAG) && !(getPropertyList().getState(constants::PROCESSEDFLAG) == basar::SS_UNSET))
		{
			basar::VarString res;
			res.format("AND processedflag = '#%s#' ", constants::PROCESSEDFLAG);
			resolve(res);
		}
		if( getPropertyList().contains(constants::CUSTOMERSUPPLIERNO) && !(getPropertyList().getState(constants::CUSTOMERSUPPLIERNO) == basar::SS_UNSET))
		{
			basar::VarString res;
			res.format("AND customersupplierno = #%s# ", constants::CUSTOMERSUPPLIERNO);
			resolve(res);
		}
		if( getPropertyList().contains(constants::INTERNAL_ACCOUNTS) &&
			!(getPropertyList().getState(constants::INTERNAL_ACCOUNTS) == basar::SS_UNSET))
		{
			basar::VarString res;
			res.format("AND customersupplierno >= #%s# ", constants::INTERNAL_ACCOUNTS);
			resolve(res);
		}
		// direct search for articlename only used in incominggoods.cpp
		if( getPropertyList().contains( constants::ARTICLENAME ) &&
			!(getPropertyList().getState( constants::ARTICLENAME ) == basar::SS_UNSET))
		{
			basar::VarString res;
			res.format( "AND artikel_name LIKE '#%s#%c' ", constants::ARTICLENAME, '%');
			resolve( res );
		}
		// direct search for customersuppliername only used in incominggoods.cpp
		if(	getPropertyList().contains( constants::CUSTOMERNAME ) &&
			!(getPropertyList().getState( constants::CUSTOMERNAME ) == basar::SS_UNSET) )
		{
			basar::VarString res;
			res.format( "AND customersuppliername LIKE '#%s#%c' ", constants::CUSTOMERNAME, '%');
			resolve( res );
		}
		if(	getPropertyList().contains( constants::CUSTOMER_SUPPLIER_NAME ) &&
			!(getPropertyList().getState( constants::CUSTOMER_SUPPLIER_NAME ) == basar::SS_UNSET) )
		{
			basar::VarString res;
			res.format( "AND customersuppliername LIKE '#%s#%c' ", constants::CUSTOMER_SUPPLIER_NAME, '%');
			resolve( res );
		}
		if( getPropertyList().contains( constants::SUPPLIERNAME ) &&
			!(getPropertyList().getState( constants::SUPPLIERNAME ) == basar::SS_UNSET) )
		{
			basar::VarString res;
			res.format( "AND customersuppliername LIKE '#%s#%c' ", constants::SUPPLIERNAME, '%');
			resolve( res );
		}

		if( ( getPropertyList().contains(constants::PURCHASEORDERNO) &&
			!(getPropertyList().getState(constants::PURCHASEORDERNO) == basar::SS_UNSET)) ||
			( getPropertyList().contains(constants::TRANSACTIONTYPE) &&
			!(getPropertyList().getState(constants::TRANSACTIONTYPE) == basar::SS_UNSET) &&
			  getPropertyList().getString(constants::TRANSACTIONTYPE) == TransactionType::DESTRUCTION) )
		{
			literal("ORDER BY 5, transactiondate desc, transactiontime desc ");
		}
		else
		{
			literal("ORDER BY transactiondate desc, transactiontime desc , catalogid desc "); //srk added catalogid desc
		}
 		BLOG_TRACE( narcotics::LoggerPool::loggerDomModules, getSQLString() );
	}

//-------------------------------------------------------------------------------------------------------//
//
//-------------------------------------------------------------------------------------------------------//
BUILDER_DEFINITION( SelectArticleNoByArticleName )

	ENSURE_PROPERTIES_ARE_SET( SelectArticleNoByArticleName, "articlename;branchno;" )

	void SelectArticleNoByArticleName::buildSQLString()
	{
		resolve("SELECT DISTINCT "
					"n.articleno AS articleno, "
					"z.packgroesse || ' ' || z.darreichform  || ' ' ||	z.artikel_name AS articlename, "
					"c.article_code AS articlecode, "
					"z.packgroesse , z.darreichform, z.artikel_name " // only for ordering !!!
				 "FROM narctransactioncatalog n, artikelzentral z, articlecodes c "
				 "WHERE n.branchno=#branchno# AND "
					"n.articleno=z.artikel_nr AND "
					"n.articleno=c.articleno AND c.preferred_flag=1 AND "
					"transactiontype!='u' AND "
					"artikel_name LIKE '#articlename#%%' "
				"ORDER BY z.packgroesse, z.darreichform, z.artikel_name ");
		BLOG_TRACE( narcotics::LoggerPool::loggerDomModules, getSQLString() );
	}

//-------------------------------------------------------------------------------------------------------//
//
//-------------------------------------------------------------------------------------------------------//
BUILDER_DEFINITION( SelDoubleBookings )
	ENSURE_PROPERTIES_ARE_SET( SelDoubleBookings, constants::BRANCHNO )

void SelDoubleBookings::buildSQLString()
{
    basar::I18nString sqlStatement;
    sqlStatement.append(
		"SELECT "
			"y.catalogid, "
			"y.qtytransaction, "
			"y.qtyinstock, "
			"y.customersuppliername, "
			"y.customersuppliercity AS customersupplierlocation, "
			"y.acknowledgementstatus, "
			"x.*, "
			"c.article_code AS articlecode, "
			"z.artikel_name AS articlename, "
			"z.darreichform AS pharmaform, "
			"z.einheit AS packageunit "
		"FROM ( "
			"SELECT     branchno, articleno, purchaseorderno, processedflag, deletedflag, "
			"           transactiondate, transactiontime, customersupplierno, transactiontype  "
			"FROM       narctransactioncatalog "
			"WHERE      branchno = #branchno# "
				"AND        transactiontype != 'u' "
				"AND        deletedflag = 0 "
				"AND        processedflag = 1 "
			"GROUP BY   1,2,3,4,5,6,7,8,9 "
			"HAVING COUNT(*) > 1 ) AS x "
			"LEFT OUTER JOIN    narctransactioncatalog y ON "
				"                   y.branchno=x.branchno "
				"AND y.articleno          = x.articleno "
				"AND y.transactiondate    = x.transactiondate "
				"AND y.transactiontime    = x.transactiontime "
				"AND y.customersupplierno = x.customersupplierno "
				"AND y.transactiontype    = x.transactiontype "
				"AND y.purchaseorderno    = x.purchaseorderno "
				"AND y.processedflag      = x.processedflag "
				"AND y.deletedflag        = x.deletedflag "
			"LEFT OUTER JOIN artikelzentral z ON y.articleno = z.artikel_nr "
			"LEFT OUTER JOIN articlecodes c ON y.articleno = c.articleno AND c.preferred_flag=1 "
		"WHERE NOT x.branchno IS NULL "
		"ORDER BY transactiondate DESC"
                       );

    resolve( sqlStatement );
	BLOG_TRACE( narcotics::LoggerPool::loggerDomModules, getSQLString() );
}

//-------------------------------------------------------------------------------------------------------//
//
//-------------------------------------------------------------------------------------------------------//
BUILDER_DEFINITION(SelIncomplete)
	ENSURE_PROPERTIES_ARE_SET(SelIncomplete,"branchno;")
	void SelIncomplete::buildSQLString()
	{
		resolve("SELECT "
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
					"n.transactiontype, "
					"n.purchaseorderno, "
					"n.qtytransaction, "
					"n.qtyinstock, "
					"n.processedflag, "
					"n.deletedflag, "
					"n.customersuppliername, "
					"n.acknowledgementstatus, "
					"n.customersuppliercity AS customersupplierlocation "
				 "FROM narctransactioncatalog n, artikelzentral z, articlecodes c "
				 "WHERE branchno=#branchno# AND "
					"n.articleno=z.artikel_nr AND "
					"n.articleno=c.articleno AND c.preferred_flag=1 AND "
					"n.transactiontype!='u' AND "
					"(n.customersupplierno==0 OR n.customersuppliername=='' OR n.customersuppliercity=='' OR n.articleno==0) ");
		if( getPropertyList().contains(constants::DELETEDFLAG) && !(getPropertyList().getState(constants::DELETEDFLAG) == basar::SS_UNSET))
		{
			basar::VarString res;
			res.format("AND deletedflag = '#%s#' ", constants::DELETEDFLAG);
			resolve(res);
		}
		if( getPropertyList().contains(constants::PROCESSEDFLAG) && !(getPropertyList().getState(constants::PROCESSEDFLAG) == basar::SS_UNSET))
		{
			basar::VarString res;
			res.format("AND processedflag = '#%s#' ", constants::PROCESSEDFLAG);
			resolve(res);
		}
 		BLOG_TRACE( narcotics::LoggerPool::loggerDomModules, getSQLString() );
	}

//-------------------------------------------------------------------------------------------------------//
//
//-------------------------------------------------------------------------------------------------------//
BUILDER_DEFINITION(SelNegativeStock)
	ENSURE_PROPERTIES_ARE_SET( SelNegativeStock, "branchno;articleno;" )
	SQL_PATTERN(SelNegativeStock,	"SELECT FIRST 1 catalogid "
									"FROM   narctransactioncatalog "
									"WHERE  branchno=#branchno# "
									"AND    qtyinstock<0 "
									"AND    articleno=#articleno#" )

//-------------------------------------------------------------------------------------------------------//
//
//-------------------------------------------------------------------------------------------------------//
BUILDER_DEFINITION(SelBookByArticleNo)
	ENSURE_PROPERTIES_ARE_SET( SelBookByArticleNo,"branchno; articleno;" )
	SQL_PATTERN( SelBookByArticleNo,	"SELECT FIRST 1 catalogid, branchno, articleno, artikel_name AS articlename, darreichform AS pharmaform, "
										"				einheit AS packageunit, transactiondate, transactiontime, "
										"				customersupplierno, transactiontype, purchaseorderno, qtytransaction, "
										"				qtyinstock, customersuppliername, customersuppliercity AS customersupplierlocation "
										"FROM		narctransactioncatalog, artikelzentral "
										"WHERE		branchno=#branchno# "
										"AND		articleno=#articleno# "
										"AND		deletetedflag='0' "
										"AND		processedflag='1' "
										"AND		articleno=artikel_nr "
										"AND		transactiontype != 'u' "
										"ORDER BY	transactiondate desc, transactiontime desc" )

//-------------------------------------------------------------------------------------------------------//
//
//-------------------------------------------------------------------------------------------------------//
BUILDER_DEFINITION(UpdQtyInStock)
	ENSURE_PROPERTIES_ARE_SET_AND_ONE_IN_STATE( UpdQtyInStock,"catalogid; qtyinstock;", "UPDATE" )
	void UpdQtyInStock::buildSQLString()
	{
		resolve("UPDATE narctransactioncatalog "
				"SET	qtyinstock=#qtyinstock# "
				"WHERE	catalogid=#catalogid#" );
        BLOG_TRACE( narcotics::LoggerPool::loggerDomModules, getSQLString() );
	}

//-------------------------------------------------------------------------------------------------------//
//
//-------------------------------------------------------------------------------------------------------//
BUILDER_DEFINITION(UpdQtyInStockNoState)
	ENSURE_PROPERTIES_ARE_SET( UpdQtyInStockNoState,"catalogid; qtyinstock;" )
	SQL_PATTERN( UpdQtyInStockNoState,	"UPDATE narctransactioncatalog "
								"SET	qtyinstock=#qtyinstock# "
								"WHERE	catalogid=#catalogid#" )

//-------------------------------------------------------------------------------------------------------//
//
//-------------------------------------------------------------------------------------------------------//
BUILDER_DEFINITION(UpdQtyInStockUsingDeltasFuture)
    ENSURE_PROPERTIES_ARE_SET(UpdQtyInStockUsingDeltasFuture, "articleno; qtyDelta; originalTransactionDate; originalTransactionTime; newTransactionDate; newTransactionTime; branchno;")

    void UpdQtyInStockUsingDeltasFuture::buildSQLString()
    {
        resolve("update narctransactioncatalog set qtyinstock=qtyinstock + #qtyDelta# where branchno = #branchno# and articleno = #articleno# and (transactiondate > #originalTransactionDate# or (transactiondate = #originalTransactionDate# and transactiontime > #originalTransactionTime#)) "
            " and (transactiondate < #newTransactionDate# or (transactiondate = #newTransactionDate# and transactiontime < #newTransactionTime#))");

        BLOG_TRACE( narcotics::LoggerPool::loggerDomModules, getSQLString() );
    }

//-------------------------------------------------------------------------------------------------------//
//
//-------------------------------------------------------------------------------------------------------//
BUILDER_DEFINITION(UpdQtyInStockUsingDeltasPast)
    ENSURE_PROPERTIES_ARE_SET(UpdQtyInStockUsingDeltasPast, "articleno; qtyDelta; originalTransactionDate; originalTransactionTime; newTransactionDate; newTransactionTime; branchno;")

    void UpdQtyInStockUsingDeltasPast::buildSQLString()
    {
        resolve("update narctransactioncatalog set qtyinstock=qtyinstock + #qtyDelta# where branchno = #branchno# and articleno = #articleno# and (transactiondate < #originalTransactionDate# or (transactiondate = #originalTransactionDate# and transactiontime < #originalTransactionTime#)) "
            " and (transactiondate > #newTransactionDate# or (transactiondate = #newTransactionDate# and transactiontime > #newTransactionTime#))");

        BLOG_TRACE( narcotics::LoggerPool::loggerDomModules, getSQLString() );
    }

//-------------------------------------------------------------------------------------------------------//
//
//-------------------------------------------------------------------------------------------------------//
BUILDER_DEFINITION(SelQuickPreviousBooking)
	ENSURE_PROPERTIES_ARE_SET( SelQuickPreviousBooking, "branchno; articleno; transactiondate; transactiontime;" )
	void SelQuickPreviousBooking::buildSQLString()
    { resolve("SELECT FIRST 1 qtyinstock "
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
        BLOG_TRACE( narcotics::LoggerPool::loggerDomModules, getSQLString() );
    }

//-------------------------------------------------------------------------------------------------------//
//
//-------------------------------------------------------------------------------------------------------//
BUILDER_DEFINITION(SelFollowingBookings)
	ENSURE_PROPERTIES_ARE_SET( SelFollowingBookings,"branchno; articleno; transactiondate; transactiontime;" )
	void SelFollowingBookings::buildSQLString()
	{
		resolve("SELECT		catalogid, qtyinstock, qtytransaction, transactiontype, transactiondate, transactiontime "
				"FROM		narctransactioncatalog "
				"WHERE		processedflag='1' AND deletedflag='0' "
					"AND		branchno=#branchno# AND articleno=#articleno# "
					"AND		(	(transactiondate=#transactiondate# AND transactiontime>=#transactiontime#) "
					"OR			(transactiondate>#transactiondate#)	) "
				"ORDER BY	transactiondate ASC, transactiontime ASC");
        BLOG_TRACE( narcotics::LoggerPool::loggerDomModules, getSQLString() );
    }

//-------------------------------------------------------------------------------------------------------//
//
//-------------------------------------------------------------------------------------------------------//
BUILDER_DEFINITION(SelInventoryCountData)

	ENSURE_PROPERTIES_ARE_SET(SelInventoryCountData,"branchno;")

	SQL_PATTERN(SelInventoryCountData,
				"SELECT		f.lagerfachnr AS storagelocation, z.artikel_nr AS articleno, c.article_code AS articlecode, "
				"           z.einheit AS packageunit, "
				"			z.artikel_name AS articlename, z.darreichform AS pharmaform, 0 as qtyinstock "
				"FROM       artikelzentral z "
				"INNER JOIN artikellokal f ON f.artikel_nr = z.artikel_nr "
				"INNER JOIN articlecodes c on c.articleno = z.artikel_nr and c.preferred_flag = 1 "
				"WHERE		f.filialnr		= #branchno# "
				"AND		bitand(z.etartklasse1, 512) = 512 "
				"AND		f.lagerfachnr	!= '' "
				"AND		f.artikelaktiv IN (1,4) "
				"ORDER BY 1")


BUILDER_DEFINITION(SelInventoryCountDataQuick)

	ENSURE_PROPERTIES_ARE_SET(SelInventoryCountDataQuick,"branchno; transactiondate; transactiontime;")

	void SelInventoryCountDataQuick::buildSQLString()
	{
		resolve("SELECT "
			"f.lagerfachnr AS storagelocation, "
			"z.artikel_nr AS articleno, "
			"c.article_code AS articlecode, "
			"z.einheit AS packageunit, "
			"z.artikel_name AS articlename, "
			"z.darreichform AS pharmaform, "
			"NVL(n.qtyinstock, 0) AS qtyinstock "
			"FROM artikelzentral z "
			"INNER JOIN artikellokal f ON f.artikel_nr = z.artikel_nr "
			"INNER JOIN articlecodes c ON c.articleno = z.artikel_nr AND c.preferred_flag = 1 "
			"LEFT JOIN narctransactioncatalog n ON n.articleno = f.artikel_nr AND n.branchno = f.filialnr "
			"WHERE bitand(z.etartklasse1, 512) = 512 "
			"AND f.lagerfachnr != '' "
			"AND f.artikelaktiv IN (1,4) "
			"AND n.branchno = #branchno# "
			"AND n.processedflag='1' "
			"AND n.deletedflag='0' "
			"AND n.transactiondate = (select max(transactiondate) as maxdate from narctransactioncatalog where branchno = #branchno# AND processedflag='1' AND deletedflag='0' and articleno = f.artikel_nr "
			"						  and ((transactiondate = #transactiondate# and transactiontime < #transactiontime#) or transactiondate < #transactiondate#)) "
			"AND n.transactiontime = (select max(transactiontime) from narctransactioncatalog where branchno = #branchno# AND processedflag='1' AND deletedflag='0' and articleno = f.artikel_nr and transactiondate = (select max(transactiondate) as maxdate from narctransactioncatalog where branchno = #branchno# AND processedflag='1' AND deletedflag='0' and articleno = f.artikel_nr "
			"                         and ((transactiondate = #transactiondate# and transactiontime < #transactiontime#) or transactiondate < #transactiondate#)) ) "
			"ORDER BY 1;");

        BLOG_TRACE( narcotics::LoggerPool::loggerDomModules, getSQLString() );
    }


//-------------------------------------------------------------------------------------------------------//
//
//-------------------------------------------------------------------------------------------------------//
BUILDER_DEFINITION(SelNarcArticle)

	bool SelNarcArticle::isExecutable() const
	{
		return true;
	}

	//	new: with the trunc-function the table 'artikelzentral' can be selected -
	void SelNarcArticle::buildSQLString()
	{
		resolve("SELECT	"
					"z.artikel_nr as articleno, "
					"c.article_code AS articlecode, "
					"z.einheit as packageunit, "
					"z.artikel_name as articlename "
				"FROM artikelzentral z, articlecodes c "
				"WHERE "
					"z.artikel_nr=c.articleno AND c.preferred_flag=1 AND "
					"trunc( etartklasse1/512 ) - 2 * trunc( etartklasse1/(512*2)) = 1 ");
//                    "bitand(etartklasse1, 4608) = 4608 ");//bit-magic: 4608 (hex: 0x1200) means narcotic article - since etartklasse1 has to contain 4608 (or 0x1200) bitand-masking works

		if( getPropertyList().contains(constants::ARTICLENO) &&
			!(getPropertyList().getState(constants::ARTICLENO) == basar::SS_UNSET))
		{
			basar::VarString res;
			res.format("AND artikel_nr = #%s# ", constants::ARTICLENO);
			resolve(res);
		}

		if( getPropertyList().contains(constants::ARTICLECODE) &&
			!(getPropertyList().getState(constants::ARTICLECODE) == basar::SS_UNSET))
		{
			basar::VarString res;
			res.format(" AND c.article_code = '#%s#' ", constants::ARTICLECODE);
			resolve(res);
		}

		literal("ORDER BY 4, 3" );
 		BLOG_TRACE( narcotics::LoggerPool::loggerDomModules, getSQLString() );
	}
//-------------------------------------------------------------------------------------------------------//
//
//-------------------------------------------------------------------------------------------------------//
BUILDER_DEFINITION(SelCountOfBookings)

	ENSURE_PROPERTIES_ARE_SET(SelCountOfBookings,"branchno;")

	void SelCountOfBookings::buildSQLString()
	{
		resolve(	"SELECT count(*) as countofbookings "
					"FROM   narctransactioncatalog "
					"WHERE  branchno        =  #branchno# "
					"AND	processedflag   =  '1' "
					"AND	deletedflag     =  '0' "
					"AND    transactiontype != 'u' ");

		if( getPropertyList().contains(constants::ARTICLENO) &&
			!(getPropertyList().getState(constants::ARTICLENO) == basar::SS_UNSET))
		{
			basar::VarString res;
			res.format("AND articleno = #%s# ", constants::ARTICLENO);
			resolve(res);
		}

		if( getPropertyList().contains(constants::TODATE)	&&
			!(getPropertyList().getState(constants::TODATE) == basar::SS_UNSET ))
		{
			basar::VarString res;
			res.format("AND transactiondate <= #%s# ", constants::TODATE);
			resolve(res);
		}

		if( getPropertyList().contains(constants::FROMDATE)	&&
			!(getPropertyList().getState(constants::FROMDATE) == basar::SS_UNSET ))
		{
			basar::VarString res;
			res.format("AND transactiondate >= #%s# ", constants::FROMDATE);
			resolve(res);
		}
 		BLOG_TRACE( narcotics::LoggerPool::loggerDomModules, getSQLString() );
	}

//-------------------------------------------------------------------------------------------------------//
//
//-------------------------------------------------------------------------------------------------------//
BUILDER_DEFINITION(SelectPrintBookData)

	ENSURE_PROPERTIES_ARE_SET( SelectPrintBookData,"branchno; articleno;" )

	void SelectPrintBookData::buildSQLString()
	{
		resolve("SELECT "
					"n.articleno, "
					"c.article_code AS articlecode, "
					"n.transactiondate, "
					"n.transactiontime, "
					"n.customersupplierno, "
					"n.customersuppliername, "
					"n.customersuppliercity AS customersupplierlocation, "
					"n.customersupplierstreet, "
					"n.customersuppliercip, "
					"n.transactiontype, "
					"n.purchaseorderno, "
					"n.qtytransaction, "
					"n.qtyinstock, "
					"n.contributionvoucherno "
				"FROM narctransactioncatalog n, articlecodes c "
				"WHERE "
					"n.articleno=c.articleno AND c.preferred_flag=1 AND "
					"n.articleno=#articleno# AND "
					"n.branchno=#branchno# AND "
					"n.processedflag = '1' AND "
					"n.deletedflag = '0' AND "
					"n.transactiontype != 'u' ");

		if( getPropertyList().contains(constants::TODATE)	&&
			!(getPropertyList().getState(constants::TODATE) == basar::SS_UNSET ))
		{
			basar::VarString res;
			res.format("AND transactiondate <= #%s# ", constants::TODATE);
			resolve(res);
		}

		if( getPropertyList().contains(constants::FROMDATE)	&&
			!(getPropertyList().getState(constants::FROMDATE) == basar::SS_UNSET ))
		{
			basar::VarString res;
			res.format("AND transactiondate >= #%s# ", constants::FROMDATE);
			resolve(res);
		}

		literal("ORDER BY articlecode, transactiondate, transactiontime ");
 		BLOG_TRACE( narcotics::LoggerPool::loggerDomModules, getSQLString() );
	}

//-------------------------------------------------------------------------------------------------------//
//
//-------------------------------------------------------------------------------------------------------//
BUILDER_DEFINITION( UpdSupplier )
	ENSURE_PROPERTIES_ARE_SET(UpdSupplier, "purchaseorderno; branchno; customersupplierno;")
	void UpdSupplier::buildSQLString()
	{
		resolve(
					"UPDATE narctransactioncatalog "
					"SET	customersupplierno=#customersupplierno#, "
					"		customersuppliername = (select besla_name "
					"				from pusupplier where besla_nr = "
					"					liefer_besla_nr and bga_nr = #customersupplierno#), "
					"		customersuppliercity = (select ort "
					"				from pusupplier where besla_nr = "
					"					liefer_besla_nr and bga_nr = #customersupplierno#), "
					"		customersupplierstreet = (select strasse "
					"				from pusupplier where besla_nr = "
					"					liefer_besla_nr and bga_nr = #customersupplierno#), "
					"		customersuppliercip = (select plz_strasse "
					"				from pusupplier where besla_nr = "
					"					liefer_besla_nr and bga_nr = #customersupplierno#) "
					"WHERE  branchno=#branchno# AND purchaseorderno=#purchaseorderno# AND processedflag = 0");
		BLOG_TRACE( narcotics::LoggerPool::loggerDomModules, getSQLString() );
	}

//-------------------------------------------------------------------------------------------------------//
//
//-------------------------------------------------------------------------------------------------------//
BUILDER_DEFINITION( UpdBook )

    bool UpdBook::isExecutable() const
    {
        basar::ConstString fun = "UpdBook::isExecutable()"; fun;
        bool executable = false;
        basar::db::aspect::AccessorPropertyTable_YIterator yit = getPropertyList();

        if( yit.arePropertiesSetAndOneMustBeInState(    "catalogid; articleno; transactiondate; transactiontime; customersupplierno; "
                                                        "customersupplierlocation; transactiontype; purchaseorderno; qtytransaction; "
                                                        "qtyinstock; processedflag; customersuppliername; acknowledgementstatus;",
                                                    basar::SS_UPDATE) ){
            executable = true;
        }

        return executable;
    }

    void UpdBook::buildSQLString()
	{
        resolve( 	"UPDATE narctransactioncatalog "
					"SET	articleno=#articleno#, transactiondate=#transactiondate#, transactiontime=#transactiontime#, "
							"customersupplierno=#customersupplierno#, transactiontype='#transactiontype#', "
							"purchaseorderno=#purchaseorderno#, qtytransaction=#qtytransaction#, qtyinstock=#qtyinstock#, "
							"processedflag = '#processedflag#', customersuppliername = '#customersuppliername#', "
							"customersuppliercity = '#customersupplierlocation#' " );

        if (getPropertyList().contains(constants::ACKNOWLEDGEMENTSTATUS) &&
            !(getPropertyList().getState(constants::ACKNOWLEDGEMENTSTATUS) == basar::SS_UNSET))
        {
            basar::VarString res;
            res.format(", acknowledgementstatus = #%s# ", constants::ACKNOWLEDGEMENTSTATUS);
            resolve(res);
        }
		if( getPropertyList().contains(constants::NARCUNIT) &&
			!(getPropertyList().getState(constants::NARCUNIT) == basar::SS_UNSET))
		{
			basar::VarString res;
			res.format(", narcunit = '#%s#' ", constants::NARCUNIT);
			resolve(res);
		}
		if( getPropertyList().contains(constants::NARCGROUP) &&
			!(getPropertyList().getState(constants::NARCGROUP) == basar::SS_UNSET))
		{
			basar::VarString res;
			res.format(", narcgroup = '#%s#' ", constants::NARCGROUP);
			resolve(res);
		}
		if( getPropertyList().contains(constants::NARCNAME) &&
			!(getPropertyList().getState(constants::NARCNAME) == basar::SS_UNSET))
		{
			basar::VarString res;
			res.format(", narcname = '#%s#' ", constants::NARCNAME);
			resolve(res);
		}
		if( getPropertyList().contains(constants::NARCPOTENCY) &&
			!(getPropertyList().getState(constants::NARCPOTENCY) == basar::SS_UNSET))
		{
			basar::VarString res;
			res.format(", narcpotency = #%s# ", constants::NARCPOTENCY);
			resolve(res);
		}
		if( getPropertyList().contains(constants::NARCFACTOR) &&
			!(getPropertyList().getState(constants::NARCFACTOR) == basar::SS_UNSET))
		{
			basar::VarString res;
			res.format(", narcfactor = #%s# ", constants::NARCFACTOR);
			resolve(res);
		}
		if( getPropertyList().contains(constants::CUSTOMER_SUPPLIER_STREET) &&
			!(getPropertyList().getState(constants::CUSTOMER_SUPPLIER_STREET) == basar::SS_UNSET))
		{
			basar::VarString res;
			res.format(", customersupplierstreet = '#%s#' ", constants::CUSTOMER_SUPPLIER_STREET);
			resolve(res);
		}
		if (getPropertyList().contains(constants::CUSTOMER_SUPPLIER_CIP) &&
			!(getPropertyList().getState(constants::CUSTOMER_SUPPLIER_CIP) == basar::SS_UNSET))
		{
			basar::VarString res;
			res.format(", customersuppliercip = '#%s#' ", constants::CUSTOMER_SUPPLIER_CIP);
			resolve(res);
		}
		resolve( "WHERE catalogid = #catalogid#" );
		BLOG_TRACE( narcotics::LoggerPool::loggerDomModules, getSQLString() );
	}

//-------------------------------------------------------------------------------------------------------//
//
//-------------------------------------------------------------------------------------------------------//
BUILDER_DEFINITION( InsBook )

    bool InsBook::isExecutable() const
    {
        basar::ConstString fun = "InsBook::isExecutable()"; fun;
        bool executable = false;
        basar::db::aspect::AccessorPropertyTable_YIterator yit = getPropertyList();

		if( yit.arePropertiesSetAndOneMustBeInState("articleno; transactiondate; transactiontime; customersupplierno; customersuppliername; customersupplierlocation; "
													"customersupplierstreet; customersuppliercip; "
												    "transactiontype; purchaseorderno; qtytransaction; branchno; narcunit; narcgroup; narcname; narcpotency; narcfactor;",
                                                    basar::SS_INSERT))
        {
            executable = true;
        }

        return executable;
    }

	void InsBook::buildSQLString()
	{
		resolve(
					"INSERT		into narctransactioncatalog "
					"			(articleno, transactiondate, transactiontime, customersupplierno, transactiontype, "
					"			 purchaseorderno, qtytransaction, deletedflag, processedflag, branchno, "
					"			 customersuppliername, customersuppliercity, customersupplierstreet, customersuppliercip, narcunit, narcgroup, narcname, narcpotency, narcfactor  "
                    );
                if (isContainedAndSet("acknowledgementstatus"))
                {
                    resolve(", acknowledgementstatus ");
                }
    resolve(        " ) "
					"VALUES		(#articleno#, #transactiondate#, #transactiontime#, #customersupplierno#, '#transactiontype#', "
					"			 #purchaseorderno#, #qtytransaction#, '0', '1', #branchno#, "
					"			 '#customersuppliername#', '#customersupplierlocation#', '#customersupplierstreet#', '#customersuppliercip#', '#narcunit#', '#narcgroup#', '#narcname#', #narcpotency#, #narcfactor# " );
                if (isContainedAndSet("acknowledgementstatus"))
                {
                    resolve(", #acknowledgementstatus# ");
                }
    resolve (")");

		BLOG_TRACE( narcotics::LoggerPool::loggerDomModules, getSQLString() );
	}

//-------------------------------------------------------------------------------------------------------//
//
//-------------------------------------------------------------------------------------------------------//
BUILDER_DEFINITION(UpdStrikeOutBook)

	ENSURE_PROPERTIES_ARE_SET(UpdStrikeOutBook,"catalogid;")

	SQL_PATTERN(UpdStrikeOutBook,
	            "UPDATE narctransactioncatalog "
				"SET  deletedflag='1' "
				"WHERE catalogid=#catalogid#")

//-------------------------------------------------------------------------------------------------------//
//
//-------------------------------------------------------------------------------------------------------//
BUILDER_DEFINITION( UpdRestoreBook )
	ENSURE_PROPERTIES_ARE_SET( UpdRestoreBook, "catalogid;" )
	SQL_PATTERN(UpdRestoreBook,	"UPDATE narctransactioncatalog "
								"SET  deletedflag='0'"
								"WHERE catalogid=#catalogid#")

//-------------------------------------------------------------------------------------------------------//
//
//-------------------------------------------------------------------------------------------------------//
BUILDER_DEFINITION( UpdOrderChanges )
	ENSURE_PROPERTIES_ARE_SET( UpdOrderChanges, "purchaseorderno; customersupplierno; customersuppliername; "
	"customersupplierlocation; transactiondate; transactiontime; transactiontype;")

	void UpdOrderChanges::buildSQLString()
	{
		basar::VarString res;
		bool needKomma = false;

		resolve( "UPDATE narctransactioncatalog SET ");
		if( getPropertyList().contains( constants::CUSTOMERSUPPLIERNO ) &&
		  ( getPropertyList().getState( constants::CUSTOMERSUPPLIERNO ) == basar::SS_UPDATE))
		{
			res.format("customersupplierno = #%s#", constants::CUSTOMERSUPPLIERNO);
			resolve(res);
			needKomma = true;
		}
		if( getPropertyList().contains( constants::CUSTOMER_SUPPLIER_NAME ) &&
		  ( getPropertyList().getState( constants::CUSTOMER_SUPPLIER_NAME ) == basar::SS_UPDATE))
		{
			if (needKomma)
				resolve(", ");
			res.format("customersuppliername = '#%s#'", constants::CUSTOMER_SUPPLIER_NAME);
			resolve(res);
			needKomma = true;
		}
		if( getPropertyList().contains( constants::CUSTOMER_SUPPLIER_LOCATION ) &&
		  ( getPropertyList().getState( constants::CUSTOMER_SUPPLIER_LOCATION ) == basar::SS_UPDATE))
		{
			if (needKomma)
				resolve(", ");
			res.format("customersuppliercity = '#%s#'", constants::CUSTOMER_SUPPLIER_LOCATION);
			resolve(res);
			needKomma = true;
		}
		if( getPropertyList().contains( constants::TRANSACTIONTIME ) &&
		  ( getPropertyList().getState( constants::TRANSACTIONTIME ) == basar::SS_UPDATE))
		{
			if (needKomma)
				resolve(", ");
			res.format("transactiontime = #%s#", constants::TRANSACTIONTIME);
			resolve( res );
			needKomma = true;
		}

		if( getPropertyList().contains( constants::NEWTRANSACTIONDATE ) &&
		  ( getPropertyList().getState( constants::NEWTRANSACTIONDATE ) == basar::SS_CLEAN))
		{
			if (needKomma)
				resolve(", ");
			res.format("transactiondate = #%s#", constants::NEWTRANSACTIONDATE);
			resolve(res);
			needKomma = true;
		}

        if( getPropertyList().contains( constants::NEWTRANSACTIONTIME ) &&
		  ( getPropertyList().getState( constants::NEWTRANSACTIONTIME ) == basar::SS_CLEAN))
		{
			if (needKomma)
				resolve(", ");
			res.format("transactiontime = #%s#", constants::NEWTRANSACTIONTIME);
			resolve(res);
			needKomma = true;
		}

		if( isContainedAndSet( constants::NEWTRANSACTIONTYPE ))
		{
			if (needKomma)
				resolve(", ");
			res.format("transactiontype = '#%s#'", constants::NEWTRANSACTIONTYPE);
			resolve(res);
			needKomma = true;
		}

		if( isContainedAndSet( constants::ACKNOWLEDGEMENTSTATUS ))
		{
			if (needKomma)
				resolve(", ");
			res.format("acknowledgementstatus = #%s#", constants::ACKNOWLEDGEMENTSTATUS);
			resolve(res);
			needKomma = true;
		}

		if( isContainedAndSet( constants::CUSTOMER_SUPPLIER_STREET ) )
		{
			if (needKomma)
				resolve(", ");
			res.format("customersupplierstreet = '#%s#'", constants::CUSTOMER_SUPPLIER_STREET);
			resolve(res);
			needKomma = true;
		}

		if (isContainedAndSet(constants::CUSTOMER_SUPPLIER_CIP))
		{
			if (needKomma)
				resolve(", ");
			res.format("customersuppliercip = '#%s#'", constants::CUSTOMER_SUPPLIER_CIP);
			resolve(res);
			needKomma = true;
		}
		
		resolve("WHERE "
			"purchaseorderno = #purchaseorderno# AND "
			"transactiondate = #transactiondate# AND "
			"branchno = #branchno# AND "
			"transactiontype = '#transactiontype#'");

		if (isContainedAndSet(constants::CONTRIBUTIONVOUCHERNO))
		{
			resolve(" AND contributionvoucherno = #contributionvoucherno#");
		}

		// debugging and tracing of sqlstatement
		BLOG_TRACE( narcotics::LoggerPool::loggerDomModules, getSQLString() );
	}

//-------------------------------------------------------------------------------------------------------//
//
//-------------------------------------------------------------------------------------------------------//
BUILDER_DEFINITION( SelectInitialInventory )
	ENSURE_PROPERTIES_ARE_SET( SelectInitialInventory, "branchno;articleno;fromdate;" )
	SQL_PATTERN( SelectInitialInventory,	"SELECT FIRST 1 qtyinstock "
											"FROM	narctransactioncatalog "
											"WHERE	branchno		= #branchno# "
											"AND	articleno		= #articleno# "
											"AND	transactiondate < #fromdate# "
											"AND    deletedflag = 0	  AND processedflag = 1 "
											"ORDER BY transactiondate DESC, transactiontime DESC")

//-------------------------------------------------------------------------------------------------------//
// ACCESSOR DEFINITION
//-------------------------------------------------------------------------------------------------------//
BEGIN_ACCESSOR_DEFINITION( "BookingCatalog" )
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
        "string customersupplierstreet; "
		"string customersuppliercip; "
		"string customersupplierlocation; "
		"string transactiontype; "
		"string newtransactiontype; "
		"int32 purchaseorderno; "
		"int32 qtytransaction; "
		"int32 qtyinstock; "
		"string deletedflag; "
		"string processedflag; "
		"string storagelocation; "
		"int32 balance; "
		"decimal countofbookings; "
		"string narcunit; "
		"string narcgroup; "
		"string narcname; "
		"decimal narcpotency; "
		"decimal narcfactor; "
		"int32 contributionvoucherno; "
		"int32 newTransactionDate; "
		"string selection; "
		"int16 acknowledgementstatus; "
		"string username; "
		"int32 printeddate; "
		"int32 printedtime; "
        "int32 originalTransactionDate; "
        "int32 originalTransactionTime; "
        "int32 newTransactionTime; "
        "int32 qtyDelta; "
        )

	//----------------------------------------------------------//
	// SELECTS
	//----------------------------------------------------------//
	ACCESS_METHOD( "SelCatalog" )
		SQL_BUILDER_CREATE(SelCat)
		SQL_BUILDER_PUSH_BACK(SelCat)

	ACCESS_METHOD( "SelIncomplete" )
		SQL_BUILDER_CREATE(SelIncomplete)
		SQL_BUILDER_PUSH_BACK(SelIncomplete)

	ACCESS_METHOD( "SelBookingByArticleNo" )
		SQL_BUILDER_CREATE(SelBookByArticleNo)
		SQL_BUILDER_PUSH_BACK(SelBookByArticleNo)

	ACCESS_METHOD( "SelNegativeStock" )
		SQL_BUILDER_CREATE(SelNegativeStock)
		SQL_BUILDER_PUSH_BACK(SelNegativeStock)

	ACCESS_METHOD( "SelQuickPrevForUpdQtyInStock" )
		SQL_BUILDER_CREATE(SelQuickPreviousBooking)
		SQL_BUILDER_PUSH_BACK(SelQuickPreviousBooking)

	ACCESS_METHOD( "SelFollowForUpdQtyInStock" )
		SQL_BUILDER_CREATE(SelFollowingBookings)
		SQL_BUILDER_PUSH_BACK(SelFollowingBookings)

	ACCESS_METHOD( "SelectInventoryCountDataQuick" )
   		SQL_BUILDER_CREATE(SelInventoryCountDataQuick)
		SQL_BUILDER_PUSH_BACK(SelInventoryCountDataQuick)

	ACCESS_METHOD( "SelectInventoryCountData" )
		SQL_BUILDER_CREATE(SelInventoryCountData)
		SQL_BUILDER_PUSH_BACK(SelInventoryCountData)

	ACCESS_METHOD( "SelNarcArticle" )
		SQL_BUILDER_CREATE(SelNarcArticle)
		SQL_BUILDER_PUSH_BACK(SelNarcArticle)

	ACCESS_METHOD( "SelCountOfBookings" )
		SQL_BUILDER_CREATE(SelCountOfBookings)
		SQL_BUILDER_PUSH_BACK(SelCountOfBookings)

	ACCESS_METHOD( "SelPrintBookData" )
		SQL_BUILDER_CREATE(SelectPrintBookData)
		SQL_BUILDER_PUSH_BACK(SelectPrintBookData)

	ACCESS_METHOD( "SelectInitialInventory" )
		SQL_BUILDER_CREATE( SelectInitialInventory )
		SQL_BUILDER_PUSH_BACK( SelectInitialInventory )

	ACCESS_METHOD( "SelectArticleNoByArticleName" )
		SQL_BUILDER_CREATE( SelectArticleNoByArticleName )
		SQL_BUILDER_PUSH_BACK( SelectArticleNoByArticleName )

    ACCESS_METHOD( "SelDoubleBookings" )
		SQL_BUILDER_CREATE( SelDoubleBookings )
		SQL_BUILDER_PUSH_BACK( SelDoubleBookings )

	//----------------------------------------------------------//
	// UPDATES
	//----------------------------------------------------------//
	ACCESS_METHOD( "UpdRestoreBook" )
		SQL_BUILDER_CREATE(UpdRestoreBook)
		SQL_BUILDER_PUSH_BACK(UpdRestoreBook)

	ACCESS_METHOD( "UpdStrikeOutBook" )
		SQL_BUILDER_CREATE(UpdStrikeOutBook)
		SQL_BUILDER_PUSH_BACK(UpdStrikeOutBook)

	ACCESS_METHOD( "UpdateQtyInStock" )
		SQL_BUILDER_CREATE(UpdQtyInStock)
		SQL_BUILDER_PUSH_BACK(UpdQtyInStock)

    ACCESS_METHOD( "UpdateQtyInStockNoState" )
        SQL_BUILDER_CREATE(UpdQtyInStockNoState)
        SQL_BUILDER_PUSH_BACK(UpdQtyInStockNoState)

	ACCESS_METHOD( "UpdSupplier" )
		SQL_BUILDER_CREATE(UpdSupplier)
		SQL_BUILDER_PUSH_BACK(UpdSupplier)

	ACCESS_METHOD( "UpdOrderChanges" )
		SQL_BUILDER_CREATE(UpdOrderChanges)
		SQL_BUILDER_PUSH_BACK(UpdOrderChanges)

    ACCESS_METHOD( "UpdateQtyInStockUsingDeltasFuture" )
        SQL_BUILDER_CREATE(UpdQtyInStockUsingDeltasFuture)
        SQL_BUILDER_PUSH_BACK(UpdQtyInStockUsingDeltasFuture)

    ACCESS_METHOD( "UpdateQtyInStockUsingDeltasPast" )
        SQL_BUILDER_CREATE(UpdQtyInStockUsingDeltasPast)
        SQL_BUILDER_PUSH_BACK(UpdQtyInStockUsingDeltasPast)

	//----------------------------------------------------------//
	// INSERTS + UPDATES
	//----------------------------------------------------------//
	ACCESS_METHOD( "SaveBooking" )
		SQL_BUILDER_CREATE(UpdBook)
		SQL_BUILDER_PUSH_BACK(UpdBook)
		SQL_BUILDER_CREATE(InsBook)
		SQL_BUILDER_PUSH_BACK(InsBook)

END_ACCESSOR_DEFINITION

}//acc_bookingcatalog