//-------------------------------------------------------------------------------------------------//
/*! \file 
 *  \brief  Accessor for matching contribution voucher entries
 *  \author Benjamin Endlicher
 *  \date   03.05.2010
 */
//-------------------------------------------------------------------------------------------------//

#ifndef GUARD_CONTRIBUTIONVOUCHERACC_H
#define GUARD_CONTRIBUTIONVOUCHERACC_H

//-------------------------------------------------------------------------------------------------//
// namespaces
//-------------------------------------------------------------------------------------------------//
namespace acc_contributionvoucher
{

//----------------------------------------------------------------------------//
// SELECTS
//----------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////
//! class for selecting open acknowledgements of selloffs \n final class
////////////////////////////////////////////////////////////////////////////////
//BEGIN_QUERY_BUILDER_DECLARATION(SelAcknowledgementsCustomer)
//END_BUILDER_DECLARATION

////////////////////////////////////////////////////////////////////////////////
//! class for selecting open acknowledgements of supplier returns \n final class
////////////////////////////////////////////////////////////////////////////////
//BEGIN_QUERY_BUILDER_DECLARATION(SelAcknowledgementsSupplier)
//END_BUILDER_DECLARATION

////////////////////////////////////////////////////////////////////////////////
//! class for selecting data for delivery note copies \n final class
////////////////////////////////////////////////////////////////////////////////
BEGIN_QUERY_BUILDER_DECLARATION(SelOrderPositions)
END_BUILDER_DECLARATION

////////////////////////////////////////////////////////////////////////////////
//! class for selecting an order for selloffs \n final class
////////////////////////////////////////////////////////////////////////////////
BEGIN_QUERY_BUILDER_DECLARATION(SelOrderUnion)
END_BUILDER_DECLARATION

////////////////////////////////////////////////////////////////////////////////
//! class for selecting an order for selloffs \n final class
////////////////////////////////////////////////////////////////////////////////
//BEGIN_QUERY_BUILDER_DECLARATION(SelOrderCustomer)
//END_BUILDER_DECLARATION

////////////////////////////////////////////////////////////////////////////////
//! class for selecting an order for supplier returns \n final class
////////////////////////////////////////////////////////////////////////////////
//BEGIN_QUERY_BUILDER_DECLARATION(SelOrderSupplier)
//END_BUILDER_DECLARATION

////////////////////////////////////////////////////////////////////////////////
//! class for selecting bookings that have been inserted into narctransactioncatalog recently \n final class
////////////////////////////////////////////////////////////////////////////////
//BEGIN_QUERY_BUILDER_DECLARATION(SelNewInsertedBookings)
//END_BUILDER_DECLARATION

BEGIN_QUERY_BUILDER_DECLARATION(SelAcknowledgementStatus)
END_BUILDER_DECLARATION

BEGIN_QUERY_BUILDER_DECLARATION(CheckForInPrintBookings)
END_BUILDER_DECLARATION


BEGIN_QUERY_BUILDER_DECLARATION(FindBookingsInPrint)
END_BUILDER_DECLARATION

BEGIN_QUERY_BUILDER_DECLARATION(SelNewInsertedOrder)
END_BUILDER_DECLARATION


//BEGIN_QUERY_BUILDER_DECLARATION(SelNewInsertedBooking)
//END_BUILDER_DECLARATION


BEGIN_QUERY_BUILDER_DECLARATION(FindPrintLockByBranch)
END_BUILDER_DECLARATION

BEGIN_QUERY_BUILDER_DECLARATION(SelAllNewInsertedBookings)
END_BUILDER_DECLARATION

//----------------------------------------------------------------------------//
// UPDATES
//----------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////
//! class for updating acknowledgementstatus in narctransactioncatalog \n final class
////////////////////////////////////////////////////////////////////////////////
BEGIN_WRITE_BUILDER_DECLARATION(UpdAcknowledgementstatus)
END_BUILDER_DECLARATION

//BEGIN_WRITE_BUILDER_DECLARATION(UpdAckStatusFromInPrintToForPrint)
//END_BUILDER_DECLARATION

BEGIN_WRITE_BUILDER_DECLARATION(UpdAcknowledgementstatusOfOrder)
END_BUILDER_DECLARATION

BEGIN_WRITE_BUILDER_DECLARATION(UpdAcknowledgementstatusOfOrderMark)
END_BUILDER_DECLARATION

BEGIN_WRITE_BUILDER_DECLARATION( UpdContributionVoucherNo  )
END_BUILDER_DECLARATION


//reset queries
BEGIN_WRITE_BUILDER_DECLARATION( SelectNotPrintedBookingsIntoTemp  )
END_BUILDER_DECLARATION
BEGIN_WRITE_BUILDER_DECLARATION( UpdateNotPrintedBookingsUsingTemp  )
END_BUILDER_DECLARATION
BEGIN_WRITE_BUILDER_DECLARATION( DropNotPrintedBookingsTempTbl  )
END_BUILDER_DECLARATION
}

#endif //GUARD_CONTRIBUTIONVOUCHERACC_H