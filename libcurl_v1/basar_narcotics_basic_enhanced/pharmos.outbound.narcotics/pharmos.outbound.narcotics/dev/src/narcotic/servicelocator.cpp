//--------------------------------------------------------------------------------------------------//
// include section
//--------------------------------------------------------------------------------------------------//
#include "commonheader.h"

#include "servicelocator.h"

#include "iprintdeliverynote.h"
#include "printdeliverynote.h"

#include "iprintcontributionvoucher.h"
#include "printcontributionvoucher.h"

#include "iftpservice.h"

#include "missingdependencyinjectionexception.h"
    
//--------------------------------------------------------------------------------------------------//
// namespace section
//--------------------------------------------------------------------------------------------------//
namespace infrastructure
{
	//---------------------------------------------------------------------------
    ServiceLocator::ServiceLocator( basar::Int16 branchNo, basar::db::aspect::ConnectionRef conRef ) 
		: m_BranchNo(branchNo), m_ConnectionRef(conRef)
    {
    }

	//---------------------------------------------------------------------------
    ServiceLocator::~ServiceLocator()
	{
	}
 
	//---------------------------------------------------------------------------   
    void ServiceLocator::injectFtpService( IFtpServicePtr ftpSerivce )
    {
        m_IFtpServicePtr = ftpSerivce;
    }

	//---------------------------------------------------------------------------
    void ServiceLocator::injectBranchDM( boost::shared_ptr<domMod::BranchDM> sp_BranchDM )
    {
        m_BranchDM = sp_BranchDM;
    }
	
	//---------------------------------------------------------------------------
    void ServiceLocator::injectContributionVoucherDM( boost::shared_ptr<domMod::ContributionVoucherDM> sp_ContributionDM )
    {
        m_ContributionVoucherDM = sp_ContributionDM;
    }
	
	//---------------------------------------------------------------------------
    void ServiceLocator::injectSearchDM( boost::shared_ptr<domMod::SearchDM> sp_SearchDM )
    {
        m_SearchDM = sp_SearchDM;
    }
	
	//---------------------------------------------------------------------------
    void ServiceLocator::injectCustomerDM( boost::shared_ptr<domMod::CustomerDM> sp_CustomerDM )
    {
        m_CustomerDM = sp_CustomerDM;
    }

	//---------------------------------------------------------------------------
	void ServiceLocator::injectSupplierDM( boost::shared_ptr<domMod::SupplierDM> sp_SupplierDM )
    {
        m_SupplierDM = sp_SupplierDM;
    }
  
	//---------------------------------------------------------------------------  
    void ServiceLocator::checkInjections()
    {
        if (0 == m_ContributionVoucherDM.get())
        {
            throw MissingDependencyInjectionException( basar::ExceptInfo("PrintDeliveryNote::checkInjections", "ContributionVoucherDM missing! Inject ContributionVoucherDM first!", __FILE__, __LINE__) );;
        }
        if (0 == m_BranchDM.get())
        {
            throw MissingDependencyInjectionException( basar::ExceptInfo("PrintDeliveryNote::checkInjections", "BranchDM missing! Inject BranchDM first!", __FILE__, __LINE__) );;
        }
        if (0 == m_CustomerDM.get())
        {
            throw MissingDependencyInjectionException( basar::ExceptInfo("PrintDeliveryNote::checkInjections", "CustomerDM missing! Inject CustomerDM first!", __FILE__, __LINE__) );;
        }
		if (0 == m_SupplierDM.get())
        {
            throw MissingDependencyInjectionException( basar::ExceptInfo("PrintDeliveryNote::checkInjections", "SupplierDM missing! Inject SupplierDM first!", __FILE__, __LINE__) );;
        }
        if (0 == m_SearchDM.get())
        {
            throw MissingDependencyInjectionException( basar::ExceptInfo("PrintDeliveryNote::checkInjections", "SearchDM missing! Inject SearchDM first!", __FILE__, __LINE__) );;
        }
        if (0 == m_IFtpServicePtr.get())
        {
            throw MissingDependencyInjectionException( basar::ExceptInfo("PrintDeliveryNote::checkInjections", "IFtpServicePtr missing! Inject IFtpServicePtr first!", __FILE__, __LINE__) );;
        }
    }

	//---------------------------------------------------------------------------
    void ServiceLocator::shutdown()
    {
        if (0 != m_IPrintDeliveryNotePtr.get()) 
        {
            m_IPrintDeliveryNotePtr->shutdown();
        }
        if (0 != m_IPrintContributionVoucherPtr.get())
        {
            m_IPrintContributionVoucherPtr->shutdown();
        }
        if (0 != m_IFtpServicePtr.get())
        {
            m_IFtpServicePtr->shutdown();
        }
    }

	//---------------------------------------------------------------------------
    IFtpServicePtr ServiceLocator::getFtpService()
    {
        checkInjections();

        return m_IFtpServicePtr;
    }

	//---------------------------------------------------------------------------
    IPrintContributionVoucherPtr ServiceLocator::getContributionVoucherCreator()
    {
        if (m_IPrintContributionVoucherPtr.get() == 0)
        {
            checkInjections();

            boost::shared_ptr<PrintContributionVoucher> pcv = boost::shared_ptr<PrintContributionVoucher>(new PrintContributionVoucher(m_BranchNo, m_ConnectionRef));
            pcv->injectBranchDM(m_BranchDM);
            pcv->injectContributionVoucherDM(m_ContributionVoucherDM);
            pcv->injectSearchDM(m_SearchDM);
            pcv->injectCustomerDM(m_CustomerDM);
			pcv->injectSupplierDM(m_SupplierDM);

            pcv->init(); //throws

            m_IPrintContributionVoucherPtr = IPrintContributionVoucherPtr(pcv);

        }

        return m_IPrintContributionVoucherPtr;
    }

	//---------------------------------------------------------------------------
    IPrintDeliveryNotePtr ServiceLocator::getPrintService()
    {
        if (m_IPrintDeliveryNotePtr.get() == 0)
        {
            checkInjections();

            boost::shared_ptr<PrintDeliveryNote> pdn =  boost::shared_ptr<PrintDeliveryNote>(new PrintDeliveryNote(m_BranchNo, m_ConnectionRef));

            pdn->injectBranchDM(m_BranchDM);
            pdn->injectContributionVoucherDM(m_ContributionVoucherDM);
            pdn->injectSearchDM(m_SearchDM);
            pdn->injectCustomerDM(m_CustomerDM);
			pdn->injectSupplierDM(m_SupplierDM);

            pdn->init(); //throws

            m_IPrintDeliveryNotePtr = IPrintDeliveryNotePtr(pdn);
        }
        
        return m_IPrintDeliveryNotePtr;
    }

} //namespace infrastructure