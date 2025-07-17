//-----------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  shows missing acknowledgements and creates delivery note copies
 *  \author Benjamin Endlicher
 *  \date   14.09.2010
 */
//-----------------------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------------------------//
// includes
//-------------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "contributionvoucher.h"
#include "narcoticsappl.h"
#include "deliverynotecopyprintdata.h"
#include "deliverynotecopyftptransfer.h"
#include "loggerpool.h"
#include "receiptnoticeprintdata.h"
#include "constants.h"
#include "contributionvoucherftptransfer.h"
#include "printfolderlocator.h"

#include "na18deliverynotecopyvc.h"

#pragma warning (push) // to avoid warnings from QT always put QT-includes between push - pop
#pragma warning(disable: 4127 4244 4311 4312 4481 4512 4800)
#include <QtCore/QUrl>
#include <QtGui/QDesktopServices>
#pragma warning (pop)

//services
#include "servicelocator.h"
#include "iprintdeliverynoteptr.h"
#include "iprintdeliverynote.h"
//exceptions
#include "missingdependencyinjectionexception.h"
#include "missingdataforprintingexception.h"

#include "acknowledgementlistprintdata.h"

//-------------------------------------------------------------------------------------------------//
// namespaces
//-------------------------------------------------------------------------------------------------//
namespace narcotics 
{
namespace useCase 
{

using basar::db::aspect::AccessorPropertyTable_YIterator;
using basar::db::aspect::AccessorPropertyTableRef;

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
ContributionVoucherUC::ContributionVoucherUC() : UseCaseController()
{
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
ContributionVoucherUC::~ContributionVoucherUC()
{
}

//-------------------------------------------------------------------------------------------------//
// macro for registrating and deregistrating EventHandlers!
// 1st para: name of UC
// 2nd para: name of handle-Method
BEGIN_HANDLERS_REGISTRATION(ContributionVoucherUC)
	SYSTEM_EVENT_HANDLER_REGISTRATION( ContributionVoucherUC, ReprintDeliveryNote					)
	SYSTEM_EVENT_HANDLER_REGISTRATION( ContributionVoucherUC, PrintDeliveryNoteCopy					)
	SYSTEM_EVENT_HANDLER_REGISTRATION( ContributionVoucherUC, MarkAsReceived						)
	SYSTEM_EVENT_HANDLER_REGISTRATION( ContributionVoucherUC, MarkAsPrinted							)
	SYSTEM_EVENT_HANDLER_REGISTRATION( ContributionVoucherUC, OpenEmptyDeliveryNote					)
	SYSTEM_EVENT_HANDLER_REGISTRATION( ContributionVoucherUC, OpenEmptyContributionVoucherDelCopy	)
	SYSTEM_EVENT_HANDLER_REGISTRATION( ContributionVoucherUC, SearchAcknowledgements				)
	SYSTEM_EVENT_HANDLER_REGISTRATION( ContributionVoucherUC, RefreshAcknowledgement				)
    SYSTEM_EVENT_HANDLER_REGISTRATION( ContributionVoucherUC, PrintAcknowledgementList				)
END_HANDLERS_REGISTRATION()

//-------------------------------------------------------------------------------------------------//
void ContributionVoucherUC::injectParentWindow( QWidget * mw )
{
    m_ContributionVoucherVC.setParent(mw);	
}
	
//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
basar::appl::EventReturnType ContributionVoucherUC::run()
{
	registerEventHandlers( true );

	narcotic::useCase::services::PrintFolderLocator printFolderLocator;
	m_PrintFolderPath = printFolderLocator.getPrintFolderPath();
	
	m_ContributionVoucherVC->init(m_ContributionVoucherDM->getAccNoAcknowledgementPropTab());
	basar::appl::SystemEventManager::getInstance().fire("RefreshAcknowledgement");
	
	m_ContributionVoucherVC->show();
	m_ContributionVoucherVC->shutdown();
	m_ContributionVoucherVC.reset();

	// clear search prop tab
	m_SearchDM->clearSearchPropTab();
	
	registerEventHandlers( false );

	return basar::appl::EventReturnStruct();
}

//-------------------------------------------------------------------------------------------------//
void ContributionVoucherUC::injectBranchDM( boost::shared_ptr<domMod::BranchDM> branchDM )
{
    m_BranchDM = branchDM;
}

//-------------------------------------------------------------------------------------------------//
void ContributionVoucherUC::injectCustomerDM( boost::shared_ptr<domMod::CustomerDM> customerDM )
{
    m_CustomerDM = customerDM;
}

//-------------------------------------------------------------------------------------------------//
void ContributionVoucherUC::injectContributionVoucherDM( boost::shared_ptr<domMod::ContributionVoucherDM> contributionVoucherDM )
{
    m_ContributionVoucherDM = contributionVoucherDM;
}

//-------------------------------------------------------------------------------------------------//
void ContributionVoucherUC::injectNarcoticsDM( boost::shared_ptr<domMod::NarcoticsDM> narcoticsDM )
{
    m_NarcoticsDM = narcoticsDM;
}

//-------------------------------------------------------------------------------------------------//
void ContributionVoucherUC::injectSearchDM( boost::shared_ptr<domMod::SearchDM> searchDM )
{
    m_SearchDM = searchDM;
}

//-------------------------------------------------------------------------------------------------//
void ContributionVoucherUC::injectServiceLocator( infrastructure::ServiceLocatorPtr sp )
{
    m_ServiceLocatorPtr = sp;
}

//-------------------------------------------------------------------------------------------------//
void ContributionVoucherUC::checkInjections()
{
    if( 0 == m_BranchDM.get() ){
        throw infrastructure::MissingDependencyInjectionException( basar::ExceptInfo("ContributionVoucherUC::checkInjections", "BranchDM missing! Inject BranchDM first!", __FILE__, __LINE__) );
    }
    if( 0 == m_NarcoticsDM.get() ){
        throw infrastructure::MissingDependencyInjectionException( basar::ExceptInfo("ContributionVoucherUC::checkInjections", "NarcoticsDM missing! Inject NarcoticsDM first!", __FILE__, __LINE__) );
    }
    if( 0 == m_SearchDM.get() ){
        throw infrastructure::MissingDependencyInjectionException( basar::ExceptInfo("ContributionVoucherUC::checkInjections", "SearchDM missing! Inject SearchDM first!", __FILE__, __LINE__) );
    }
    if( 0 == m_CustomerDM.get() ){
        throw infrastructure::MissingDependencyInjectionException( basar::ExceptInfo("ContributionVoucherUC::checkInjections", "CustomerDM missing! Inject CustomerDM first!", __FILE__, __LINE__) );
    }
    if( 0 == m_ContributionVoucherDM.get() ){
        throw infrastructure::MissingDependencyInjectionException( basar::ExceptInfo("ContributionVoucherUC::checkInjections", "ContributionVoucherDM missing! Inject ContributionVoucherDM first!", __FILE__, __LINE__) );
    }
    if( 0 == m_ServiceLocatorPtr.get() ){
        throw infrastructure::MissingDependencyInjectionException( basar::ExceptInfo("ContributionVoucherUC::checkInjections", "ServiceLocatorPtr missing! Inject ServiceLocatorPtr first!", __FILE__, __LINE__) );
    }
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
SYSTEM_EVENT_HANDLER_DEFINITION( ContributionVoucherUC, PrintDeliveryNoteCopy )
{
	BLOG_TRACE_METHOD(LoggerPool::loggerUseCases, "ContributionVoucherUC::PrintDeliveryNoteCopy");

	rSource = rSource; //if source not needed otherwise (e.g. for comparing); to avoid warnings

	basar::appl::EventReturnStruct ret;

	// get selected order from contributionvouchervc
	basar::db::aspect::AccessorPropertyTable_YIterator deliveryNoteIt = m_ContributionVoucherVC->getFirstSelectedContributionVoucher();
	
	if( deliveryNoteIt.isNull() || deliveryNoteIt.isEnd() )
	{
		ret.ret = basar::appl::HANDLER_INFO;
		ret.message = QApplication::translate( "ContributionVoucherUC", 
											   "No selected delivery found!!" ).toLocal8Bit().data();
		return ret;
	}
	
	LOG_YIT("his",narcotics::LoggerPool::loggerNarcotics,deliveryNoteIt);
	
	basar::Int16 acknowledgementStatus = deliveryNoteIt.getInt16(constants::ACKNOWLEDGEMENTSTATUS);
	
	//deliverynotecopy has already been transfered!! no second creation!!
    if( acknowledgementStatus == constants::DELIVERY_NOTE_COPY_CREATED )
	{
		ret.ret = basar::appl::HANDLER_INFO;
		ret.message = QApplication::translate( "ContributionVoucherUC", 
											   "Deliverynotecopy has already been created and transmitted!! No second transmission allowed!" ).toLocal8Bit().data();
		return ret;
	}
	
	if( constants::ACKNOWLEDGEMENT_RECEIVED != acknowledgementStatus && constants::PRINTED != acknowledgementStatus )
	{
		ret.ret = basar::appl::HANDLER_INFO;
		ret.message = QApplication::translate( "ContributionVoucherUC", 
											   "Voucher is not in status printed / received!" ).toLocal8Bit().data();
		return ret;
	}
	
	// get branch data
	basar::db::aspect::AccessorPropertyTable_YIterator yitBranch = m_BranchDM->findBranch();

	// get customer data
	basar::db::aspect::AccessorPropertyTable_YIterator yitCustomerSearch;
	m_CustomerDM->getAccCustomerPropTab().clear();
	yitCustomerSearch = m_CustomerDM->getAccCustomerPropTab().insert(basar::FOR_CLEAN);
	yitCustomerSearch.setInt32(constants::CUSTOMERNO,	deliveryNoteIt.getInt32(constants::CUSTOMERSUPPLIERNO));
	yitCustomerSearch.setInt16(constants::BRANCHNO,		deliveryNoteIt.getInt16(constants::BRANCHNO));
	basar::db::aspect::AccessorPropertyTable_YIterator yitCustomerSupplier = m_CustomerDM->findCustomerByPattern(yitCustomerSearch);
	// get supplier data
	if( yitCustomerSupplier.isEnd() )
	{
		yitCustomerSupplier = m_NarcoticsDM->findNarcoticsSupplierByBgaNo( deliveryNoteIt.getInt32( constants::CUSTOMERSUPPLIERNO ) );
		yitCustomerSupplier.setString( constants::SUPPLIERNAME, deliveryNoteIt.getString( constants::CUSTOMER_SUPPLIER_NAME ) );
	}
	else
	{
		//just copy customername in case of name changes
		yitCustomerSupplier.setString( constants::CUSTOMERNAME, deliveryNoteIt.getString( constants::CUSTOMER_SUPPLIER_NAME ) );
	}
	// get article data
	basar::db::aspect::AccessorPropertyTableRef tabOrderPos = m_ContributionVoucherDM->findOrderPositionsDelivery(deliveryNoteIt);

	m_NA18DeliveryNoteCopyVC->init();
	m_NA18DeliveryNoteCopyVC->matchFromContributionVoucherHead(deliveryNoteIt);
	m_NA18DeliveryNoteCopyVC->matchFromContributionVoucherPos(tabOrderPos);
	if (m_NA18DeliveryNoteCopyVC->show() == basar::gui::tie::WIDGET_OK)
	{
		// get search iterator to get value of rectification note
		m_ContributionVoucherDM->clearSearchPropTab();
		basar::db::aspect::AccessorPropertyTable_YIterator searchIt = m_ContributionVoucherDM->getAccSearchPropTab().insert( basar::FOR_CLEAN );
		m_NA18DeliveryNoteCopyVC->matchToRectificationNote(searchIt);

		narcotics::ExecuteReturnType retVal;
		basar::Int16 rectificationReason = searchIt.getInt16(constants::RECTIFICATIONREASON);
		basar::db::aspect::AccessorPropertyTable_YIterator yitPos = tabOrderPos.begin();
		while (false == yitPos.isEnd())
		{
			// deep copy is needed for SaveBooking...
			m_NarcoticsDM->deepCopy(yitPos);
			// match only current row because of deep copy/SaveBooking mechanism... krükke!
			m_NA18DeliveryNoteCopyVC->matchToContributionVoucherPos(tabOrderPos, yitPos.getRowNumber());

			// all positions acknowledgementstatus = 5
			yitPos.setInt16(constants::ACKNOWLEDGEMENTSTATUS, constants::DELIVERY_NOTE_COPY_CREATED);
			if (rectificationReason == constants::THEFT_LOSS)
			{
				//! \note Alle (markierten) Positionen des Abgabebeleges auf Satzart "y" ändern + Bestandsupdate (NarcoticsDM::saveBooking)!
				if (yitPos.getString(constants::SELECTION) == constants::SELECTION_ON)
				{
					yitPos.setString(constants::TRANSACTIONTYPE,			TransactionType::STOCK_DIFFERENCE_DECREASE);
					yitPos.setInt32(constants::CUSTOMERSUPPLIERNO,			yitBranch.getInt32(constants::BGANO));
					yitPos.setString(constants::CUSTOMER_SUPPLIER_NAME,		yitBranch.getString(constants::COMPANYNAME));
					yitPos.setString(constants::CUSTOMER_SUPPLIER_STREET,	yitBranch.getString(constants::STREET)); 
					yitPos.setString(constants::CUSTOMER_SUPPLIER_LOCATION, yitBranch.getString(constants::LOCATION_LONG));
                    yitPos.setString(constants::CUSTOMER_SUPPLIER_CIP,		yitBranch.getString(constants::POSTCODE));
				}
			}
			if (constants::QUANTITY_CORRECTION == rectificationReason)
			{
				//! \note Positionen mit geänderten Mengen updaten + Bestandsupdate (NarcoticsDM::saveBooking)!
				if (yitPos.getInt32(constants::QTYTRANSACTION) != yitPos.getInt32(constants::ORIGINALQTYTRANSACTION))
				{
					//! \note nothing to do here, because matcher has already updated qtytransaction!
				}
			}

			// append article data by rectification note
			yitPos.setString(constants::RECTIFICATIONTEXT, searchIt.getString(constants::RECTIFICATIONTEXT));
			yitPos.setString(constants::RECTIFICATIONCOMMENT, searchIt.getString(constants::RECTIFICATIONCOMMENT));
			retVal = m_NarcoticsDM->saveBooking(yitPos, false, true);
			if (retVal.hasError())
			{
				break;
			}

			if (constants::REFUSAL_OF_ACCEPTANCE == rectificationReason)
			{
				//! \note Alle (markierten) Positionen des Abgabebeleges stornieren (NarcoticsDM::strikeOut) + Bestandsupdate (NarcoticsDM::saveBooking)!
				retVal = m_NarcoticsDM->strikeOut(yitPos);
				if (retVal.hasError())
				{
					break;
				}
			}
			++yitPos;
		}

		// check if data manipulation was OK:
		if (retVal.hasError())
		{
			ret.ret = basar::appl::HANDLER_INFO;
			ret.message = QApplication::translate( "ContributionVoucherUC", 
													"Error while saving data!" ).toLocal8Bit().data();
			return ret;
		}

		// check if data is valid
		if( tabOrderPos.empty() || yitBranch.isEnd() || yitCustomerSupplier.isEnd() )
		{
			ret.ret = basar::appl::HANDLER_INFO;
			ret.message = QApplication::translate( "ContributionVoucherUC", 
													"Not enough data found to create a delivery note copy!" ).toLocal8Bit().data();
			return ret;
		}

		//transfers the deliverynotecopy to bfarm
 		printDeliveryNoteCopy( tabOrderPos, yitBranch, yitCustomerSupplier );
	}
	else
	{
		ret.ret = basar::appl::HANDLER_INFO;
		ret.message = QApplication::translate("ContributionVoucherUC",
		   "As dialog has been closed no delivery note copy could be created!").toLocal8Bit().data();
	}
	m_NA18DeliveryNoteCopyVC->shutdown();
	m_NA18DeliveryNoteCopyVC.reset();
	return ret;
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
SYSTEM_EVENT_HANDLER_DEFINITION( ContributionVoucherUC, SearchAcknowledgements )
{
	rSource = rSource; //if source not needed otherwise (e.g. for comparing); to avoid warnings

	if( LoggerPool::loggerUseCases.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "Event SearchOrder caught in ContributionVoucherUC.";
		LoggerPool::loggerUseCases.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	// get values for contributionvoucherno and transactiondate
	m_ContributionVoucherDM->clearSearchPropTab();
	basar::db::aspect::AccessorPropertyTable_YIterator searchIt = m_ContributionVoucherDM->getAccSearchPropTab().insert( basar::FOR_CLEAN );
	
    m_ContributionVoucherVC->matchToSearch( searchIt );
	
    // get order
	basar::db::aspect::AccessorPropertyTableRef orderData = m_ContributionVoucherDM->findAcknowledgements( searchIt );
	// check if data is valid
	if ( orderData.empty() )
	{
		basar::appl::EventReturnEnum ret = basar::appl::HANDLER_INFO;
		basar::I18nString msg = QApplication::translate( "ContributionVoucherUC", 
														"No order for given search criteria found!" ).toLocal8Bit().data();
		m_ContributionVoucherVC->matchFromSearch();
		return basar::appl::EventReturnStruct( ret, msg );
	}
	// match order results into gui
	m_ContributionVoucherVC->matchFromSearch();

	return basar::appl::EventReturnStruct();
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
SYSTEM_EVENT_HANDLER_DEFINITION( ContributionVoucherUC, ReprintDeliveryNote )
{
	rSource = rSource; //if source not needed otherwise (e.g. for comparing); to avoid warnings

	if( LoggerPool::loggerUseCases.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "Event ReprintDeliveryNote caught in ContributionVoucherUC.";
		LoggerPool::loggerUseCases.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	basar::appl::EventReturnStruct ret;
	basar::db::aspect::AccessorPropertyTable_YIterator yitContributionVoucher;

	// get selected order(s) from contributionvouchervc
	for (yitContributionVoucher = m_ContributionVoucherVC->getFirstSelectedContributionVoucher();
		false == yitContributionVoucher.isNull() && false == yitContributionVoucher.isEnd();
		yitContributionVoucher = m_ContributionVoucherVC->getNextSelectedContributionVoucher())
	{
		try
		{
			infrastructure::IPrintDeliveryNotePtr p = m_ServiceLocatorPtr->getPrintService();
			p->reprintDeliveryNote(yitContributionVoucher);
		}
		catch( infrastructure::MissingDependencyInjectionException& ex )
		{
			if( LoggerPool::loggerUseCases.isEnabledFor(log4cplus::ERROR_LOG_LEVEL) ){
				LoggerPool::loggerUseCases.forcedLog(log4cplus::ERROR_LOG_LEVEL, ex.what(), __FILE__, __LINE__);
			}
			throw ex;
		}
		catch( infrastructure::MissingDataForPrintingException& ex )
		{
			if( LoggerPool::loggerUseCases.isEnabledFor(log4cplus::ERROR_LOG_LEVEL) ){
				LoggerPool::loggerUseCases.forcedLog(log4cplus::ERROR_LOG_LEVEL, ex.what(), __FILE__, __LINE__);
			}
			ret.ret = basar::appl::HANDLER_ERROR;
			ret.message = ex.what();
			continue;
		}
	}

	if (ret.ret == basar::appl::HANDLER_OK)
	{
		ret.message = QApplication::translate( "ContributionVoucherUC", "Booking was sent to printer.").toLocal8Bit().data();
	}
	return ret;
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
SYSTEM_EVENT_HANDLER_DEFINITION( ContributionVoucherUC, OpenEmptyContributionVoucherDelCopy )
{
	rSource = rSource; //if source not needed otherwise (e.g. for comparing); to avoid warnings

	if( LoggerPool::loggerUseCases.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "Event OpenEmptyContributionVoucherDelCopy caught in ContributionVoucherUC.";
		LoggerPool::loggerUseCases.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	QUrl deliverynoteUrl = QUrl::fromLocalFile( QString::fromLocal8Bit( m_PrintFolderPath.c_str() ).append("/AbgLiefdoppel.pdf") );

	QString path = deliverynoteUrl.toString();

	if( false == QDesktopServices::openUrl( deliverynoteUrl ) )
	{
		basar::VarString msg; 
		msg.format( QApplication::translate( "ContributionVoucherUC",
                                             "opening of file:////print/AbgLiefdoppel.pdf failed!").toLocal8Bit().data());
		
		LoggerPool::loggerUseCases.forcedLog(log4cplus::ERROR_LOG_LEVEL, msg, __FILE__, __LINE__);

		return basar::appl::EventReturnStruct( basar::appl::HANDLER_ERROR, msg );
	}
 
	return basar::appl::EventReturnStruct( basar::appl::HANDLER_OK );
}

//-------------------------------------------------------------------------------------------------//
SYSTEM_EVENT_HANDLER_DEFINITION( ContributionVoucherUC, OpenEmptyDeliveryNote )
{
	rSource = rSource; //if source not needed otherwise (e.g. for comparing); to avoid warnings

	if( LoggerPool::loggerUseCases.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "Event OpenEmptyDeliveryNote caught in ContributionVoucherUC.";
		LoggerPool::loggerUseCases.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	QUrl deliverynoteUrl = QUrl::fromLocalFile( QString::fromLocal8Bit( m_PrintFolderPath.c_str() ).append("/Empfangsbestaetigung.pdf") );

	QString path = deliverynoteUrl.toString();

	if( false == QDesktopServices::openUrl( deliverynoteUrl ) )
	{
		basar::VarString msg; 
		msg.format( QApplication::translate( "ContributionVoucherUC",
                                             "opening of file:////print/Empfangsbestaetigung.pdf failed!").toLocal8Bit().data());
		
		LoggerPool::loggerUseCases.forcedLog(log4cplus::ERROR_LOG_LEVEL, msg, __FILE__, __LINE__);

		return basar::appl::EventReturnStruct( basar::appl::HANDLER_ERROR, msg );
	}
 
	return basar::appl::EventReturnStruct( basar::appl::HANDLER_OK );
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
SYSTEM_EVENT_HANDLER_DEFINITION( ContributionVoucherUC, MarkAsReceived )
{
	rSource = rSource; //if source not needed otherwise (e.g. for comparing); to avoid warnings

	if( LoggerPool::loggerUseCases.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "Event MarkAsReceived caught in ContributionVoucherUC.";
		LoggerPool::loggerUseCases.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}
    
	basar::appl::EventReturnStruct ret;
	basar::db::aspect::AccessorPropertyTable_YIterator yitContributionVoucher;

	// get selected order(s) from contributionvouchervc
	for (yitContributionVoucher = m_ContributionVoucherVC->getFirstSelectedContributionVoucher();
		false == yitContributionVoucher.isNull() && false == yitContributionVoucher.isEnd();
		yitContributionVoucher = m_ContributionVoucherVC->getNextSelectedContributionVoucher())
	{
		LOG_YIT("no",narcotics::LoggerPool::loggerNarcotics,yitContributionVoucher);
		// get article data
		basar::db::aspect::AccessorPropertyTableRef articleData = m_ContributionVoucherDM->findOrderPositionsDelivery( yitContributionVoucher );

		// check if data is valid
		if( articleData.empty() )
		{
			ret.ret = basar::appl::HANDLER_INFO;
			ret.message = QApplication::translate( "ContributionVoucherUC", "No bookings for selected order found!" ).toLocal8Bit().data();
			continue;
		}
		//set acknowledgementstatus to ACKNOWLEDGEMENT_RECEIVED
		//also check former acknowledgementstatus, has to be PRINTED
		basar::db::aspect::AccessorPropertyTable_YIterator yit = articleData.begin();

		bool notAllowedStatusFound = false;
		bool acknowledgementAlreadyReceived = true;

		basar::Int16 rowAcknowledgementStatus = 0;

		while( !yit.isEnd() )
		{
			basar::Int32 catalogId=-1;
			basar::VarString changedAttribute="NN";
			basar::VarString oldValue="NN";
			basar::VarString newValue="NN";

			rowAcknowledgementStatus = yit.getInt16( constants::ACKNOWLEDGEMENTSTATUS );

			if( rowAcknowledgementStatus != constants::PRINTED ){
				notAllowedStatusFound = true;
			}
			if( constants::ACKNOWLEDGEMENT_RECEIVED == rowAcknowledgementStatus || 
				constants::DELIVERY_NOTE_COPY_CREATED == rowAcknowledgementStatus )
			{
				acknowledgementAlreadyReceived &= true;
			}
			else{
				acknowledgementAlreadyReceived &= false;
			}

			if( yit.isContainedAndSet( constants::ACKNOWLEDGEMENTSTATUS ) ){
				oldValue.itos( yit.getInt16( constants::ACKNOWLEDGEMENTSTATUS ) );
			}

			if( yit.isContainedAndSet( constants::CATALOGID ) ){
				catalogId = yit.getInt32( constants::CATALOGID );
			}

			changedAttribute = constants::ACKNOWLEDGEMENTSTATUS;
			newValue.itos( constants::ACKNOWLEDGEMENT_RECEIVED );
			getManager().getLogDM()->saveLogEntryUpdate( catalogId, changedAttribute, oldValue, newValue );
			yit.setInt16( constants::ACKNOWLEDGEMENTSTATUS, constants::ACKNOWLEDGEMENT_RECEIVED );

			++yit;
		}

		if( acknowledgementAlreadyReceived )
		{
			ret.ret = basar::appl::HANDLER_INFO;
			ret.message = QApplication::translate( "ContributionVoucherUC", "Contribution Voucher already maked as received" ).toLocal8Bit().data();
			continue;
		}

		if( notAllowedStatusFound )
		{
			ret.ret = basar::appl::HANDLER_ERROR;
			ret.message = QApplication::translate( "ContributionVoucherUC", "Booking is not in status 'printed'" ).toLocal8Bit().data();
			continue;
		}

		yit = articleData.begin();

		narcotics::ExecuteReturnType result = m_ContributionVoucherDM->setAcknowledgementstatus( articleData );

		if( result.hasError() )
		{
			ret.ret = basar::appl::HANDLER_INFO;
			ret.message = QApplication::translate( "ContributionVoucherUC", "Acknowledgementstatus could not be updated!" ).toLocal8Bit().data();
			continue;
		}
	}
	return ret;
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
SYSTEM_EVENT_HANDLER_DEFINITION( ContributionVoucherUC, MarkAsPrinted )
{
	rSource = rSource; //if source not needed otherwise (e.g. for comparing); to avoid warnings

	if( LoggerPool::loggerUseCases.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "Event MarkAsPrinted caught in ContributionVoucherUC.";
		LoggerPool::loggerUseCases.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	basar::appl::EventReturnStruct ret;
	basar::db::aspect::AccessorPropertyTable_YIterator yitContributionVoucher;

	// get selected order(s) from contributionvouchervc
	for (yitContributionVoucher = m_ContributionVoucherVC->getFirstSelectedContributionVoucher();
		false == yitContributionVoucher.isNull() && false == yitContributionVoucher.isEnd();
		yitContributionVoucher = m_ContributionVoucherVC->getNextSelectedContributionVoucher())
	{
		LOG_YIT("no",narcotics::LoggerPool::loggerNarcotics,yitContributionVoucher);
		// get article data
		basar::db::aspect::AccessorPropertyTableRef articleData = m_ContributionVoucherDM->findOrderPositionsDelivery( yitContributionVoucher );

		// check if data is valid
		if( articleData.empty() )
		{
			ret.ret = basar::appl::HANDLER_INFO;
			ret.message = QApplication::translate( "ContributionVoucherUC", "No bookings for selected order found!" ).toLocal8Bit().data();
			continue;
		}
		//set acknowledgementstatus to ACKNOWLEDGEMENT_RECEIVED
		//also check former acknowledgementstatus, has to be PRINTED
		basar::db::aspect::AccessorPropertyTable_YIterator yit = articleData.begin();

		bool notAllowedStatusFound = false;
		bool acknowledgementAlreadyPrinted = true;

		basar::Int16 rowAcknowledgementStatus = 0;

		while( !yit.isEnd() )
		{
			basar::Int32 catalogId=-1;
			basar::VarString changedAttribute="NN";
			basar::VarString oldValue="NN";
			basar::VarString newValue="NN";

			rowAcknowledgementStatus = yit.getInt16( constants::ACKNOWLEDGEMENTSTATUS );

			if( rowAcknowledgementStatus != constants::ACKNOWLEDGEMENT_RECEIVED ){
				notAllowedStatusFound = true;
			}
			if( constants::PRINTED == rowAcknowledgementStatus )
			{
				acknowledgementAlreadyPrinted &= true;
			}
			else{
				acknowledgementAlreadyPrinted &= false;
			}

			if( yit.isContainedAndSet( constants::ACKNOWLEDGEMENTSTATUS ) ){
				oldValue.itos( yit.getInt16( constants::ACKNOWLEDGEMENTSTATUS ) );
			}

			if( yit.isContainedAndSet( constants::CATALOGID ) ){
				catalogId = yit.getInt32( constants::CATALOGID );
			}

			changedAttribute = constants::ACKNOWLEDGEMENTSTATUS;
			newValue.itos( constants::PRINTED );
			getManager().getLogDM()->saveLogEntryUpdate( catalogId, changedAttribute, oldValue, newValue );
			yit.setInt16( constants::ACKNOWLEDGEMENTSTATUS, constants::PRINTED );

			++yit;
		}

		if( acknowledgementAlreadyPrinted )
		{
			ret.ret = basar::appl::HANDLER_INFO;
			ret.message = QApplication::translate( "ContributionVoucherUC", "Contribution Voucher already marked as printed" ).toLocal8Bit().data();
			continue;
		}

		if( notAllowedStatusFound )
		{
			ret.ret = basar::appl::HANDLER_ERROR;
			ret.message = QApplication::translate( "ContributionVoucherUC", "Booking is not in status 'received'" ).toLocal8Bit().data();
			continue;
		}

		yit = articleData.begin();

		narcotics::ExecuteReturnType result = m_ContributionVoucherDM->setAcknowledgementstatus( articleData );

		if( result.hasError() )
		{
			ret.ret = basar::appl::HANDLER_INFO;
			ret.message = QApplication::translate( "ContributionVoucherUC", "Acknowledgementstatus could not be updated!" ).toLocal8Bit().data();
			continue;
		}
	}
	return ret;
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
SYSTEM_EVENT_HANDLER_DEFINITION( ContributionVoucherUC, RefreshAcknowledgement )
{
	rSource = rSource; //if source not needed otherwise (e.g. for comparing); to avoid warnings

	if( LoggerPool::loggerUseCases.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "Event RefreshAcknowledgement caught in ContributionVoucherUC.";
		LoggerPool::loggerUseCases.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	// search and match results for open acknowledgements
	basar::db::aspect::AccessorPropertyTable_YIterator yitSearch;
	m_ContributionVoucherDM->clearSearchPropTab();
	yitSearch = m_ContributionVoucherDM->getAccSearchPropTab().insert( basar::FOR_CLEAN );
	yitSearch.setInt16(constants::ACKNOWLEDGEMENTSTATUS, constants::PRINTED );
	m_ContributionVoucherDM->findAcknowledgements(yitSearch);
	m_ContributionVoucherVC->matchFromOpenAcknowledgements();	
	return basar::appl::EventReturnStruct();
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
SYSTEM_EVENT_HANDLER_DEFINITION( ContributionVoucherUC, PrintAcknowledgementList )
{
    rSource; //if source not needed otherwise (e.g. for comparing); to avoid warnings
    
    if( LoggerPool::loggerUseCases.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "Event PrintAcknowledgementList caught in ContributionVoucherUC.";
		LoggerPool::loggerUseCases.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}
    
    basar::appl::EventReturnStruct ret;

    //get data from dialog - we are printing already matched data
    AccessorPropertyTableRef prAcknowledgementData = m_ContributionVoucherVC->getCurrentDisplayedData();

    //sanity checking
    if( prAcknowledgementData.isNull() || prAcknowledgementData.empty() )
    {
        ret.ret = basar::appl::HANDLER_INFO;    
        ret.message = QApplication::translate("ContributionVoucherUC", "No Data to display").toLocal8Bit().data();

        return ret;
    }

    //1st param: ps-filename, 2nd is form name
    AcknowledgementListPrintData printData( "AcknowledgementPrintList.ps", "AcknowledgementPrintList", prAcknowledgementData );

	basar::gui::qt::PrintPreview preview( printData, 0, 
					QApplication::translate( "ContributionVoucherUC", "AcknowledgementDataPrintList" ).toLocal8Bit().data() );
	
	preview.setOutputFormats( basar::gui::qt::PDF );
		
	preview.show();

    return ret;
}

//-------------------------------------------------------------------------------------------------//
void ContributionVoucherUC::reprintDeliveryNote(	AccessorPropertyTableRef articleData, 
													AccessorPropertyTable_YIterator yitBranch, 
													AccessorPropertyTable_YIterator yitCustomer, 
													AccessorPropertyTable_YIterator yitLink, 
													basar::Int32 beginOrder, 
													basar::Int32 endOrder	)
{
	basar::I18nString printerName;

	basar::I18nString branchnotxt;
	branchnotxt.itos( basar::login::Manager::getInstance().getAreaID() );

	basar::login::Manager::getInstance().getParaValue( branchnotxt, UADM_NARCOTICSPRINTER, printerName );

	ReceiptNoticePrintData printData( articleData, yitBranch, yitCustomer, yitLink, beginOrder, endOrder );
	
	basar::cmnutil::RawPrint rPrint;
	if( rPrint.print( printerName, printData )!= basar::cmnutil::PRINTER_SUCCESS )
	{
		if( narcotics::LoggerPool::loggerDomModules.isEnabledFor(log4cplus::ERROR_LOG_LEVEL) )
		{
			basar::VarString msg = "error while printing receiptnotice: " ;
			msg.append(printData.getPostPrintingInformation());
			narcotics::LoggerPool::loggerDomModules.forcedLog(log4cplus::ERROR_LOG_LEVEL, msg, __FILE__, __LINE__);
		}
	}
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
void ContributionVoucherUC::printDeliveryNoteCopy(	basar::db::aspect::AccessorPropertyTableRef articleData, 
													basar::db::aspect::AccessorPropertyTable_YIterator yitBranch,
													basar::db::aspect::AccessorPropertyTable_YIterator yitCustomer )
{
	if (LoggerPool::loggerUseCases.isEnabledFor(log4cplus::TRACE_LOG_LEVEL))
	{
		basar::ConstString msg = "Event printDeliveryNoteCopy caught in ContributionVoucherUC.";
		LoggerPool::loggerUseCases.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	basar::Int32 beginOfOrder = 0;
	basar::Int32 endOfOrder = 0;
	// first booking
	basar::db::aspect::AccessorPropertyTable_YIterator currentBooking = articleData.begin();
	// counter for bookings of same order
	basar::Int32 bookingsCounter = 0;
	while( !currentBooking.isEnd() )
	{
		currentBooking++;
		bookingsCounter++;
		endOfOrder++;
	}
	if( bookingsCounter <= constants::BOOKINGS_PER_FORM ){
		transferDeliveryNoteCopy( articleData, yitBranch, yitCustomer, beginOfOrder, endOfOrder );
	}
	else // if bookingsCounter > 6 then more than one form have to be printed (-> bookings have to be spread over the forms)
	{
		// number of printable bookings
		basar::Int32 beginOfPrintableBookings = beginOfOrder;
		basar::Int32 endOfPrintableBookings = beginOfOrder;
		// print and decrease bookings counter
		while( bookingsCounter > constants::BOOKINGS_PER_FORM )
		{
// ToDo: set new contributionvoucherno from second turn on
			endOfPrintableBookings += constants::BOOKINGS_PER_FORM;
			transferDeliveryNoteCopy( articleData, yitBranch, yitCustomer, beginOfPrintableBookings, endOfPrintableBookings );

			// decrease bookingsCounter by number of already printed bookings
			bookingsCounter -= constants::BOOKINGS_PER_FORM;
			// set new begin for bookings to print
			beginOfPrintableBookings += constants::BOOKINGS_PER_FORM;
		}
		if( bookingsCounter > 0 )
		{
			endOfPrintableBookings += bookingsCounter;
			transferDeliveryNoteCopy( articleData, yitBranch, yitCustomer, beginOfPrintableBookings, endOfPrintableBookings );
		}
	}
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
void ContributionVoucherUC::transferDeliveryNoteCopy( basar::db::aspect::AccessorPropertyTableRef articleData, 
										basar::db::aspect::AccessorPropertyTable_YIterator yitBranch, 
										basar::db::aspect::AccessorPropertyTable_YIterator yitCustomer, 
										basar::Int32 startPosition, 
										basar::Int32 endPosition)
{
	BLOG_TRACE_METHOD(LoggerPool::loggerUseCases, "ContributionVoucherUC::printForms");

	// get printerName
	basar::I18nString printerName = getManager().getPrinterName();

	// print delivery note copy
	
	basar::I18nString signatureParam;
	signatureParam.format("%s=%s %s;%s=%s.bmp;",
		constants::USERNAME,
		basar::login::Manager::getInstance().getForeName().c_str(),
		basar::login::Manager::getInstance().getSurName().c_str(),
		constants::SIGNATURELNK,
		basar::login::Manager::getInstance().getUserName().c_str());
		
	LOG_YIT("his",narcotics::LoggerPool::loggerNarcotics,articleData.begin());
	LOG_YIT("his",narcotics::LoggerPool::loggerNarcotics,yitBranch);
	LOG_YIT("his",narcotics::LoggerPool::loggerNarcotics,yitCustomer);
		
	basar::Int32 contributionVoucherNo;
	basar::Int32 bgaNo;
	basar::I18nString branchnotxt;

	branchnotxt.itos( basar::login::Manager::getInstance().getAreaID() );

	AccessorPropertyTable_YIterator articleDataYit = articleData.begin();

	if( !articleDataYit.isNull() && !articleDataYit.isEnd() && articleDataYit.isContainedAndSet( constants::CONTRIBUTIONVOUCHERNO ) )
	{
		contributionVoucherNo = articleDataYit.getInt32( constants::CONTRIBUTIONVOUCHERNO );
	}
	else
	{
		basar::cmnutil::ExceptionInfoStruct excInfo;
		excInfo.context = "SYSTEM_EVENT_HANDLER_DEFINITION( ContributionVoucherUC, PrintDeliveryNoteCopy )";
		excInfo .file =__FILE__;
		excInfo.line=__LINE__;
		excInfo.reason = "contribution voucher no not set!!";

		throw basar::cmnutil::BasarInvalidParameterException( excInfo );	
	}
		
	if( !yitBranch.isNull() && !yitBranch.isEnd() && yitBranch.isContainedAndSet( constants::BGANO ) )
	{
		bgaNo = yitBranch.getInt32( constants::BGANO );
	}
	else
	{
		basar::cmnutil::ExceptionInfoStruct excInfo;
		excInfo.context = "SYSTEM_EVENT_HANDLER_DEFINITION( ContributionVoucherUC, PrintDeliveryNoteCopy )";
		excInfo .file =__FILE__;
		excInfo.line=__LINE__;
		excInfo.reason = "bgano no not set!!";

		throw basar::cmnutil::BasarInvalidParameterException( excInfo );	
	}

	basar::I18nString startDatetxt;
	basar::login::Manager::getInstance().getParaValue( branchnotxt, UADM_STARTDATE, startDatetxt );

	if( startDatetxt.empty() )
	{
		basar::cmnutil::ExceptionInfoStruct excInfo;
		excInfo.context = "SYSTEM_EVENT_HANDLER_DEFINITION( ContributionVoucherUC, PrintDeliveryNoteCopy )";
		excInfo .file =__FILE__;
		excInfo.line=__LINE__;
		excInfo.reason = "startdate not found in configuration!!";

		throw basar::cmnutil::BasarInvalidParameterException( excInfo );	
	}

	basar::I18nString hostname;
	basar::I18nString user;
	basar::I18nString pw;
	basar::I18nString localTmpDirectory;
	basar::I18nString directoryname;

	basar::login::Manager::getInstance().getParaValue( UADM_CONTRIBUTIONVOUCHERDIR, directoryname );

	if( directoryname.empty() )
	{
		basar::cmnutil::ExceptionInfoStruct excInfo;
		excInfo.context = "SYSTEM_EVENT_HANDLER_DEFINITION( ContributionVoucherUC, PrintDeliveryNoteCopy )";
		excInfo .file =__FILE__;
		excInfo.line=__LINE__;
		excInfo.reason = "directoryname not found in configuration!!";

		throw basar::cmnutil::BasarInvalidParameterException( excInfo );	
	}

	basar::login::Manager::getInstance().getParaValue( UADM_FTP_CITRIX_HOST, hostname );

	if( hostname.empty() )
	{
		basar::cmnutil::ExceptionInfoStruct excInfo;
		excInfo.context = "SYSTEM_EVENT_HANDLER_DEFINITION( ContributionVoucherUC, PrintDeliveryNoteCopy )";
		excInfo .file =__FILE__;
		excInfo.line=__LINE__;
		excInfo.reason = "ftp hostname not found in configuration!!";

		throw basar::cmnutil::BasarInvalidParameterException( excInfo );	
	}

	basar::login::Manager::getInstance().getParaValue(UADM_FTP_CITRIX_USER, user);	

	if( user.empty() )
	{
		basar::cmnutil::ExceptionInfoStruct excInfo;
		excInfo.context = "SYSTEM_EVENT_HANDLER_DEFINITION( ContributionVoucherUC, PrintDeliveryNoteCopy )";
		excInfo .file =__FILE__;
		excInfo.line=__LINE__;
		excInfo.reason = "ftp username not found in configuration!!";

		throw basar::cmnutil::BasarInvalidParameterException( excInfo );	
	}

	basar::login::Manager::getInstance().getParaValue( UADM_FTP_CITRIX_PW, pw );	

	if( pw.empty() )
	{
		basar::cmnutil::ExceptionInfoStruct excInfo;
		excInfo.context = "SYSTEM_EVENT_HANDLER_DEFINITION( ContributionVoucherUC, PrintDeliveryNoteCopy )";
		excInfo .file =__FILE__;
		excInfo.line=__LINE__;
		excInfo.reason = "citrix ftp pw not found in configuration!!";

		throw basar::cmnutil::BasarInvalidParameterException( excInfo );	
	}

	basar::login::Manager::getInstance().getParaValue( UADM_FTP_LOCAL_TMP, localTmpDirectory );	

	if( localTmpDirectory.empty() )
	{
		basar::cmnutil::ExceptionInfoStruct excInfo;
		excInfo.context = "SYSTEM_EVENT_HANDLER_DEFINITION( ContributionVoucherUC, PrintDeliveryNoteCopy )";
		excInfo .file =__FILE__;
		excInfo.line=__LINE__;
		excInfo.reason = "localtmpdirectory not found in configuration!!";

		throw basar::cmnutil::BasarInvalidParameterException( excInfo );	
	}

	DeliveryNoteCopyFTPTransfer deliveryNoteCopyTransfer( hostname, user, pw, directoryname, localTmpDirectory );

	AccessorPropertyTable_YIterator yitSignature = 
		basar::db::aspect::Manager::getInstance().createAccessorYIterator( signatureParam );

	DeliveryNoteCopyPrintData deliveryNoteCopy( articleData, yitBranch, yitCustomer, yitSignature, startPosition, endPosition );

	deliveryNoteCopyTransfer.storeFile( deliveryNoteCopy,bgaNo, contributionVoucherNo, startDatetxt );
}

//-------------------------------------------------------------------------------------------------//

} //namespace useCase
} //namespace narcotics