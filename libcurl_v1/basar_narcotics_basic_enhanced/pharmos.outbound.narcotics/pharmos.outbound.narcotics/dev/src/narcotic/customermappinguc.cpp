//-------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  implemtation of use case customer mapping
 *  \author	Bjoern Bischof
 *  \date   27.04.2006
 */
//-------------------------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------------------------//
// includes
//-------------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "customermappinguc.h"
#include "narcoticsappl.h"
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
CustomerMappingUC::CustomerMappingUC()
{
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////
CustomerMappingUC::~CustomerMappingUC()
{
}

//-------------------------------------------------------------------------------------------------//
// macro for registrating and deregistrating EventHandlers!
// 1st para: name of UC
// 2nd para: name of handle-Method
BEGIN_HANDLERS_REGISTRATION( CustomerMappingUC )
	SYSTEM_EVENT_HANDLER_REGISTRATION( CustomerMappingUC, SearchCustomerMappings )
	SYSTEM_EVENT_HANDLER_REGISTRATION( CustomerMappingUC, DeleteCustomerMapping )
	SYSTEM_EVENT_HANDLER_REGISTRATION( CustomerMappingUC, EditCustomerMapping )
	SYSTEM_EVENT_HANDLER_REGISTRATION( CustomerMappingUC, CreateCustomerMapping )
	SYSTEM_EVENT_HANDLER_REGISTRATION( CustomerMappingUC, SaveCustomerMapping )
	SYSTEM_EVENT_HANDLER_REGISTRATION( CustomerMappingUC, CheckCustomerExtern )
	SYSTEM_EVENT_HANDLER_REGISTRATION( CustomerMappingUC, CheckCustomerIntern )
END_HANDLERS_REGISTRATION()

/////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////
basar::appl::EventReturnType CustomerMappingUC::run()
{
	registerEventHandlers( true );

	getManager().m_NarcoticsDM->init(	basar::login::Manager::getInstance().getAreaID(), 
										getManager().m_Connection.getCurrentConnection(), 
										basar::login::Manager::getInstance().getUserName() );

	getManager().m_CustomerDM->init(	basar::login::Manager::getInstance().getAreaID(), 
										getManager().m_Connection.getCurrentConnection() );

	getManager().m_CustomerMappingVC->init( getManager().m_CustomerDM->getAccCustMappingPropTab() );
	getManager().m_CustomerMappingVC->show();

	getManager().m_CustomerMappingVC->shutdown();
	getManager().m_CustomerMappingVC.reset();
	
	getManager().m_CustomerDM->shutdown();
	getManager().m_NarcoticsDM->shutdown();
	
	registerEventHandlers( false );

	return basar::appl::EventReturnStruct(); //std with ret = OK, msg = ""
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////
SYSTEM_EVENT_HANDLER_DEFINITION( CustomerMappingUC, CheckCustomerExtern )
{
	BLOG_TRACE_METHOD( LoggerPool::loggerUseCases, "CustomerMappingUC::CheckCustomerExtern" );

	rSource = rSource; //if source not needed otherwise (e.g. for comparing); to avoid warnings

	basar::appl::EventReturnStruct						ret;
	basar::db::aspect::AccessorPropertyTable_YIterator	yitSearch;
	yitSearch = getManager().m_NarcoticsDM->getSearchIterator();
	getManager().m_NarcoticsDM->resetSearchIterator();
	m_CustomerMappingDetailsVC->matchFromCustExtern( yitSearch );

	// yitSearch.setString( constants::SHOW_LOCKED_CUST, "1" );
	yitSearch.setString( constants::TRANSACTIONTYPE, TransactionType::CUSTOMER_ORDER ); // not really needed, but for searching customer by name ...

	ret = getManager().m_ResultsetSearch.findResultset();
		
	if( ret.ret != basar::appl::HANDLER_OK )
		return ret;
		
	yitFromEvSrc.setInt32(constants::CUSTOMERNO, atoi(yitSearch.getString(constants::CUSTOMERSUPPLIERNO).c_str()));

	basar::db::aspect::AccessorPropertyTable_YIterator	yitCust = getManager().m_CustomerDM->findCustomerByPattern(yitFromEvSrc);

	if (!yitCust.isNull() && !yitCust.isEnd())
	{
		yitFromEvSrc.setInt32(constants::CUSTOMERNO_EXTERN, yitCust.getInt32(constants::CUSTOMERNO) );
		yitFromEvSrc.setString(constants::STREET_EXTERN, yitCust.getString(constants::STREET));
		yitFromEvSrc.setString(constants::CIP_EXTERN, yitCust.getString(constants::POSTCODE));
		yitFromEvSrc.setString(constants::LOCATION_EXTERN, yitCust.getString(constants::CUSTOMERLOCATION));
		yitFromEvSrc.setString(constants::NAME_EXTERN, yitCust.getString(constants::CUSTOMERNAME));
		m_CustomerMappingDetailsVC->matchToCustomerData(yitFromEvSrc);
	}
	else
	{
		return basar::appl::EventReturnStruct( basar::appl::HANDLER_ERROR, 
			QApplication::translate( "CustomerMappingUC", "customer not known!" ).toLocal8Bit().data() );
	}
		
	return ret;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////
SYSTEM_EVENT_HANDLER_DEFINITION( CustomerMappingUC, CheckCustomerIntern )
{
	BLOG_TRACE_METHOD( LoggerPool::loggerUseCases, "CustomerMappingUC::CheckCustomerIntern" );

	rSource = rSource; //if source not needed otherwise (e.g. for comparing); to avoid warnings

	basar::appl::EventReturnStruct						ret;
	basar::db::aspect::AccessorPropertyTable_YIterator	yitSearch;
	yitSearch = getManager().m_NarcoticsDM->getSearchIterator();
	getManager().m_NarcoticsDM->resetSearchIterator();
	m_CustomerMappingDetailsVC->matchFromCustIntern( yitSearch );

	// yitSearch.setString( constants::SHOW_LOCKED_CUST, "1" );
	yitSearch.setString( constants::TRANSACTIONTYPE, TransactionType::CUSTOMER_ORDER ); // not really needed, but for searching customer by name ...

	ret = getManager().m_ResultsetSearch.findResultset();
		
	if( ret.ret != basar::appl::HANDLER_OK )
		return ret;
		
	yitFromEvSrc.setInt32(constants::CUSTOMERNO, atoi(yitSearch.getString(constants::CUSTOMERSUPPLIERNO).c_str()));

	basar::db::aspect::AccessorPropertyTable_YIterator	yitCust = getManager().m_CustomerDM->findCustomerByPattern(yitFromEvSrc);

	if (!yitCust.isNull() && !yitCust.isEnd())
	{
		yitFromEvSrc.setInt32(constants::CUSTOMERNO_INTERN, yitCust.getInt32(constants::CUSTOMERNO) );
		yitFromEvSrc.setString(constants::STREET_INTERN, yitCust.getString(constants::STREET));
		yitFromEvSrc.setString(constants::CIP_INTERN, yitCust.getString(constants::POSTCODE));
		yitFromEvSrc.setString(constants::LOCATION_INTERN, yitCust.getString(constants::CUSTOMERLOCATION));
		yitFromEvSrc.setString(constants::NAME_INTERN, yitCust.getString(constants::CUSTOMERNAME));
		m_CustomerMappingDetailsVC->matchToCustomerData(yitFromEvSrc);
	}
	else
	{
		return basar::appl::EventReturnStruct( basar::appl::HANDLER_ERROR, 
			QApplication::translate( "CustomerMappingUC", "customer not known!" ).toLocal8Bit().data() );
	}
		
	return ret;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////
SYSTEM_EVENT_HANDLER_DEFINITION( CustomerMappingUC, SaveCustomerMapping )
{
	BLOG_TRACE_METHOD( LoggerPool::loggerUseCases, "CustomerMappingUC::SaveCustomerMapping" );

	rSource = rSource; //if source not needed otherwise (e.g. for comparing); to avoid warnings

	basar::db::aspect::ExecuteResultInfo               resInfo;

	m_CustomerMappingDetailsVC->matchFromCustomerData(getManager().m_CustomerDM->getAccCustMappingPropTab().begin());

	resInfo = getManager().m_CustomerDM->saveCustomerMapping(getManager().m_CustomerDM->getAccCustMappingPropTab().begin());

	if( resInfo.hasError() || resInfo.m_affectedRows != 1 )
	{
		return basar::appl::EventReturnStruct( basar::appl::HANDLER_ERROR, 
			QApplication::translate( "CustomerMappingUC", "customer mapping not saved!" ).toLocal8Bit().data() );
	}
	else
		return basar::appl::EventReturnStruct();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////
SYSTEM_EVENT_HANDLER_DEFINITION( CustomerMappingUC, EditCustomerMapping )
{
	BLOG_TRACE_METHOD( LoggerPool::loggerUseCases, "CustomerMappingUC::EditCustomerMapping" );

	rSource = rSource; //if source not needed otherwise (e.g. for comparing); to avoid warnings

	basar::appl::EventReturnStruct						ret;

	m_CustomerMappingDetailsVC->init( getManager().m_CustomerDM->getAccCustMappingPropTab(), false );
	m_CustomerMappingDetailsVC->matchToCustomerData(yitFromEvSrc);
	m_CustomerMappingDetailsVC->show();

	m_CustomerMappingDetailsVC->shutdown();
	m_CustomerMappingDetailsVC.reset();

	return ret;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////
SYSTEM_EVENT_HANDLER_DEFINITION( CustomerMappingUC, CreateCustomerMapping )
{
	BLOG_TRACE_METHOD( LoggerPool::loggerUseCases, "CustomerMappingUC::CreateCustomerMapping" );

	rSource = rSource; //if source not needed otherwise (e.g. for comparing); to avoid warnings

	basar::appl::EventReturnStruct						ret;

	m_CustomerMappingDetailsVC->init( getManager().m_CustomerDM->getAccCustMappingPropTab(), true );
	m_CustomerMappingDetailsVC->matchToCustomerData(getManager().m_CustomerDM->addEmptyCustMapping());
	m_CustomerMappingDetailsVC->show();

	m_CustomerMappingDetailsVC->shutdown();
	m_CustomerMappingDetailsVC.reset();

	return ret;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////
SYSTEM_EVENT_HANDLER_DEFINITION( CustomerMappingUC, SearchCustomerMappings )
{
	BLOG_TRACE_METHOD( LoggerPool::loggerUseCases, "CustomerMappingUC::SearchCustomerMappings" );

	rSource = rSource; //if source not needed otherwise (e.g. for comparing); to avoid warnings

	basar::db::aspect::AccessorPropertyTable_YIterator	yitSearch;
	basar::appl::EventReturnStruct						ret;

	yitSearch = getManager().m_NarcoticsDM->getSearchIterator();
	getManager().m_NarcoticsDM->resetSearchIterator();
	getManager().m_CustomerMappingVC->matchFromSelection( yitSearch );

	if ( yitSearch.isContainedAndSet(constants::CUSTOMERSUPPLIERNO) || yitSearch.isContainedAndSet(constants::CUSTOMERNAME))
	{
		// yitSearch.setString( constants::SHOW_LOCKED_CUST, "1" );
		yitSearch.setString( constants::TRANSACTIONTYPE, TransactionType::CUSTOMER_ORDER ); // not really needed, but for searching customer by name ...

		ret = getManager().m_ResultsetSearch.findResultset();
		
		if( ret.ret != basar::appl::HANDLER_OK )
			return ret;
		else
			getManager().m_CustomerMappingVC->matchToCustomerNo( yitSearch.getString( constants::CUSTOMERSUPPLIERNO ) );
	}
		
	if( getManager().m_CustomerDM->findCustomerMapping( yitSearch ).empty() )
	{
		// message that no hits are found
		return basar::appl::EventReturnStruct( basar::appl::HANDLER_INFO, 
			QApplication::translate( "CustomerMappingUC", "No customer mappings found!" ).toLocal8Bit().data() );
	}

	getManager().m_CustomerMappingVC->matchToCustomerMappings();
	
	return ret;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////
SYSTEM_EVENT_HANDLER_DEFINITION( CustomerMappingUC, DeleteCustomerMapping )
{
	BLOG_TRACE_METHOD(LoggerPool::loggerUseCases, "CustomerMappingUC::DeleteCustomerMapping");

	rSource = rSource; //if source not needed otherwise (e.g. for comparing); to avoid warnings

	basar::db::aspect::ExecuteResultInfo               resInfo;

	resInfo = getManager().m_CustomerDM->deleteCustomerMapping(yitFromEvSrc);

	if( resInfo.hasError() || resInfo.m_affectedRows != 1 )
	{
		return basar::appl::EventReturnStruct( basar::appl::HANDLER_ERROR, 
			QApplication::translate( "CustomerMappingUC", "No customer mapping deleted!" ).toLocal8Bit().data() );
	}
	else
		return basar::appl::EventReturnStruct();
}

} //namespace useCase
} //namespace narcotics