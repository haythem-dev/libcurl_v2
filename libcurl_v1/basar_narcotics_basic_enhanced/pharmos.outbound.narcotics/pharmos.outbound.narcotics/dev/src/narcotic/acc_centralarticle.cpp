//----------------------------------------------------------------------------
/*! \file
 *  \brief  string builder definition for central article accessor
 *  \author Roland Kiefert
 *  \date   03.04.2006
 */
//-------------------------------------------------------------------------------------------------//
// includes
//-------------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "acc_centralarticle.h"
#include "loggerpool.h"

//-------------------------------------------------------------------------------------------------//
// namespaces
//-------------------------------------------------------------------------------------------------//
namespace acc_centralarticle {

//---------------------------------------------------------------------------
BUILDER_DEFINITION(SelArticle)
	
	DONT_CHECK_PROPERTIES(SelArticle)
	
	void SelArticle::buildSQLString()
	{
		resolve("SELECT "
					"z.artikel_nr as articleno, "
					"c1.article_code AS articlecode, "
					"z.artikel_name AS articlename, "
					"z.darreichform AS pharmaform, "
					"z.einheit AS packageunit, "
					"l.lagerfachnr as storagelocation "
				"FROM artikelzentral z, artikellokal l, articlecodes c1 ");
		if( getPropertyList().isContainedAndSet("articlecode") )
		{
			resolve(", articlecodes c2 ");
		}
		resolve("WHERE "
					"z.artikel_nr=l.artikel_nr AND l.filialnr=#branchno# AND "
					"z.artikel_nr=c1.articleno and c1.preferred_flag=1 ");
		if( getPropertyList().isContainedAndSet("articleno") )
		{
			resolve("AND z.artikel_nr=#articleno# ");
		}
		if( getPropertyList().isContainedAndSet("articlecode") )
		{
			resolve("AND z.artikel_nr=c2.articleno ");
			resolve("AND c2.article_code='#articlecode#' ");
		}
		BLOG_TRACE( narcotics::LoggerPool::loggerDomModules, getSQLString() );
	}

//---------------------------------------------------------------------------
BUILDER_DEFINITION(SelArticleNoByArticleCode)
	
	ENSURE_PROPERTIES_ARE_SET(SelArticleNoByArticleCode, "articlecode")
	
	void SelArticleNoByArticleCode::buildSQLString()
	{
		resolve("SELECT articleno FROM articlecodes WHERE article_code='#articlecode#' and preferred_flag=1");
		BLOG_TRACE( narcotics::LoggerPool::loggerDomModules, getSQLString() );
	}

//---------------------------------------------------------------------------
BEGIN_ACCESSOR_DEFINITION("CentralArticle")

	PROPERTY_DESCRIPTION_LIST(
		"int32 articleno; "
		"string articlecode; "
		"string articlename; "
		"string pharmaform; "
		"string packageunit; "
		"string storagelocation; "
		"int16 branchno;")
	
	ACCESS_METHOD("SelArticle")
		SQL_BUILDER_CREATE(SelArticle)
		SQL_BUILDER_PUSH_BACK(SelArticle)

	ACCESS_METHOD("SelArticleNoByArticleCode")
		SQL_BUILDER_CREATE(SelArticleNoByArticleCode)
		SQL_BUILDER_PUSH_BACK(SelArticleNoByArticleCode)

END_ACCESSOR_DEFINITION
				
} //namespacesacc_centralarticle