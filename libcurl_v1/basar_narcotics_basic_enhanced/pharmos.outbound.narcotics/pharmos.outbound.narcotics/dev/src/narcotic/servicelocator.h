//--------------------------------------------------------------------------------------------//
/*! \file	servicelocator.h
 *  \brief  Definition of servicelocatorptr
 *  \author Marco Köppendörfer
 *  \date   10.09.2012
 */
//--------------------------------------------------------------------------------------------//
#ifndef INFRASTRUCTURE_SERVICELOCATOR_H
#define INFRASTRUCTURE_SERVICELOCATOR_H

//--------------------------------------------------------------------------------------------------//
// include section
//--------------------------------------------------------------------------------------------------//
#include "iprintdeliverynoteptr.h"
#include "iprintcontributionvoucherptr.h"
#include "iftpserviceptr.h"
#include "customermodule.h"
#include "suppliermodule.h"
#include "branchmodule.h"
#include "contributionvouchermodule.h"
#include "searchmodule.h"

//--------------------------------------------------------------------------------------------------//
// namespace section
//--------------------------------------------------------------------------------------------------//
namespace infrastructure
{
	//---------------------------------------------------------------------------
    class ServiceLocator
    {
        public:
            ServiceLocator( basar::Int16 branchNo, basar::db::aspect::ConnectionRef conRef );
            ~ServiceLocator();

            IPrintDeliveryNotePtr getPrintService();
            
            IPrintContributionVoucherPtr getContributionVoucherCreator();

            IFtpServicePtr getFtpService();

            void injectFtpService			( IFtpServicePtr ftpSerivce												);
            void injectBranchDM				( boost::shared_ptr<domMod::BranchDM> sp_BranchDM						);
            void injectContributionVoucherDM( boost::shared_ptr<domMod::ContributionVoucherDM> sp_ContributionDM	);
            void injectSearchDM				( boost::shared_ptr<domMod::SearchDM>	sp_SearchDM						);
            void injectCustomerDM			( boost::shared_ptr<domMod::CustomerDM> sp_CustomerDM					);
			void injectSupplierDM			( boost::shared_ptr<domMod::SupplierDM> sp_SupplierDM					);

            void shutdown();
        private:
            ServiceLocator	( const ServiceLocator& ); // forbidden
	        ServiceLocator& operator=( const ServiceLocator& ); // forbidden

            void checkInjections();

            IPrintDeliveryNotePtr m_IPrintDeliveryNotePtr;
            IPrintContributionVoucherPtr m_IPrintContributionVoucherPtr;

            IFtpServicePtr  m_IFtpServicePtr;
            
            basar::Int16 m_BranchNo;
            basar::db::aspect::ConnectionRef m_ConnectionRef;

            boost::shared_ptr<domMod::BranchDM>					m_BranchDM;				//!< domain module class for branch
            boost::shared_ptr<domMod::ContributionVoucherDM>	m_ContributionVoucherDM;//!< domain module class for contribution vouchers
	        boost::shared_ptr<domMod::SearchDM>					m_SearchDM;				//!< domain module class for searching
	        boost::shared_ptr<domMod::CustomerDM>				m_CustomerDM;
			boost::shared_ptr<domMod::SupplierDM>				m_SupplierDM;
    };

} //namespace infrastructure

#endif //INFRASTRUCTURE_SERVICELOCATOR_H