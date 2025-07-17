#ifndef GUARD_NARCOTICS_CONTRIBUTIONVOUCHERSCAN_H
#define GUARD_NARCOTICS_CONTRIBUTIONVOUCHERSCAN_H

//-------------------------------------------------------------------------------------------------//
//includes
//-------------------------------------------------------------------------------------------------//
#include "usecasecontroller.h"
#include "contributionvoucherscanvc.h"
#include "contributionvouchermodule.h"


namespace narcotics
{
namespace useCase
{
    class ContributionVoucherScan : public UseCaseController
    {
        public:
            ContributionVoucherScan( basar::Int16 branchNo );

            ~ContributionVoucherScan();

            basar::appl::EventReturnType run();
            
            void registerEventHandlers( const bool forSave );

            void injectContributionVoucherDM( boost::shared_ptr<domMod::ContributionVoucherDM> contributionVoucherDM );

			void injectParentWindow			( QWidget * mw );

            /*! \brief retransmit existing order
		    \n no-throw */
	        SYSTEM_EVENT_HANDLER_DECLARATION( MarkScannedContributionVoucherAsReceived );
        
        private:

            /*! \brief checks if all nessecary injections took place. \n  throws missinginjectionexception */
            void checkInjections();

            basar::Int16 m_BranchNo;

            boost::shared_ptr<domMod::ContributionVoucherDM>	m_ContributionVoucherDM;

            //member VCs
            basar::gui::tie::ViewConnPtr<viewConn::ContributionVoucherScanVC> m_ContributionVoucherScanVC;
    };

} //namespace useCase
} //namespace narcotics

#endif //GUARD_NARCOTICS_CONTRIBUTIONVOUCHERSCAN_H