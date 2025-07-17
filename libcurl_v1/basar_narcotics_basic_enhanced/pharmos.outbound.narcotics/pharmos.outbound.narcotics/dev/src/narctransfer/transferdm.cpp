//-------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  implemtation of domain module "narcotics"
 *  \author	Roland Kiefert
 *  \date   22.02.2006
 */
//-------------------------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------------------------//
// includes
//-------------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "transferdm.h"
#include "loggerpool.h"
#include "version.h"
#include "narctransfer_definitions.h"
#include <lockmanager/lockmanager.h>

//-------------------------------------------------------------------------------------------------//
// namespaces
//-------------------------------------------------------------------------------------------------//
namespace narcotics {
namespace domMod {

//-------------------------------------------------------------------------------------------------//
/*!	\throw		no-throw */
//-------------------------------------------------------------------------------------------------//
TransferDM::TransferDM() :
	m_Initialized(0),
	m_inTransaction(false)
{
	BLOG_TRACE_METHOD(LoggerPool::loggerDomModules, "TransferDM::TransferDM()");
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw		no-throw */
//-------------------------------------------------------------------------------------------------//
TransferDM::~TransferDM()
{
	BLOG_TRACE_METHOD(LoggerPool::loggerDomModules, "TransferDM::~TransferDM()");
}

//-------------------------------------------------------------------------------------------------//
//! \return		void 
//! \param		branchNo is used in several method so it is set to a member.
//! \param		activeConn important for the creating process of every AccessorInstanceRef.
//! \param		userName is used for the Log modul.
/*!	\throw		no-throw */
//-------------------------------------------------------------------------------------------------//
void TransferDM::init(const basar::db::aspect::ConnectionRef activeConn)
{
	BLOG_TRACE_METHOD(LoggerPool::loggerDomModules, "TransferDM::init()");

	if( 0 == m_Initialized )
	{
		m_TransferACC	= basar::db::aspect::Manager::getInstance().createAccessorInstance( 
										"TransferAcc", "Transfer", activeConn, basar::db::aspect::FULL_CACHING );
		m_BranchesToTransferACC	= basar::db::aspect::Manager::getInstance().createAccessorInstance( 
										"BranchesToTransferACC", "Transfer", activeConn, basar::db::aspect::FULL_CACHING );
		m_CatalogACC	= basar::db::aspect::Manager::getInstance().createAccessorInstance( 
										"CatalogAcc", "Catalog", activeConn);
		m_CustomerSupplierACC = basar::db::aspect::Manager::getInstance().createAccessorInstance( 
										"CustomerSupplierAcc", "CustomerSupplier", activeConn);
		m_CustomerMappingACC = basar::db::aspect::Manager::getInstance().createAccessorInstance( 
										"CustomerNoMappingAcc", "CustomerNoMapping", activeConn);
		m_NarcGroupACC	= basar::db::aspect::Manager::getInstance().createAccessorInstance( 
										"NarcoticsGroupAcc", "NarcoticsGroup", activeConn);

		m_LockManager = boost::shared_ptr<libnarcotics::lockmanager::LockManager>(
			new libnarcotics::lockmanager::LockManager(
				activeConn,
				basar::login::Manager::getInstance().getUserName(),
				TRANSFERLOCK_TABLE_NAME,
				LoggerPool::loggerDomModules)
			);
	}
	++m_Initialized;

}

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
void TransferDM::shutdown()
{
	BLOG_TRACE_METHOD(LoggerPool::loggerDomModules, "TransferDM::shutdown()");

	if( m_Initialized == 0 )
		return;
	--m_Initialized;
	if( 0 == m_Initialized )
	{
        m_TransferACC.reset();
		m_BranchesToTransferACC.reset();
		m_CatalogACC.reset();
        m_CustomerSupplierACC.reset();
        m_CustomerMappingACC.reset();
        m_NarcGroupACC.reset();
	}
}

//-------------------------------------------------------------------------------------------------//
bool TransferDM::checkUniqueTimestampByBranchArticle( const basar::db::aspect::AccessorPropertyTable_YIterator yit )
{
	std::pair<bool, basar::cmnutil::Decimal> res;
	res = m_CatalogACC.executeAggregate( "CheckUniqueTimestampByBranchArticle", yit );
	
	if( res.first == true )
		return res.second.toInt32() == 0;
	else
		return true; // nothing found = should be unique
}

//-------------------------------------------------------------------------------------------------//
/*!	\return	
	\param	articleNo which a lock entry is inserted with.
	\throw	no-throw */
//-------------------------------------------------------------------------------------------------//
basar::db::aspect::AccessorPropertyTableRef TransferDM::findAllTransferBookings(basar::db::aspect::AccessorPropertyTable_YIterator yit)
{
	BLOG_TRACE_METHOD(LoggerPool::loggerDomModules, "TransferDM::findAllTransferBookings()");
	
	try
	{
		m_TransferACC.execute("SelTransfer", yit);
	}
	catch(basar::Exception &ex)
	{
		basar::VarString msg = "Exception thrown: ";

		msg.append(ex.what().c_str());
		
		m_TransferACC.getPropertyTable().clear();

		BLOG_ERROR(LoggerPool::loggerDomModules, msg.c_str());

	}

	return m_TransferACC.getPropertyTable();
}

//-------------------------------------------------------------------------------------------------//
/*!	\return	
	\param	articleNo which a lock entry is inserted with.
	\throw	no-throw */
//-------------------------------------------------------------------------------------------------//
basar::db::aspect::AccessorPropertyTableRef TransferDM::findAllBranchesToTransfer()
{
	BLOG_TRACE_METHOD(LoggerPool::loggerDomModules, "TransferDM::findAllBranchesToTransfer()");
	
	try
	{
		m_BranchesToTransferACC.execute("SelBranchesToTransfer");
	}
	catch(basar::Exception &ex)
	{
		basar::VarString msg = "Exception thrown: ";

		msg.append(ex.what().c_str());
		
		m_BranchesToTransferACC.getPropertyTable().clear();

		BLOG_ERROR(LoggerPool::loggerDomModules, msg.c_str());

	}

	return m_BranchesToTransferACC.getPropertyTable();
}

//-------------------------------------------------------------------------------------------------//
/*!	\return	
	\param	articleNo which a lock entry is inserted with.
	\throw	no-throw */
//-------------------------------------------------------------------------------------------------//
basar::db::aspect::ExecuteResultInfo TransferDM::saveOriginal( basar::db::aspect::AccessorPropertyTable_YIterator yit )
{
	BLOG_TRACE_METHOD(LoggerPool::loggerDomModules, "TransferDM::saveOriginal()");
	
	return m_CatalogACC.execute( "InsOriginal", yit );
}

//-------------------------------------------------------------------------------------------------//
/*!	\return	
	\param	articleNo which a lock entry is inserted with.
	\throw	no-throw */
//-------------------------------------------------------------------------------------------------//
basar::db::aspect::ExecuteResultInfo TransferDM::saveCatBookings( basar::db::aspect::AccessorPropertyTable_YIterator yit, bool saveFlag )
{
	BLOG_TRACE_METHOD(LoggerPool::loggerDomModules, "TransferDM::saveCatBookings()");

	basar::db::aspect::ExecuteResultInfo ret;
    
	if (saveFlag)
	{
		if (LoggerPool::loggerDomModules.isEnabledFor(log4cplus::TRACE_LOG_LEVEL))
		{
			basar::ConstString msg = "TransferDM.saveCatBookings(): InsCatalog";
			LoggerPool::loggerDomModules.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
		}

		m_CatalogACC.clearLastInsertedSerials();
		ret = m_CatalogACC.execute( "InsCatalog", yit );
		if (!ret.hasError())
		{
			yit.setInt32(lit::narcotics::CATALOGID, static_cast<basar::Int32>(m_CatalogACC.getLastInsertedSerial(0)));
		}

		if (LoggerPool::loggerDomModules.isEnabledFor(log4cplus::TRACE_LOG_LEVEL))
		{
			basar::VarString TraceMsg;
			TraceMsg.format("TransferDM.saveCatBookings(): InsCatalog ret = <%ld>", ret.getError());
			LoggerPool::loggerDomModules.forcedLog(log4cplus::TRACE_LOG_LEVEL, TraceMsg, __FILE__, __LINE__);
		}
    }
	else
	{ 
		if (LoggerPool::loggerDomModules.isEnabledFor(log4cplus::TRACE_LOG_LEVEL))
		{
			basar::ConstString msg = "TransferDM.saveCatBookings(): UpdQtyTransaction";
			LoggerPool::loggerDomModules.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
		}
		ret = m_CatalogACC.execute( "UpdQtyTransaction", yit );
	}
	return ret;
}

//-------------------------------------------------------------------------------------------------//
/*!	\return	
	\param	articleNo which a lock entry is inserted with.
	\throw	no-throw */
//-------------------------------------------------------------------------------------------------//
basar::db::aspect::ExecuteResultInfo TransferDM::deleteTransferEntry( basar::db::aspect::AccessorPropertyTable_YIterator yit )
{
	BLOG_TRACE_METHOD(LoggerPool::loggerDomModules, "TransferDM::deleteTransferEntry()");

	return m_TransferACC.execute( "DelTransfer", yit );
}

//-------------------------------------------------------------------------------------------------//
/*!	\return	
	\param	articleNo which a lock entry is inserted with.
	\throw	no-throw */
//-------------------------------------------------------------------------------------------------//
basar::db::aspect::AccessorPropertyTable_YIterator TransferDM::findCustomerSupplierByID( basar::db::aspect::AccessorPropertyTable_YIterator yit )
{
	BLOG_TRACE_METHOD(LoggerPool::loggerDomModules, "TransferDM::findCustomerSupplierByID()");

	basar::db::aspect::AccessorPropertyTable_YIterator	retYit;
	basar::db::aspect::ExecuteResultInfo				resInfo;

	if (yit.getString(lit::narcotics::TRANSACTIONTYPE) == "b" ||
		yit.getString(lit::narcotics::TRANSACTIONTYPE) == "d")
	{
		resInfo = m_CustomerSupplierACC.execute("SelSupplier", yit, true, false); 
		if (false == resInfo.hasError())
			retYit = m_CustomerSupplierACC.getPropertyTable().begin();
	}
	else if (yit.getString(lit::narcotics::TRANSACTIONTYPE) == "a" || 
			yit.getString(lit::narcotics::TRANSACTIONTYPE) == "c")
	{
		resInfo = m_CustomerSupplierACC.execute("SelCustomer", yit, true, false);
		if (false == resInfo.hasError())
			retYit = m_CustomerSupplierACC.getPropertyTable().begin();
	}
	return retYit;
}

//-------------------------------------------------------------------------------------------------//
/*!	\return	
	\param	articleNo which a lock entry is inserted with.
	\throw	no-throw */
//-------------------------------------------------------------------------------------------------//
basar::db::aspect::AccessorPropertyTable_YIterator TransferDM::findNarcoticsGroupByArticleno( basar::db::aspect::AccessorPropertyTable_YIterator yit )
{
	BLOG_TRACE_METHOD(LoggerPool::loggerDomModules, "TransferDM::findNarcoticsGroupByArticleno()");

	m_NarcGroupACC.execute("FindGroupByArticleNo", yit);
	return m_NarcGroupACC.getPropertyTable().begin();
}

//-------------------------------------------------------------------------------------------------//
/*!	\return	
	\param	articleNo which a lock entry is inserted with.
	\throw	no-throw */
//-------------------------------------------------------------------------------------------------//
basar::db::aspect::AccessorPropertyTable_YIterator TransferDM::findUnprocessedPurchaseOrderPos( basar::db::aspect::AccessorPropertyTable_YIterator yit )
{
	BLOG_TRACE_METHOD(LoggerPool::loggerDomModules, "TransferDM::findUnprocessedPurchaseOrderPos()");

	m_CatalogACC.execute("SelUnprocessedPurchaseOrder", yit);
	return m_CatalogACC.getPropertyTable().begin();
}

//-------------------------------------------------------------------------------------------------//
/*!	\return	
	\param	articleNo which a lock entry is inserted with.
	\throw	no-throw */
//-------------------------------------------------------------------------------------------------//
basar::db::aspect::ExecuteResultInfo TransferDM::makeInventoryUpdate(	basar::Int32 articleNo, basar::Int16 branchNo, 
												basar::Int32 transactionDate, basar::Int32 transactionTime ) //also needed here: qty, bookingtype
{
	BLOG_TRACE_METHOD(LoggerPool::loggerDomModules, "TransferDM::makeInventoryUpdate()");

	// vorgaenger holen
	m_CatalogACC.getPropertyTable().clear();
	basar::db::aspect::AccessorPropertyTable_YIterator yit = 
	basar::db::aspect::Manager::getInstance().createAccessorYIterator ( 
	basar::cmnutil::ParameterList("articleno;branchno;transactiondate;transactiontime;") );
  
	basar::VarString str;
	str.format("%ld", articleNo);		yit.setString(lit::narcotics::ARTICLENO, str);
	str.format("%ld", branchNo);		yit.setString(lit::narcotics::BRANCHNO, str);
	str.format("%ld", transactionDate);	yit.setString(lit::narcotics::TRANSACTIONDATE, str);
	str.format("%ld", transactionTime);	yit.setString(lit::narcotics::TRANSACTIONTIME, str);

    m_CatalogACC.execute( "SelQuickPrevQtyInStock", yit );
	
	basar::Int32 prevQtyInStock;

	if (m_CatalogACC.getPropertyTable().begin().isEnd())
	{ //no start stock !?
		prevQtyInStock = 0;
	}
	else
	{
		prevQtyInStock = m_CatalogACC.getPropertyTable().begin().getInt32(lit::narcotics::QTYINSTOCK);
	}

	// since our start date/time pair is the pair of the first inserted dataset
    // and there is no way of telling how many datasets (same branch and articleno) have been inserted until now
    // thus we need to recalculate the qtyinstock from start date/time on datarow level
	m_CatalogACC.getPropertyTable().clear();
	m_CatalogACC.execute( "SelSuccQtyInStock", yit );

	basar::db::aspect::ExecuteResultInfo retVal;

	if (!m_CatalogACC.getPropertyTable().isNull() && !m_CatalogACC.getPropertyTable().empty())
	{
		yit	= m_CatalogACC.getPropertyTable().begin();

		basar::Int16 bookingFactor = 1;
		while ( !yit.isEnd() )
		{
			//	the letter "a" represents selloff, the letter "y" represents a separate debit (deu: gesonderte Abbuchung), 
			//	the letter "z" represents a separate posting (deu: gesonderte Abbuchung)
			if( "a" == yit.getString(lit::narcotics::TRANSACTIONTYPE) || 
				"d" == yit.getString(lit::narcotics::TRANSACTIONTYPE) || 
				"y" == yit.getString(lit::narcotics::TRANSACTIONTYPE) || 
				"z" == yit.getString(lit::narcotics::TRANSACTIONTYPE))
			{
				bookingFactor = -1;
			} else
			{
				bookingFactor = 1; 
			}
		
			yit.setInt32(lit::narcotics::QTYINSTOCK,
				prevQtyInStock + (bookingFactor * yit.getInt32(lit::narcotics::QTYTRANSACTION)));
			prevQtyInStock = yit.getInt32(lit::narcotics::QTYINSTOCK);
			++yit;
		}

		retVal = m_CatalogACC.execute(	"UpdQtyTransaction", m_CatalogACC.getPropertyTable().begin(), m_CatalogACC.getPropertyTable().end()  );
	}
	else
	{
		basar::VarString msg = "";
		msg.format("TransferDM::makeInventoryUpdate(): found no later orders than transactiondate: %d transactiontime: %d for articleno: %d branchno: %d - nothing to be done, returning success",transactionDate, transactionTime, articleNo, branchNo);
		BLOG_INFO(LoggerPool::loggerDomModules, msg);
		
		retVal.m_error = basar::db::aspect::SUCCESS;
		retVal.m_affectedRows = 1;

	}
// ToDo: maybe this cast should be revised, but at the moment it is necessary as the result for affected rows 
// is used within further calculation 
	return retVal;
}

//-------------------------------------------------------------------------------------------------//
/*!	\return	
	\param	articleNo which a lock entry is inserted with.
	\throw	no-throw */
//-------------------------------------------------------------------------------------------------//
void TransferDM::TransactionBegin()
{
	BLOG_TRACE_METHOD(LoggerPool::loggerDomModules, "TransferDM::TransactionBegin()");

	m_TransferACC.beginTransaction();
	m_inTransaction = true;
}

//-------------------------------------------------------------------------------------------------//
/*!	\return	
	\param	articleNo which a lock entry is inserted with.
	\throw	no-throw */
//-------------------------------------------------------------------------------------------------//
void TransferDM::TransactionCommit()
{
	BLOG_TRACE_METHOD(LoggerPool::loggerDomModules, "TransferDM::TransactionCommit()");

	if( true == m_inTransaction ){
		m_TransferACC.commitTransaction();
	}
	m_inTransaction = false;
}

//-------------------------------------------------------------------------------------------------//
/*!	\return	
	\param	articleNo which a lock entry is inserted with.
	\throw	no-throw */
//-------------------------------------------------------------------------------------------------//
void TransferDM::TransactionRollback()
{
	BLOG_TRACE_METHOD(LoggerPool::loggerDomModules, "TransferDM::TransactionRollback()");

	if( true == m_inTransaction ){
		m_TransferACC.rollbackTransaction();
	}
	m_inTransaction = false;
}

//-------------------------------------------------------------------------------------------------//
/*!	\return	
	\param	articleNo which a lock entry is inserted with.
	\param  branchNo for which lock is inserted
	\throw	no-throw */
//-------------------------------------------------------------------------------------------------//
const basar::db::aspect::ExecuteResultInfo TransferDM::lock(const basar::Int32 articleNo, const basar::Int16 branchNo)
{
	if (LoggerPool::loggerDomModules.isEnabledFor(log4cplus::TRACE_LOG_LEVEL))
	{
		basar::VarString msg;
		msg.format("TransferDM.lock - Parameter: articleNo:<%ld> and branchno:<%d>", articleNo, branchNo);
		LoggerPool::loggerDomModules.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__);
	}
	
	basar::db::aspect::ExecuteResultInfo executeResultInfo;
	
	bool lockGranted = false;

	try
	{
        lockGranted = m_LockManager->requestLock(branchNo, TRANSFER_BUSINESS_TYPE, articleNo, TRANSFER_MAX_LOCKS_PER_BOOKING);
	}
	catch (basar::db::sql::DbException& /*ifxDbException*/)
	{
		#ifdef WIN32
			Sleep(10 * 1000);
		#else
			sleep(10);
		#endif

			lockGranted = m_LockManager->requestLock(branchNo, TRANSFER_BUSINESS_TYPE, articleNo, TRANSFER_MAX_LOCKS_PER_BOOKING);
	}

	//! todo auch warten, wenn keine Exception, aber lockGranted = false ???

	//fake return value
	if (lockGranted)
	{
		executeResultInfo.m_error = basar::db::aspect::SUCCESS;
		executeResultInfo.m_affectedRows = 1;
	}
	else
	{
		executeResultInfo.m_error = basar::db::aspect::SQL_ERROR_UNKNOWN;
		executeResultInfo.m_affectedRows = 0;
	}

	if (executeResultInfo.hasError())
	{
		if (LoggerPool::loggerDomModules.isEnabledFor(log4cplus::INFO_LOG_LEVEL))
		{
			basar::VarString msg;
			msg.format("TransferDM::lock - Parameter: Execute-ErrorNo:<%d>, articleNo:<%ld> and branchno:<%d>", 
				        executeResultInfo.m_error, articleNo, branchNo);
			LoggerPool::loggerDomModules.forcedLog(log4cplus::INFO_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__);
		}	
	}
	return executeResultInfo;
}

//-------------------------------------------------------------------------------------------------//
const basar::db::aspect::ExecuteResultInfo			TransferDM::unlockAllArticleLocksByUser()
{
	if (LoggerPool::loggerDomModules.isEnabledFor(log4cplus::TRACE_LOG_LEVEL))
	{
		basar::VarString msg;
		msg="TransferDM::unlockAllArticleLocksByUser";
		LoggerPool::loggerDomModules.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__);
	}


	basar::db::aspect::ExecuteResultInfo executeResultInfo;

	bool lockReleased = false;
	
	try
	{
		lockReleased = m_LockManager->releaseAllLocksByBusinessTypeAndUser(TRANSFER_BUSINESS_TYPE);
	}
	catch (basar::db::sql::DbException &ifxDbException)
	{
		basar::VarString msg;
		
		msg="TransferDM::unlockAllArticleLocksByUser: error while unlocking ";
		msg.append(ifxDbException.what().c_str());
		msg.append(" trying second time after 10 s");

		LoggerPool::loggerDomModules.forcedLog(log4cplus::ERROR_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__);

		#ifdef WIN32
			Sleep(10 * 1000);
		#else
			sleep(10);
		#endif

		
		msg="TransferDM::unlockAllArticleLocksByUser";
		LoggerPool::loggerDomModules.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__);

		lockReleased = m_LockManager->releaseAllLocksByBusinessTypeAndUser(TRANSFER_BUSINESS_TYPE);
	}

	//fake return value
	if (lockReleased)
	{
		executeResultInfo.m_error = basar::db::aspect::SUCCESS;
		executeResultInfo.m_affectedRows = 1;
	}
	else
	{
		executeResultInfo.m_error = basar::db::aspect::SQL_ERROR_UNKNOWN;
		executeResultInfo.m_affectedRows = 0;
	}

	return executeResultInfo;
}

//-------------------------------------------------------------------------------------------------//
/*!	\return	
	\param	articleNo that is used for unlocking the article.
	\param	branchNo for which lock is released
	\throw	no-throw */
//-------------------------------------------------------------------------------------------------//
const basar::db::aspect::ExecuteResultInfo TransferDM::unlock(const basar::Int32 articleNo, const basar::Int16 branchNo)
{
	if (LoggerPool::loggerDomModules.isEnabledFor(log4cplus::TRACE_LOG_LEVEL))
	{
		basar::VarString msg;
		msg.format("TransferDM::unlock - Parameter: articleNo:<%ld> and branchno:<%d>", articleNo, branchNo);
		LoggerPool::loggerDomModules.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__);
	}
	
	basar::db::aspect::ExecuteResultInfo executeResultInfo;
	
	bool lockReleased=false;

	try{
		lockReleased = m_LockManager->releaseLock(branchNo, TRANSFER_BUSINESS_TYPE, articleNo);
	}
	catch (basar::db::sql::DbException& /*ifxDbException*/)
	{
		#ifdef WIN32
			Sleep(10 * 1000);
		#else
			sleep(10);
		#endif

			lockReleased = m_LockManager->releaseLock(branchNo, TRANSFER_BUSINESS_TYPE, articleNo);
	}

	//fake return value
	if (lockReleased)
	{
		executeResultInfo.m_error = basar::db::aspect::SUCCESS;
		executeResultInfo.m_affectedRows = 1;
	}
	else
	{
		executeResultInfo.m_error = basar::db::aspect::SQL_ERROR_UNKNOWN;
		executeResultInfo.m_affectedRows = 0;
	}

	return executeResultInfo;
}

//-------------------------------------------------------------------------------------------------//
/*!	\return	
	\param	articleNo that is used for unlocking the article.
	\throw	no-throw */
//-------------------------------------------------------------------------------------------------//
basar::db::aspect::AccessorPropertyTable_YIterator	TransferDM::findCustomerMapping(basar::db::aspect::AccessorPropertyTable_YIterator yit)
{
	BLOG_TRACE_METHOD(LoggerPool::loggerDomModules, "TransferDM::findCustomerMapping()");

	m_CustomerMappingACC.execute("SelCustomerMapping", yit);
	return m_CustomerMappingACC.getPropertyTable().begin();
}

//-------------------------------------------------------------------------------------------------//
/*!	\return	
	\param	
	\throw	no-throw */
//-------------------------------------------------------------------------------------------------//
const basar::db::aspect::ExecuteResultInfo TransferDM::saveBookingsToPrint(const basar::db::aspect::AccessorPropertyTable_YIterator yit)
{
	BLOG_TRACE_METHOD(LoggerPool::loggerDomModules, "TransferDM::saveBookingsToPrint()");	

	return m_CatalogACC.execute("UpdAcknowledgementstatus", yit);
}

//-------------------------------------------------------------------------------------------------//
/*!	\return	
	\param	
	\throw	no-throw */
//-------------------------------------------------------------------------------------------------//
const std::vector<basar::Int16> TransferDM::prepareBookingsForPrint()
{
	BLOG_TRACE_METHOD(LoggerPool::loggerDomModules, "TransferDM::prepareBookingsForPrint()");
	
	basar::db::aspect::AccessorPropertyTable_YIterator yitTransfer = m_TransferACC.getPropertyTable().begin();
	basar::db::aspect::ExecuteResultInfo resInfo;
	std::vector<basar::Int16> branchNoList;
	while(!yitTransfer.isNull() && !yitTransfer.isEnd())
	{
		basar::Int16 branchno = yitTransfer.getInt16(lit::narcotics::BRANCHNO);
		if(isNewNarcoticsProcess(yitTransfer))
		{
			// list doesn't contain branchno
			if(std::find(branchNoList.begin(), branchNoList.end(), branchno) == branchNoList.end())
			{
				branchNoList.push_back(branchno);
			}
		}
		yitTransfer++;
	}
	return branchNoList;
}

//-------------------------------------------------------------------------------------------------//
/*!	\return	
	\param	
	\throw	no-throw */
//-------------------------------------------------------------------------------------------------//
bool TransferDM::isNewNarcoticsProcess(basar::db::aspect::AccessorPropertyTable_YIterator yit)
{
	BLOG_TRACE_METHOD(LoggerPool::loggerDomModules, "isNewNarcoticsProcess()");

	static std::map<basar::Int16, bool> s_BranchNewProcessMap;
	std::map<basar::Int16, bool>::iterator it;

	basar::Int16 branchno = yit.getInt16(lit::narcotics::BRANCHNO);
	it = s_BranchNewProcessMap.find(branchno);

	if (it != s_BranchNewProcessMap.end())
		return it->second;

	basar::VarString branchnotxt;
	branchnotxt.itos(branchno);
	basar::I18nString status;

	bool ret = false;

	basar::login::Manager::getInstance().getParaValue(branchnotxt, uadm::UADM_NEW_NARCOTIC_PROCESS, status);

	if(!status.empty())
	{
		status.trim(basar::cmnutil::BString::ALL);

		if(status=="1")
		{
			ret = true;
		}
	}
	s_BranchNewProcessMap.insert(std::map<basar::Int16, bool>::value_type(branchno, ret));
	return ret;
}

} // namespace domMod
} // namespace narcotics
