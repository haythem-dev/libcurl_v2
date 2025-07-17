//----------------------------------------------------------------------------
/*! \file
 *  \brief  string builder declaration for open order accessor
 *  \author Roland Kiefert
 *  \date   13.04.2006
 */
//----------------------------------------------------------------------------

#ifndef GUARD_ACC_OPENORDER_H
#define GUARD_ACC_OPENORDER_H

//----------------------------------------------------------------------------
//! namespace for bookingcatalog-accessor
namespace acc_openorder
{

//---------------------------------------------------------------------------
//! class for selecting open orders by pattern from narcopenorders \n final class
BEGIN_QUERY_BUILDER_DECLARATION(SelOpenOrdersByPattern)
END_BUILDER_DECLARATION

//---------------------------------------------------------------------------
//! class for delete a openorder from narcopenorders \n final class
BEGIN_WRITE_BUILDER_DECLARATION(DelOpenOrder)
END_BUILDER_DECLARATION

//----------------------------------------------------------------------------
} // acc_openorder

#endif	//GUARD_ACC_OPENORDER_H