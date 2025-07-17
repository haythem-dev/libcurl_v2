//------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  string builder declaration for narcoticssupplier-accessor
 *  \author Roland Kiefert
 *  \date   17.02.2006
 */
//------------------------------------------------------------------------------------------------//

#ifndef GUARD_ACC_NARCOTICSSUPPLIER_H
#define GUARD_ACC_NARCOTICSSUPPLIER_H

//------------------------------------------------------------------------------------------------//
//! namespace for narcoticssupplier-accessor
//------------------------------------------------------------------------------------------------//
namespace acc_narcoticssupplier
{

//------------------------------------------------------------------------------------------------//
//! class for selecting user id from narctransactcatalog \n final class
//------------------------------------------------------------------------------------------------//
BEGIN_QUERY_BUILDER_DECLARATION(SelSupplier)
END_BUILDER_DECLARATION

//------------------------------------------------------------------------------------------------//
//! class for selecting user id from narctransactcatalog \n final class
//------------------------------------------------------------------------------------------------//
BEGIN_QUERY_BUILDER_DECLARATION(SelPhoenixSupplier)
END_BUILDER_DECLARATION

//------------------------------------------------------------------------------------------------//
//! class for selecting the attribute 'besla_name' of the table 'pusupplier' \n final class
//------------------------------------------------------------------------------------------------//
BEGIN_QUERY_BUILDER_DECLARATION( SelSupplierByBgaNo )
END_BUILDER_DECLARATION

//------------------------------------------------------------------------------------------------//
//! class for selecting the attribute 'bga_nr' of the table 'pusupplier' \n final class
//------------------------------------------------------------------------------------------------//
BEGIN_QUERY_BUILDER_DECLARATION( SelSupplierByName )
END_BUILDER_DECLARATION
} // acc_narcoticssupplier

#endif //GUARD_ACC_NARCOTICSSUPPLIER_H