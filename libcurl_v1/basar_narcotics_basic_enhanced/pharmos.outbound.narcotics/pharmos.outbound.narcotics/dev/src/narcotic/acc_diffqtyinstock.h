//----------------------------------------------------------------------------
/*! \file
 *  \brief  string builder declaration for diffence quantity in stock accessor
 *  \author Roland Kiefert
 *  \date   17.02.2006
 */
//----------------------------------------------------------------------------

#ifndef GUARD_ACC_DIFFQTYINSTOCK_H
#define GUARD_ACC_DIFFQTYINSTOCK_H

//----------------------------------------------------------------------------
//! namespace for difference quantity in stock accessor
namespace acc_diffqtyinstock
{

//---------------------------------------------------------------------------//
// SELECTS
//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
//! class for selecting user id from narctransactioncatalog \n final class
//---------------------------------------------------------------------------//
BEGIN_QUERY_BUILDER_DECLARATION( SelDiffQtyInStock )
END_BUILDER_DECLARATION

//! class for selecting user id from narctransactcatalog \n final class
BEGIN_QUERY_BUILDER_DECLARATION( SelCountDiffQtyInStock )
END_BUILDER_DECLARATION

//! class for selecting only one row by articleno and branchno. \n final class
BEGIN_QUERY_BUILDER_DECLARATION( SelLatestByArticleNo )
END_BUILDER_DECLARATION

//---------------------------------------------------------------------------//
// INSERTS
//---------------------------------------------------------------------------//
//BEGIN_WRITE_BUILDER_DECLARATION( InsertDiffQtyInStock )
//END_BUILDER_DECLARATION

BEGIN_WRITE_BUILDER_DECLARATION( InsertDiffQtyInStock_CreateTempTable )
END_BUILDER_DECLARATION

BEGIN_WRITE_BUILDER_DECLARATION( InsertDiffQtyInStock_FillTempTable )
END_BUILDER_DECLARATION

BEGIN_WRITE_BUILDER_DECLARATION( InsertDiffQtyInStock_CreateTempIndex )
END_BUILDER_DECLARATION

BEGIN_WRITE_BUILDER_DECLARATION( InsertDiffQtyInStock_Insert )
END_BUILDER_DECLARATION

BEGIN_WRITE_BUILDER_DECLARATION( InsertDiffQtyInStock_DropTempTable )
END_BUILDER_DECLARATION

//! class for update diffqtyinstock with negativ qty in stock. \n final class
BEGIN_WRITE_BUILDER_DECLARATION( InsertNegativeQtyInStock )
END_BUILDER_DECLARATION

//---------------------------------------------------------------------------//
// DELETES
//---------------------------------------------------------------------------//
//! class for deleting all branch no specific datasets. \n final class
BEGIN_WRITE_BUILDER_DECLARATION( DelDiffQtyInStock )
END_BUILDER_DECLARATION


} //namespace acc_numberreturngoods

//----------------------------------------------------------------------------
#endif //GUARD_ACC_DIFFQTYINSTOCK_H