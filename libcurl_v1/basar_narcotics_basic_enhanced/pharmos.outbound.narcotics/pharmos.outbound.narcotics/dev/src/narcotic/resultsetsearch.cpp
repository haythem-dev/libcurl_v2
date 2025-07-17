//------------------------------------------------------------------------------
/*! \file
 *  \brief  class for finding resultset to given name
 *  \author Benjamin Endlicher
 *  \date   12.04.2010
 */
//------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------//
// includes
//-------------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "resultsetsearch.h"
#include "narcoticsappl.h"
#include "loggerpool.h"

//-------------------------------------------------------------------------------------------------//
// namespace
//-------------------------------------------------------------------------------------------------//
namespace narcotics
{
namespace useCase 
{
/////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////
ResultsetSearch::ResultsetSearch() 
: UseCaseController(), m_searchCriteria( "" ), m_resultCounter( 0 ), m_DetailDialogCancelled( false )
{
	BLOG_TRACE_METHOD(LoggerPool::loggerUseCases, "ResultsetSearch::ResultsetSearch()");
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////
ResultsetSearch::~ResultsetSearch()
{
	BLOG_TRACE_METHOD(LoggerPool::loggerUseCases, "ResultsetSearch::~ResultsetSearch()");

	m_searchCriteria = "";
	m_resultCounter = 0;
	m_DetailDialogCancelled = false;
}

//-------------------------------------------------------------------------------------------------//
// macro for registrating and deregistrating EventHandlers!
// 1st para: name of UC
// 2nd para: name of handle-Method

BEGIN_HANDLERS_REGISTRATION( ResultsetSearch )
	forSave;
END_HANDLERS_REGISTRATION()

/////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////
basar::appl::EventReturnType ResultsetSearch::run()
{
	BLOG_TRACE_METHOD(LoggerPool::loggerUseCases, "ResultsetSearch::run()");

	registerEventHandlers( true );
	registerEventHandlers( false );
	return basar::appl::EventReturnStruct(); //std with ret = OK, msg = ""
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////
basar::appl::EventReturnStruct ResultsetSearch::findResultset()
{
	BLOG_TRACE_METHOD(LoggerPool::loggerUseCases, "ResultsetSearch::findResultset()");

	basar::appl::EventReturnStruct	ret;

	m_searchCriteria = differentiateSearchCriteria();
	
	if( m_searchCriteria != resultsetdefines::NO_SEARCH_CRITERIA_SET )
	{
		if( m_searchCriteria != resultsetdefines::DIRECTSEARCH )
		{
			if( m_searchCriteria == resultsetdefines::SEARCH_FOR_ARTICLENO_AND_CUSTOMERNO )
			{
				ret = findNoByName( resultsetdefines::SEARCH_FOR_ARTICLENO );
				if( ret.ret != basar::appl::HANDLER_INFO ){
					ret = findNoByName( resultsetdefines::SEARCH_FOR_CUSTOMERNO );
				}
				else{
					return ret;
				}
			}
			else if( m_searchCriteria == resultsetdefines::SEARCH_FOR_ARTICLENO_AND_SUPPLIERNO )
			{
				ret = findNoByName( resultsetdefines::SEARCH_FOR_ARTICLENO );
				if( ret.ret != basar::appl::HANDLER_INFO ){
					ret = findNoByName( resultsetdefines::SEARCH_FOR_SUPPLIERNO );
				}
				else{
					return ret;
				}
			}
			else{
				ret = findNoByName( m_searchCriteria );
			}
		}
	}
	else
	{
		return basar::appl::EventReturnStruct(basar::appl::HANDLER_INFO,
			 QApplication::translate( "ResultsetSearch", "No search criteria set!" ).toLocal8Bit().data());
	}
	return ret;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////
basar::I18nString ResultsetSearch::differentiateSearchCriteria()
{
	BLOG_TRACE_METHOD(LoggerPool::loggerUseCases, "ResultsetSearch::differentiateSearchCriteria()");
	
	bool articleNoSet		= false;
	bool articleNameSet		= false;
	bool customerNoSet		= false;
	bool customerNameSet	= false;
	bool supplierNoSet		= false;
	bool supplierNameSet	= false;
	bool orderNoSet			= false;
	bool narcoticGroupSet	= false;
	bool narcoticNameSet	= false;
	bool toDateSet			= false;
	bool fromDateSet		= false;

	basar::db::aspect::AccessorPropertyTable_YIterator searchIt = getManager().m_NarcoticsDM->getSearchIterator();

// PZN8 Geknödel
	if( searchIt.contains( constants::ARTICLECODE ) && searchIt.getState( constants::ARTICLECODE ) != basar::SS_UNSET )
	{
		basar::Int32		articleNo;
		basar::VarString	articleNoStr;
		articleNo = getManager().m_ArticleDM.findArticleNoByArticleCode( searchIt );
		articleNoStr.itos( articleNo );
		searchIt.setString( constants::ARTICLENO, articleNoStr );
	}
// PZN8 Geknödel

	if( searchIt.contains( constants::ARTICLENO ) && searchIt.getState( constants::ARTICLENO ) != basar::SS_UNSET ){
		articleNoSet = true;
	}

	if( searchIt.contains( constants::ARTICLENAME ) && searchIt.getState( constants::ARTICLENAME ) != basar::SS_UNSET ){
		articleNameSet = true;
	}

	// ToDo: the fact that transactiontype is always set if customersupplierno is set, maybe has to be changed
	if( searchIt.contains( constants::CUSTOMERSUPPLIERNO ) && searchIt.getState( constants::CUSTOMERSUPPLIERNO ) != basar::SS_UNSET &&
		searchIt.contains( constants::TRANSACTIONTYPE ) && searchIt.getState( constants::TRANSACTIONTYPE ) != basar::SS_UNSET &&
		searchIt.getString( constants::TRANSACTIONTYPE ) == TransactionType::CUSTOMER_ORDER )
	{
		customerNoSet = true;
	}

	if( searchIt.contains( constants::CUSTOMERNAME ) && searchIt.getState( constants::CUSTOMERNAME ) != basar::SS_UNSET ){
		customerNameSet = true;
	}

	if( searchIt.contains( constants::CUSTOMERSUPPLIERNO ) && searchIt.getState( constants::CUSTOMERSUPPLIERNO ) != basar::SS_UNSET &&
		searchIt.contains( constants::TRANSACTIONTYPE ) && searchIt.getState( constants::TRANSACTIONTYPE ) != basar::SS_UNSET &&
		searchIt.getString( constants::TRANSACTIONTYPE ) == TransactionType::GOODSIN_DELIVERY )
	{
		supplierNoSet = true;
	}

	// within the following passage supplierno is set as a default value if only customersupplierno is set without any information about transactiontype
	if( searchIt.contains( constants::CUSTOMERSUPPLIERNO ) && searchIt.getState( constants::CUSTOMERSUPPLIERNO ) != basar::SS_UNSET &&
		searchIt.contains( constants::TRANSACTIONTYPE ) && searchIt.getState( constants::TRANSACTIONTYPE ) != basar::SS_UNSET )
	{
		supplierNoSet = true;
	}

	if( searchIt.contains( constants::SUPPLIERNAME ) && searchIt.getState( constants::SUPPLIERNAME ) != basar::SS_UNSET ){
		supplierNameSet = true;
	}

	if( searchIt.contains( constants::PURCHASEORDERNO ) && searchIt.getState( constants::PURCHASEORDERNO ) != basar::SS_UNSET ){
		orderNoSet = true;
	}

	if( searchIt.contains( constants::GROUP ) && searchIt.getState( constants::GROUP ) != basar::SS_UNSET ){
		narcoticGroupSet = true;
	}

	if( searchIt.contains(constants::NARCOTICNAME) && searchIt.getState(constants::NARCOTICNAME) != basar::SS_UNSET ){
		narcoticNameSet = true;
	}

	if( searchIt.contains( constants::TODATE ) && searchIt.getState( constants::TODATE ) != basar::SS_UNSET ){
		toDateSet = true;
	}

	if( searchIt.contains( constants::FROMDATE ) && searchIt.getState( constants::FROMDATE ) != basar::SS_UNSET ){
		fromDateSet = true;
	}

	// at the moment in the case of narcoticGroup being set it isn't possible that narcoticName is set
	// nevertheless because of matters of reusability a direct exclusion between those two is introduced 
	// in the following if-cascade
	if( ( articleNoSet || customerNoSet || supplierNoSet || orderNoSet || narcoticGroupSet || toDateSet || fromDateSet ) && 
		( !articleNameSet && !customerNameSet && !supplierNameSet && !narcoticNameSet ) )
	{
		return resultsetdefines::DIRECTSEARCH;
	}
	if( articleNameSet && !customerNameSet && !supplierNameSet ){
		return resultsetdefines::SEARCH_FOR_ARTICLENO;
	}
	if( customerNameSet && !articleNameSet ){
		return resultsetdefines::SEARCH_FOR_CUSTOMERNO;
	}
	if( supplierNameSet && !articleNameSet ){
		return resultsetdefines::SEARCH_FOR_SUPPLIERNO;
	}
	if( articleNameSet && customerNameSet ){
		return resultsetdefines::SEARCH_FOR_ARTICLENO_AND_CUSTOMERNO;
	}
	if( articleNameSet && supplierNameSet ){
		return resultsetdefines::SEARCH_FOR_ARTICLENO_AND_SUPPLIERNO;
	}
	if( narcoticNameSet ){
		return resultsetdefines::SEARCH_FOR_NARCOTICGROUP;
	}
	
	return resultsetdefines::NO_SEARCH_CRITERIA_SET;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////
basar::appl::EventReturnStruct ResultsetSearch::findNoByName( basar::I18nString searchCriteria )
{
	BLOG_TRACE_METHOD(LoggerPool::loggerUseCases, "ResultsetSearch::findNoByName()");

	m_resultCounter = getSearchResults( searchCriteria );

	if( m_resultCounter == resultsetdefines::NO_HIT )
	{
		return basar::appl::EventReturnStruct( basar::appl::HANDLER_INFO,
			QApplication::translate( "ResultsetSearch", "No results for given name found!" ).toLocal8Bit().data() );
	}
	// if only one result is found for given name, it can be matched into Gui directly
	if( m_resultCounter == resultsetdefines::ONE_HIT ){
		return basar::appl::EventReturnStruct();
	}
	// if more than one result has been found, subdialog has to be shown
	if( m_resultCounter == resultsetdefines::MORE_THAN_ONE_HIT )
	{
		basar::gui::tie::WidgetReturnEnum ret = basar::gui::tie::WIDGET_CANCELED;

		if( searchCriteria == resultsetdefines::SEARCH_FOR_ARTICLENO )
		{
			ret = showDetailDialog(getManager().m_NarcoticsDM->getAccNarCatalogPropTab(),
								constants::ARTICLECODE,
								constants::ARTICLENAME,
								QApplication::translate("ResultsetSearch", "Article-No").toLocal8Bit().constData(),
								QApplication::translate("ResultsetSearch", "Article-Name").toLocal8Bit().constData());
		}

		if( searchCriteria == resultsetdefines::SEARCH_FOR_CUSTOMERNO )
		{
			ret = showDetailDialog(getManager().m_CustomerDM->getAccCustomerPropTab(),
								constants::CUSTOMERNO,
								constants::CUSTOMERNAME,
								QApplication::translate("ResultsetSearch", "Bga-No").toLocal8Bit().constData(),
								QApplication::translate("ResultsetSearch", "Customer").toLocal8Bit().constData());
		}

		if( searchCriteria == resultsetdefines::SEARCH_FOR_SUPPLIERNO )
		{
			ret = showDetailDialog(getManager().m_NarcoticsDM->getAccNarSupplierPropTab(),
								constants::NARCOTICSSUPPLIERNO,
								constants::SUPPLIERNAME,
								QApplication::translate("ResultsetSearch", "Supplier-No").toLocal8Bit().constData(),
								QApplication::translate("ResultsetSearch", "Supplier").toLocal8Bit().constData());
		}

		if( searchCriteria == resultsetdefines::SEARCH_FOR_NARCOTICGROUP )
		{
			ret = showDetailDialog(getManager().m_NarcoticsGroupDM->getAccNarCatalogPropTab(),
								constants::GROUP,
								constants::NARCOTICNAME,
								QApplication::translate("ResultsetSearch", "Narcotic-Group").toLocal8Bit().constData(),
								QApplication::translate("ResultsetSearch", "Narcotic-Name").toLocal8Bit().constData());
		}
		// if something has been selected in detail dialog this value can now be evaluated
		m_DetailDialogCancelled = ( ret == basar::gui::tie::WIDGET_CANCELED );

		return getCurrentSelection( searchCriteria );
	}
	return 	basar::appl::EventReturnStruct();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////
basar::gui::tie::WidgetReturnEnum ResultsetSearch::showDetailDialog( basar::db::aspect::AccessorPropertyTableRef resultTable,
																	 basar::ConstString property1,
																	 basar::ConstString property2,
																	 basar::I18nString headerlabel1, 
																	 basar::I18nString headerlabel2 )
{
	BLOG_TRACE_METHOD(LoggerPool::loggerUseCases, "ResultsetSearch::showDetailDialog()");

	getManager().m_ResultSetOverviewVC->init( resultTable, property1, property2, headerlabel1, headerlabel2 );
	getManager().m_ResultSetOverviewVC->matchToWidget();
	return getManager().m_ResultSetOverviewVC->show();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////
basar::appl::EventReturnStruct ResultsetSearch::getCurrentSelection( basar::I18nString searchcriteria )
{
	BLOG_TRACE_METHOD(LoggerPool::loggerUseCases, "ResultsetSearch::getCurrentSelection()");

	basar::db::aspect::AccessorPropertyTable_YIterator	searchIt;
	
	searchIt = getManager().m_ResultSetOverviewVC->getCurrentSelection();
	
	if( searchIt.isNull() || searchIt.isEnd() )
	{
		m_DetailDialogCancelled = true;
		getManager().m_ResultSetOverviewVC->shutdown();
		return basar::appl::EventReturnStruct(basar::appl::HANDLER_INFO, 
			QApplication::translate("ResultsetSearch", "No entry has been selected. Consequently search criteria is invalid!").toLocal8Bit().data());
	}
	matchNumberForName( searchcriteria, true );
	getManager().m_ResultSetOverviewVC->shutdown();

	return basar::appl::EventReturnStruct();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////
void ResultsetSearch::matchNumberForName( basar::I18nString searchcriteria, bool detailDialogShown )
{
	BLOG_TRACE_METHOD(LoggerPool::loggerUseCases, "ResultsetSearch::matchNumberForName()");

	basar::db::aspect::AccessorPropertyTable_YIterator yitSelected;

	if( searchcriteria == resultsetdefines::SEARCH_FOR_ARTICLENO )
	{
		if( detailDialogShown ){
			yitSelected = getManager().m_ResultSetOverviewVC->getCurrentSelection();
		}
		else{
			yitSelected = getManager().m_NarcoticsDM->getAccNarCatalogPropTab().begin();
		}
// PZN8 Geknödel
		basar::VarString articleNo;
		articleNo.itos( yitSelected.getInt32( constants::ARTICLENO ) );
		getManager().m_NarcoticsDM->getSearchIterator().setString( constants::ARTICLENO, articleNo );
		getManager().m_NarcoticsDM->getSearchIterator().setString( constants::ARTICLECODE, yitSelected.getString( constants::ARTICLECODE ) );
// PZN8 Geknödel
	}
	if( searchcriteria == resultsetdefines::SEARCH_FOR_CUSTOMERNO )
	{
		basar::Int32 customerNo;
		if( detailDialogShown ){
			customerNo = getManager().m_ResultSetOverviewVC->getCurrentSelection().getInt32( constants::CUSTOMERNO );
		}
		else{
			customerNo = getManager().m_CustomerDM->getAccCustomerPropTab().begin().getInt32( constants::CUSTOMERNO );
		}
		basar::I18nString customerNoValue;
		customerNoValue.itos( customerNo );
		getManager().m_NarcoticsDM->getSearchIterator().setString( constants::CUSTOMERSUPPLIERNO, customerNoValue );
	}
	if( searchcriteria == resultsetdefines::SEARCH_FOR_SUPPLIERNO )
	{
		basar::Int32 supplierNo;
		if( detailDialogShown ){
			supplierNo = getManager().m_ResultSetOverviewVC->getCurrentSelection().getInt32( constants::NARCOTICSSUPPLIERNO );
		}
		else{
			supplierNo = getManager().m_NarcoticsDM->getAccNarSupplierPropTab().begin().getInt32( constants::NARCOTICSSUPPLIERNO );
		}
		basar::I18nString supplierNoValue;
		supplierNoValue.itos( supplierNo );
		getManager().m_NarcoticsDM->getSearchIterator().setString( constants::CUSTOMERSUPPLIERNO, supplierNoValue );
	}
	if( searchcriteria == resultsetdefines::SEARCH_FOR_NARCOTICGROUP )
	{
		basar::I18nString narcoticGroup;
		if( detailDialogShown ){
			narcoticGroup = getManager().m_ResultSetOverviewVC->getCurrentSelection().getString( constants::GROUP );
		}
		else{
			narcoticGroup = getManager().m_NarcoticsGroupDM->getAccNarCatalogPropTab().begin().getString( constants::GROUP );
		}
		getManager().m_NarcoticsDM->getSearchIterator().setString( constants::GROUP, narcoticGroup );
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////
resultsetdefines::searchResultHits ResultsetSearch::getSearchResults( basar::I18nString searchcriteria )
{
	BLOG_TRACE_METHOD(LoggerPool::loggerUseCases, "ResultsetSearch::getSearchResults()");

	basar::db::aspect::AccessorPropertyTableRef resultTable;
	basar::db::aspect::AccessorPropertyTable_YIterator yitSearch;

	// decide which DM function has to be called
	yitSearch = getManager().m_NarcoticsDM->getSearchIterator();

	if( searchcriteria == resultsetdefines::SEARCH_FOR_ARTICLENO ){
		resultTable = getManager().m_NarcoticsDM->findArticleNoByName( yitSearch );
	}

	if( searchcriteria == resultsetdefines::SEARCH_FOR_CUSTOMERNO ){
		resultTable = getManager().m_CustomerDM->findCustomerNoByName( yitSearch );
	}

	if( searchcriteria == resultsetdefines::SEARCH_FOR_SUPPLIERNO ){
		resultTable = getManager().m_NarcoticsDM->findNarcoticsSupplierByName( yitSearch.getString( constants::SUPPLIERNAME ) );
	}

	if( searchcriteria == resultsetdefines::SEARCH_FOR_NARCOTICGROUP ){
		resultTable = getManager().m_NarcoticsGroupDM->findGroupByName( yitSearch );
	}

	if( resultTable.empty() ){
		return resultsetdefines::NO_HIT;
	}

	// as this UseCase might be reused, the caching policy shouldn't be changed (OnDemandCaching -> FullCaching)
	// nevertheless fullCaching is needed here to make a successful decision if DetailVC has to be shown or not
	// therefore void forwardCaching() is implemented here to adjust FullCaching mode here
	if( searchcriteria == resultsetdefines::SEARCH_FOR_ARTICLENO ){
		forwardCaching( getManager().m_NarcoticsDM->getAccNarCatalogPropTab() );
	}
	if( searchcriteria == resultsetdefines::SEARCH_FOR_CUSTOMERNO ){
		forwardCaching( getManager().m_CustomerDM->getAccCustomerPropTab() );
	}
	if( searchcriteria == resultsetdefines::SEARCH_FOR_SUPPLIERNO ){
		forwardCaching( getManager().m_NarcoticsDM->getAccNarSupplierPropTab() );
	}
	if( searchcriteria == resultsetdefines::SEARCH_FOR_NARCOTICGROUP ){
		forwardCaching( getManager().m_NarcoticsGroupDM->getAccNarCatalogPropTab() );
	}

	// if only one result is found for given narcotic it can be shown directly in Gui
	if( resultTable.size() == 1 )
	{
		matchNumberForName( searchcriteria, false );
		return resultsetdefines::ONE_HIT;
	}
	else{
		return resultsetdefines::MORE_THAN_ONE_HIT;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	void
	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
void ResultsetSearch::forwardCaching( basar::db::aspect::AccessorPropertyTableRef table )
{
	// two rows of the table are cached in order to check if table contains of more than one row
	// needed to decide if DetailVC has to be shown or not
	basar::db::aspect::AccessorPropertyTable_YIterator yit = table.begin();
	yit++;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////
void ResultsetSearch::setAdditionalSearchCriteriaProcessed()
{
	BLOG_TRACE_METHOD(LoggerPool::loggerUseCases, "ResultsetSearch::setAdditionalSearchCriteriaProcessed()");

	getManager().m_NarcoticsDM->getSearchIterator().setString( constants::PROCESSEDFLAG, constants::ISPROCESSED );
	getManager().m_NarcoticsDM->getSearchIterator().setString( constants::DELETEDFLAG, constants::ISNOTDELETED );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////
void ResultsetSearch::setAdditionalSearchCriteriaNotProcessed()
{
	BLOG_TRACE_METHOD(LoggerPool::loggerUseCases, "ResultsetSearch::setAdditionalSearchCriteriaNotProcessed()");

	getManager().m_NarcoticsDM->getSearchIterator().setString( constants::PROCESSEDFLAG, constants::ISNOTPROCESSED );
	getManager().m_NarcoticsDM->getSearchIterator().setString( constants::DELETEDFLAG, constants::ISNOTDELETED );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////
bool ResultsetSearch::matchingFromSearchCriteriaAllowed()
{
	BLOG_TRACE_METHOD(LoggerPool::loggerUseCases, "ResultsetSearch::matchingFromSearchCriteriaAllowed()");

	return ( ( m_resultCounter != resultsetdefines::NO_HIT ) && !m_DetailDialogCancelled );
}	

/////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////
basar::I18nString ResultsetSearch::getSearchCriteria()
{
	return this->m_searchCriteria;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////
void ResultsetSearch::resetSearchCriteria()
{
	m_searchCriteria = "";
}

} //namespace useCase
} //namespace narcotics