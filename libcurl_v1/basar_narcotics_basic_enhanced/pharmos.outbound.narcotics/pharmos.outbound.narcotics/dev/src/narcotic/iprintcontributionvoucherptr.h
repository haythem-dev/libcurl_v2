//--------------------------------------------------------------------------------------------//
/*! \file	iprintcontributionvoucher.h
 *  \brief  Definition of IPrintContributionVoucherPtr
 *  \author Marco K�ppend�rfer
 *  \date   08.10.2012
 */
//--------------------------------------------------------------------------------------------//

#ifndef INFRASTRUCTURE_IPRINTCONTRIBUTIONVOUHCERPTR_H
#define INFRASTRUCTURE_IPRINTCONTRIBUTIONVOUHCERPTR_H

//--------------------------------------------------------------------------------------------//
// includes
//--------------------------------------------------------------------------------------------//
#include <boost/shared_ptr.hpp>

//--------------------------------------------------------------------------------------------//
// namespace
//--------------------------------------------------------------------------------------------//
namespace infrastructure
{
    class  IPrintContributionVoucher;
    typedef boost::shared_ptr<IPrintContributionVoucher> IPrintContributionVoucherPtr;

} //namespace infrastructure

#endif //INFRASTRUCTURE_IPRINTCONTRIBUTIONVOUHCERPTR_H