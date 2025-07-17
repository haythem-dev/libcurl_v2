//----------------------------------------------------------------------------
/*! \file
 *  \brief  string builder declaration for bookingcatalog-accessor
 *  \author Roland Kiefert
 *  \date   17.02.2006
 */
//----------------------------------------------------------------------------

#ifndef GUARD_ACC_TRANSFER_H
#define GUARD_ACC_TRANSFER_H

////////////////////////////////////////////////////////////////////////////////
//! namespace for bookingcatalog-accessor
namespace acc_transfer
{

//----------------------------------------------------------------------------//
// SELECTS
//----------------------------------------------------------------------------//
BEGIN_QUERY_BUILDER_DECLARATION( SelTransfer )
END_BUILDER_DECLARATION

BEGIN_QUERY_BUILDER_DECLARATION( SelBranchesToTransfer )
END_BUILDER_DECLARATION

BEGIN_WRITE_BUILDER_DECLARATION( DelTransfer )
END_BUILDER_DECLARATION


} //namespace acc_bookingcatalog

#endif	//GUARD_ACC_TRANSFER_H