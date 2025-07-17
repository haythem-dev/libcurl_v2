//---------------------------------------------------------------------------//
/*! \file 
 *  \brief interface for use case TransferUC
 *  \author 
 *  \date 31.08.2009
 *  \version 00.00.00.01 first version
 */ //-----------------------------------------------------------------------//

//---------------------------------------------------------------------------//
// include section
//---------------------------------------------------------------------------//
#include "commonheader.h"
#include "transferuc.h"
#include "parameter.h"
#include "loggerpool.h"
#include "narctransferappl.h"
#include "narctransfer_definitions.h"

//---------------------------------------------------------------------------//
// namespace section
//---------------------------------------------------------------------------//
namespace narcotics 
{
namespace useCase 
{

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
TransferUC::TransferUC()
{
	BLOG_TRACE_METHOD(LoggerPool::loggerUseCases, "TransferUC::TransferUC()");
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
TransferUC::~TransferUC()
{
	BLOG_TRACE_METHOD(LoggerPool::loggerUseCases, "TransferUC::~TransferUC()");
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
BEGIN_HANDLERS_REGISTRATION(TransferUC)
	SYSTEM_EVENT_HANDLER_REGISTRATION(TransferUC, TransferNarcTransactions)
END_HANDLERS_REGISTRATION()

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
basar::appl::EventReturnType TransferUC::run(basar::appl::IEventSource& rSource,
											basar::db::aspect::AccessorPropertyTable_YIterator yitFromEvSrc)
{
	BLOG_TRACE_METHOD(LoggerPool::loggerUseCases, "TransferUC::run");

	basar::appl::EventReturnStruct ret;

	basar::db::aspect::ConnectionRef currentConnection = getManager().m_Connection.getCurrentConnection();

	registerEventHandlers(true);

	getManager().m_TransferDM.init(currentConnection);

	try
	{
		ret = basar::appl::SystemEventManager::getInstance().fire("TransferNarcTransactions", yitFromEvSrc, rSource);
	}
	catch(basar::Exception& ex)
	{
		basar::VarString msg;

		msg.format("Consumed exception in TransferUC::run(): %s", ex.what().c_str());
		
		BLOG_ERROR(LoggerPool::loggerUseCases, msg);
	}

	if(!currentConnection.isNull() && currentConnection.isOpen() && currentConnection.isInTransaction()	)
	{
		currentConnection.rollbackTransaction();

		basar::VarString msg;

		msg = "TransferUC::run:  CurrentConnection still in transaction. Transaction rolled back !!";

		BLOG_ERROR(LoggerPool::loggerUseCases, msg);
	}

	getManager().m_TransferDM.unlockAllArticleLocksByUser();
	
	getManager().m_TransferDM.shutdown();
	registerEventHandlers(false);
	return ret;
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
basar::appl::EventReturnStruct TransferUC::processTransfersByBranch(basar::db::aspect::AccessorPropertyTable_YIterator yit)
{
	basar::appl::EventReturnStruct						ret;
	Counter												cnt;
	bool												inTransaction = false;
	basar::db::aspect::ExecuteResultInfo				resInfo;
	basar::VarString									msg;
	basar::db::aspect::AccessorPropertyTableRef			transferTab;
	basar::db::aspect::AccessorPropertyTable_YIterator	yitTransfer;
	
	m_ControlBreak.init();

	//find all transfers by branch / completed flag from narctransactiontransfer
	transferTab = getManager().m_TransferDM.findAllTransferBookings(yit);

	if(!transferTab.isNull() && !transferTab.empty())
	{
		yitTransfer = transferTab.begin();

		while (!yitTransfer.isEnd())
		{
		// 1. check for control break
			if (true == isControlBreak(yitTransfer))
			{
				if(-1 == checkTransactionAndUnlock(inTransaction, cnt))
				{
					ret.ret = basar::appl::HANDLER_ERROR;
					break;
				}
	            
				m_PurchaseOrderMap.clear();  // clearing map

				assignControlBreak(yitTransfer);
				++(cnt.m_Group);
				
				// locken
				if( 0 >= getManager().m_TransferDM.lock(yitTransfer.getInt32(lit::narcotics::ARTICLENO),
														yitTransfer.getInt16(lit::narcotics::BRANCHNO)).getAffectedRows())
				{
					msg.format("The following group could not be locked articleno:<%ld> and branchno <%d>!",  
						yitTransfer.getInt32(lit::narcotics::ARTICLENO),
						yitTransfer.getInt16(lit::narcotics::BRANCHNO));
					BLOG_WARN(LoggerPool::loggerUseCases, msg);
					cnt.m_LockedGroups++;
                    
					fetchNextControlBreak( yitTransfer );
               	    continue;
               	}

				getManager().m_TransferDM.TransactionBegin();	
				inTransaction = true;
			} // end control break

		// 2. save original
			resInfo = getManager().m_TransferDM.saveOriginal(yitTransfer);
			
			if (resInfo.hasError())
			{
				basar::VarString str( "Error with saving on table 'narctransactionoriginal: ");
				RollbackHandle(cnt.m_RolledbackGroups, inTransaction, yitTransfer, str);
				// wird bereits in RollbackHandle gemacht: fetchNextControlBreak(yitTransfer);
				continue;
			}

		// 3. handling invalid qty
			if (0 >= yitTransfer.getInt32(lit::narcotics::QTYTRANSACTION))
			{
				msg.format(	"The attribute 'qtytransaction' <%ld> is invalid with articleno:<%ld>, branchno <%d> and purchaseorderno:<%ld>!",  
					yitTransfer.getInt32(lit::narcotics::QTYTRANSACTION),
					yitTransfer.getInt32(lit::narcotics::ARTICLENO),
					yitTransfer.getInt16(lit::narcotics::BRANCHNO),
					yitTransfer.getInt32(lit::narcotics::PURCHASEORDERNO));
				BLOG_INFO(LoggerPool::loggerUseCases, msg);

				//delete from narctransactiontransfer
				resInfo = getManager().m_TransferDM.deleteTransferEntry(yitTransfer);

				if (resInfo.hasError())
				{
					basar::VarString str( "Error with deleting on table 'narctransactiontransfer: ");
					RollbackHandle(cnt.m_RolledbackGroups, inTransaction, yitTransfer, str);
				}
				else
				{
					++yitTransfer;
				}
				continue;
			}
			
		// 4. handling invalid transaction type
			if (!isTransactionTypeValid(yitTransfer.getString(lit::narcotics::TRANSACTIONTYPE)))
			{
				msg.format(	"Invalid transactiontype '<%s>' with articleno:<%ld>, branchno <%d> and purchaseorderno:<%ld>!",  
					yitTransfer.getString(lit::narcotics::TRANSACTIONTYPE).c_str(),
					yitTransfer.getInt32(lit::narcotics::ARTICLENO),
					yitTransfer.getInt16(lit::narcotics::BRANCHNO),
					yitTransfer.getInt32(lit::narcotics::PURCHASEORDERNO));
				BLOG_ERROR(LoggerPool::loggerUseCases, msg);

				resInfo = getManager().m_TransferDM.deleteTransferEntry(yitTransfer);

				if (resInfo.hasError())
				{
					basar::VarString str( "Error with deleting on table 'narctransactiontransfer: ");
					RollbackHandle(cnt.m_RolledbackGroups, inTransaction, yitTransfer, str);
				}
				else
				{
					++yitTransfer;
				}
				continue;
			}

		// 5. default acknowledgementstatus for bookings is set to AS_INSERTED
			yitTransfer.setInt16(lit::narcotics::ACKNOWLEDGEMENTSTATUS, narcotics::AS_INSERTED); // status INSERTED (0)
			
		// 6. fill with narcotic and customer / supplier specific data
			ProcessStateEnum retProcessState = processBookings(yitTransfer); 
			
		// 7. save catalog
			if (PS_TO_INSERT == retProcessState)
			{
				createUniqueTransactionTime(yitTransfer);
				resInfo = getManager().m_TransferDM.saveCatBookings(yitTransfer, true);
				setCatalogID2NarcoticBooking(yitTransfer);
			}
			else
			{
				resInfo = getManager().m_TransferDM.saveCatBookings(yitTransfer, false);
				cnt.m_MergeUpdatedRows++;
			}

			if (resInfo.hasError())
			{
				basar::VarString str( "Error with saving on table 'narctransactioncatalog: ");
				RollbackHandle(cnt.m_RolledbackGroups, inTransaction, yitTransfer, str);
				continue;
			}

		// 8. delete entries from narctransactiontransfer
			resInfo = getManager().m_TransferDM.deleteTransferEntry(yitTransfer);
			if (resInfo.hasError())
			{
				basar::VarString str( "Error with deleting on table 'narctransactiontransfer: ");
				RollbackHandle(cnt.m_RolledbackGroups, inTransaction, yitTransfer, str);
				continue;
			}

			cnt.m_TransferredRows++;
			++yitTransfer;
		} // End While

		if (-1 == checkTransactionAndUnlock(inTransaction, cnt))
		{
			ret.ret = basar::appl::HANDLER_ERROR;
			ret.message = "Error in checkTransactionAndUnlock()!";
		}

		// logging counter block (per branch!)
		msg.format(	"\n"
					"  looped groups            %6d\n"
					"  transferred groups       %6d\n"
					"  transferred rows         %6d\n"
					"  merge rows               %6d\n"
					"  update rows (catalog)    %6d\n"
					"  locked groups (by other) %6d\n"
					"  rollbacked groups        %6d",
					cnt.m_Group,
					cnt.m_TransferredGroups,
					cnt.m_TransferredRows,
					cnt.m_MergeUpdatedRows,
					cnt.m_UpdatedRows,
					cnt.m_LockedGroups,
					cnt.m_RolledbackGroups);
		BLOG_INFO(LoggerPool::loggerUseCases, msg);

		// 9. update all sell off´s to printable
		yitTransfer = transferTab.begin(); // better to use search iterator instead of using first yit of result list !?

		yitTransfer.setInt16(lit::narcotics::ACKNOWLEDGEMENTSTATUS, narcotics::AS_FOR_PRINT);
		yitTransfer.setString(lit::narcotics::TRANSACTIONTYPE, lit::narcotics::SELLOFF);
			
		resInfo = getManager().m_TransferDM.saveBookingsToPrint(yitTransfer);

		if(resInfo.hasError())
		{
			ret.ret = basar::appl::HANDLER_ERROR;
			ret.message += "Error while updating narctransactioncatalog to set printstatus 'AS_FOR_PRINT (1)'.";
		}
	}
	else
	{
		BLOG_INFO(LoggerPool::loggerUseCases, "No transfer data found for branch!!");
	}

	return ret;
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
SYSTEM_EVENT_HANDLER_DEFINITION(TransferUC, TransferNarcTransactions)
{
	BLOG_TRACE_METHOD(LoggerPool::loggerUseCases, "TransferUC::TransferNarcTransactions()");

	rSource = rSource;
    yitFromEvSrc = yitFromEvSrc;

	basar::appl::EventReturnStruct						ret;
	basar::db::aspect::AccessorPropertyTableRef			branchesToTransferTab;
	basar::db::aspect::AccessorPropertyTable_YIterator	yitBranchesToTransfer;

	// ermitteln der VZ´s, für die es Transfereinträge gibt (Unterscheidung nach Nord/Süd nicht notwendig!)
	BLOG_DEBUG(LoggerPool::loggerUseCases, "looking for branches to transfer data for ...");
	branchesToTransferTab = getManager().m_TransferDM.findAllBranchesToTransfer();

	if(!branchesToTransferTab.isNull() && !branchesToTransferTab.empty())
	{
		yitBranchesToTransfer = branchesToTransferTab.begin();
		basar::I18nString	searchValues;
		basar::VarString	branch;
		basar::I18nString	relevantCompletedState;
		basar::db::aspect::AccessorPropertyTable_YIterator	searchIt;

		// Schleife für alle VZ´s: pro VZ Parameter aus UADM lesen, ob completedflag = 1 / 2 relevant 
		while (!yitBranchesToTransfer.isEnd())
		{
			branch.itos(yitBranchesToTransfer.getInt16(lit::narcotics::BRANCHNO));

			if (false == basar::login::Manager::getInstance().getParaValue(branch, uadm::UADM_RELEVANT_COMPLETED_STATE, relevantCompletedState))
				relevantCompletedState = "1"; // wenn kein UADM-Parameter gefunden, dann weiter mit der alten Logik (==> completedflag=1)

			searchValues.format( "%s=%s; %s=%s;",
									lit::narcotics::BRANCHNO.getName().c_str(), branch.c_str(), 
									lit::narcotics::COMPLETEDFLAG.getName().c_str(), relevantCompletedState.c_str());
			searchIt = basar::db::aspect::Manager::getInstance().createAccessorYIterator(searchValues);
	
			BLOG_INFO(LoggerPool::loggerUseCases, searchValues.c_str());

			ret = processTransfersByBranch(searchIt);

			if ( ret.ret == basar::appl::HANDLER_ERROR )
			{
				BLOG_ERROR(LoggerPool::loggerUseCases, ret.message.c_str());
				break;
			}

			++yitBranchesToTransfer;
		} // end branch-loop
	} 
	else
	{
		BLOG_INFO(LoggerPool::loggerUseCases, "No branches to transfer data for found!!");
	}

	return ret;
}

//-------------------------------------------------------------------------------------------------//
void TransferUC::createUniqueTransactionTime(basar::db::aspect::AccessorPropertyTable_YIterator yit)
{
	// check, whether combination transactiondate / transactiontime already exists for this article / branch.
	while (false == getManager().m_TransferDM.checkUniqueTimestampByBranchArticle(yit))
	{
		// If yes, increase by 1 second for a + d / decrease by 1 second for b + c 
		basar::DateTime dt (yit.getInt32(lit::narcotics::TRANSACTIONDATE), yit.getInt32(lit::narcotics::TRANSACTIONTIME) * 1000);
		basar::Int16 secondsToAdd = 1;

		if (isIncomingTransactionType(yit.getString(lit::narcotics::TRANSACTIONTYPE)))
			secondsToAdd = -1;

		dt.addTime(secondsToAdd * 1000);
					
		yit.setInt32(lit::narcotics::TRANSACTIONTIME, dt.getTime() / 1000); // TRANSACTIONDATE shouldn´t change !
					
		if (isIncomingTransactionType(yit.getString(lit::narcotics::TRANSACTIONTYPE)))
			m_ControlBreak.m_TransactionTime = dt.getTime() / 1000; // makeInventoryUpdate is called with this time

		// check again = loop
	}			
}

//-------------------------------------------------------------------------------------------------//
bool TransferUC::isTransactionTypeValid(const basar::VarString& transactionType)
{
	BLOG_TRACE_METHOD(LoggerPool::loggerUseCases, "TransferUC::isTransactionTypeValid()");

    if( transactionType == "a" || 
		transactionType == "b" || 
		transactionType == "c" || 
		transactionType == "s" || 
		transactionType == "S" ||	// new transactiontype "S" is created in KSC and has to be transformed to "a" for BASAR
		transactionType == "u" ){
		return true;	
	}
	return false;
}

//-------------------------------------------------------------------------------------------------//
bool TransferUC::isIncomingTransactionType(const basar::VarString& transactionType)
{
	BLOG_TRACE_METHOD(LoggerPool::loggerUseCases, "TransferUC::isIncomingTransactionType()");

    if( transactionType == "b" || transactionType == "c" )
		return true;	
	else
		return false;
}

//-------------------------------------------------------------------------------------------------//
bool TransferUC::fetchCustomerSupplierAndNarcGroupData(basar::db::aspect::AccessorPropertyTable_YIterator yit)
{
	BLOG_TRACE_METHOD(LoggerPool::loggerUseCases, "TransferUC::fetchCustomerSupplierAndNarcGroupData()");

	bool ret = true;

	basar::Int32 custNoFromCsc = yit.getInt32(lit::narcotics::CUSTOMERSUPPLIERNO); // intern
	basar::Int32 custNoForDelivery = 0; //extern
	bool switchAdress = false;
	basar::db::aspect::AccessorPropertyTable_YIterator yitCustMapping;
	

	if (isIncomingTransactionType(yit.getString(lit::narcotics::TRANSACTIONTYPE)))
	{
		custNoForDelivery = custNoFromCsc; // no customer mapping for incoming transactions

		// WE oder Retoure = Zugang
		if (0 >= yit.getInt32(lit::narcotics::CUSTOMERSUPPLIERNO))
		{
			yit.setString(lit::narcotics::PROCESSEDFLAG, "0");

			if (LoggerPool::loggerUseCases.isEnabledFor(log4cplus::INFO_LOG_LEVEL))
			{
				basar::VarString msg; 
				msg.format( "TransferUC::fetchCustomerSupplierAndNarcGroupData: The automatic incoming goods processing has been cancelled as no supplierno is set: "
							"branch <%d>, articleno <%ld>, transactiondate <%ld>, transactiontime <%ld>, "
							"customersupplierno <%ld> ", 
							yit.getInt16(lit::narcotics::BRANCHNO),
							yit.getInt32(lit::narcotics::ARTICLENO),  
							yit.getInt32(lit::narcotics::TRANSACTIONDATE),
							yit.getInt32(lit::narcotics::TRANSACTIONTIME),
							yit.getInt32(lit::narcotics::CUSTOMERSUPPLIERNO));
				LoggerPool::loggerUseCases.forcedLog( log4cplus::INFO_LOG_LEVEL, msg, __FILE__, __LINE__ );
			}		
		}
		else
		{
			basar::VarString branchnotxt;
			basar::I18nString markAsProcessed = "0";

			branchnotxt.itos(yit.getInt16(lit::narcotics::BRANCHNO));

			if (true == basar::login::Manager::getInstance().getParaValue(branchnotxt, uadm::UADM_MARK_GI_ITEM_AS_PROCESSED, markAsProcessed))
			{
				basar::VarString msg; 
				msg.format( "UADM- Parameter <%s> exists for branch <%s> and is set to value <%s>.",
					uadm::UADM_MARK_GI_ITEM_AS_PROCESSED, branchnotxt.c_str(), markAsProcessed.c_str() );
				LoggerPool::loggerUseCases.forcedLog( log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__ );

				if ("1" == markAsProcessed)
					yit.setString(lit::narcotics::PROCESSEDFLAG, "1");
				else
					yit.setString(lit::narcotics::PROCESSEDFLAG, "0");
			}
			else
			{
				// UADM- parameter not found --> leave GI´s unprocessed!
				basar::VarString msg; 
				msg.format( "UADM- Parameter <%s> not available for branch <%s>.", uadm::UADM_MARK_GI_ITEM_AS_PROCESSED, branchnotxt.c_str() );
				LoggerPool::loggerUseCases.forcedLog( log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__ );

				yit.setString(lit::narcotics::PROCESSEDFLAG, "0");
			}
		}
    }
	else
	{
		// Verkauf, Rücksendung, Vernichtung, ... = Abgang
		yit.setString(lit::narcotics::PROCESSEDFLAG, "1");

		/*  get customernomapping */		
		yitCustMapping = getManager().m_TransferDM.findCustomerMapping( yit );

		if (!yitCustMapping.isNull() && !yitCustMapping.isEnd())
		{
			if (1 == yitCustMapping.getInt16(lit::narcotics::SWITCHADDRESSFLAG))
			{
				// für findCustomerSupplierByID zunächst die interne IDF setzen, da an deren Adresse geliefert werden soll - aber mit externer IDF
				//		--> daher erst NACH findCustomerSupplierByID die CUSTOMERSUPPLIERNO auf die CUSTOMERNO_EXTERN setzen
				custNoForDelivery = custNoFromCsc; 
				switchAdress = true;
			}
			else
				custNoForDelivery = yitCustMapping.getInt32(lit::narcotics::CUSTOMERNO_EXTERN);
		}
		else
			custNoForDelivery = custNoFromCsc; // no mapping found --> csc customer = delivered customer
	}

	yit.setInt32(lit::narcotics::CUSTOMERSUPPLIERNO, custNoForDelivery);
	basar::db::aspect::AccessorPropertyTable_YIterator yitCustomerSupplier;

	yit.setString(lit::narcotics::CUSTOMERSUPPLIERNAME, "");
	yit.setString(lit::narcotics::CUSTOMERSUPPLIERSTREET, "");
    yit.setString(lit::narcotics::CUSTOMERSUPPLIERCITY, "");
	yit.setString(lit::narcotics::CUSTOMERSUPPLIERCIP, "");
    
	try
	{
		if (0 < yit.getInt32(lit::narcotics::CUSTOMERSUPPLIERNO))
		{
			// real customersupplierno passed
			yitCustomerSupplier = getManager().m_TransferDM.findCustomerSupplierByID(yit);
		
			if (yitCustomerSupplier.isNull() || yitCustomerSupplier.isEnd())
			{
				if (LoggerPool::loggerUseCases.isEnabledFor(log4cplus::ERROR_LOG_LEVEL))
				{
					basar::VarString msg; 
					msg.format( "No Item found with selecting transactiontype <%s> and customersupplierno <%ld>!", 
								yit.getString(lit::narcotics::TRANSACTIONTYPE).c_str(),
								yit.getInt32(lit::narcotics::CUSTOMERSUPPLIERNO));
					LoggerPool::loggerUseCases.forcedLog(log4cplus::ERROR_LOG_LEVEL, msg, __FILE__, __LINE__);	
				}
				ret = false;
			}
			else
			{
				yit.setString(lit::narcotics::CUSTOMERSUPPLIERNAME,	yitCustomerSupplier.getString(lit::narcotics::CUSTOMERSUPPLIERNAME));
                yit.setString(lit::narcotics::CUSTOMERSUPPLIERSTREET, yitCustomerSupplier.getString(lit::narcotics::CUSTOMERSUPPLIERSTREET));
				yit.setString(lit::narcotics::CUSTOMERSUPPLIERCITY,	yitCustomerSupplier.getString(lit::narcotics::CUSTOMERSUPPLIERCITY));
				yit.setString(lit::narcotics::CUSTOMERSUPPLIERCIP, yitCustomerSupplier.getString(lit::narcotics::CUSTOMERSUPPLIERCIP));
			}

			if (true == switchAdress) // erst NACH findCustomerSupplierByID die CUSTOMERSUPPLIERNO auf die CUSTOMERNO_EXTERN setzen
				yit.setInt32(lit::narcotics::CUSTOMERSUPPLIERNO, yitCustMapping.getInt32(lit::narcotics::CUSTOMERNO_EXTERN));

		}
	}
	catch (...) {
		if( LoggerPool::loggerUseCases.isEnabledFor(log4cplus::ERROR_LOG_LEVEL) ){
			basar::VarString msg; 
			msg.format("Error with selecting transactiontype <%s> and customersupplierno <%ld>!", 
						yit.getString(lit::narcotics::TRANSACTIONTYPE).c_str(),
						yit.getInt32(lit::narcotics::CUSTOMERSUPPLIERNO));
			LoggerPool::loggerUseCases.forcedLog(log4cplus::ERROR_LOG_LEVEL, msg, __FILE__, __LINE__);	
		}
		ret = false;
	}

	// copy data from narcotics group into yit
	yit.setString(lit::narcotics::NARCGROUP, "");
	yit.setString(lit::narcotics::NARCNAME, "");
	yit.setDecimal(lit::narcotics::CONCENTRATION, basar::Decimal(0.0));
	yit.setString(lit::narcotics::PACKAGEUNIT, "");
	yit.setDecimal(lit::narcotics::NARCFACTOR, basar::Decimal(0.0));

	try {
		basar::db::aspect::AccessorPropertyTable_YIterator yitNarcGroup;

		yitNarcGroup = getManager().m_TransferDM.findNarcoticsGroupByArticleno(yit);

		if (yitNarcGroup.isNull() || yitNarcGroup.isEnd())
		{
			if( LoggerPool::loggerUseCases.isEnabledFor(log4cplus::ERROR_LOG_LEVEL) ){
				basar::VarString msg; 
				msg.format( "No Item found by group with selecting articleno <%ld>!", 
					yit.getInt32(lit::narcotics::ARTICLENO));
				LoggerPool::loggerUseCases.forcedLog(log4cplus::ERROR_LOG_LEVEL, msg, __FILE__, __LINE__);	
			}
			ret = false;
		}
		else
		{ 
			yit.setString(lit::narcotics::NARCGROUP, yitNarcGroup.getString(lit::narcotics::NARCGROUP) );
			yit.setString(lit::narcotics::NARCNAME, yitNarcGroup.getString(lit::narcotics::NARCNAME) );
			yit.setDecimal(lit::narcotics::CONCENTRATION, yitNarcGroup.getDecimal(lit::narcotics::CONCENTRATION) );
			yit.setString(lit::narcotics::PACKAGEUNIT, yitNarcGroup.getString(lit::narcotics::PACKAGEUNIT) );
			yit.setDecimal(lit::narcotics::NARCFACTOR, yitNarcGroup.getDecimal(lit::narcotics::NARCFACTOR));
		}
	}
	catch (...) {
		if( LoggerPool::loggerUseCases.isEnabledFor(log4cplus::ERROR_LOG_LEVEL) ){
			basar::VarString msg; 
			msg.format("Error with selecting articleno <%ld>!", 
						yit.getInt32(lit::narcotics::ARTICLENO));
			LoggerPool::loggerUseCases.forcedLog(log4cplus::ERROR_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__);	
		}
		ret = false;
	}
	return ret;
}

//-------------------------------------------------------------------------------------------------//
void TransferUC::setCatalogID2NarcoticBooking(basar::db::aspect::AccessorPropertyTable_YIterator yit)
{
	BLOG_TRACE_METHOD(LoggerPool::loggerUseCases, "TransferUC::setCatalogID2NarcoticBooking()");

    std::map<basar::Int32, NarcoticBooking>::iterator it;
	it = m_PurchaseOrderMap.find(yit.getInt32(lit::narcotics::PURCHASEORDERNO));
	if (m_PurchaseOrderMap.end() != it )
	{
	    it->second.catalogid = yit.getInt32(lit::narcotics::CATALOGID);
    }
}

//-------------------------------------------------------------------------------------------------//
std::map<basar::Int32, NarcoticBooking>::iterator TransferUC::findUnprocessedPurchaseOrderPos( basar::db::aspect::AccessorPropertyTable_YIterator& yit )
{
	BLOG_TRACE_METHOD(LoggerPool::loggerUseCases, "TransferUC::findUnprocessedPurchaseOrderPos()");

	std::map<basar::Int32, NarcoticBooking>::iterator it;
	it = m_PurchaseOrderMap.find(yit.getInt32(lit::narcotics::PURCHASEORDERNO));
	if (m_PurchaseOrderMap.end() == it)
	{
		// 1. search in db
		basar::db::aspect::AccessorPropertyTable_YIterator upYit = getManager().m_TransferDM.findUnprocessedPurchaseOrderPos(yit);
		if (!upYit.isNull() && !upYit.isEnd())
		{
			// 2. if found, insert into map   
			NarcoticBooking booking; 
			booking.articleNo	= yit.getInt32(lit::narcotics::ARTICLENO);
			booking.bookingDate = yit.getInt32(lit::narcotics::TRANSACTIONDATE);
			booking.bookingTime	= yit.getInt32(lit::narcotics::TRANSACTIONTIME);
			booking.branchNo	= yit.getInt16(lit::narcotics::BRANCHNO);
			booking.qty			= upYit.getInt32(lit::narcotics::QTYTRANSACTION);
			booking.catalogid   = upYit.getInt32(lit::narcotics::CATALOGID); 
			std::pair< std::map<basar::Int32, NarcoticBooking>::iterator, bool >pairIns = 
				m_PurchaseOrderMap.insert(std::map<basar::Int32, NarcoticBooking>::value_type(yit.getInt32(lit::narcotics::PURCHASEORDERNO), booking));
			if (true == pairIns.second)
			{
				it = pairIns.first;
			} 
		}
	} 
	return it;
}

//-------------------------------------------------------------------------------------------------//
// We-Positionen aufsummieren und 
//-------------------------------------------------------------------------------------------------//
ProcessStateEnum TransferUC::processBookings( basar::db::aspect::AccessorPropertyTable_YIterator yit)
{
	BLOG_TRACE_METHOD(LoggerPool::loggerUseCases, "TransferUC::processBookings()");

	ProcessStateEnum									ret = PS_UNINITIALZED;
	std::map<basar::Int32, NarcoticBooking>::iterator	it;
	basar::VarString									msg; 

	// Für WE können mehrere Sätze pro Artikel vorhanden sein (wg. WE-Etiketten) -> aufsummieren.
	if( yit.getString(lit::narcotics::TRANSACTIONTYPE) == "b" )
	{
		it = findUnprocessedPurchaseOrderPos( yit );

        if (m_PurchaseOrderMap.end() == it)
		{
			NarcoticBooking booking; 
			booking.articleNo	= yit.getInt32(lit::narcotics::ARTICLENO);
			booking.bookingDate = yit.getInt32(lit::narcotics::TRANSACTIONDATE);
			booking.bookingTime	= yit.getInt32(lit::narcotics::TRANSACTIONTIME);
			booking.branchNo	= yit.getInt16(lit::narcotics::BRANCHNO);
			booking.qty			= yit.getInt32(lit::narcotics::QTYTRANSACTION);

			if (false == fetchCustomerSupplierAndNarcGroupData(yit))
			{
				msg.format("TransferUC::processBookings: No valid customersupplier found with: customersupplierno: <%ld>, "
					"purchaseorderno: <%ld>, branchno: <%d>, artNo: <%ld>, transactiondate: <%ld>, transactiontime: <%ld>", 
					yit.getInt32(lit::narcotics::CUSTOMERSUPPLIERNO),
					yit.getInt32(lit::narcotics::PURCHASEORDERNO),
					yit.getInt16(lit::narcotics::BRANCHNO),
					yit.getInt32(lit::narcotics::ARTICLENO), 
					yit.getInt32(lit::narcotics::TRANSACTIONDATE), 
					yit.getInt32(lit::narcotics::TRANSACTIONTIME));
				BLOG_TRACE(LoggerPool::loggerUseCases, msg);
            }
			m_PurchaseOrderMap.insert(std::map<basar::Int32, NarcoticBooking>::value_type( yit.getInt32("purchaseorderno"), booking));
			ret = PS_TO_INSERT;
		}
		else // schon eine pos gefunden -> Menge aufsummieren
		{
            yit.setInt32(lit::narcotics::CATALOGID, it->second.catalogid);
			it->second.qty += yit.getInt32(lit::narcotics::QTYTRANSACTION);
			yit.setInt32(lit::narcotics::QTYTRANSACTION, it->second.qty);
			ret = PS_TO_UPDATE;
		}
	}
	else
	{
		if ((yit.getString(lit::narcotics::TRANSACTIONTYPE) == "u") ||
			(yit.getString(lit::narcotics::TRANSACTIONTYPE) == "s") ||
			(yit.getString(lit::narcotics::TRANSACTIONTYPE) == "S"))  // new transactiontype "S" is created in KSC and has to be transformed to "a" for BASAR
			// also take care of acc_catalog::UpdAcknowledgementstatus when these values are changed !!!
		{
			yit.setString(lit::narcotics::TRANSACTIONTYPE, "a");
		}
		if (false == fetchCustomerSupplierAndNarcGroupData(yit))
		{
			msg.format("TransferUC::processBookings: No valid customersupplier found with: customersupplierno: <%ld>, "
				"purchaseorderno: <%ld>, branchno: <%d>, artNo: <%ld>, transactiondate: <%ld>, transactiontime: <%ld>", 
				yit.getInt32(lit::narcotics::CUSTOMERSUPPLIERNO),
				yit.getInt32(lit::narcotics::PURCHASEORDERNO),
				yit.getInt16(lit::narcotics::BRANCHNO),
				yit.getInt32(lit::narcotics::ARTICLENO),
				yit.getInt32(lit::narcotics::TRANSACTIONDATE),
				yit.getInt32(lit::narcotics::TRANSACTIONTIME));
			BLOG_INFO(LoggerPool::loggerUseCases, msg.c_str());
		}
		ret = PS_TO_INSERT;
	}

	msg.format("TransferUC::processBookings: ret<%ld> - [UNINITIALZED = -1, ERROR  = 0, TO_INSERT = 1, TO_UPDATE = 2].",
		ret);
	BLOG_TRACE(LoggerPool::loggerUseCases, msg.c_str());

	return ret;
}

//-------------------------------------------------------------------------------------------------//
bool TransferUC::isControlBreak( basar::db::aspect::AccessorPropertyTable_YIterator yit)
{
	basar::VarString msg;

	msg.format("TransferUC::isControlBreak: "
		"ArticleNo: cB<%ld> -  yit<%ld>, "
		"BranchNo: cB<%ld> -  yit<%ld>, ",
		m_ControlBreak.m_ArticleNo, yit.getInt32(lit::narcotics::ARTICLENO),
		m_ControlBreak.m_BranchNo, yit.getInt16(lit::narcotics::BRANCHNO));
	BLOG_TRACE(LoggerPool::loggerUseCases, msg.c_str());

	return	m_ControlBreak.m_ArticleNo	!= yit.getInt32(lit::narcotics::ARTICLENO) || 
			m_ControlBreak.m_BranchNo	!= yit.getInt16(lit::narcotics::BRANCHNO);
}

//-------------------------------------------------------------------------------------------------//
void TransferUC::assignControlBreak( basar::db::aspect::AccessorPropertyTable_YIterator yit)
{
	if (LoggerPool::loggerUseCases.isEnabledFor(log4cplus::TRACE_LOG_LEVEL))
	{
		basar::VarString msg;
        
        msg.format( "main::assignControlBreak:  "
                    "ArticleNo: old<%ld> -  new<%ld>, "
                    "BranchNo: old<%ld> -  new<%ld>, "
                    "TransactionDate: old<%ld> -  new<%ld>, "
                    "TransactionTime: old<%ld> -  new<%ld> ", 
                    m_ControlBreak.m_ArticleNo, yit.getInt32(lit::narcotics::ARTICLENO),
	            	m_ControlBreak.m_BranchNo, yit.getInt16(lit::narcotics::BRANCHNO),
    		        m_ControlBreak.m_TransactionDate, yit.getInt32(lit::narcotics::TRANSACTIONDATE), 
		            m_ControlBreak.m_TransactionTime, yit.getInt32(lit::narcotics::TRANSACTIONTIME));
		LoggerPool::loggerUseCases.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__);
	}

    if (!yit.isEnd())
	{
		m_ControlBreak.m_ArticleNo			= yit.getInt32(lit::narcotics::ARTICLENO);
		m_ControlBreak.m_BranchNo			= yit.getInt16(lit::narcotics::BRANCHNO);
		m_ControlBreak.m_TransactionDate	= yit.getInt32(lit::narcotics::TRANSACTIONDATE);
		m_ControlBreak.m_TransactionTime	= yit.getInt32(lit::narcotics::TRANSACTIONTIME);
	}
	else
	{
		m_ControlBreak.init();
	}
}

//-------------------------------------------------------------------------------------------------//
void TransferUC::fetchNextControlBreak( basar::db::aspect::AccessorPropertyTable_YIterator& yit)
{
	basar::VarString msg;
    
	if(!yit.isNull())
	{
		msg.format("TransferUC::fetchNextControlBreak: "
			"ArticleNo: <%ld>, "
			"BranchNo: <%d> ",
			yit.getInt32(lit::narcotics::ARTICLENO),
			yit.getInt16(lit::narcotics::BRANCHNO));
	}	
	else
	{
		//exit here, nothing in yit, nothin to be done
        msg="yit is null";
        BLOG_TRACE(LoggerPool::loggerUseCases, msg.c_str());
        return;
	}

	BLOG_TRACE(LoggerPool::loggerUseCases, msg.c_str());

    while(!yit.isEnd() && !isControlBreak(yit))
	{
		++yit;
	}

    //ask if yit is end
	if(!yit.isEnd())
	{
		msg.format("TransferUC::fetchNextControlBreak: "
			"ArticleNo: <%ld>, "
			"BranchNo: <%d> ",
			yit.getInt32(lit::narcotics::ARTICLENO),
			yit.getInt16(lit::narcotics::BRANCHNO));		
	}
	else
	{
		msg = "yit is end!";
	}

	BLOG_TRACE(LoggerPool::loggerUseCases, msg.c_str());
}

//-------------------------------------------------------------------------------------------------//
int TransferUC::checkTransactionAndUnlock( bool& inTransaction, Counter& cnt )
{
	BLOG_TRACE_METHOD(LoggerPool::loggerUseCases, "checkTransactionAndUnlock()");

	int ret = 0;
	if (true == inTransaction)
	{
		basar::db::aspect::ExecuteResultInfo result = getManager().m_TransferDM.makeInventoryUpdate(m_ControlBreak.m_ArticleNo,
			m_ControlBreak.m_BranchNo, m_ControlBreak.m_TransactionDate, m_ControlBreak.m_TransactionTime);
	
		
		if (result.hasError())
		{
			if( LoggerPool::loggerUseCases.isEnabledFor(log4cplus::INFO_LOG_LEVEL) )
			{
				basar::VarString msg;
				msg.format( "Error with updating inventory with articleNo <%ld>, branchNo <%d>, transactionDate <%ld> and transactionTime <%ld>!",
					m_ControlBreak.m_ArticleNo, m_ControlBreak.m_BranchNo, m_ControlBreak.m_TransactionDate, m_ControlBreak.m_TransactionTime );
				LoggerPool::loggerUseCases.forcedLog(log4cplus::INFO_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__);
			}
		}
		else
		{
			cnt.m_UpdatedRows += result.getAffectedRows();
		}

		getManager().m_TransferDM.TransactionCommit();
		
		inTransaction = false;
		
		cnt.m_TransferredGroups++;

		// any control required if the unlock does not work ???
		if( 0 >= getManager().m_TransferDM.unlock(m_ControlBreak.m_ArticleNo, m_ControlBreak.m_BranchNo).getAffectedRows() ){

			if( LoggerPool::loggerUseCases.isEnabledFor(log4cplus::ERROR_LOG_LEVEL) ){
			
				basar::VarString msg;
				msg.format(		"Lock item could not be deleted with values articleno:<%ld> and branchno <%d>!",  
								m_ControlBreak.m_ArticleNo, m_ControlBreak.m_BranchNo );
				LoggerPool::loggerUseCases.forcedLog(log4cplus::ERROR_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__);

			}
			ret = -1;
		}
	}
    
    return ret;
}

//-------------------------------------------------------------------------------------------------//
void TransferUC::RollbackHandle(basar::UInt32& rolledbackGroups, bool& inTransaction, 
					basar::db::aspect::AccessorPropertyTable_YIterator& yit, 
					basar::VarString msg)
{
	BLOG_TRACE_METHOD(LoggerPool::loggerUseCases, "TransferUC::RollbackHandle()");

	getManager().m_TransferDM.TransactionRollback();
	++rolledbackGroups;
	inTransaction = false;
	getManager().m_TransferDM.unlock(yit.getInt32(lit::narcotics::ARTICLENO), yit.getInt16(lit::narcotics::BRANCHNO));
	// ToDo: find out what m_pFetchNextControlBreak as it seems to destroy iterator m_yit (having been executed rowNumber is set to -99)
	fetchNextControlBreak(yit);
	if (LoggerPool::loggerUseCases.isEnabledFor(log4cplus::ERROR_LOG_LEVEL))
	{
		basar::VarString logmsg;
		logmsg.format("articleno:<%ld>, branchno:<%d>, purchaseorderno:<%ld>'!",
			yit.getInt32(lit::narcotics::ARTICLENO),
			yit.getInt16(lit::narcotics::BRANCHNO),
			yit.getInt32(lit::narcotics::PURCHASEORDERNO));
		msg += logmsg;
		LoggerPool::loggerUseCases.forcedLog(log4cplus::ERROR_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__);
	}
}

} //namespace useCase
} //namespace narcotics