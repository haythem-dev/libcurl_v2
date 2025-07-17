//--------------------------------------------------------------------------------------------//
// includes
//--------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "printdeliverynote.h"
#include "narcoticsappl.h"//??
#include "loggerpool.h"
#include "receiptnoticeprintdata.h"

#include "customermodule.h"
#include "branchmodule.h"
#include "contributionvouchermodule.h"
#include "searchmodule.h"
#include "customerandsuppliersearcher.h"

#include "missingdependencyinjectionexception.h"
#include "missingdataforprintingexception.h"

//--------------------------------------------------------------------------------------------//
// namespace
//--------------------------------------------------------------------------------------------//
namespace infrastructure
{
	//-------------------------------------------------------------------
    PrintDeliveryNote::PrintDeliveryNote(basar::Int16 branchNo, basar::db::aspect::ConnectionRef conRef) : m_BranchNo(branchNo), m_ConnectionRef(conRef), m_Initialized(false)
    {
    }

	//-------------------------------------------------------------------
    PrintDeliveryNote::~PrintDeliveryNote()
    {
        shutdown();
    }

	//-------------------------------------------------------------------
    void PrintDeliveryNote::injectBranchDM(boost::shared_ptr<domMod::BranchDM> sp_BranchDM)
    {
        m_BranchDM = sp_BranchDM;
    }

	//-------------------------------------------------------------------
    void PrintDeliveryNote::injectContributionVoucherDM(boost::shared_ptr<domMod::ContributionVoucherDM> sp_ContributionDM)
    {
        m_ContributionVoucherDM = sp_ContributionDM;
    }

	//-------------------------------------------------------------------
    void PrintDeliveryNote::injectSearchDM(boost::shared_ptr<domMod::SearchDM>	sp_SearchDM)
    {
        m_SearchDM = sp_SearchDM;
    }

	//-------------------------------------------------------------------
    void PrintDeliveryNote::injectCustomerDM(boost::shared_ptr<domMod::CustomerDM> sp_CustomerDM)
    {
        m_CustomerDM = sp_CustomerDM;
    }

	//-------------------------------------------------------------------
	void PrintDeliveryNote::injectSupplierDM(boost::shared_ptr<domMod::SupplierDM> sp_SupplierDM)
    {
        m_SupplierDM = sp_SupplierDM;
    }

	//-------------------------------------------------------------------
    void PrintDeliveryNote::shutdown()
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
    void PrintDeliveryNote::init()
    {
        if (!m_Initialized)
        {
            checkInjections();

            m_ContributionVoucherDM->init(m_BranchNo, m_ConnectionRef);
            m_BranchDM->init(m_BranchNo, m_ConnectionRef);
            m_SearchDM->init(m_ConnectionRef);
            m_CustomerDM->init(m_BranchNo, m_ConnectionRef);
			m_SupplierDM->init(m_BranchNo, m_ConnectionRef);

            m_Initialized = true;
        }
    }

	//-------------------------------------------------------------------
    void PrintDeliveryNote::checkInjections()
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
    }

 	//-------------------------------------------------------------------
   void PrintDeliveryNote::reprintDeliveryNote(basar::db::aspect::AccessorPropertyTable_YIterator noacknowledgementIt)
    {      
		noacknowledgementIt.setInt16(constants::BRANCHNO, m_BranchNo);

		CustomerAndSupplierSearcher customerAndSupplierSearcher;
		customerAndSupplierSearcher.setContributionVoucherData( noacknowledgementIt, m_ContributionVoucherDM, m_BranchDM, m_SearchDM, m_SupplierDM, m_CustomerDM );

        basar::db::aspect::AccessorPropertyTableRef			articleData				= customerAndSupplierSearcher.getArticleData();
		basar::db::aspect::AccessorPropertyTable_YIterator	yitBranch				= customerAndSupplierSearcher.getYitBranch();
		basar::db::aspect::AccessorPropertyTable_YIterator	yitCustomerOrSupplier	= customerAndSupplierSearcher.getYitCustomerOrSupplier();
		basar::db::aspect::AccessorPropertyTable_YIterator	yitSignature			= customerAndSupplierSearcher.getYitSignature();
		basar::UInt32										rows					= customerAndSupplierSearcher.getRows();

		basar::UInt32 printedBookings = 0;

		if( rows<=constants::BOOKINGS_PER_FORM && rows != 0 )
		{
			reprintDeliveryNote( articleData,yitBranch,yitCustomerOrSupplier,yitSignature,0,rows );
			printedBookings = rows;
		}
		else
		{
			while( printedBookings < rows )
			{		
				reprintDeliveryNote( articleData,yitBranch,yitCustomerOrSupplier,yitSignature,printedBookings,printedBookings + constants::BOOKINGS_PER_FORM );
				printedBookings+=constants::BOOKINGS_PER_FORM;

				if( rows-printedBookings <= constants::BOOKINGS_PER_FORM && rows-printedBookings != 0 )
				{
					reprintDeliveryNote( articleData,yitBranch,yitCustomerOrSupplier,yitSignature,printedBookings,rows );
					printedBookings=rows;
				}
			}
		}
    }
    
	//-------------------------------------------------------------------
    void PrintDeliveryNote::reprintDeliveryNote(	basar::db::aspect::AccessorPropertyTableRef articleData, 
											basar::db::aspect::AccessorPropertyTable_YIterator yitBranch, 
											basar::db::aspect::AccessorPropertyTable_YIterator yitCustomer, 
											basar::db::aspect::AccessorPropertyTable_YIterator yitLink, 
											basar::Int32 beginOrder, 
											basar::Int32 endOrder	)
    {
	    basar::I18nString printerName;

	    basar::I18nString branchnotxt;
	    branchnotxt.itos(basar::login::Manager::getInstance().getAreaID());

	    basar::login::Manager::getInstance().getParaValue(branchnotxt, UADM_NARCOTICSPRINTER, printerName);

        narcotics::ReceiptNoticePrintData printData(articleData,yitBranch,yitCustomer,yitLink,beginOrder,endOrder);
        
	    basar::cmnutil::RawPrint rPrint;
	    if(rPrint.print(printerName, printData)!= basar::cmnutil::PRINTER_SUCCESS)
	    {
		    if( narcotics::LoggerPool::loggerDomModules.isEnabledFor(log4cplus::ERROR_LOG_LEVEL) )
		    {
			    basar::VarString msg = "error while printing receiptnotice: " ;
			    msg.append(printData.getPostPrintingInformation());
			    narcotics::LoggerPool::loggerDomModules.forcedLog(log4cplus::ERROR_LOG_LEVEL, msg, __FILE__, __LINE__);
		    }
	    }
    }

} //namespace infrastructure