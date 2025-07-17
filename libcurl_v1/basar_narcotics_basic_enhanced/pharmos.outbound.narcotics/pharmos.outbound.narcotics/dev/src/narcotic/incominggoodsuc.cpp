//-------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  implemtation of use case "incoming goods"
 *  \author	Roland Kiefert
 *  \date   14.02.2006
 */
//-------------------------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------------------------//
// includes
//-------------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "incominggoodsuc.h"
#include "narcoticsappl.h"
#include "loggerpool.h"

//-------------------------------------------------------------------------------------------------//
// using declaratrion section
//-------------------------------------------------------------------------------------------------//
using basar::Int32;
using basar::I18nString;
using basar::db::aspect::AccessorPropertyTable_YIterator;
using basar::db::aspect::AccessorPropertyTableRef;

//-------------------------------------------------------------------------------------------------//
// namespaces
//-------------------------------------------------------------------------------------------------//
namespace narcotics
{
namespace useCase
{

//-------------------------------------------------------------------------------------------------//
// macro for registrating and deregistrating EventHandlers!
// 1st para: name of UC
// 2nd para: name of handle-Method
BEGIN_HANDLERS_REGISTRATION( IncomingGoodsUC )
	SYSTEM_EVENT_HANDLER_REGISTRATION( IncomingGoodsUC, SearchIncomingGoods                  )
	SYSTEM_EVENT_HANDLER_REGISTRATION( IncomingGoodsUC, ReplaceSupplier                      )
	SYSTEM_EVENT_HANDLER_REGISTRATION( IncomingGoodsUC, DisplayUndealedIncomingGoods         )
	SYSTEM_EVENT_HANDLER_REGISTRATION( IncomingGoodsUC, DisplayUndealedReturnedGoods         )
	SYSTEM_EVENT_HANDLER_REGISTRATION( IncomingGoodsUC, SaveIncomingGoodsList                )
	SYSTEM_EVENT_HANDLER_REGISTRATION( IncomingGoodsUC, RegisteredIncomingGood               )
	SYSTEM_EVENT_HANDLER_REGISTRATION( IncomingGoodsUC, NewBookingIncomingGood               )
	SYSTEM_EVENT_HANDLER_REGISTRATION( IncomingGoodsUC, ReadArticleDataIncomingGood          )
	SYSTEM_EVENT_HANDLER_REGISTRATION( IncomingGoodsUC, ReadCustomerDataIncomingGood         )
	SYSTEM_EVENT_HANDLER_REGISTRATION( IncomingGoodsUC, DeleteIncomingGoods                  )
	SYSTEM_EVENT_HANDLER_REGISTRATION( IncomingGoodsUC, SetCurrentIteratorIncomingGoods      )
	SYSTEM_EVENT_HANDLER_REGISTRATION( IncomingGoodsUC, AllUnprocessedIncomingGoodsRequested )
	SYSTEM_EVENT_HANDLER_REGISTRATION( IncomingGoodsUC, ReloadLastIncomingGoodsRequested     )
END_HANDLERS_REGISTRATION()

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
IncomingGoodsUC::IncomingGoodsUC()
:m_ChangingState( "" )
{
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
IncomingGoodsUC::~IncomingGoodsUC()
{
	m_ChangingState = "";
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
basar::appl::EventReturnType IncomingGoodsUC::run()
{
	basar::ConstString fun = "IncomingGoodsUC::run()";
    BLOG_TRACE( LoggerPool::loggerUseCases, fun );

    basar::appl::EventReturnStruct eventReturnStruct;

    registerEventHandlers( true );
    init();

    eventReturnStruct = findNarcoticsSupplier();
    PROCESS_EVENT_RETURN( eventReturnStruct, getManager().m_IncomingGoodsOverviewVC.getWidgetPtr() )

    eventReturnStruct = findUnprocessedIncomingGoods();
    PROCESS_EVENT_RETURN( eventReturnStruct, getManager().m_IncomingGoodsOverviewVC.getWidgetPtr() )

    eventReturnStruct = findNumberOfIncomingGoods();
    PROCESS_EVENT_RETURN( eventReturnStruct, getManager().m_IncomingGoodsOverviewVC.getWidgetPtr() )
	// initialize search iterator for article and customer specific data
	getManager().m_SearchDM->clearSearchPropTab();
	m_SearchIt = getManager().m_SearchDM->getAccSearchPropTab().insert( basar::FOR_CLEAN );

	getManager().m_IncomingGoodsOverviewVC->show();

    shutdown();
	registerEventHandlers( false );

	return eventReturnStruct;
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
void IncomingGoodsUC::init()
{
    basar::ConstString fun = "IncomingGoodsUC::init()";
    BLOG_TRACE( LoggerPool::loggerUseCases, fun );

    m_state = ONLY_QUANTITY_INPUT;
    getManager().m_NarcoticsDM->init(	basar::login::Manager::getInstance().getAreaID(),
										getManager().m_Connection.getCurrentConnection(),
										basar::login::Manager::getInstance().getUserName() );
	getManager().m_NarcoticsGroupDM->init(	getManager().m_Connection.getCurrentConnection() );
	getManager().m_CustomerDM->init(	basar::login::Manager::getInstance().getAreaID(),
										getManager().m_Connection.getCurrentConnection() );
	getManager().m_ArticleDM.init( basar::login::Manager::getInstance().getAreaID(),  getManager().m_Connection.getCurrentConnection() );
	getManager().m_SearchDM->init( getManager().m_Connection.getCurrentConnection() );
	getManager().m_IncomingGoodsOverviewVC->init( getManager().m_NarcoticsDM->getAccNarCatalogPropTab(),
													getManager().m_NarcoticsDM->getAccNarSupplierPropTab(),
													getManager().m_NarcoticsDM->getAccNoReturnGoodsPropTab(),
													getManager().m_NarcoticsDM->getAccNoIncomingGoodsPropTab() );
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
void IncomingGoodsUC::shutdown()
{
    basar::ConstString fun = "IncomingGoodsUC::shutdown()";
    BLOG_TRACE( LoggerPool::loggerUseCases, fun );

    getManager().m_IncomingGoodsOverviewVC->shutdown();
	getManager().m_IncomingGoodsOverviewVC.reset();

	getManager().m_currentBooking.clear();
	getManager().m_SearchDM->shutdown();
	getManager().m_ArticleDM.shutdown();
	getManager().m_CustomerDM->shutdown();
	getManager().m_NarcoticsGroupDM->shutdown();
	getManager().m_NarcoticsDM->shutdown();
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
basar::appl::EventReturnStruct IncomingGoodsUC::findNarcoticsSupplier()
{
    basar::ConstString fun = "IncomingGoodsUC::findNarcoticsSupplier()";
    BLOG_TRACE( LoggerPool::loggerUseCases, fun );

    basar::appl::EventReturnStruct result;

    AccessorPropertyTableRef suppliersPropTab = getManager().m_NarcoticsDM->findNarcoticsSupplier();
    if( suppliersPropTab.isNull() || suppliersPropTab.empty() ){
        result.ret = basar::appl::HANDLER_ERROR;
        result.message = QApplication::translate( "IncomingGoodsUC",
                                                  "Error with finding narcotics suppliers."
                                                ).toLocal8Bit().data();
        return result;
    }

	getManager().m_IncomingGoodsOverviewVC->matchFromSupplier();
    return result;
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
basar::appl::EventReturnStruct IncomingGoodsUC::findUnprocessedIncomingGoods()
{
    basar::ConstString fun = "IncomingGoodsUC::findUnprocessedIncomingGoods()";
    BLOG_TRACE( LoggerPool::loggerUseCases, fun );

    basar::appl::EventReturnStruct eventReturnStruct;

    getManager().m_NarcoticsDM->resetSearchIterator();
	getManager().m_NarcoticsDM->getSearchIterator().setString( constants::PROCESSEDFLAG, constants::ISNOTPROCESSED );
	getManager().m_NarcoticsDM->getSearchIterator().setString( constants::DELETEDFLAG, constants::ISNOTDELETED );

	eventReturnStruct = findIncomingGoods();

    return eventReturnStruct;
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
basar::appl::EventReturnStruct IncomingGoodsUC::findNumberOfIncomingGoods()
{
    basar::ConstString fun = "IncomingGoodsUC::findNumberOfIncomingGoods()";
    BLOG_TRACE( LoggerPool::loggerUseCases, fun );

    basar::appl::EventReturnStruct eventReturnStruct;

    AccessorPropertyTableRef result = getManager().m_NarcoticsDM->findNoIncomingGoods();
    if( result.isNull() || result.empty() )
	{
        eventReturnStruct.ret = basar::appl::HANDLER_ERROR;
        eventReturnStruct.message = QApplication::translate( "IncomingGoodsUC",
                                                             "Error with finding number of "
                                                             "(customers and suppliers) incoming goods."
                                                           ).toLocal8Bit().data();
        return eventReturnStruct;
    }

	getManager().m_IncomingGoodsOverviewVC->matchFromNoIncomingGoods();

    return eventReturnStruct;
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
basar::appl::EventReturnStruct IncomingGoodsUC::findIncomingGoods()
{
    basar::ConstString fun = "IncomingGoodsUC::findIncomingGoods()";
    BLOG_TRACE( LoggerPool::loggerUseCases, fun );

    basar::appl::EventReturnStruct eventReturnStruct;

    AccessorPropertyTableRef catalogPropTab =
        getManager().m_NarcoticsDM->findCatalogByPattern( getManager().m_NarcoticsDM->getSearchIterator() );

    if( catalogPropTab.isNull() )
	{
        eventReturnStruct.ret     = basar::appl::HANDLER_ERROR;
        eventReturnStruct.message = QApplication::translate( "IncomingGoodsUC",
                                                             "Error with finding narcotics transaction datasets!"
                                                           ).toLocal8Bit().data();
        return eventReturnStruct;
    }

    if( catalogPropTab.empty() )
	{
        eventReturnStruct.ret     = basar::appl::HANDLER_INFO;
        eventReturnStruct.message = QApplication::translate( "IncomingGoodsUC",
                                                             "No narcotics transaction datasets found."
                                                           ).toLocal8Bit().data();
        return eventReturnStruct;
    }

    getManager().m_IncomingGoodsOverviewVC->matchFromIncomingGoods();
    getManager().m_IncomingGoodsOverviewVC->switchSupplierReplacement( getManager().m_NarcoticsDM->isSupplierReplaceable() );

    return eventReturnStruct;
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
SYSTEM_EVENT_HANDLER_DEFINITION( IncomingGoodsUC, DisplayUndealedIncomingGoods )
{
    rSource = rSource; //if source not needed otherwise (e.g. for comparing); to avoid warnings

    basar::ConstString fun = "IncomingGoodsUC::DisplayUndealedIncomingGoods()";
    BLOG_TRACE( LoggerPool::loggerUseCases, fun );

    basar::appl::EventReturnStruct eventReturnStruct;

	getManager().m_NarcoticsDM->resetSearchIterator();
	getManager().m_NarcoticsDM->getSearchIterator().setString( constants::PROCESSEDFLAG, constants::ISNOTPROCESSED );
	getManager().m_NarcoticsDM->getSearchIterator().setString( constants::DELETEDFLAG, constants::ISNOTDELETED );
    getManager().m_NarcoticsDM->getSearchIterator().setString( constants::TRANSACTIONTYPE, TransactionType::GOODSIN_DELIVERY );

    eventReturnStruct = findIncomingGoods();
	if( false == eventReturnStruct.isHandlerOK() ){
		return eventReturnStruct;
	}

    return findNumberOfIncomingGoods();
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
SYSTEM_EVENT_HANDLER_DEFINITION( IncomingGoodsUC, DisplayUndealedReturnedGoods )
{
    rSource = rSource; //if source not needed otherwise (e.g. for comparing); to avoid warnings

	basar::ConstString fun = "IncomingGoodsUC::DisplayUndealedReturnedGoods()";
    BLOG_TRACE( LoggerPool::loggerUseCases, fun );

    basar::appl::EventReturnStruct eventReturnStruct;

	getManager().m_NarcoticsDM->resetSearchIterator();
	getManager().m_NarcoticsDM->getSearchIterator().setString( constants::PROCESSEDFLAG, constants::ISNOTPROCESSED );
	getManager().m_NarcoticsDM->getSearchIterator().setString( constants::DELETEDFLAG, constants::ISNOTDELETED );
	getManager().m_NarcoticsDM->getSearchIterator().setString( constants::TRANSACTIONTYPE, TransactionType::CUSTOMER_RETURNS );

    eventReturnStruct = findIncomingGoods();
	if( false == eventReturnStruct.isHandlerOK() ){
		return eventReturnStruct;
	}

    return findNumberOfIncomingGoods();
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
SYSTEM_EVENT_HANDLER_DEFINITION( IncomingGoodsUC, SearchIncomingGoods )
{
    basar::ConstString fun = "IncomingGoodsUC::SearchIncomingGoods()";
    BLOG_TRACE( LoggerPool::loggerUseCases, fun );
    rSource = rSource; //if source not needed otherwise (e.g. for comparing); to avoid warnings

    getManager().m_NarcoticsDM->resetSearchIterator();
	getManager().m_NarcoticsDM->getSearchIterator().setString( constants::PROCESSEDFLAG, constants::ISNOTPROCESSED );
	getManager().m_NarcoticsDM->getSearchIterator().setString( constants::DELETEDFLAG, constants::ISNOTDELETED );

    getManager().m_IncomingGoodsOverviewVC->matchToSelection( getManager().m_NarcoticsDM->getSearchIterator() );

    LOG_YIT( fun, LoggerPool::loggerUseCases, getManager().m_NarcoticsDM->getSearchIterator() );

    basar::appl::EventReturnStruct eventReturnStruct = findIncomingGoods();
    if (false == eventReturnStruct.isHandlerOK() ){
		return eventReturnStruct;
	}

	return findNumberOfIncomingGoods();;
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
SYSTEM_EVENT_HANDLER_DEFINITION( IncomingGoodsUC, SaveIncomingGoodsList )
{
    basar::ConstString fun = "IncomingGoodsUC::SaveIncomingGoodsList()";
    BLOG_TRACE( LoggerPool::loggerUseCases, fun );
    rSource = rSource; //if source not needed otherwise (e.g. for comparing); to avoid warnings

    ExecuteReturnType ret;

	bool save = false;
	if( ONLY_QUANTITY_INPUT == m_state )
	{
		// set changing state to only quantity input
		m_ChangingState = constants::ONLY_QTY_CHANGED;

		getManager().m_currentBooking = getManager().m_IncomingGoodsOverviewVC->getCurrentIncomingGood();
		Int32 qty;
		getManager().m_IncomingGoodsOverviewVC->getQtyFromCurrentRow( qty );

		if( qty == getManager().m_currentBooking.getInt32("qtytransaction") ){
			save = true;
		}
		else
		{
			save = ( basar::gui::tie::ButtonYes == basar::gui::tie::questionMsgBox(
								getManager().m_IncomingGoodsOverviewVC.getWidgetPtr(),
								QApplication::translate( "IncomingGoodsUC", "A different quantity was booked in the incoming goods. "
											"Do you want to save anyway? ").toLocal8Bit().data(),
											basar::gui::tie::ButtonYes | basar::gui::tie::ButtonNo,
											basar::gui::tie::ButtonNo) );
		}

		if( save )
		{
			getManager().m_NarcoticsDM->deepCopy( getManager().m_currentBooking );
			getManager().m_currentBooking.setInt32( "qtytransaction" , qty );
			getManager().m_currentBooking.setString( constants::PROCESSEDFLAG, constants::ISPROCESSED );

			ret = getManager().m_NarcoticsDM->saveBooking( getManager().m_currentBooking, true, true);
			if( ret.getError() == ExecuteReturnType::INVALID_VALUE )
			{
				Int32 quantitySave = basar::gui::tie::questionMsgBox(
									getManager().m_IncomingGoodsOverviewVC.getWidgetPtr(),
									QApplication::translate( "IncomingGoodsUC", "One parameter isn't valid. Do you want to store this action?").toLocal8Bit().data());

				rollbackWorkaroundSaving();

				if( quantitySave == basar::gui::tie::ButtonYes ){
					ret = getManager().m_NarcoticsDM->saveBooking( getManager().m_currentBooking, true, false);
				}
				else
				{
					getManager().m_currentBooking = getManager().m_IncomingGoodsOverviewVC->getCurrentIncomingGood();
					//getManager().m_IncomingGoodsOverviewVC->setQtyFromCurrentRow( 0 );
					return basar::appl::EventReturnStruct(basar::appl::HANDLER_ERROR);
				}
			}
			if( ret.getError() == ExecuteReturnType::NEGATIVE_QTY_IN_STOCK )
			{
				Int32 quantitySave = basar::gui::tie::questionMsgBox(
									getManager().m_IncomingGoodsOverviewVC.getWidgetPtr(),
									QApplication::translate( "IncomingGoodsUC", "Your last action has caused a negative quantity in stock! Do you want to store this action / quantity?").toLocal8Bit().data());

				rollbackWorkaroundSaving();

				if( quantitySave == basar::gui::tie::ButtonYes )
				{
					getManager().m_NarcoticsDM->saveBooking( getManager().m_currentBooking, false, false );
					newDisplayToSearch();
				}
				else
				{
					getManager().m_currentBooking = getManager().m_IncomingGoodsOverviewVC->getCurrentIncomingGood();
					//getManager().m_IncomingGoodsOverviewVC->setQtyFromCurrentRow( 0 );
					return basar::appl::EventReturnStruct(basar::appl::HANDLER_ERROR);
				}
			}
            //srk added to return the error
            if(ret.getError() == ExecuteReturnType::OUTDATED_DATE)
            {
              return  basar::appl::EventReturnStruct(basar::appl::HANDLER_ERROR,"OUTDATED_DATE");
            }
            else if(ret.getError() == ExecuteReturnType::ARTICLE_LOCKED)
            {
              return  basar::appl::EventReturnStruct(basar::appl::HANDLER_ERROR,"ARTICLE_LOCKED");
            }
            else
			{
				newDisplayToSearch();
			}

		}
		else
		{
			getManager().m_currentBooking = getManager().m_IncomingGoodsOverviewVC->getCurrentIncomingGood();
			//getManager().m_IncomingGoodsOverviewVC->setQtyFromCurrentRow( 0 );
			return basar::appl::EventReturnStruct(basar::appl::HANDLER_ERROR);
		}
	}
	else
	{
		if (narcotics::LoggerPool::loggerUseCases.isEnabledFor(log4cplus::TRACE_LOG_LEVEL))
		{
			basar::VarString msg = "current Booking Incoming Goods UC: \n ";
			for (basar::db::aspect::AccessorPropertyTable_XIterator xit = getManager().m_currentBooking.begin(); xit != getManager().m_currentBooking.end(); xit++)
			{
				if (!(xit.getState() == basar::SS_UNSET)) {
					msg = msg + xit.getName() + ":" + xit.getSQLString() + "\n";
				}
			}
			narcotics::LoggerPool::loggerUseCases.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__);
		}

		//! \todo	check if the getManager().m_currentBooking is set?!
		save = true;
		// find narcotic group specific data
		basar::appl::EventReturnStruct retStruct = findNarcoticGroupData();
		if( retStruct.ret == basar::appl::HANDLER_INFO ){
			return retStruct;
		}
		else{
			matchNarcoticGroupData();
		}

		getManager().m_NarcoticsDM->deepCopy( getManager().m_currentBooking );

		if (narcotics::LoggerPool::loggerUseCases.isEnabledFor(log4cplus::TRACE_LOG_LEVEL))
		{
			basar::VarString msg = "current Booking Incoming Goods UC: \n ";
			for (basar::db::aspect::AccessorPropertyTable_XIterator xit = getManager().m_currentBooking.begin(); xit != getManager().m_currentBooking.end(); xit++)
			{
				if (!(xit.getState() == basar::SS_UNSET)) {
					msg = msg + xit.getName() + ":" + xit.getSQLString() + "\n";
				}
			}
			narcotics::LoggerPool::loggerUseCases.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__);
		}

		getManager().m_IncomingGoodsOverviewVC->matchFromNewBooking( getManager().m_currentBooking );

		if (narcotics::LoggerPool::loggerUseCases.isEnabledFor(log4cplus::TRACE_LOG_LEVEL))
		{
			basar::VarString msg = "current Booking Incoming Goods UC: \n ";
			for (basar::db::aspect::AccessorPropertyTable_XIterator xit = getManager().m_currentBooking.begin(); xit != getManager().m_currentBooking.end(); xit++)
			{
				if (!(xit.getState() == basar::SS_UNSET)) {
					msg = msg + xit.getName() + ":" + xit.getSQLString() + "\n";
				}
			}
			narcotics::LoggerPool::loggerUseCases.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__);
		}

		getManager().m_currentBooking.setString( constants::PROCESSEDFLAG, constants::ISPROCESSED );

		ret = getManager().m_NarcoticsDM->saveBooking( getManager().m_currentBooking, true, true);
		if( ret.getError() == ExecuteReturnType::INVALID_VALUE )
		{
			Int32 quantitySave = basar::gui::tie::questionMsgBox(
								getManager().m_IncomingGoodsOverviewVC.getWidgetPtr(),
								QApplication::translate( "IncomingGoodsUC", "One parameter isn't valid. Do you want to store this action?").toLocal8Bit().data());

			rollbackWorkaroundSaving();

			if( quantitySave == basar::gui::tie::ButtonYes ){
				ret = getManager().m_NarcoticsDM->saveBooking( getManager().m_currentBooking, true, false );
			}
			else{
				return basar::appl::EventReturnStruct(basar::appl::HANDLER_ERROR);
			}
		}

		if (ret.getError() == ExecuteReturnType::OUTDATED_DATE)
		{
			basar::gui::tie::infoMsgBox(
				getManager().m_IncomingGoodsOverviewVC.getWidgetPtr(),
				QApplication::translate("IncomingGoodsUC", "The date is invalid and won't be saved.").toLocal8Bit().data());

			rollbackWorkaroundSaving();

			return basar::appl::EventReturnStruct(basar::appl::HANDLER_ERROR);
		}

		if( ret.getError() == ExecuteReturnType::NEGATIVE_QTY_IN_STOCK )
		{
			Int32 quantitySave = basar::gui::tie::questionMsgBox(
								getManager().m_IncomingGoodsOverviewVC.getWidgetPtr(),
								QApplication::translate( "IncomingGoodsUC", "Your last action has caused a negative quantity in stock! Do you want to store this action / quantity?").toLocal8Bit().data());

			rollbackWorkaroundSaving();

			if( quantitySave == basar::gui::tie::ButtonYes )
			{
				getManager().m_NarcoticsDM->saveBooking( getManager().m_currentBooking, false, false );
			}
			else
			{
				return basar::appl::EventReturnStruct(basar::appl::HANDLER_ERROR);
			}
		}
	}

	return basar::appl::EventReturnStruct(basar::appl::HANDLER_OK);
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
SYSTEM_EVENT_HANDLER_DEFINITION( IncomingGoodsUC, RegisteredIncomingGood )
{
    basar::ConstString fun = "IncomingGoodsUC::RegisteredIncomingGood()";
    BLOG_TRACE_METHOD( LoggerPool::loggerUseCases, fun );
    rSource = rSource; //if source not needed otherwise (e.g. for comparing); to avoid warnings

    basar::appl::EventReturnStruct eventReturnStruct;

	if( m_state == ONLY_QUANTITY_INPUT ){
		m_state = ALL_INPUT;
	}
	else
	{
        m_state = ONLY_QUANTITY_INPUT;
		eventReturnStruct = basar::appl::SystemEventManager::getInstance().fire( "SearchIncomingGoods" );
	}

	getManager().m_IncomingGoodsOverviewVC->switchNextBookingsCreation( false );

	return eventReturnStruct;
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
SYSTEM_EVENT_HANDLER_DEFINITION( IncomingGoodsUC, NewBookingIncomingGood )
{
    rSource = rSource; //if source not needed otherwise (e.g. for comparing); to avoid warnings

	if( LoggerPool::loggerUseCases.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "Event new caught in IncomingGoodsUC.";
		LoggerPool::loggerUseCases.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	// set changing state according to former user interaction
	m_ChangingState = constants::CREATE;
	// create new booking
	getManager().m_currentBooking = getManager().m_NarcoticsDM->addEmptyBooking();
	getManager().m_IncomingGoodsOverviewVC->matchToNewBooking( getManager().m_currentBooking );

	getManager().m_IncomingGoodsOverviewVC->switchNextBookingsCreation( true );

	return basar::appl::EventReturnStruct();
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
SYSTEM_EVENT_HANDLER_DEFINITION( IncomingGoodsUC, ReadArticleDataIncomingGood )
{
	rSource = rSource; //if source not needed otherwise (e.g. for comparing); to avoid warnings

	if( LoggerPool::loggerUseCases.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "Event read article data caught in IncomingGoodsUC.";
		LoggerPool::loggerUseCases.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	// ToDo: logical problem with data being immediately matched into currentBooking has to be solved
	// therefore a data iterator for matching and searching has been created (m_SearchIt)
	getManager().m_IncomingGoodsOverviewVC->matchFromNewBooking( m_SearchIt );
	// check if articleno is set
	if( m_SearchIt.getState( constants::ARTICLECODE ) == basar::SS_UNSET )
	{
		return basar::appl::EventReturnStruct();
	}
	AccessorPropertyTable_YIterator it = getManager().m_ArticleDM.findArticleByPattern( m_SearchIt );
	if( it.isEnd() )
	{
		return basar::appl::EventReturnStruct( basar::appl::HANDLER_INFO,
			QApplication::translate( "IncomingGoodsUC", "No records for the article number found!" ).toLocal8Bit().data() );
	}

	m_SearchIt.setInt32("articleno",	it.getInt32("articleno"));
	m_SearchIt.setString("articlecode",	it.getString("articlecode"));
	m_SearchIt.setString("articlename",	it.getString("articlename"));
	m_SearchIt.setString("pharmaform",	it.getString("pharmaform"));
	m_SearchIt.setString("packageunit",	it.getString("packageunit"));
	getManager().m_IncomingGoodsOverviewVC->matchToNewBooking( m_SearchIt );

	return basar::appl::EventReturnStruct();
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
SYSTEM_EVENT_HANDLER_DEFINITION( IncomingGoodsUC, ReadCustomerDataIncomingGood )
{
    rSource = rSource; //if source not needed otherwise (e.g. for comparing); to avoid warnings

	if( LoggerPool::loggerUseCases.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "Event read customer data caught in IncomingGoodsUC.";
		LoggerPool::loggerUseCases.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	// ToDo: logical problem with data being immediately matched into currentBooking has to be solved
	// therefore a search iterator for matching and searching is created (m_SearchIt)
	I18nString para;
	getManager().m_IncomingGoodsOverviewVC->matchFromNewBooking( m_SearchIt );
	// check if customersupplierno ist set
	if( m_SearchIt.getState(constants::CUSTOMERSUPPLIERNO) == basar::SS_UNSET ){
		return basar::appl::EventReturnStruct();
	}
	para.format("customerno=%d;", m_SearchIt.getInt32("customersupplierno"));
	AccessorPropertyTable_YIterator yitSearch = basar::db::aspect::Manager::getInstance().createAccessorYIterator( para );
	AccessorPropertyTable_YIterator it = getManager().m_CustomerDM->findCustomerByPattern( yitSearch );

	basar::I18nString msg = "";
	basar::appl::EventReturnEnum ret = basar::appl::HANDLER_OK;

	if( it.isEnd() )
	{
		m_SearchIt.setString("customersuppliername","");
		m_SearchIt.setString("customersupplierlocation","");
		m_SearchIt.setString("customersupplierstreet","");
		m_SearchIt.setString("customersuppliercip", "");

		ret = basar::appl::HANDLER_INFO;
		msg = QApplication::translate( "IncomingGoodsUC", "No records for customer number found!" ).toLocal8Bit().data();
	}
	else
	{
		m_SearchIt.setString("customersuppliername",it.getString("customername"));
		m_SearchIt.setString("customersupplierlocation",it.getString("customerlocation"));
		m_SearchIt.setString("customersupplierstreet", it.getString("street"));
		m_SearchIt.setString("customersuppliercip", it.getString("postcode"));

	}

	getManager().m_IncomingGoodsOverviewVC->matchToNewBooking( m_SearchIt );

	return basar::appl::EventReturnStruct( ret, msg.c_str() );
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
SYSTEM_EVENT_HANDLER_DEFINITION( IncomingGoodsUC, DeleteIncomingGoods )
{
    basar::ConstString fun = "IncomingGoodsOverviewVC::DeleteIncomingGoods()";
    BLOG_TRACE_METHOD( LoggerPool::loggerViewConn, fun );
    rSource = rSource; //if source not needed otherwise (e.g. for comparing); to avoid warnings

    basar::appl::EventReturnStruct result;

	getManager().m_currentBooking = getManager().m_IncomingGoodsOverviewVC->getCurrentIncomingGood();
    if( getManager().m_currentBooking.isNull() )
	{
        result.ret = basar::appl::HANDLER_NOT_FOUND;
        result.message = QApplication::translate( "IncomingGoodsUC",
                                                  "First an incoming good item has to be selected for deletion.").toLocal8Bit().data();
        return result;
    }

	getManager().m_NarcoticsDM->deepCopy( getManager().m_currentBooking );
    //magic number has been removed here: NEGATIVE_QTY_IN_STOCK	replaced -5
	if( ExecuteReturnType::NEGATIVE_QTY_IN_STOCK == ( getManager().m_NarcoticsDM->strikeOut( getManager().m_currentBooking, true ) ).getError() )
	{
		Int32 quantitySave = basar::gui::tie::questionMsgBox(
							getManager().m_IncomingGoodsOverviewVC.getWidgetPtr(),
							QApplication::translate( "IncomingGoodsUC", "Your last action has caused a negative quantity in stock! Do you want to store this action / quantity?").toLocal8Bit().data() );

		rollbackWorkaroundDeleting();

		if( quantitySave == basar::gui::tie::ButtonYes ){
			getManager().m_NarcoticsDM->strikeOut( getManager().m_currentBooking );
        }
	}
	newDisplayToSearch();

	return result;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////
basar::appl::EventReturnStruct IncomingGoodsUC::searchBookingsAndMatch()
{
	getManager().m_ResultsetSearch.setAdditionalSearchCriteriaNotProcessed();
	basar::db::aspect::AccessorPropertyTableRef resultTable1 = getManager().m_NarcoticsDM->findCatalogByPattern( getManager().m_NarcoticsDM->getSearchIterator() );
	basar::db::aspect::AccessorPropertyTableRef resultTable2 = getManager().m_NarcoticsDM->findNoIncomingGoods();

	if( resultTable1.empty() && !resultTable2.empty() )
	{
		getManager().m_IncomingGoodsOverviewVC->matchFromNoIncomingGoods();
		// message that no hits are found
		return basar::appl::EventReturnStruct( basar::appl::HANDLER_INFO,
											  QApplication::translate( "IncomingGoodsUC", "No incoming goods data found!" ).toLocal8Bit().data() );
	}
	if( !resultTable1.empty() && resultTable2.empty() )
	{
		getManager().m_IncomingGoodsOverviewVC->matchToIncomingGoods();
		// message that no hits are found
		return basar::appl::EventReturnStruct( basar::appl::HANDLER_INFO,
											  QApplication::translate( "IncomingGoodsUC", "No data for no incoming goods found!" ).toLocal8Bit().data() );
	}
	if( resultTable1.empty() && resultTable2.empty() )
	{
		// message that no hits are found
		return basar::appl::EventReturnStruct( basar::appl::HANDLER_INFO,
											  QApplication::translate( "IncomingGoodsUC", "No data for both incoming and no incoming goods found!" ).toLocal8Bit().data() );
	}

	getManager().m_IncomingGoodsOverviewVC->matchFromIncomingGoods();
	getManager().m_IncomingGoodsOverviewVC->matchFromNoIncomingGoods();

	return basar::appl::EventReturnStruct();
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
SYSTEM_EVENT_HANDLER_DEFINITION( IncomingGoodsUC, ReloadLastIncomingGoodsRequested )
{
    basar::ConstString fun = "IncomingGoodsUC::ReloadLastIncomingGoodsRequested()";
    BLOG_TRACE_METHOD( LoggerPool::loggerUseCases, fun );
    rSource;

    basar::appl::EventReturnStruct eventReturnStruct = findIncomingGoods();
    if( false == eventReturnStruct.isHandlerOK() ){
		return eventReturnStruct;
	}

    return findNumberOfIncomingGoods();
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
SYSTEM_EVENT_HANDLER_DEFINITION( IncomingGoodsUC, AllUnprocessedIncomingGoodsRequested )
{
    basar::ConstString fun = "IncomingGoodsUC::AllUnprocessedIncomingGoodsRequested()";
    BLOG_TRACE_METHOD( LoggerPool::loggerUseCases, fun );
    rSource;

    basar::appl::EventReturnStruct eventReturnStruct = findUnprocessedIncomingGoods();
    if( false == eventReturnStruct.isHandlerOK() ){
		return eventReturnStruct;
	}

    return findNumberOfIncomingGoods();
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
SYSTEM_EVENT_HANDLER_DEFINITION( IncomingGoodsUC, SetCurrentIteratorIncomingGoods )
{
	basar::ConstString fun = "IncomingGoodsOverviewVC::SetCurrentIteratorIncomingGoods()";
    BLOG_TRACE_METHOD( LoggerPool::loggerViewConn, fun );
    rSource = rSource; //if source not needed otherwise (e.g. for comparing); to avoid warnings

	// set changing state according to former user interaction
	m_ChangingState = constants::EDIT;
	// get current booking iterator for goods in transaction and set it to getManager().m_currentBooking
	getManager().m_currentBooking = getManager().m_IncomingGoodsOverviewVC->getCurrentIncomingGood();

	return basar::appl::EventReturnStruct();
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
 basar::appl::EventReturnStruct IncomingGoodsUC::preEvaluateSupplierReplacement()
{
	basar::ConstString fun = "IncomingGoodsOverviewVC::preEvaluateSupplierReplacement()";
    BLOG_TRACE_METHOD( LoggerPool::loggerViewConn, fun );

    basar::appl::EventReturnStruct eventReturnStruct;

    if( false == getManager().m_NarcoticsDM->isSupplierReplaceable() )
	{
        eventReturnStruct.ret = basar::appl::HANDLER_ERROR;
        eventReturnStruct.message = QApplication::translate( "IncomingGoodsUC",
                                                             "The current supplier could not be replaced."
                                                           ).toLocal8Bit().data();
        return eventReturnStruct;
    }

    AccessorPropertyTable_YIterator yitSupplierReplacement = getManager().m_IncomingGoodsOverviewVC->getCurrentAlternativeSupplier();
    if( yitSupplierReplacement.isNull() )
	{
        eventReturnStruct.ret = basar::appl::HANDLER_ERROR;
        eventReturnStruct.message = QApplication::translate( "IncomingGoodsUC",
                                                             "There is no supplier selected for replacement."
                                                           ).toLocal8Bit().data();
        return eventReturnStruct;
    }

	return eventReturnStruct;
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
basar::appl::EventReturnStruct IncomingGoodsUC::postEvaluateSupplierReplacement()
{
	basar::ConstString fun = "IncomingGoodsOverviewVC::postEvaluateSupplierReplacement()";
    BLOG_TRACE_METHOD( LoggerPool::loggerViewConn, fun );

    basar::appl::EventReturnStruct eventReturnStruct;

    AccessorPropertyTableRef catalogPropTab =
        getManager().m_NarcoticsDM->findCatalogByPattern( getManager().m_NarcoticsDM->getSearchIterator() );
    if( catalogPropTab.isNull() )
	{
        eventReturnStruct.ret     = basar::appl::HANDLER_ERROR;
        eventReturnStruct.message = QApplication::translate( "IncomingGoodsUC",
                                                             "Error with finding narcotics transaction "
                                                             "datasets after supplier replacement!"
                                                           ).toLocal8Bit().data();
        return eventReturnStruct;
    }

	if( catalogPropTab.empty() )
	{
        eventReturnStruct.ret     = basar::appl::HANDLER_INFO;
        eventReturnStruct.message = QApplication::translate( "IncomingGoodsUC",
                                                             "No narcotics transaction datasets found "
                                                             "after supplier replacement."
                                                           ).toLocal8Bit().data();
    }

    getManager().m_IncomingGoodsOverviewVC->matchFromIncomingGoods();
 	getManager().m_IncomingGoodsOverviewVC->matchToIncomingGoods();
	getManager().m_NarcoticsDM->findNoIncomingGoods();
	getManager().m_IncomingGoodsOverviewVC->matchFromNoIncomingGoods();
	getManager().m_IncomingGoodsOverviewVC->switchSupplierReplacement( getManager().m_NarcoticsDM->isSupplierReplaceable() );

	return eventReturnStruct;
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
SYSTEM_EVENT_HANDLER_DEFINITION( IncomingGoodsUC, ReplaceSupplier )
{
	basar::ConstString fun = "IncomingGoodsOverviewVC::ReplaceSupplier()";
    BLOG_TRACE_METHOD( LoggerPool::loggerViewConn, fun );
    rSource;

    basar::appl::EventReturnStruct eventReturnStruct = preEvaluateSupplierReplacement();
    if( false == eventReturnStruct.isHandlerOK() ){
		return eventReturnStruct;
	}

	AccessorPropertyTable_YIterator yitSupplierReplacement = getManager().m_IncomingGoodsOverviewVC->getCurrentAlternativeSupplier();
	basar::I18nString orderNovalue;
	orderNovalue.itos( getManager().m_NarcoticsDM->getAccNarCatalogPropTab().begin().getInt32( constants::PURCHASEORDERNO ) );
	const ExecuteReturnType executeReturnType =
        getManager().m_NarcoticsDM->saveSupplier( orderNovalue, yitSupplierReplacement.getInt32( constants::NARCOTICSSUPPLIERNO ) );

    if( executeReturnType.hasError() )
	{
        eventReturnStruct.ret = basar::appl::HANDLER_ERROR;
        eventReturnStruct.message = QApplication::translate( "IncomingGoodsUC",
                                                             "Error with saving the replaced supplier."
                                                           ).toLocal8Bit().data();
        return eventReturnStruct;
    }

    return postEvaluateSupplierReplacement();
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
void IncomingGoodsUC::newDisplayToSearch()
{
	getManager().m_NarcoticsDM->findCatalogByPattern( getManager().m_NarcoticsDM->getSearchIterator() );
 	getManager().m_IncomingGoodsOverviewVC->matchFromIncomingGoods();
	getManager().m_NarcoticsDM->findNoIncomingGoods();
	getManager().m_IncomingGoodsOverviewVC->matchFromNoIncomingGoods();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////
basar::appl::EventReturnStruct IncomingGoodsUC::findNarcoticGroupData()
{
	if( ( m_SearchIt.getState( constants::ARTICLENO ) ).getState() != basar::SS_UNSET )
	{
		basar::Int32 articleNo = m_SearchIt.getInt32( constants::ARTICLENO );
		basar::db::aspect::AccessorPropertyTable_YIterator yitExecute
			= getManager().m_NarcoticsGroupDM->findStockLocationByArticleNo( articleNo,
			basar::login::Manager::getInstance().getAreaID() );

		if( yitExecute.isEnd() )
		{
			// message box that no hits are found
			std::ostringstream os;
			os	<< QApplication::translate("IncomingGoodsUC", "Narcotic group data for article number ").toLocal8Bit().data()
				<< articleNo
				<< QApplication::translate("IncomingGoodsUC", " not found!").toLocal8Bit().data()
				<< QApplication::translate("IncomingGoodsUC", " Consequently saving the current record is not possible!").toLocal8Bit().data();
			return basar::appl::EventReturnStruct( basar::appl::HANDLER_INFO, os.str().c_str() );
		}
		return basar::appl::EventReturnStruct();
	}
	else if( ( getManager().m_currentBooking.getState( constants::ARTICLENO ) ).getState() != basar::SS_UNSET )
	{
		basar::Int32 articleNo = getManager().m_currentBooking.getInt32( constants::ARTICLENO );
		basar::db::aspect::AccessorPropertyTable_YIterator yitExecute
			= getManager().m_NarcoticsGroupDM->findStockLocationByArticleNo( articleNo,
			basar::login::Manager::getInstance().getAreaID() );

		if( yitExecute.isEnd() )
		{
			// message box that no hits are found
			std::ostringstream os;
			os	<< QApplication::translate("IncomingGoodsUC", "Narcotic group data for article number ").toLocal8Bit().data()
				<< articleNo
				<< QApplication::translate("IncomingGoodsUC", " not found!").toLocal8Bit().data()
				<< QApplication::translate("IncomingGoodsUC", " Consequently saving the current record is not possible!").toLocal8Bit().data();
			return basar::appl::EventReturnStruct( basar::appl::HANDLER_INFO, os.str().c_str() );
		}
		return basar::appl::EventReturnStruct();
	}
	// message box that article no is not set
		std::ostringstream os;
		os	<< QApplication::translate("IncomingGoodsUC", "Article number ").toLocal8Bit().data()
			<< QApplication::translate("IncomingGoodsUC", "hasn't been set!").toLocal8Bit().data();
		return basar::appl::EventReturnStruct( basar::appl::HANDLER_INFO, os.str().c_str() );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////
void IncomingGoodsUC::matchNarcoticGroupData()
{
	basar::db::aspect::AccessorPropertyTable_YIterator yitExecute
		= getManager().m_NarcoticsGroupDM->getAccNarCatalogPropTab().begin();

	getManager().m_currentBooking.setString(	constants::NARCGROUP,	yitExecute.getString(constants::GROUP)			);
	getManager().m_currentBooking.setPropertyState( constants::NARCGROUP, basar::SS_UPDATE );
	getManager().m_currentBooking.setString(	constants::NARCNAME,	yitExecute.getString(constants::NARCOTICNAME)	);
	getManager().m_currentBooking.setPropertyState( constants::NARCNAME, basar::SS_UPDATE );
	getManager().m_currentBooking.setString(	constants::NARCUNIT,	yitExecute.getString(constants::PACKAGEUNIT)	);
	getManager().m_currentBooking.setPropertyState( constants::NARCUNIT, basar::SS_UPDATE );
	getManager().m_currentBooking.setDecimal(	constants::NARCPOTENCY,	yitExecute.getDecimal(constants::CONCENTRATION)	);
	getManager().m_currentBooking.setPropertyState( constants::NARCPOTENCY, basar::SS_UPDATE );
	getManager().m_currentBooking.setDecimal(	constants::NARCFACTOR,	yitExecute.getDecimal(constants::NARCFACTOR)	);
	getManager().m_currentBooking.setPropertyState( constants::NARCFACTOR, basar::SS_UPDATE );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//! \no retval
/*!	\throw	no-throw */
/*! Workaround: clear m_currentBooking and match again, as iterator might have been destroyed due to rollback problems */
/////////////////////////////////////////////////////////////////////////////////////////////////////
void IncomingGoodsUC::rollbackWorkaroundDeleting()
{
	if( LoggerPool::loggerUseCases.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "Event rollbackWorkaroundDeleting caught in IncomingGoodsUC.";
		LoggerPool::loggerUseCases.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	// clear iterator
	getManager().m_currentBooking = basar::db::aspect::AccessorPropertyTable_YIterator();
	// reinitialize m_currentBooking
	getManager().m_currentBooking = getManager().m_IncomingGoodsOverviewVC->getCurrentIncomingGood();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//! \no retval
/*!	\throw	no-throw */
/*! Workaround: clear m_currentBooking and match again, as iterator might have been destroyed due to rollback problems */
/////////////////////////////////////////////////////////////////////////////////////////////////////
void IncomingGoodsUC::rollbackWorkaroundSaving()
{
	if( LoggerPool::loggerUseCases.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) ){
		basar::ConstString msg = "Event rollbackWorkaroundSaving caught in IncomingGoodsUC.";
		LoggerPool::loggerUseCases.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	// clear iterator
	getManager().m_currentBooking = basar::db::aspect::AccessorPropertyTable_YIterator();
	// reinitialize m_currentBooking
	// it has to be differentiated whether a new booking shall be created or an already existing one edited or if only the entered booking quantity has been changed
	if( m_ChangingState == constants::CREATE )
	{
		getManager().m_currentBooking = getManager().m_NarcoticsDM->addEmptyBooking();
		// refill iterator with recommended values
		getManager().m_IncomingGoodsOverviewVC->matchFromNewBooking( getManager().m_currentBooking );
		matchNarcoticGroupData();
	}
	if( m_ChangingState == constants::EDIT )
	{
		getManager().m_currentBooking = getManager().m_IncomingGoodsOverviewVC->getCurrentIncomingGood();
		// refill iterator with recommended values
		getManager().m_IncomingGoodsOverviewVC->matchFromNewBooking( getManager().m_currentBooking );
		matchNarcoticGroupData();
	}
	if( m_ChangingState == constants::ONLY_QTY_CHANGED )
	{
		getManager().m_currentBooking = getManager().m_IncomingGoodsOverviewVC->getCurrentIncomingGood();
		basar::Int32 qty;
		getManager().m_IncomingGoodsOverviewVC->getQtyFromCurrentRow( qty );
		getManager().m_currentBooking.setInt32( "qtytransaction" , qty );
	}
	getManager().m_currentBooking.setString( constants::PROCESSEDFLAG, constants::ISPROCESSED );
}

} //namespace useCase
} //namespace narcotics