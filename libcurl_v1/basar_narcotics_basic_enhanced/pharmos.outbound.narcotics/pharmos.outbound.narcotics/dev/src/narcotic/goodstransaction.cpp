//-------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  implemtation of use case "goods transaction"
 *  \author	Bjoern Bischof
 *  \date   27.04.2006
 */
//-------------------------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------------------------//
// includes
//-------------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "goodstransaction.h"
#include "narcoticsappl.h"
#include "transactarticleprintdata.h"
#include "transactcustsupplprintdata.h"
#include "loggerpool.h"

//-------------------------------------------------------------------------------------------------//
// namespaces
//-------------------------------------------------------------------------------------------------//
namespace narcotics
{
namespace useCase 
{

/////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////
GoodsTransactionUC::GoodsTransactionUC()
{
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////
GoodsTransactionUC::~GoodsTransactionUC()
{
}

//-------------------------------------------------------------------------------------------------//
// macro for registrating and deregistrating EventHandlers!
// 1st para: name of UC
// 2nd para: name of handle-Method
BEGIN_HANDLERS_REGISTRATION( GoodsTransactionUC )
	SYSTEM_EVENT_HANDLER_REGISTRATION( GoodsTransactionUC, SearchGoodsTransaction )
	SYSTEM_EVENT_HANDLER_REGISTRATION( GoodsTransactionUC, PrintGoodsTransaction )
END_HANDLERS_REGISTRATION()

/////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////
basar::appl::EventReturnType GoodsTransactionUC::run()
{
	registerEventHandlers( true );

	getManager().m_NarcoticsDM->init(	basar::login::Manager::getInstance().getAreaID(), 
										getManager().m_Connection.getCurrentConnection(), 
										basar::login::Manager::getInstance().getUserName() );
	getManager().m_ArticleDM.init(		basar::login::Manager::getInstance().getAreaID(),
										getManager().m_Connection.getCurrentConnection() );
	getManager().m_BranchDM->init(		basar::login::Manager::getInstance().getAreaID(), 
										getManager().m_Connection.getCurrentConnection() );
	getManager().m_CustomerDM->init(	basar::login::Manager::getInstance().getAreaID(), 
										getManager().m_Connection.getCurrentConnection() );

	getManager().m_TransactionDataVC->init( getManager().m_NarcoticsDM->getAccBookingControlPropTab() );
	getManager().m_TransactionDataVC->show();
	getManager().m_TransactionDataVC->shutdown();
	getManager().m_TransactionDataVC.reset();
	
	getManager().m_currentBooking.clear();
	getManager().m_NarcoticsDM->shutdown();
	getManager().m_ArticleDM.shutdown();
	getManager().m_BranchDM->shutdown();
	getManager().m_CustomerDM->shutdown();

	registerEventHandlers( false );

	return basar::appl::EventReturnStruct(); //std with ret = OK, msg = ""
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////
SYSTEM_EVENT_HANDLER_DEFINITION( GoodsTransactionUC, SearchGoodsTransaction )
{
	BLOG_TRACE_METHOD( LoggerPool::loggerUseCases, "GoodsTransactionUC::SearchGoodsTransaction" );

	rSource = rSource; //if source not needed otherwise (e.g. for comparing); to avoid warnings

	basar::db::aspect::AccessorPropertyTable_YIterator	yitSearch;
	basar::db::aspect::AccessorPropertyTableRef			narcDataPropTab;
	basar::appl::EventReturnStruct						ret;

	yitSearch = getManager().m_NarcoticsDM->getSearchIterator();
	getManager().m_NarcoticsDM->resetSearchIterator();
	getManager().m_TransactionDataVC->matchFromSelection( yitSearch );

	ret = getManager().m_ResultsetSearch.findResultset();
	if( ret.ret != basar::appl::HANDLER_OK ){
		return ret;
	}

	getManager().m_ResultsetSearch.setAdditionalSearchCriteriaProcessed();
	narcDataPropTab = getManager().m_NarcoticsDM->findBookingControlByPattern( yitSearch );
	
	if( narcDataPropTab.empty() )
	{
		matchFromSearchCriteria(); //Result number will be insert in field
		// message that no hits are found
		return basar::appl::EventReturnStruct( basar::appl::HANDLER_INFO, 
			QApplication::translate( "GoodsTransactionUC", "No transaction data found!" ).toLocal8Bit().data() );
	}

	getManager().m_TransactionDataVC->matchToNarcoticBookings();
	matchFromSearchCriteria();
	return ret;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////
SYSTEM_EVENT_HANDLER_DEFINITION( GoodsTransactionUC, PrintGoodsTransaction )
{
	BLOG_TRACE_METHOD(LoggerPool::loggerUseCases, "GoodsTransactionUC::PrintGoodsTransaction");

	rSource = rSource; //if source not needed otherwise (e.g. for comparing); to avoid warnings

	basar::db::aspect::AccessorPropertyTable_YIterator	yitSearch;
	basar::db::aspect::AccessorPropertyTableRef			narcDataPropTab;
	basar::db::aspect::AccessorPropertyTable_YIterator	yitAdditionalData;

	yitSearch = getManager().m_NarcoticsDM->getSearchIterator();
	getManager().m_NarcoticsDM->resetSearchIterator();
	getManager().m_TransactionDataVC->matchFromSelection( yitSearch );

	getManager().m_ResultsetSearch.setAdditionalSearchCriteriaProcessed();
	getManager().m_ResultsetSearch.findResultset();
	narcDataPropTab = getManager().m_NarcoticsDM->findBookingControlByPattern( yitSearch );
	if( narcDataPropTab.empty() )
	{
		return basar::appl::EventReturnStruct( basar::appl::HANDLER_INFO, 
			QApplication::translate("GoodsTransactionUC", "no transaction data found for printing!").toLocal8Bit().data());
	}

	if( yitSearch.getState( constants::CUSTOMERSUPPLIERNO ) != basar::SS_UNSET && 
		false == yitSearch.getString( constants::CUSTOMERSUPPLIERNO ).empty() )
	{
		basar::VarString type = narcDataPropTab.begin().getString( constants::TRANSACTIONTYPE );

		if( type == TransactionType::CUSTOMER_ORDER || 
			type == TransactionType::CUSTOMER_RETURNS )
		{
			// read customer
			basar::I18nString pl;
			pl.format( "branchno=%d;customerno=%s;", 
				basar::login::Manager::getInstance().getAreaID(), 
				yitSearch.getString( constants::CUSTOMERSUPPLIERNO ).c_str() );
				
			yitAdditionalData = getManager().m_CustomerDM->findCustomerByPattern( pl );
		}
		else if( type == TransactionType::GOODSIN_DELIVERY || 
			type == TransactionType::SUPPLIER_RETURNS )
		{
			// read supplier 
			yitAdditionalData = getManager().m_NarcoticsDM->findNarcoticsSupplierByBgaNo( yitSearch.getString( constants::CUSTOMERSUPPLIERNO ).stoi() );
		}
		else if( type == TransactionType::STOCK_DIFFERENCE_INCREASE || 
			type == TransactionType::STOCK_DIFFERENCE_DECREASE		||
			type == TransactionType::DESTRUCTION )
		{
			// read branch
			yitAdditionalData = getManager().m_BranchDM->findBranch();			
		}
		else
		{
			return basar::appl::EventReturnStruct(basar::appl::HANDLER_INFO, 
				QApplication::translate("GoodsTransactionUC", "Transactiontype not known!").toLocal8Bit().data());
		}

		if( yitAdditionalData.isEnd() )
		{
			return basar::appl::EventReturnStruct(basar::appl::HANDLER_INFO, 
				QApplication::translate("GoodsTransactionUC", "Given supplierno / customerno is not valid!").toLocal8Bit().data());
		}

		//preview
		TransactCustSupplPrintData printData( "NarcArticleToCust", "NarcArticleToCust", narcDataPropTab, yitAdditionalData );
		basar::gui::qt::PrintPreview preview( printData, 0, 
					QApplication::translate( "GoodsTransactionUC", "Transactions of 1 customer / supplier" ).toLocal8Bit().data() );
	
		// change: printing directly without creating pdf shall be possible
		preview.setOutputFormats( static_cast<basar::gui::qt::OutputEnum>( basar::gui::qt::PDF | basar::gui::qt::WIN_PRINT ) );
		preview.show();
	}
	else if( yitSearch.getState( constants::ARTICLENO ) != basar::SS_UNSET && false == yitSearch.getString( constants::ARTICLENO ).empty() || 
			 yitSearch.getState( constants::ARTICLECODE ) != basar::SS_UNSET && false == yitSearch.getString( constants::ARTICLECODE ).empty() )
	{
		// read article info in iterator
		yitAdditionalData = getManager().m_ArticleDM.findArticleByPattern( yitSearch );

		if( yitAdditionalData.isEnd() )
		{
			return basar::appl::EventReturnStruct(basar::appl::HANDLER_INFO, 
				QApplication::translate( "GoodsTransactionUC", "Given articleno is not valid!").toLocal8Bit().data() );
		}

		//preview
		TransactArticlePrintData printData( "NarcCustToArticle", "NarcCustToArticle", narcDataPropTab, yitAdditionalData );
		basar::gui::qt::PrintPreview preview( printData, 0, QApplication::translate( "GoodsTransactionUC", "Transactions of 1 article" ).toLocal8Bit().data() );
	
		// change: printing directly without creating pdf shall be possible
		preview.setOutputFormats( static_cast<basar::gui::qt::OutputEnum>( basar::gui::qt::PDF | basar::gui::qt::WIN_PRINT ) );
		preview.show();
	}
	else
	{
		return basar::appl::EventReturnStruct(basar::appl::HANDLER_INFO, 
			QApplication::translate( "GoodsTransactionUC", "To print transactions, either bgano or articleno have to be set!").toLocal8Bit().data() );
	}

	return basar::appl::EventReturnStruct();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////
void GoodsTransactionUC::matchFromSearchCriteria()
{
	if( getManager().m_ResultsetSearch.matchingFromSearchCriteriaAllowed() )
	{
		basar::I18nString searchCriteria = getManager().m_ResultsetSearch.getSearchCriteria();
		basar::db::aspect::AccessorPropertyTable_YIterator searchIt = getManager().m_NarcoticsDM->getSearchIterator();
		if ( searchCriteria == resultsetdefines::SEARCH_FOR_ARTICLENO ){
			getManager().m_TransactionDataVC->matchToArticleNo( searchIt );
		}
		if ( searchCriteria == resultsetdefines::SEARCH_FOR_CUSTOMERNO ){
			getManager().m_TransactionDataVC->matchToCustomerNo( searchIt.getString( constants::CUSTOMERSUPPLIERNO ) );
		}
		if ( searchCriteria == resultsetdefines::SEARCH_FOR_SUPPLIERNO ){
			getManager().m_TransactionDataVC->matchToSupplierNo( searchIt.getString( constants::CUSTOMERSUPPLIERNO ) );
		}
		if ( searchCriteria == resultsetdefines::SEARCH_FOR_ARTICLENO_AND_CUSTOMERNO )
		{
			getManager().m_TransactionDataVC->matchToArticleNo( searchIt );
			getManager().m_TransactionDataVC->matchToCustomerNo( searchIt.getString( constants::CUSTOMERSUPPLIERNO ) );
		}
		if ( searchCriteria == resultsetdefines::SEARCH_FOR_ARTICLENO_AND_SUPPLIERNO )
		{
			getManager().m_TransactionDataVC->matchToArticleNo( searchIt );
			getManager().m_TransactionDataVC->matchToSupplierNo( searchIt.getString( constants::CUSTOMERSUPPLIERNO ) );
		}
	}
	getManager().m_ResultsetSearch.resetSearchCriteria();
}

} //namespace useCase
} //namespace narcotics