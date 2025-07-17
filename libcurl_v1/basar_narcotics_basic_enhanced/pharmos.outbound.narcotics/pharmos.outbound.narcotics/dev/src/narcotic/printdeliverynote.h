//--------------------------------------------------------------------------------------------//
/*! \file	printdeliverynote.h
 *  \brief  Serviceclass for printing delivery notes
 *  \author Marco Köppendörfer
 *  \date   07.09.2012
 */
//--------------------------------------------------------------------------------------------//

#ifndef NARCOTICS_INFRASTRUCTURE_SERVICES_PRINTDELIVERYNOTE_H
#define NARCOTICS_INFRASTRUCTURE_SERVICES_PRINTDELIVERYNOTE_H

//--------------------------------------------------------------------------------------------//
// includes
//--------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "iprintdeliverynote.h"
#include "customermodule.h"
#include "suppliermodule.h"
#include "branchmodule.h"
#include "contributionvouchermodule.h"
#include "searchmodule.h"

//--------------------------------------------------------------------------------------------//
// namespace
//--------------------------------------------------------------------------------------------//
namespace infrastructure
{
	//-------------------------------------------------------------------
    class PrintDeliveryNote : public IPrintDeliveryNote
    {
        public:
            PrintDeliveryNote( basar::Int16 branchNo, basar::db::aspect::ConnectionRef conRef );
            ~PrintDeliveryNote();
            //injects
            void injectBranchDM( boost::shared_ptr<domMod::BranchDM> sp_BranchDM );
            void injectContributionVoucherDM( boost::shared_ptr<domMod::ContributionVoucherDM> sp_ContributionDM );
            void injectSearchDM( boost::shared_ptr<domMod::SearchDM> sp_SearchDM );
            void injectCustomerDM( boost::shared_ptr<domMod::CustomerDM> sp_CustomerDM );
			void injectSupplierDM( boost::shared_ptr<domMod::SupplierDM> sp_SupplierDM );
            
            void init(); //throws missing injection exception

            //serivce methods
            void reprintDeliveryNote( basar::db::aspect::AccessorPropertyTable_YIterator noacknowledgementIt );

            void shutdown();

        private:

	        PrintDeliveryNote	( const PrintDeliveryNote& ); // forbidden
	        PrintDeliveryNote& operator=			( const PrintDeliveryNote& ); // forbidden
            
            void checkInjections();

            void reprintDeliveryNote(	basar::db::aspect::AccessorPropertyTableRef articleData, 
											basar::db::aspect::AccessorPropertyTable_YIterator yitBranch, 
											basar::db::aspect::AccessorPropertyTable_YIterator yitCustomer, 
											basar::db::aspect::AccessorPropertyTable_YIterator yitLink, 
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

#endif //NARCOTICS_INFRASTRUCTURE_SERVICES_PRINTDELIVERYNOTE_H