//----------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  string builder definition for diffence quantity in stock accessor
 *  \author Roland Kiefert
 *  \date   17.02.2006
 */
//----------------------------------------------------------------------------------------------//
//includes
//----------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "acc_diffqtyinstock.h"
#include "loggerpool.h"

//----------------------------------------------------------------------------------------------//
// namespace section
//----------------------------------------------------------------------------------------------//
namespace acc_diffqtyinstock {

//----------------------------------------------------------------------------------------------//
// accessor definition
//----------------------------------------------------------------------------------------------//
BEGIN_ACCESSOR_DEFINITION("DiffQtyInStock")

	PROPERTY_DESCRIPTION_LIST(	"int32 articleno;	 string articlename; string articlecode; "
								"int32 creationdate; int32 diffqtyinstock; decimal countofdiff;")

	ACCESS_METHOD("SelDiffQtyInStock")
		SQL_BUILDER_CREATE(SelDiffQtyInStock)
		SQL_BUILDER_PUSH_BACK(SelDiffQtyInStock)

	ACCESS_METHOD("SelCountDiffQtyInStock")
		SQL_BUILDER_CREATE(SelCountDiffQtyInStock)
		SQL_BUILDER_PUSH_BACK(SelCountDiffQtyInStock)

	ACCESS_METHOD("SelLatestByArticleNo")
		SQL_BUILDER_CREATE(SelLatestByArticleNo)
		SQL_BUILDER_PUSH_BACK(SelLatestByArticleNo)

	ACCESS_METHOD("DelDiffQtyInStock")
		SQL_BUILDER_CREATE(DelDiffQtyInStock)
		SQL_BUILDER_PUSH_BACK(DelDiffQtyInStock)

	ACCESS_METHOD("InsertDiffQtyInStock")
		SQL_BUILDER_CREATE(InsertDiffQtyInStock_CreateTempTable)
		SQL_BUILDER_PUSH_BACK(InsertDiffQtyInStock_CreateTempTable)
		SQL_BUILDER_CREATE(InsertDiffQtyInStock_FillTempTable)
		SQL_BUILDER_PUSH_BACK(InsertDiffQtyInStock_FillTempTable)
		SQL_BUILDER_CREATE(InsertDiffQtyInStock_CreateTempIndex)
		SQL_BUILDER_PUSH_BACK(InsertDiffQtyInStock_CreateTempIndex)
		SQL_BUILDER_CREATE(InsertDiffQtyInStock_Insert)
		SQL_BUILDER_PUSH_BACK(InsertDiffQtyInStock_Insert)
		SQL_BUILDER_CREATE(InsertDiffQtyInStock_DropTempTable)
		SQL_BUILDER_PUSH_BACK(InsertDiffQtyInStock_DropTempTable)

	ACCESS_METHOD("InsertNegativeQtyInStock")
		SQL_BUILDER_CREATE(InsertNegativeQtyInStock)
		SQL_BUILDER_PUSH_BACK(InsertNegativeQtyInStock)

END_ACCESSOR_DEFINITION

//----------------------------------------------------------------------------------------------//
// builder definition
//----------------------------------------------------------------------------------------------//
BUILDER_DEFINITION(SelDiffQtyInStock)

	ENSURE_PROPERTIES_ARE_SET(SelDiffQtyInStock,"branchno;")

	SQL_PATTERN( SelDiffQtyInStock,
				"SELECT n.articleno, lpad(trim(c.article_code), 8, '0') AS articlecode, artikel_name AS articlename, creationdate, diffqtyinstock  "
				"FROM	narcdiffqtyinstock n, artikelzentral, articlecodes c "
                "WHERE	branchno=#branchno# "
				"AND	n.articleno=artikelzentral.artikel_nr AND n.articleno=c.articleno and c.preferred_flag=1 ORDER BY articlecode ")

//----------------------------------------------------------------------------------------------//
BUILDER_DEFINITION(SelCountDiffQtyInStock)

	ENSURE_PROPERTIES_ARE_SET(SelCountDiffQtyInStock,"branchno;")

SQL_PATTERN(SelCountDiffQtyInStock,
			"SELECT count(*) as countofdiff "
			"FROM narcdiffqtyinstock "
            "WHERE branchno=#branchno# ")

//----------------------------------------------------------------------------------------------//
BUILDER_DEFINITION( SelLatestByArticleNo )
	ENSURE_PROPERTIES_ARE_SET( SelLatestByArticleNo, "branchno; articleno;" )
	SQL_PATTERN( SelLatestByArticleNo,	"SELECT n1.branchno, n1.articleno, n1.creationdate, n1.diffqtyinstock, c.article_code AS articlecode "
										"FROM   narcdiffqtyinstock n1, articlecodes c "
                                        "WHERE  n1.branchno=#branchno# "
                                        "AND    n1.articleno=#articleno# "
										"AND    n1.articleno=c.articleno and c.preferred_flag=1 "
										"AND    creationdate = (SELECT	max(creationdate) "
										"						FROM	narcdiffqtyinstock "
										"						WHERE	articleno = n1.articleno "
										"						AND		branchno = n1.branchno)" )

//----------------------------------------------------------------------------------------------//
BUILDER_DEFINITION( DelDiffQtyInStock )
	ENSURE_PROPERTIES_ARE_SET( DelDiffQtyInStock, "branchno;" )

	void DelDiffQtyInStock::buildSQLString()
	{
		resolve("DELETE FROM narcdiffqtyinstock WHERE  branchno=#branchno#");

		BLOG_TRACE(narcotics::LoggerPool::loggerDomModules, getSQLString());
	}

//----------------------------------------------------------------------------------------------//

BUILDER_DEFINITION(InsertDiffQtyInStock_CreateTempTable)
ENSURE_PROPERTIES_ARE_SET(InsertDiffQtyInStock_CreateTempTable, "branchno; creationdate;")
void InsertDiffQtyInStock_CreateTempTable::buildSQLString()
{
	resolve(
		"CREATE TEMP TABLE tmp_btm "
		"("
			"articleno INTEGER DEFAULT 0 NOT NULL, "
			"catalogid INT8 DEFAULT 0 NOT NULL, "
			"qtyinstock INTEGER DEFAULT 0 NOT NULL"
		") WITH NO LOG;"
	);
	BLOG_TRACE(narcotics::LoggerPool::loggerDomModules, getSQLString());
}

BUILDER_DEFINITION(InsertDiffQtyInStock_FillTempTable)
ENSURE_PROPERTIES_ARE_SET(InsertDiffQtyInStock_FillTempTable, "branchno; creationdate;")
void InsertDiffQtyInStock_FillTempTable::buildSQLString()
{
	resolve(
		"INSERT INTO tmp_btm "
		"SELECT z.artikel_nr, NVL(c.catalogid, 0), NVL(c.qtyinstock, 0) "
		"FROM artikelzentral z, outer narctransactioncatalog c "
		"WHERE c.branchno=#branchno# AND "
		"z.artikel_nr = c.articleno and bitand(z.etartklasse1, 512) = 512 "
		"and (c.transactiondate*1000000+c.transactiontime) = "
			"(select max (transactiondate*1000000 + transactiontime) from narctransactioncatalog "
			"where articleno = c.articleno and branchno = c.branchno and deletedflag = '0' AND processedflag = '1' AND transactiondate < #creationdate#); "
	);
	BLOG_TRACE(narcotics::LoggerPool::loggerDomModules, getSQLString());
}

BUILDER_DEFINITION(InsertDiffQtyInStock_CreateTempIndex)
ENSURE_PROPERTIES_ARE_SET(InsertDiffQtyInStock_CreateTempIndex, "branchno; creationdate;")
void InsertDiffQtyInStock_CreateTempIndex::buildSQLString()
{
	resolve(
		"CREATE INDEX i_tmp_btm ON tmp_btm (articleno) USING BTREE;"
	);
	BLOG_TRACE(narcotics::LoggerPool::loggerDomModules, getSQLString());
}

BUILDER_DEFINITION(InsertDiffQtyInStock_Insert)
ENSURE_PROPERTIES_ARE_SET(InsertDiffQtyInStock_Insert, "branchno; creationdate;")
void InsertDiffQtyInStock_Insert::buildSQLString()
{
	resolve(
		"INSERT INTO narcdiffqtyinstock (branchno, articleno, creationdate, diffqtyinstock) "
			"SELECT #branchno#, articleno, #creationdate#, bestandtagesanfang-qtyinstock as diff "
			"FROM tmp_btm, artikellokal "
			"WHERE filialnr= #branchno# AND qtyinstock <> bestandtagesanfang and articleno=artikel_nr; "
	);
	BLOG_TRACE(narcotics::LoggerPool::loggerDomModules, getSQLString());
}

BUILDER_DEFINITION(InsertDiffQtyInStock_DropTempTable)
ENSURE_PROPERTIES_ARE_SET(InsertDiffQtyInStock_DropTempTable, "branchno; creationdate;")
void InsertDiffQtyInStock_DropTempTable::buildSQLString()
{
	resolve(
		"DROP TABLE tmp_btm;"
	);
	BLOG_TRACE(narcotics::LoggerPool::loggerDomModules, getSQLString());
}

//----------------------------------------------------------------------------------------------//
//----------------------------------------------------------------------------------------------//

BUILDER_DEFINITION( InsertNegativeQtyInStock )
	ENSURE_PROPERTIES_ARE_SET( InsertNegativeQtyInStock, "branchno; creationdate;" )

	void InsertNegativeQtyInStock::buildSQLString()
	{
		resolve("INSERT into narcdiffqtyinstock ( branchno, articleno, creationdate, diffqtyinstock )  "
											"SELECT #branchno#, articleno, #creationdate#, 0 "
											"FROM narctransactioncatalog "
                                            "WHERE branchno = #branchno# and qtyinstock < 0 "
											"AND processedflag = '1' and deletedflag = '0' "
											"AND articleno not in (select articleno from narcdiffqtyinstock) "
											"group by articleno; ");

		BLOG_TRACE(narcotics::LoggerPool::loggerDomModules, getSQLString());
	}

} //namespace acc_numberreturngoods