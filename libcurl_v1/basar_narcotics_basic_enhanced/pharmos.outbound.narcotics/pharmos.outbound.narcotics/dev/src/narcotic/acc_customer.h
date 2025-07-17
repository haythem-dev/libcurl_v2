//----------------------------------------------------------------------------
/*! \file
 *  \brief  string builder declaration for customer-accessor
 *  \author Roland Kiefert
 *  \date   03.04.2006
 */
//----------------------------------------------------------------------------
#ifndef GUARD_ACC_CUSTOMER_H
#define GUARD_ACC_CUSTOMER_H

//----------------------------------------------------------------------------
//! namespace for bookingcatalog-accessor
namespace acc_customer
{

//---------------------------------------------------------------------------
//! class for selecting article data from customer \n final class
BEGIN_QUERY_BUILDER_DECLARATION(SelCustomer)
END_BUILDER_DECLARATION

BEGIN_QUERY_BUILDER_DECLARATION(SelCustomerForReprint)
END_BUILDER_DECLARATION

//! class for selecting specific customer data for a given name from customer \n final class
BEGIN_QUERY_BUILDER_DECLARATION(SelCustomerByName)
END_BUILDER_DECLARATION

//! class for selecting a customer address for a given customerno \n final class
BEGIN_QUERY_BUILDER_DECLARATION(SelCustomerMapping)
END_BUILDER_DECLARATION

BEGIN_WRITE_BUILDER_DECLARATION(DelCustomerMapping)
END_BUILDER_DECLARATION

BEGIN_WRITE_BUILDER_DECLARATION(InsCustomerMapping)
END_BUILDER_DECLARATION

BEGIN_WRITE_BUILDER_DECLARATION(UpdCustomerMapping)
END_BUILDER_DECLARATION

//----------------------------------------------------------------------------
} //namespace acc_customer

//----------------------------------------------------------------------------
#endif //GUARD_ACC_CUSTOMER_H