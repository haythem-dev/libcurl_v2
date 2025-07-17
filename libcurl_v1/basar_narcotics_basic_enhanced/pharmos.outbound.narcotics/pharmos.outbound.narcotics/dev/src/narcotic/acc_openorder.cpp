//----------------------------------------------------------------------------
/*! \file
 *  \brief  string builder definition for open order accessor
 *  \author Roland Kiefert
 *  \date   13.04.2006
 */
//----------------------------------------------------------------------------

#include "commonheader.h"
#include "acc_openorder.h"
#include "loggerpool.h"

//----------------------------------------------------------------------------
namespace acc_openorder
{

//---------------------------------------------------------------------------
BUILDER_DEFINITION(SelOpenOrdersByPattern)
	
	ENSURE_PROPERTIES_ARE_SET(SelOpenOrdersByPattern,"branchno;datefrom;dateto;")

	void SelOpenOrdersByPattern::buildSQLString()
	{
		resolve(
			"SELECT "
				"n.branchno, "
				"n.articleno, "
				"c.article_code AS articlecode, "
				"z.artikel_name AS articlename, "
				"n.orderdate, "
				"n.purchaseorderno, "
				"n.supplierno, "
				"bl.besla_name as suppliername, "
				"n.qtyorder * z.btm_faktor as qtyorder, "
				"bl.bga_nr as bgano, "
				"b.btm_einheit as unity "
			"FROM narcopenorders n, artikelzentral z, pusupplier bl, narcgroup b, tradegroup w, articlecodes c "
			"WHERE n.branchno = #branchno# "
				"AND n.orderdate >= #datefrom# "
				"AND n.orderdate <= #dateto# "
				"AND n.articleno = z.artikel_nr "
				"AND w.artikel_nr = z.artikel_nr "
				"AND w.art = '6' "
				"AND w.warengruppe = b.btm_gruppe "
				"AND n.articleno=c.articleno AND c.preferred_flag=1 "
				"AND bl.besla_nr = n.supplierno "
			"ORDER BY "
				"n.orderdate, "
				"n.purchaseorderno");
		BLOG_TRACE(narcotics::LoggerPool::loggerDomModules, getSQLString());
	}

//---------------------------------------------------------------------------
BUILDER_DEFINITION(DelOpenOrder)
	
	ENSURE_PROPERTIES_ARE_SET(DelOpenOrder,"branchno;orderdate;purchaseorderno;")

	void DelOpenOrder::buildSQLString()
	{
		resolve(
            "DELETE FROM narcopenorders "
			"WHERE orderdate = #orderdate# "
				"AND branchno = #branchno# "
				"AND purchaseorderno = #purchaseorderno#");
		BLOG_TRACE(narcotics::LoggerPool::loggerDomModules, getSQLString());
	}

//---------------------------------------------------------------------------
BEGIN_ACCESSOR_DEFINITION("NarcoticsOpenOrders")

	PROPERTY_DESCRIPTION_LIST(
		"int16 branchno; "
		"int32 articleno; "
		"string articlecode; "
		"string articlename; "
		"int32 orderdate; "
		"int32 datefrom; "
		"int32 dateto; "
		"int32 purchaseorderno; "
		"int32 supplierno; "
		"string suppliername; "
		"decimal qtyorder; "
		"int32 bgano; "
		"string unity;"
		)

	ACCESS_METHOD("SelOpenOrdersByPattern")
		SQL_BUILDER_CREATE(SelOpenOrdersByPattern)
		SQL_BUILDER_PUSH_BACK(SelOpenOrdersByPattern)

	ACCESS_METHOD("DelOpenOrder")
		SQL_BUILDER_CREATE(DelOpenOrder)
		SQL_BUILDER_PUSH_BACK(DelOpenOrder)

END_ACCESSOR_DEFINITION
				
} //namespace acc_openorder