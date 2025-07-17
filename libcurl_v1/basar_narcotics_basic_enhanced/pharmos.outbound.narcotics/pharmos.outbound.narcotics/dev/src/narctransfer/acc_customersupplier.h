//----------------------------------------------------------------------------
/*! \file
 *  \brief  string builder declaration for customer-accessor
 *  \author Roland Kiefert
 *  \date   03.04.2006
 */
//----------------------------------------------------------------------------

#ifndef GUARD_ACC_CUSTOMERSUPPLIER_H
#define GUARD_ACC_CUSTOMERSUPPLIER_H

//----------------------------------------------------------------------------
//! namespace for bookingcatalog-accessor
namespace acc_customersupplier
{

//---------------------------------------------------------------------------
//! class for selecting article data from customer \n final class
BEGIN_QUERY_BUILDER_DECLARATION(SelCustomer)
END_BUILDER_DECLARATION

//! class for selecting article data from supplier \n final class
BEGIN_QUERY_BUILDER_DECLARATION(SelSupplier)
END_BUILDER_DECLARATION

} //namespace acc_customersupplier

#endif	//GUARD_ACC_CUSTOMERSUPPLIER_H