//--------------------------------------------------------------------------------------------//
/*! \file	printcontributionvoucher.h
 *  \brief  
 *  \author Marco Köppendörfer
 *  \date   10.09.2012
 */
//--------------------------------------------------------------------------------------------//

#ifndef INFRASTRUCTURE_PRINTCONTRIBUTIONVOUHCER_H
#define INFRASTRUCTURE_PRINTCONTRIBUTIONVOUHCER_H

//--------------------------------------------------------------------------------------------//
// includes
//--------------------------------------------------------------------------------------------//
#include "iprintcontributionvoucher.h"
#include "contributionvoucherprintdata.h"

//--------------------------------------------------------------------------------------------//
// namespace
//--------------------------------------------------------------------------------------------//
namespace infrastructure
{
    class PrintContributionVoucher : public IPrintContributionVoucher
    {
        public:
            PrintContributionVoucher( basar::Int16 branchNo, basar::db::aspect::ConnectionRef conRef );
            ~PrintContributionVoucher();

            //serivce methods
            narcotics::ContributionVoucherPrintData createContributionVoucherDataProvider( basar::db::aspect::AccessorPropertyTable_YIterator noacknowledgementIt );

            //injects
            void injectBranchDM( boost::shared_ptr<domMod::BranchDM> sp_BranchDM );
            void injectContributionVoucherDM( boost::shared_ptr<domMod::ContributionVoucherDM> sp_ContributionDM );
            void injectSearchDM( boost::shared_ptr<domMod::SearchDM> sp_SearchDM );
            void injectCustomerDM( boost::shared_ptr<domMod::CustomerDM> sp_CustomerDM );
			void injectSupplierDM( boost::shared_ptr<domMod::SupplierDM> sp_SupplierDM );
            
            void init(); //throws missing injection exception

            void shutdown();

            private:

	            PrintContributionVoucher	( const PrintContributionVoucher& ); // forbidden
	            PrintContributionVoucher&	operator=					( const PrintContributionVoucher& ); // forbidden

                                
                void checkInjections();

                narcotics::ContributionVoucherPrintData createContributionVoucherDataProvider(	basar::db::aspect::AccessorPropertyTableRef articleData, 
												basar::db::aspect::AccessorPropertyTable_YIterator yitBranch, 
												basar::db::aspect::AccessorPropertyTable_YIterator yitCustomer, 
												basar::db::aspect::AccessorPropertyTable_YIterator yitSignature, 
												basar::Int32 beginOrder, 
												basar::Int32 endOrder	);

				basar::Int16                            m_BranchNo;
                basar::db::aspect::ConnectionRef        m_ConnectionRef;
                bool                                    m_Initialized;
                //todo: typedef classes
                boost::shared_ptr<domMod::BranchDM>					m_BranchDM;				//!< domain module class for branch
                boost::shared_ptr<domMod::ContributionVoucherDM>	m_ContributionVoucherDM;//!< domain module class for contribution vouchers
	            boost::shared_ptr<domMod::SearchDM>					m_SearchDM;				//!< domain module class for searching
	            boost::shared_ptr<domMod::CustomerDM>				m_CustomerDM;
				boost::shared_ptr<domMod::SupplierDM>				m_SupplierDM;
    };

} //namespace infrastructure

#endif //INFRASTRUCTURE_PRINTCONTRIBUTIONVOUHCER_H