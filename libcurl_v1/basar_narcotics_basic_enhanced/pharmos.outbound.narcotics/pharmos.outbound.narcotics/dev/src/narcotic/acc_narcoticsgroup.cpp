//-------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  string builder definition for user-accessor + registration of user-accessor
 *  \author Roland Kiefert
 *  \date   17.02.2006
 */
//-------------------------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------------------------//
// includes
//-------------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "acc_narcoticsgroup.h"
#include "loggerpool.h"

//-------------------------------------------------------------------------------------------------//
// namespaces
//-------------------------------------------------------------------------------------------------//
namespace acc_narcoticsgroup
{

//-------------------------------------------------------------------------------------------------//
// accessor definition
//-------------------------------------------------------------------------------------------------//
BEGIN_ACCESSOR_DEFINITION("NarcoticsGroup")

	PROPERTY_DESCRIPTION_LIST(
		"string group; "
		"string name; "
		"decimal concentration; "
		"string packageunit; "
		"decimal narcfactor; "
		"int32 articleno; "
		"string articlename; "
		"string articlecode; "
		"string articlepackageunit; "
		"string articlepharmaform; "
		"string articlestocklocation; "
		"int32 qtyinstock; "
		"int16 acknowledgementstatus;")

	ACCESS_METHOD("SelGroup")
		SQL_BUILDER_CREATE(SelGroup)
		SQL_BUILDER_PUSH_BACK(SelGroup)

	ACCESS_METHOD("SelGroupByName")
		SQL_BUILDER_CREATE(SelGroupByName)
		SQL_BUILDER_PUSH_BACK(SelGroupByName)

	ACCESS_METHOD("SelStockLocationByArticleNo")
		SQL_BUILDER_CREATE(SelStockLocationByArticleNo)
		SQL_BUILDER_PUSH_BACK(SelStockLocationByArticleNo)

	ACCESS_METHOD("SelStockLocation2ByArticleNo")
		SQL_BUILDER_CREATE(SelStockLocation2ByArticleNo)
		SQL_BUILDER_PUSH_BACK(SelStockLocation2ByArticleNo)

END_ACCESSOR_DEFINITION

//-------------------------------------------------------------------------------------------------//
// builder definition
//-------------------------------------------------------------------------------------------------//
BUILDER_DEFINITION(SelGroup)

	ENSURE_PROPERTIES_ARE_SET(SelGroup,"group;")
	void SelGroup::buildSQLString()
	{
		resolve("SELECT b.btm_gruppe	AS group, "
						"b.btm_bez		AS name, "
						"b.btm_gehalt	AS concentration, "
						"b.btm_einheit	AS packageunit, "
						"w.artikel_nr	AS articleno, "
						"c.article_code AS articlecode, "
						"z.artikel_name	AS articlename, "
						"z.darreichform	AS articlepharmaform, "
						"z.einheit		AS articlepackageunit "
				"FROM narcgroup b, tradegroup w, artikelzentral z, articlecodes c "
				"WHERE w.art = '6' AND "
						"w.warengruppe = b.btm_gruppe AND "
						"w.artikel_nr = z.artikel_nr AND "
						"z.artikel_nr = c.articleno AND c.preferred_flag=1 "
						);
		if( getPropertyList().contains(constants::GROUP) &&
			!(getPropertyList().getState(constants::GROUP) == basar::SS_UNSET))
		{
			basar::VarString res;
			res.format("AND btm_gruppe = #%s# ", constants::GROUP);
			resolve(res);
		}
		BLOG_TRACE( narcotics::LoggerPool::loggerDomModules, getSQLString() );
	}

//-------------------------------------------------------------------------------------------------//
// builder definition
//-------------------------------------------------------------------------------------------------//
BUILDER_DEFINITION(SelGroupByName)

	ENSURE_PROPERTIES_ARE_SET(SelGroupByName,"name;")
	void SelGroupByName::buildSQLString()
	{
		resolve("SELECT btm_gruppe	AS group, "
				"		btm_bez		AS name "
				"FROM	narcgroup ");
		if( getPropertyList().contains(constants::NARCOTICNAME) &&
			!(getPropertyList().getState(constants::NARCOTICNAME) == basar::SS_UNSET))
		{
			basar::VarString res;
			res.format("WHERE btm_bez LIKE '#%s#%c' ", constants::NARCOTICNAME, '%');
			resolve(res);
		}
		literal("ORDER BY btm_bez ");
		BLOG_TRACE( narcotics::LoggerPool::loggerDomModules, getSQLString() );
	}

//-------------------------------------------------------------------------------------------------//
BUILDER_DEFINITION( SelStockLocationByArticleNo )

	ENSURE_PROPERTIES_ARE_SET( SelStockLocationByArticleNo, "branchno; articleno;" )

	void SelStockLocationByArticleNo::buildSQLString()
	{
		resolve("SELECT b.btm_gruppe AS group, "
						"b.btm_bez AS name, "
						"b.btm_gehalt AS concentration, "
						"b.btm_einheit AS packageunit, "
						"w.artikel_nr AS articleno, "
						"c.article_code AS articlecode, "
						"z.artikel_name AS articlename, "
						"z.darreichform AS articlepharmaform, "
						"z.einheit AS articlepackageunit, "
						"z.btm_faktor AS narcfactor, "  //as btm_faktor is only available on artikelzentral but not on artikelzentral the join has been changed from artikelzentral to artikelzentral
						"f.lagerfachnr AS articlestocklocation "
				"FROM narcgroup b, tradegroup w, artikelzentral z, artikellokal f, articlecodes c "
				"WHERE z.artikel_nr = w.artikel_nr AND "
						"z.artikel_nr = f.artikel_nr AND "
						"w.warengruppe = b.btm_gruppe AND "
						"z.artikel_nr = c.articleno AND c.preferred_flag=1 AND "
						"z.artikel_nr = #articleno# AND "
						"f.filialnr = #branchno# AND "
						"w.art = '6'");
		BLOG_TRACE(narcotics::LoggerPool::loggerDomModules, getSQLString());
	}

//-------------------------------------------------------------------------------------------------//
BUILDER_DEFINITION( SelStockLocation2ByArticleNo )

	ENSURE_PROPERTIES_ARE_SET( SelStockLocation2ByArticleNo, "branchno; articleno;" )

	void SelStockLocation2ByArticleNo::buildSQLString()
	{
		resolve("SELECT b.narcgroup AS group, "
						"b.narcname AS name, "
						"b.narcpotency AS concentration, "
						"b.narcunit AS packageunit, "
						"b.articleno AS articleno, "
						"c.article_code AS articlecode, "
						"z.artikel_name AS articlename, "
						"z.darreichform AS articlepharmaform, "
						"z.einheit AS articlepackageunit, "
						"z.btm_faktor AS narcfactor, " //as btm_faktor is only available on artikelzentral but not on artikelzentral the join has been changed from artikelzentral to artikelzentral
						"f.lagerfachnr AS articlestocklocation "
				"FROM narcinvalidarticle b, artikelzentral z, artikellokal f, articlecodes c "
				"WHERE z.artikel_nr = b.articleno AND "
						"z.artikel_nr = f.artikel_nr AND "
						"z.artikel_nr = c.articleno AND c.preferred_flag=1 AND "
						"z.artikel_nr = #articleno# AND "
						"f.filialnr = #branchno#");
		BLOG_TRACE(narcotics::LoggerPool::loggerDomModules, getSQLString());
	}

} //namespace acc_bookingcatalog