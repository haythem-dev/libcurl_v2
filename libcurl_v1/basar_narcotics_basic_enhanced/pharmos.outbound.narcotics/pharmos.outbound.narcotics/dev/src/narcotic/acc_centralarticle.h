//----------------------------------------------------------------------------
/*! \file
 *  \brief  string builder declaration for article-accessor
 *  \author Roland Kiefert
 *  \date   03.04.2006
 */
//----------------------------------------------------------------------------
#ifndef GUARD_ACC_ARTICLE_H
#define GUARD_ACC_ARTICLE_H

//----------------------------------------------------------------------------
//! namespace for bookingcatalog-accessor
namespace acc_centralarticle {
//---------------------------------------------------------------------------
//! class for selecting article data from central aricle \n final class
BEGIN_QUERY_BUILDER_DECLARATION(SelArticle)
END_BUILDER_DECLARATION

BEGIN_AGGREGATE_QUERY_BUILDER_DECLARATION(SelArticleNoByArticleCode)
END_BUILDER_DECLARATION

//---------------------------------------------------------------------------
//! class for selecting article data from central aricle with storage location from branch article \n final class
//BEGIN_QUERY_BUILDER_DECLARATION(SelArtWithStorageLoc)
//END_BUILDER_DECLARATION

} //namespaces acc_centralarticle

#endif	//GUARD_ACC_ARTICLE_H