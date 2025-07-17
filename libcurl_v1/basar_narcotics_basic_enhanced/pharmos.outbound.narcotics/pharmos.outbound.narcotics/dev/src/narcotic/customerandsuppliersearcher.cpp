//--------------------------------------------------------------------------------------------//
/*! \file	customerandsuppliersearcher.cpp
 *  \brief  Help class for printcontributionvoucher and printdeliverynote
 *  \author Frank Attmannspacher
 *  \date   16.05.2013
 */
//--------------------------------------------------------------------------------------------//
// includes
//--------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "narcoticsappl.h"
#include "loggerpool.h"

#include "customermodule.h"
#include "suppliermodule.h"
#include "branchmodule.h"

#include "customerandsuppliersearcher.h"

#include "missingdependencyinjectionexception.h"
#include "missingdataforprintingexception.h"

//--------------------------------------------------------------------------------------------//
// namespaces
//--------------------------------------------------------------------------------------------//
namespace infrastructure
{
	//-------------------------------------------------------------------
	CustomerAndSupplierSearcher::CustomerAndSupplierSearcher()
	{
	}

	//-------------------------------------------------------------------
	CustomerAndSupplierSearcher::~CustomerAndSupplierSearcher()
	{
	}

	//-------------------------------------------------------------------
	void CustomerAndSupplierSearcher::setContributionVoucherData( const AccessorPropertyTable_YIterator noAcknowledgementIt,
																  const boost::shared_ptr<domMod::ContributionVoucherDM> contributionVoucherDM,
																  const boost::shared_ptr<domMod::BranchDM> branchDM,
																  const boost::shared_ptr<domMod::SearchDM> searchDM,
																  const boost::shared_ptr<domMod::SupplierDM> supplierDM, 
																  const boost::shared_ptr<domMod::CustomerDM> customerDM )
	{
		setArticleData( contributionVoucherDM, noAcknowledgementIt );
		setRow();
		setCurrentBooking();
		setYitBranch( branchDM );
		setSearchIt( searchDM );
		findCustomerOrSupplierByPattern( supplierDM, customerDM );
		setYitSignature();
	}

	//-------------------------------------------------------------------
	void CustomerAndSupplierSearcher::setArticleData( const boost::shared_ptr<domMod::ContributionVoucherDM> contributionVoucherDM, const AccessorPropertyTable_YIterator noAcknowledgementIt )
	{
		m_ArticleData = contributionVoucherDM->findOrderPositionsDelivery( noAcknowledgementIt );
	    
		if( m_ArticleData.isNull() || m_ArticleData.empty() ){
			throw MissingDataForPrintingException( basar::ExceptInfo( "CustomerAndSupplierSearcher::setArticleData", "No article data returned from database", __FILE__, __LINE__ ) );
		}
		else
		{
			// Accessor NoAcknowledgement wg NA17 auf OnDemand- Caching ge‰ndert
			// dann muss hier das gesamte Resultset manuell eingelesen werden, sonst ist anschlieﬂendes setRow() nicht korrekt
			// was zur Folge hat, dass bei Nachdrucken immer nur der erste Artikel eines Lieferscheins angedruckt wird.
			AccessorPropertyTable_YIterator yitArticle = m_ArticleData.begin();
			while( yitArticle.isEnd() == false ){
				++yitArticle;
			}
		}
	}

	//-------------------------------------------------------------------
	void CustomerAndSupplierSearcher::setRow()
	{
		m_Rows = static_cast<basar::UInt32> ( m_ArticleData.size() );
	}

	//-------------------------------------------------------------------
	void CustomerAndSupplierSearcher::setCurrentBooking()
	{
		m_CurrentBooking = m_ArticleData.begin();
		
		if( m_CurrentBooking.isNull() || m_CurrentBooking.isEnd() ){
			throw MissingDataForPrintingException( basar::ExceptInfo( "CustomerAndSupplierSearcher::setCurrentBooking", "No booking data returned from database", __FILE__, __LINE__ ) );
		}
	}

	//-------------------------------------------------------------------
	void CustomerAndSupplierSearcher::setYitBranch( const boost::shared_ptr<domMod::BranchDM> branchDM )
	{
		m_YitBranch = branchDM->findBranch();

		if( m_YitBranch.isNull() || m_YitBranch.isEnd() ){
			throw MissingDataForPrintingException( basar::ExceptInfo( "CustomerAndSupplierSearcher::setYitBranch", "No branch data returned from database", __FILE__, __LINE__ ) );
		}
	}

	//-------------------------------------------------------------------
	void CustomerAndSupplierSearcher::setSearchIt( const boost::shared_ptr<domMod::SearchDM> searchDM )
	{
		searchDM->clearSearchPropTab();
		m_SearchIt = searchDM->getAccSearchPropTab().insert( basar::FOR_CLEAN );
		m_SearchIt.setInt32( constants::CUSTOMERNO, m_CurrentBooking.getInt32( constants::CUSTOMERSUPPLIERNO ) );
		m_SearchIt.setInt16( constants::BRANCHNO, m_CurrentBooking.getInt16( constants::BRANCHNO ) );
		m_SearchIt.setString( constants::TRANSACTIONTYPE, m_CurrentBooking.getString( constants::TRANSACTIONTYPE ) );
	}

	//-------------------------------------------------------------------
	void CustomerAndSupplierSearcher::findCustomerOrSupplierByPattern( const boost::shared_ptr<domMod::SupplierDM> supplierDM, const boost::shared_ptr<domMod::CustomerDM> customerDM )
	{
		if( isSearchItEqualGoodsinOrSupplierReturns() ) {
			m_YitCustomerOrSupplier = supplierDM->findSupplierByPattern( m_SearchIt ); 	
		}
		else{
			// for reprinting delivery note / resolving priniting errors, customer data must not be read from kunde but from narctransactioncatalog 
			//					(so that customernomapping- data processed during narctransfer are concerned!)
			//m_YitCustomerOrSupplier = customerDM->findCustomerByPattern( m_SearchIt ); 
			m_YitCustomerOrSupplier = customerDM->findCustomerForReprint( m_ArticleData.begin() );
		}

		if( m_YitCustomerOrSupplier.isEnd() || m_YitBranch.isEnd() ){			
			throw MissingDataForPrintingException( basar::ExceptInfo( "CustomerAndSupplierSearcher::findCustomerOrSupplierByPattern", "No customer data returned from database", __FILE__, __LINE__ ) );
		}
	}

	//-------------------------------------------------------------------
	bool CustomerAndSupplierSearcher::isSearchItEqualGoodsinOrSupplierReturns() const
	{
		if( m_SearchIt.isContainedAndSet( constants::TRANSACTIONTYPE ) && (
			m_SearchIt.getString( constants::TRANSACTIONTYPE ) == TransactionType::GOODSIN_DELIVERY ||
			m_SearchIt.getString( constants::TRANSACTIONTYPE ) == TransactionType::SUPPLIER_RETURNS ) )
		{
			return true;
		}
		return false;
	}

	//-------------------------------------------------------------------
	void CustomerAndSupplierSearcher::setYitSignature()
	{
		basar::I18nString searchParam;
		searchParam.format( "username=%s %s;signaturelnk=%s.bmp;",
		basar::login::Manager::getInstance().getForeName().c_str(),
		basar::login::Manager::getInstance().getSurName().c_str(),
		basar::login::Manager::getInstance().getUserName().c_str() );

		m_YitSignature = basar::db::aspect::Manager::getInstance().createAccessorYIterator( searchParam );

		if( m_YitSignature.isEnd() || m_YitSignature.isEnd() ){			
			throw MissingDataForPrintingException( basar::ExceptInfo( "PrintContributionVoucher::createContributionVoucherDataProvider", "Creation of yitSignature failed", __FILE__, __LINE__ ) );
		}
	}

	//-------------------------------------------------------------------
	AccessorPropertyTableRef CustomerAndSupplierSearcher::getArticleData() const
	{
		return m_ArticleData;
	}

	//-------------------------------------------------------------------
	AccessorPropertyTable_YIterator CustomerAndSupplierSearcher::getYitBranch() const
	{
		return m_YitBranch;
	}

	//-------------------------------------------------------------------
	AccessorPropertyTable_YIterator CustomerAndSupplierSearcher::getYitCustomerOrSupplier() const
	{
		return m_YitCustomerOrSupplier;
	}

	//-------------------------------------------------------------------
	AccessorPropertyTable_YIterator CustomerAndSupplierSearcher::getYitSignature() const
	{
		return m_YitSignature;
	}

	//-------------------------------------------------------------------
	basar::UInt32 CustomerAndSupplierSearcher::getRows() const
	{
		return m_Rows;
	}

}//namespace infrastructure