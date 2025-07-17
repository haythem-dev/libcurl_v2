//-------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  implementation of use case "narcotics protocol"
 *  \author	Kerstin Reuther
 *  \date   23.03.2006
 */
//-------------------------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------------------------//
// includes
//-------------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "narcoticsprotocol.h"
#include "narcoticsappl.h"
#include "protocolprintdata.h"
#include "loggerpool.h"

//-------------------------------------------------------------------------------------------------//
// namespaces
//-------------------------------------------------------------------------------------------------//
namespace narcotics
{
namespace useCase 
{
//--------------------------------------------------------------------------------------------------------------//
//! Constructor of the Use Case "Narcotics Protocol".
/*! throw no-throw. */
//--------------------------------------------------------------------------------------------------------------//
NarcoticsProtocolUC::NarcoticsProtocolUC()
{
}

//--------------------------------------------------------------------------------------------------------------//
//! Destructor of the Use Case "Narcotics Protocol".
/*! throw no-throw. */
//--------------------------------------------------------------------------------------------------------------//
NarcoticsProtocolUC::~NarcoticsProtocolUC()
{
}

//-------------------------------------------------------------------------------------------------//
// macro for registrating and deregistrating EventHandlers!
// 1st para: name of UC
// 2nd para: name of handle-Method
BEGIN_HANDLERS_REGISTRATION( NarcoticsProtocolUC )
	SYSTEM_EVENT_HANDLER_REGISTRATION( NarcoticsProtocolUC, SearchProtocol  )
	SYSTEM_EVENT_HANDLER_REGISTRATION( NarcoticsProtocolUC, SearchCatalog	)
	SYSTEM_EVENT_HANDLER_REGISTRATION( NarcoticsProtocolUC, PrintProtocol	)
END_HANDLERS_REGISTRATION()

//--------------------------------------------------------------------------------------------------------------//
//! Handle Function for Searching in the table narcprotocol.
/*! Setting the Search Iterator and calling Function from Domain Module that executes the Select.
	Afterwards Matching the result to the GuiPropertyTable. throw no-throw. */
//--------------------------------------------------------------------------------------------------------------//
SYSTEM_EVENT_HANDLER_DEFINITION( NarcoticsProtocolUC, SearchProtocol )
{
	rSource = rSource; //if source not needed otherwise (e.g. for comparing); to avoid warnings

	if( LoggerPool::loggerUseCases.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "Event search protocol caught in NarcoticsProtocolUC.";
		LoggerPool::loggerUseCases.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	basar::db::aspect::AccessorPropertyTable_YIterator currentIt = getManager().m_NarcoticsProtocolVC->getCatalogId();
	basar::VarString txt;

	if( currentIt.isContainedAndSet( constants::CATALOGID ) )
	{
		txt.format( "%ld", currentIt.getInt32( constants::CATALOGID ) );
		getManager().m_LogDM->getSearchIterator().setString( constants::CATALOGID, txt );
	}
	else{
		getManager().m_LogDM->getSearchIterator().setString( constants::CATALOGID, "" );
	}
	
	basar::db::aspect::AccessorPropertyTableRef table = getManager().m_LogDM->findLogEntryByPattern( getManager().m_LogDM->getSearchIterator() );

	getManager().m_NarcoticsProtocolVC->matchToProtocol();

	return basar::appl::EventReturnStruct();
}

//--------------------------------------------------------------------------------------------------------------//
//! Handle Function for Searching in the table narctransactioncatalog.
/*! Setting the Search Iterator with the values that the user has set in the Gui and calling Function from Domain Module 
	that executes the Select. Showing Message Box for the user if there are no results.
	Afterwards Matching the result to the GuiPropertyTable. throw no-throw. */
//--------------------------------------------------------------------------------------------------------------//
SYSTEM_EVENT_HANDLER_DEFINITION( NarcoticsProtocolUC, SearchCatalog )
{
	rSource = rSource; //if source not needed otherwise (e.g. for comparing); to avoid warnings

	if( LoggerPool::loggerUseCases.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "Event search catalog caught in NarcoticsProtocolUC.";
		LoggerPool::loggerUseCases.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	getManager().m_LogDM->clearSearchIterator();
	getManager().m_NarcoticsProtocolVC->matchFromFirstSelection( getManager().m_LogDM->getSearchIterator() );

	// Suche PZN zu Code, da diese vorher nicht im yit stehen kann!
	// read article info in iterator
	if( getManager().m_LogDM->getSearchIterator().isContainedAndSet( constants::ARTICLECODE ) )
	{
		getManager().m_LogDM->getSearchIterator().setString( constants::ARTICLECODE, 
			removeLeadingZeros( getManager().m_LogDM->getSearchIterator().getString( constants::ARTICLECODE ) ) );
		getManager().m_ArticleDM.init( basar::login::Manager::getInstance().getAreaID(), getManager().m_Connection.getCurrentConnection() );
		basar::db::aspect::AccessorPropertyTable_YIterator yitArticle = 
			getManager().m_ArticleDM.findArticleByPattern( getManager().m_LogDM->getSearchIterator() );

		if( yitArticle.isEnd() )
		{
			getManager().m_ArticleDM.shutdown();
			getManager().m_LogDM->getAccNarTransactionPropTab().clear();
			return basar::appl::EventReturnStruct( basar::appl::HANDLER_INFO, 
													QApplication::translate( "GoodsTransactionUC", "Given articleno is not valid!" ).toLocal8Bit().data() );
		}
		else
		{
			getManager().m_LogDM->getSearchIterator().setString( constants::ARTICLENO, yitArticle.getSQLString( constants::ARTICLENO ) );
			getManager().m_ArticleDM.shutdown();
		}
		
	}

	basar::db::aspect::AccessorPropertyTableRef tab = getManager().m_LogDM->findCatalogByPattern( getManager().m_LogDM->getSearchIterator() );

	basar::I18nString msg = "";
	basar::appl::EventReturnEnum ret = basar::appl::HANDLER_OK; 

	if( tab.empty() )
	{
		ret = basar::appl::HANDLER_INFO;
		msg = QApplication::translate( "NarcoticsProtocolUC", "No records for search criteria found!" ).toLocal8Bit().data();
	}
	else{
		getManager().m_NarcoticsProtocolVC->matchToCatalog();
	}

	return basar::appl::EventReturnStruct( ret, msg );
}

//--------------------------------------------------------------------------------------------------------------//
//! Handle Function for printing from table narcprotocol.
/*! Get Search Iterator and send selected data to print class.
	\n throw no-throw. */
//--------------------------------------------------------------------------------------------------------------//
SYSTEM_EVENT_HANDLER_DEFINITION( NarcoticsProtocolUC, PrintProtocol )
{
	rSource = rSource; //if source not needed otherwise (e.g. for comparing); to avoid warnings

	if( LoggerPool::loggerUseCases.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "Event print caught in NarcoticsProtocolUC.";
		LoggerPool::loggerUseCases.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	getManager().m_LogDM->clearSearchIterator();
	getManager().m_NarcoticsProtocolVC->matchFromFirstSelection( getManager().m_LogDM->getSearchIterator() );
	
	basar::db::aspect::AccessorPropertyTable_YIterator yit = 
		getManager().m_LogDM->getSearchIterator();

	if( yit.getState(constants::TODATE)   == basar::SS_UNSET ||
		yit.getState(constants::FROMDATE) == basar::SS_UNSET )
	{
		return basar::appl::EventReturnType( basar::appl::HANDLER_INFO,
											QApplication::translate( "BookingCatalogUC", "please put in from- and to-date before printing protocol!" ).toLocal8Bit().data()	);
	}

	// Suche PZN zu Code, da diese vorher nicht im yit stehen kann!
	// read article info in iterator
	if( yit.isContainedAndSet( constants::ARTICLECODE ) )
	{
		yit.setString( constants::ARTICLECODE, removeLeadingZeros( yit.getString( constants::ARTICLECODE ) ) );
		getManager().m_ArticleDM.init(	basar::login::Manager::getInstance().getAreaID(), getManager().m_Connection.getCurrentConnection() );
		basar::db::aspect::AccessorPropertyTable_YIterator yitArticle = getManager().m_ArticleDM.findArticleByPattern( yit );

		if( yitArticle.isEnd() )
		{
			return basar::appl::EventReturnStruct( basar::appl::HANDLER_INFO, 
													QApplication::translate( "GoodsTransactionUC", "Given articleno is not valid!" ).toLocal8Bit().data() );
		}
		else{
			yit.setString( constants::ARTICLENO, yitArticle.getSQLString( constants::ARTICLENO ) );
		}
		getManager().m_ArticleDM.shutdown();
	}

	basar::db::aspect::AccessorPropertyTableRef prTabChangedBookings = 
		getManager().m_LogDM->findCatalogByPattern( yit );

	//collection of propertytables (one for each changed booking)
	CollOfBookingLogs collLogs;

	basar::VarString catId;

	basar::db::aspect::AccessorPropertyTable_YIterator yitBooking = prTabChangedBookings.begin();
	while( !yitBooking.isEnd() )
	{
		catId.format( "%ld", yitBooking.getInt32( constants::CATALOGID ) );
		yit.setString( constants::CATALOGID, catId );
		
		basar::db::aspect::AccessorPropertyTableRef prTabLogs = 
				getManager().m_LogDM->findLogsByPattern( getManager().m_Connection.getCurrentConnection(), yit );
				
		collLogs.insert( CollOfNarcArtBookings::value_type( yitBooking.getInt32( constants::CATALOGID ), prTabLogs ) );
								
		yitBooking++;	
	}

	if( prTabChangedBookings.empty() )
	{
		return basar::appl::EventReturnType(basar::appl::HANDLER_INFO,
											QApplication::translate( "BookingCatalogUC", "No Bookings found which have logged changes!" ).toLocal8Bit().data() );
	}

	basar::VarString	date;
	basar::DateTime		period;
	basar::Long32		help;
	help = atoi( yit.getString( constants::FROMDATE ).c_str() );
	period.setDate( help );
	date = period.toStrDate();
	date.append(" - ");
	help = atoi( yit.getString( constants::TODATE ).c_str() );
	period.setDate( help );
	date.append( period.toStrDate() );
	
	ProtocolPrintData printData( "NarcProtocol", "NarcProtocol", prTabChangedBookings, &collLogs, date );
	basar::gui::qt::PrintPreview preview( printData, 0, 
					QApplication::translate( "NarcoticsProtocolUC", "Narcotics - Protocol" ).toLocal8Bit().data() );
	
	preview.setOutputFormats( basar::gui::qt::PDF );
	
	preview.show();

	collLogs.clear();
	getManager().m_LogDM->resetAccPrintProtList();
	return basar::appl::EventReturnType();
}

//--------------------------------------------------------------------------------------------------------------//
//! Function for running the Use Case "Narcotics Protocol".
/*! Calls the Functions init() and show() from the View Connector. throw no-throw. */
//--------------------------------------------------------------------------------------------------------------//
basar::appl::EventReturnType NarcoticsProtocolUC::run()
{
	registerEventHandlers(true);

	// initialization of the modules
	getManager().m_LogDM->init(	getManager().m_Connection.getCurrentConnection(), 
								basar::login::Manager::getInstance().getAreaID(),
								basar::login::Manager::getInstance().getUserName() );
	
	// initialization of the view connector(s)/dialogs
	getManager().m_NarcoticsProtocolVC->init	( getManager().m_LogDM->getAccNarProtocolPropTab(), 
												  getManager().m_LogDM->getAccNarTransactionPropTab() );	
	getManager().m_NarcoticsProtocolVC->show();
	
	getManager().m_NarcoticsProtocolVC->shutdown();
	getManager().m_NarcoticsProtocolVC.reset();
	
	getManager().m_currentBooking.clear();
	getManager().m_LogDM->shutdown();

	registerEventHandlers( false );

	return basar::appl::EventReturnStruct(); //std with ret = OK, msg = ""
}

} //namespace useCase
} //namespace narcotics