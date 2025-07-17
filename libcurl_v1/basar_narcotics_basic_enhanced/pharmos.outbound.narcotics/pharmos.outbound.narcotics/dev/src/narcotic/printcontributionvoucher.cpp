//--------------------------------------------------------------------------------------------//
// includes
//--------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "contributionvoucherprintdata.h"
#include "narcoticsappl.h"
#include "loggerpool.h"
#include "printcontributionvoucher.h"

#include "customermodule.h"
#include "suppliermodule.h"
#include "branchmodule.h"
#include "contributionvouchermodule.h"
#include "searchmodule.h"
#include "customerandsuppliersearcher.h"

#include "missingdependencyinjectionexception.h"
#include "missingdataforprintingexception.h"

//todo: externalize similiar methods to class printbase
//--------------------------------------------------------------------------------------------//
// namespace
//--------------------------------------------------------------------------------------------//
namespace infrastructure
{
	//-------------------------------------------------------------------
    PrintContributionVoucher::PrintContributionVoucher( basar::Int16 branchNo, basar::db::aspect::ConnectionRef conRef ) 
		: m_BranchNo( branchNo ), m_ConnectionRef( conRef ), m_Initialized( false )
    {
    }

	//-------------------------------------------------------------------
    PrintContributionVoucher::~PrintContributionVoucher()
    {
        shutdown();
    }

	//-------------------------------------------------------------------
    void PrintContributionVoucher::injectBranchDM( boost::shared_ptr<domMod::BranchDM> sp_BranchDM )
    {
        m_BranchDM = sp_BranchDM;
    }
	
	//-------------------------------------------------------------------
    void PrintContributionVoucher::injectContributionVoucherDM( boost::shared_ptr<domMod::ContributionVoucherDM> sp_ContributionDM )
    {
        m_ContributionVoucherDM = sp_ContributionDM;
    }
	
	//-------------------------------------------------------------------
    void PrintContributionVoucher::injectSearchDM( boost::shared_ptr<domMod::SearchDM> sp_SearchDM )
    {
        m_SearchDM = sp_SearchDM;
    }
	
	//-------------------------------------------------------------------
    void PrintContributionVoucher::injectCustomerDM( boost::shared_ptr<domMod::CustomerDM> sp_CustomerDM )
    {
        m_CustomerDM = sp_CustomerDM;
    }
	
	//-------------------------------------------------------------------
	void PrintContributionVoucher::injectSupplierDM( boost::shared_ptr<domMod::SupplierDM> sp_SupplierDM )
    {
        m_SupplierDM = sp_SupplierDM;
    }

	//-------------------------------------------------------------------
    void PrintContributionVoucher::shutdown()
    {
        if (m_Initialized)
        {
            m_ContributionVoucherDM->shutdown();
            m_BranchDM->shutdown();
            m_SearchDM->shutdown();
            m_CustomerDM->shutdown();
			m_SupplierDM->shutdown();

            m_Initialized = false;
        }
    }

	//-------------------------------------------------------------------
    void PrintContributionVoucher::init()
    {
        if (!m_Initialized)
        {
            checkInjections();

            m_ContributionVoucherDM->init( m_BranchNo, m_ConnectionRef );
            m_BranchDM->init( m_BranchNo, m_ConnectionRef );
            m_SearchDM->init( m_ConnectionRef );
            m_CustomerDM->init( m_BranchNo, m_ConnectionRef );
			m_SupplierDM->init( m_BranchNo, m_ConnectionRef );

            m_Initialized = true;
        }
    }

	//-------------------------------------------------------------------
	void PrintContributionVoucher::checkInjections()
	{
            if( 0 == m_ContributionVoucherDM.get() )
            {
                throw MissingDependencyInjectionException( basar::ExceptInfo("PrintContributionVoucher::checkInjections", "ContributionVoucherDM missing! Inject ContributionVoucherDM first!", __FILE__, __LINE__) );
            }
            if( 0 == m_BranchDM.get() )
            {
                throw MissingDependencyInjectionException( basar::ExceptInfo("PrintContributionVoucher::checkInjections", "BranchDM missing! Inject BranchDM first!", __FILE__, __LINE__) );
            }
            if( 0 == m_CustomerDM.get() )
            {
                throw MissingDependencyInjectionException( basar::ExceptInfo("PrintContributionVoucher::checkInjections", "CustomerDM missing! Inject CustomerDM first!", __FILE__, __LINE__) );
            }
			if( 0 == m_SupplierDM.get() )
			{
				throw MissingDependencyInjectionException( basar::ExceptInfo("PrintContributionVoucher::checkInjections", "SupplierDM missing! Inject SupplierDM first!", __FILE__, __LINE__) );;
			}
            if( 0 == m_SearchDM.get() )
            {
                throw MissingDependencyInjectionException( basar::ExceptInfo("PrintContributionVoucher::checkInjections", "SearchDM missing! Inject SearchDM first!", __FILE__, __LINE__) );
            }
	}

	//-------------------------------------------------------------------
    narcotics::ContributionVoucherPrintData PrintContributionVoucher::createContributionVoucherDataProvider( basar::db::aspect::AccessorPropertyTable_YIterator noacknowledgementIt )
    {
		noacknowledgementIt.setInt16( constants::BRANCHNO, m_BranchNo );

		CustomerAndSupplierSearcher customerAndSupplierSearcher;
		customerAndSupplierSearcher.setContributionVoucherData( noacknowledgementIt, m_ContributionVoucherDM, m_BranchDM, m_SearchDM, m_SupplierDM, m_CustomerDM );

		return createContributionVoucherDataProvider(	customerAndSupplierSearcher.getArticleData(), 
														customerAndSupplierSearcher.getYitBranch(), 
														customerAndSupplierSearcher.getYitCustomerOrSupplier(), 
														customerAndSupplierSearcher.getYitSignature(), 
														0, 
														customerAndSupplierSearcher.getRows() );
    }

	//-------------------------------------------------------------------
    narcotics::ContributionVoucherPrintData PrintContributionVoucher::createContributionVoucherDataProvider( basar::db::aspect::AccessorPropertyTableRef articleData, 
												basar::db::aspect::AccessorPropertyTable_YIterator yitBranch, 
												basar::db::aspect::AccessorPropertyTable_YIterator yitCustomer, 
												basar::db::aspect::AccessorPropertyTable_YIterator yitSignature, 
												basar::Int32 beginOrder, 
												basar::Int32 endOrder )
    {
        narcotics::ContributionVoucherPrintData pdfAbgabe ( articleData, yitBranch, yitCustomer, yitSignature, beginOrder, endOrder, m_BranchNo );
        
        return pdfAbgabe;
    }

} //namespace infrastructure