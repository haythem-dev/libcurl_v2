//----------------------------------------------------------------------------
/*! \file
 *  \brief  string builder declaration for open order accessor
 *  \author Harald Bayer
 *  \date   02.08.2012
 */
//----------------------------------------------------------------------------

#ifndef GUARD_ACC_ORDERPOSITIONS_H
#define GUARD_ACC_ORDERPOSITIONS_H

//-------------------------------------------------------------------------------------------------------------------------//
// namespace section
//-------------------------------------------------------------------------------------------------------------------------//
namespace acc_orderpositions
{

//---------------------------------------------------------------------------
//! class for selecting orders by pattern from narctransactioncatalog \n final class
//BEGIN_QUERY_BUILDER_DECLARATION(SelOrderByPattern)
//END_BUILDER_DECLARATION

//---------------------------------------------------------------------------
//! class for delete an order from narctransactioncatalog \n final class
BEGIN_QUERY_BUILDER_DECLARATION(SelOrderPositionsByPattern)
END_BUILDER_DECLARATION


BEGIN_WRITE_BUILDER_DECLARATION(DelOrderPositionsBySelection)
END_BUILDER_DECLARATION

} //namespace acc_openorder

#endif //GUARD_ACC_ORDERPOSITIONS_H