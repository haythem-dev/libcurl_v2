//----------------------------------------------------------------------------
/*! \file
 *  \brief  string builder declaration for open order accessor
 *  \author Harald Bayer
 *  \date   02.08.2012
 */
//----------------------------------------------------------------------------

#ifndef GUARD_ACC_ORDER_H
#define GUARD_ACC_ORDER_H

//-------------------------------------------------------------------------------------------------------------------------//
// namespace section
//-------------------------------------------------------------------------------------------------------------------------//
namespace acc_order
{

//---------------------------------------------------------------------------
//! class for delete an order from narctransactioncatalog \n final class
BEGIN_WRITE_BUILDER_DECLARATION(DelOrderByPattern)
END_BUILDER_DECLARATION

//---------------------------------------------------------------------------
//! class for stetting the acknowledgement status of an order to for_print \n final class
BEGIN_WRITE_BUILDER_DECLARATION(UpdateOrderAckStatusByPatternSetToForPrint)
END_BUILDER_DECLARATION
//----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
//! class for selecting single previous booking (base for UpdQtyInStock) \n final class
////////////////////////////////////////////////////////////////////////////////
BEGIN_QUERY_BUILDER_DECLARATION(SelQuickPreviousBooking)
END_BUILDER_DECLARATION

////////////////////////////////////////////////////////////////////////////////
//! class for selecting all following bookings (all these will be updated for QtyInStock) \n final class
////////////////////////////////////////////////////////////////////////////////
BEGIN_QUERY_BUILDER_DECLARATION(SelFollowingBookings)
END_BUILDER_DECLARATION

////////////////////////////////////////////////////////////////////////////////
//! class for updating quantity in stock for specific article \n final class
////////////////////////////////////////////////////////////////////////////////
BEGIN_WRITE_BUILDER_DECLARATION(UpdQtyInStock)
END_BUILDER_DECLARATION

} //namespace acc_order

//----------------------------------------------------------------------------
#endif //GUARD_ACC_ORDER_H