//--------------------------------------------------------------------------------------------//
/*! \file	iprintcontributionvoucher.h
 *  \brief  base class for printdeliverynote
 *  \author Marco Köppendörfer
 *  \date   10.09.2012
 */
//--------------------------------------------------------------------------------------------//

#ifndef INFRASTRUCTURE_IPRINTCONTRIBUTIONVOUHCER_H
#define INFRASTRUCTURE_IPRINTCONTRIBUTIONVOUHCER_H

//--------------------------------------------------------------------------------------------//
// includes
//--------------------------------------------------------------------------------------------//
#include "iservice.h"

//forward
//--------------------------------------------------------------------------------------------//
namespace narcotics
{
    class ContributionVoucherPrintData;
} //namespace narcotics

//--------------------------------------------------------------------------------------------//
namespace infrastructure
{
    class IPrintContributionVoucher : public IService
    {
        public:
            virtual ~IPrintContributionVoucher(){};
            virtual narcotics::ContributionVoucherPrintData createContributionVoucherDataProvider( basar::db::aspect::AccessorPropertyTable_YIterator noacknowledgementIt ) = 0;
    };

}//namespace infrastructure

#endif //INFRASTRUCTURE_IPRINTCONTRIBUTIONVOUHCER_H