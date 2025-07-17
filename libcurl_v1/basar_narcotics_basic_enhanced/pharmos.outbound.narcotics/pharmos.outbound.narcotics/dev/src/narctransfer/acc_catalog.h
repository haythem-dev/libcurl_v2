//----------------------------------------------------------------------------
/*! \file
 *  \brief  string builder declaration for bookingcatalog-accessor
 *  \author Roland Kiefert
 *  \date   17.02.2006
 */
//----------------------------------------------------------------------------

#ifndef GUARD_ACC_CATALOG_H
#define GUARD_ACC_CATALOG_H

////////////////////////////////////////////////////////////////////////////////
//! namespace for catalog-accessor
namespace acc_catalog
{

//----------------------------------------------------------------------------//
// SELECTS
//----------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////
//!  \n final class
////////////////////////////////////////////////////////////////////////////////
BEGIN_QUERY_BUILDER_DECLARATION( SelQuickPrevQtyInStock )
END_BUILDER_DECLARATION

BEGIN_QUERY_BUILDER_DECLARATION( SelSuccQtyInStock )
END_BUILDER_DECLARATION

BEGIN_QUERY_BUILDER_DECLARATION( SelUnprocessedPurchaseOrder )
END_BUILDER_DECLARATION

BEGIN_WRITE_BUILDER_DECLARATION( InsCatalog )
END_BUILDER_DECLARATION

BEGIN_WRITE_BUILDER_DECLARATION( InsOriginal )
END_BUILDER_DECLARATION

BEGIN_WRITE_BUILDER_DECLARATION( UpdQtyTransaction  )
END_BUILDER_DECLARATION

BEGIN_WRITE_BUILDER_DECLARATION( UpdAcknowledgementstatus  )
END_BUILDER_DECLARATION

BEGIN_AGGREGATE_QUERY_BUILDER_DECLARATION(CheckUniqueTimestampByBranchArticle)
END_BUILDER_DECLARATION

} //namespace acc_catalog

#endif	// GUARD_ACC_CATALOG_H