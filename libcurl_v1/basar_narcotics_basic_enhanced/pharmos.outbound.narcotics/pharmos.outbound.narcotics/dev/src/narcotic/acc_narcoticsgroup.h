#ifndef GUARD_ACC_NARCOTICSGROUP_H
#define GUARD_ACC_NARCOTICSGROUP_H
//-------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  string builder declaration for narcoticsgroup-accessor
 *  \author Roland Kiefert
 *  \date   17.02.2006
 */
//-------------------------------------------------------------------------------------------------//


//-------------------------------------------------------------------------------------------------//
//! namespace for bookingcatalog-accessor
namespace acc_narcoticsgroup
{

//-------------------------------------------------------------------------------------------------//
//! class for selecting group from narcgroup and artikelzentral \n final class
BEGIN_QUERY_BUILDER_DECLARATION(SelGroup)
END_BUILDER_DECLARATION

//-------------------------------------------------------------------------------------------------//
//! class for selecting group name from narcgroup and artikelzentral \n final class
BEGIN_QUERY_BUILDER_DECLARATION(SelGroupByName)
END_BUILDER_DECLARATION

//-------------------------------------------------------------------------------------------------//
//! class for selecting a group by articleno and by branchno\n final class
BEGIN_QUERY_BUILDER_DECLARATION(SelStockLocationByArticleNo)
END_BUILDER_DECLARATION

//-------------------------------------------------------------------------------------------------//
//! class for selecting a group by articleno and by branchno\n final class
BEGIN_QUERY_BUILDER_DECLARATION(SelStockLocation2ByArticleNo)
END_BUILDER_DECLARATION

} //namespace acc_narcoticsgroup

#endif //GUARD_ACC_NARCOTICSGROUP_H