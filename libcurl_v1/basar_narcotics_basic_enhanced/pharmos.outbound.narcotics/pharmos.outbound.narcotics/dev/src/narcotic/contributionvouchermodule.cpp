k//-------------------------------------------------------------------------------------------------//
/*! \file 
 *  \brief  Domain module for handling contribution voucher entries
 *  \author Benjamin Endlicher
 *  \date   03.05.2010
 */
//-------------------------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "contributionvouchermodule.h"
#include "loggerpool.h"

//-------------------------------------------------------------------------------------------------//
// using declarations section
//-------------------------------------------------------------------------------------------------//
using basar::db::aspect::AccessorInstanceRef;
using basar::db::aspect::AccessorPropertyTableRef;

//-------------------------------------------------------------------------------------------------//
// namespace section
//-------------------------------------------------------------------------------------------------//
namespace domMod
{
	//-------------------------------------------------------------------------------------------------//
    ContributionVoucherDM::ContributionVoucherDM() 
    : m_Initialized(0)
	{
	}

    //-------------------------------------------------------------------------------------------------//	
	ContributionVoucherDM::~ContributionVoucherDM()
	{
		shutdown();
	}

    //-------------------------------------------------------------------------------------------------//
	// all Accessors needing an active conn are initialized here
	void ContributionVoucherDM::init( const basar::Int16 branchNo, const basar::db::aspect::ConnectionRef activeConn )
	{
		BLOG_TRACE_METHOD(narcotics::LoggerPool::loggerDomModules, "ContributionVoucherDM::init()");
		
		if( 0 == m_Initialized )
		{
			m_ConnectionRef = activeConn;
			m_BranchNo = branchNo;

			basar::VarString defParam;
			defParam.format("%s=%d;",
				constants::BRANCHNO,
				branchNo);
			getNoAcknowledgementACC().setDefaultParameter(defParam);
		}
		++m_Initialized;
	}

	//-------------------------------------------------------------------------------------------------//
    void ContributionVoucherDM::injectLogDM( boost::shared_ptr<domMod::LogDM> logDM )
    {
        m_LogDM = logDM;
    }

	//-------------------------------------------------------------------------------------------------//
	// Lazy Instantiation of Accessors in this Section: Getter-Methods defined here, won't be executed before really being needed
	// Consequently, Accessors won't be initialized before being needed
	CREATE_ACCESSOR_INSTANCE_DEF( ContributionVoucherDM, ContributionVoucher, ContributionVoucher, basar::db::aspect::ON_DEMAND_CACHING );
	CREATE_ACCESSOR_INSTANCE_DEF( ContributionVoucherDM, ManualContributionVoucher, ContributionVoucher, basar::db::aspect::FULL_CACHING );
	CREATE_ACCESSOR_INSTANCE_DEF( ContributionVoucherDM, DeliveryNoteCopy, ContributionVoucher, basar::db::aspect::FULL_CACHING );
	CREATE_ACCESSOR_INSTANCE_DEF( ContributionVoucherDM, NoAcknowledgement, ContributionVoucher, basar::db::aspect::ON_DEMAND_CACHING );
	CREATE_ACCESSOR_INSTANCE_DEF( ContributionVoucherDM, Order, ContributionVoucher, basar::db::aspect::FULL_CACHING );
	CREATE_ACCESSOR_INSTANCE_DEF( ContributionVoucherDM, Search, ContributionVoucher, basar::db::aspect::FULL_CACHING );
	CREATE_ACCESSOR_INSTANCE_DEF( ContributionVoucherDM, PrintSearch, ContributionVoucher, basar::db::aspect::FULL_CACHING );
	CREATE_ACCESSOR_INSTANCE_DEF( ContributionVoucherDM, NewBookings, ContributionVoucher, basar::db::aspect::FULL_CACHING );
	CREATE_ACCESSOR_INSTANCE_DEF( ContributionVoucherDM, CustomerSupplier, CustomerSupplier, basar::db::aspect::FULL_CACHING );
    
    //-------------------------------------------------------------------------------------------------//
    void ContributionVoucherDM::shutdown()
    {
		BLOG_TRACE_METHOD(narcotics::LoggerPool::loggerDomModules, "ContributionVoucherDM::shutdown()");

		if( m_Initialized == 0 ){
			return;
		}
		m_Initialized--;
		if( 0 == m_Initialized )
		{
			m_ContributionVoucherACC.reset();
			m_ManualContributionVoucherACC.reset();
			m_DeliveryNoteCopyACC.reset();
			m_NoAcknowledgementACC.reset();
			m_OrderACC.reset();
			m_SearchACC.reset();
			m_PrintSearchACC.reset();
			m_NewBookingsACC.reset();
			m_CustomerSupplierACC.reset();
		}
    }

	//-------------------------------------------------------------------------------------------------//
    void ContributionVoucherDM::splitScannedVoucherNo( const basar::VarString& scannedVoucherNo, basar::Int32& voucherTransactionDate, basar::Int32& voucherNo )
    {
        basar::VarString contributionVoucherNoStr = scannedVoucherNo.substr(8,scannedVoucherNo.size());
        voucherNo = contributionVoucherNoStr.stoi();

        basar::VarString contributionVoucherTransactionDateStr = scannedVoucherNo.substr(0,8);
        voucherTransactionDate = contributionVoucherTransactionDateStr.stoi();
    }

	//-------------------------------------------------------------------------------------------------//
	/*!	\return	
		\param	branchno and acknowledgementstatus are used.
		\throw	no-throw */
	//-------------------------------------------------------------------------------------------------//
	AccessorPropertyTableRef ContributionVoucherDM::findOrderPositionsDelivery( basar::db::aspect::AccessorPropertyTable_YIterator deliveryNote )
	{
		BLOG_TRACE_METHOD(narcotics::LoggerPool::loggerDomModules, "ContributionVoucherDM::findOrderPositionsDelivery()");

		try
		{
			getDeliveryNoteCopyACC().execute( "SelOrderPositions", deliveryNote );
		}
		catch( basar::Exception& e )
		{
			e;
			std::stringstream ss;
			ss << "ContributionVoucherDM:: Exception caught while executing 'SelOrderPositions'.";
			narcotics::LoggerPool::loggerDomModules.forcedLog( log4cplus::ERROR_LOG_LEVEL, ss.str().c_str(), __FILE__, __LINE__ );
			throw;
		}
		return getDeliveryNoteCopyACC().getPropertyTable();
	}

	//-------------------------------------------------------------------------------------------------//
	/*!	\return	
		\param	branchno and acknowledgementstatus are used.
		\throw	no-throw */
	//-------------------------------------------------------------------------------------------------//
	//AccessorPropertyTableRef ContributionVoucherDM::findOrderPositionsAcknowledgement( basar::db::aspect::AccessorPropertyTable_YIterator acknowledgement )
	//{
	//	BLOG_TRACE_METHOD(narcotics::LoggerPool::loggerDomModules, "ContributionVoucherDM::findOrderPositionsAcknowledgement()");

	//	try
	//	{
	//		getNoAcknowledgementACC().execute( "SelOrderPositions", acknowledgement );
	//	}
	//	catch( basar::Exception& e )
	//	{
	//		e;
	//		std::stringstream ss;
	//		ss << "ContributionVoucherDM:: Exception caught while executing 'SelOrderPositions'.";
	//		narcotics::LoggerPool::loggerDomModules.forcedLog( log4cplus::ERROR_LOG_LEVEL, ss.str().c_str(), __FILE__, __LINE__ );
	//		throw;
	//	}
	//	return getNoAcknowledgementACC().getPropertyTable();
	//}

	//-------------------------------------------------------------------------------------------------//
    constants::ContributionVoucherScanResults ContributionVoucherDM::markContributionVoucherAsReceived( basar::VarString scannedContributionVoucherNo )
    {
        BLOG_TRACE_METHOD(narcotics::LoggerPool::loggerDomModules, "ContributionVoucherDM::markContributionVoucherAsReceived()");
        
        basar::db::aspect::ExecuteResultInfo ret;

        constants::ContributionVoucherScanResults scanResults = constants::INVALID_SCAN_RESULT;
        
        //done: split scanned no
        //todo: try to update status
        //rule: only if status is 2 update to 3
        //nothing affected: not found

        basar::Int32 contributionVoucherNo = 0;
        basar::Int32 contributionVoucherTransactionDate = 0;
        
        splitScannedVoucherNo(scannedContributionVoucherNo, contributionVoucherTransactionDate, contributionVoucherNo);

        //todo: get yit

        getContributionVoucherACC().getPropertyTable().clear();
        basar::db::aspect::AccessorPropertyTable_YIterator yit = getContributionVoucherACC().getPropertyTable().insert(basar::FOR_UNKNOWN);

        yit.setInt16( constants::BRANCHNO, m_BranchNo );
        yit.setInt32( constants::TRANSACTIONDATE, contributionVoucherTransactionDate );
        yit.setInt32( constants::CONTRIBUTIONVOUCHERNO, contributionVoucherNo );
        yit.setInt16( constants::FORMER_ACKNOWLEDGEMENTSTATUS, constants::PRINTED );
        yit.setInt16( constants::ACKNOWLEDGEMENTSTATUS, constants::ACKNOWLEDGEMENT_RECEIVED );
        
        bool transactionActive = true;
		
        getContributionVoucherACC().beginTransaction();
        
        try
        {
            //done: new accessmethod -> added new sql-string-builder, should work
            ret = getContributionVoucherACC().execute( "UpdAcknowledgementstatusOfOrder", yit );
  
			if( ret.hasError() )
			{
				getContributionVoucherACC().rollbackTransaction();
                return scanResults;
			}
			
            getContributionVoucherACC().commitTransaction();
		} 
        catch( const basar::Exception& e) 
		{             
            if( true == transactionActive ){
				getContributionVoucherACC().rollbackTransaction();
			}
            throw basar::Exception(basar::ExceptInfo("ContributionVoucherDM::markContributionVoucherAsReceived", e.what(), __FILE__, __LINE__), "UpdateAcknowledgementException");
        }

        if( ret.getAffectedRows() > 0 ){
            scanResults = constants::MARKED_AS_RECEIVED;
        }
        else
        {
            //todo: check if voucher exists and if it does, if voucher is already scanned
            bool voucherAlreadyMarked = false;
            
            try
            {
                voucherAlreadyMarked = hasContributionVoucherAlreadyBeenReceived(scannedContributionVoucherNo);
            }
            catch( const basar::Exception& e ) 
            {
                throw basar::Exception(basar::ExceptInfo("ContributionVoucherDM::markContributionVoucherAsReceived", e.what(), __FILE__, __LINE__), "UpdateAcknowledgementStatusException");
            }

            if( voucherAlreadyMarked ){
                scanResults = constants::VOUCHER_ALREADY_MARKED;
            }
            else{
                scanResults = constants::VOUCHER_NOT_FOUND_IN_DB;
            }
        }

        return scanResults;
    }

	//-------------------------------------------------------------------------------------------------//
	/*!	\return	
		\param	branchno and acknowledgementstatus are used.
		\throw	no-throw */
	//-------------------------------------------------------------------------------------------------//
	AccessorPropertyTableRef ContributionVoucherDM::findOrderPositionsRegister( basar::db::aspect::AccessorPropertyTable_YIterator orderIt )
	{
		BLOG_TRACE_METHOD(narcotics::LoggerPool::loggerDomModules, "ContributionVoucherDM::findOrderPositionsRegister()");

		try
		{
			getOrderACC().execute( "SelOrderPositions", orderIt );
		}
		catch( basar::Exception& e )
		{
			e;
			std::stringstream ss;
			ss << "ContributionVoucherDM:: Exception caught while executing 'SelOrderPositions'.";
			narcotics::LoggerPool::loggerDomModules.forcedLog( log4cplus::ERROR_LOG_LEVEL, ss.str().c_str(), __FILE__, __LINE__ );
			throw;
		}
		return getOrderACC().getPropertyTable();
	}

	//-------------------------------------------------------------------------------------------------//
	/*!	\return	
		\param	contributionvoucherno and transactiondate are used.
		\throw	no-throw */
	//-------------------------------------------------------------------------------------------------//
	AccessorPropertyTableRef ContributionVoucherDM::findAcknowledgements( basar::db::aspect::AccessorPropertyTable_YIterator searchIt )
	{
		BLOG_TRACE_METHOD(narcotics::LoggerPool::loggerDomModules, "ContributionVoucherDM::findAcknowledgements()");

        //todo: check if we got a scanned contrib voucher no, split it
        if( searchIt.isContainedAndSet(constants::SCANNED_CONTRIBUTION_VOUCHER_NO) )
        {
            basar::Int32 contributionVoucherNo = 0;
            basar::Int32 contributionVoucherTransactionDate = 0;
        
            splitScannedVoucherNo(searchIt.getString(constants::SCANNED_CONTRIBUTION_VOUCHER_NO), contributionVoucherTransactionDate, contributionVoucherNo);

            searchIt.setInt32(constants::TRANSACTIONDATE, contributionVoucherTransactionDate);
            searchIt.setInt32(constants::CONTRIBUTIONVOUCHERNO, contributionVoucherNo);
        }

        try {
            getNoAcknowledgementACC().execute( "SelOrderUnion", searchIt );
		} 
		catch( basar::Exception& e )
		{
			e;
			std::stringstream ss;
			ss << "ContributionVoucherDM:: Exception caught while executing 'SelOrder'.";
			narcotics::LoggerPool::loggerDomModules.forcedLog( log4cplus::ERROR_LOG_LEVEL, ss.str().c_str(), __FILE__, __LINE__ );
			throw;
		}
		return getNoAcknowledgementACC().getPropertyTable();
	}

	//-------------------------------------------------------------------------------------------------//
	/*!	\return	
		\param	catalogid is used for identifying target booking in order to set current acknowledgementstatus
				bool delivery is used to decide whether delivery or acknowledgement accessor is used
		\throw	no-throw */
	//-------------------------------------------------------------------------------------------------//
	basar::db::aspect::ExecuteResultInfo ContributionVoucherDM::setAcknowledgementstatus( basar::db::aspect::AccessorPropertyTableRef newState )
	{
		BLOG_TRACE_METHOD(narcotics::LoggerPool::loggerDomModules, "ContributionVoucherDM::setAcknowledgementstatus()");

		basar::db::aspect::ExecuteResultInfo ret;
		basar::db::aspect::AccessorPropertyTable_YIterator newStateIt = newState.begin();
		// open transaction for updating whole propertytable
		bool transactionActive = true;
		getContributionVoucherACC().beginTransaction();

		try
		{
			while( !newStateIt.isEnd() )
			{
                ret = getContributionVoucherACC().execute("UpdAcknowledgementstatus", newStateIt, true, false);
				if( ret.hasError() )
				{
					getContributionVoucherACC().rollbackTransaction();
					return ret;
				}
				else{
                    ++newStateIt;
				}
			}
			getContributionVoucherACC().commitTransaction();
		} 
		catch( const basar::Exception& ) 
		{ 
			if( true == transactionActive ){
				getContributionVoucherACC().rollbackTransaction();
			}
		}
		return ret;
	}

	//-------------------------------------------------------------------------------------------------//
	/*!	\return	
		\param	catalogid is used for identifying target booking in order to set current acknowledgementstatus
				bool delivery is used to decide whether delivery or acknowledgement accessor is used
		\throw	no-throw */
	//-------------------------------------------------------------------------------------------------//
    basar::db::aspect::ExecuteResultInfo ContributionVoucherDM::setAcknowledgementstatus( basar::db::aspect::AccessorPropertyTable_YIterator newStateYit )
	{
		BLOG_TRACE_METHOD(narcotics::LoggerPool::loggerDomModules, "ContributionVoucherDM::setAcknowledgementstatus()");

		basar::db::aspect::ExecuteResultInfo ret;
		// open transaction for updating whole propertytable
		bool transactionActive = true;
		getContributionVoucherACC().beginTransaction();

		try
		{
			ret = getContributionVoucherACC().execute("UpdAcknowledgementstatus", newStateYit, true, false);
			if( ret.hasError() )
			{
				getContributionVoucherACC().rollbackTransaction();
				return ret;
			}
			getContributionVoucherACC().commitTransaction();
		} 
		catch( const basar::Exception& ) 
		{ 
			if( true == transactionActive ){
				getContributionVoucherACC().rollbackTransaction();
			}
		}
		return ret;
	}

	//-------------------------------------------------------------------------------------------------//
	/*!	\return	
		\param	AccessorPropertyTable_YIterator pointing to entry containing purchaseordernumber, customerordernumber, 
				branchno and acknowledgmentstatus of a printed order
		\throw	no-throw */
	//-------------------------------------------------------------------------------------------------//
	basar::db::aspect::ExecuteResultInfo ContributionVoucherDM::setAcknowledgementstatusOfOrder( basar::db::aspect::AccessorPropertyTable_YIterator yitNewState )
	{
		BLOG_TRACE_METHOD(narcotics::LoggerPool::loggerDomModules, "ContributionVoucherDM::setAcknowledgementstatus()");

		basar::db::aspect::ExecuteResultInfo ret;
		// open transaction for updating whole propertytable
		bool transactionActive = true;
		getContributionVoucherACC().beginTransaction();

		try
		{
			if( !yitNewState.isNull() && !yitNewState.isEnd() )
			{
				ret = getContributionVoucherACC().execute("UpdAcknowledgementstatusOfOrder", yitNewState, true, false);
				if( ret.hasError() )
				{
					getContributionVoucherACC().rollbackTransaction();
					return ret;
				}
			}
			getContributionVoucherACC().commitTransaction();
		} 
		catch( const basar::Exception& ) 
		{ 
			if( true == transactionActive ){
				getContributionVoucherACC().rollbackTransaction();
			}
		}
		return ret;
	}

	//-------------------------------------------------------------------------------------------------//
	AccessorPropertyTableRef ContributionVoucherDM::getAllNewInsertedOrders( basar::db::aspect::AccessorPropertyTable_YIterator searchIt )
	{
		if( narcotics::LoggerPool::loggerDomModules.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
		{
			basar::VarString msg;
			msg.format( "ContributionVoucherDM.getAllNewInsertedOrders");
			narcotics::LoggerPool::loggerDomModules.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__);
		}

		try {
			getNewBookingsACC().execute( "SelAllNewInsertedBookings", searchIt );
		} 
		catch( basar::Exception& e )
		{
			e;
			std::stringstream ss;
			ss << "ContributionVoucherDM:: Exception caught while executing 'SelAllNewInsertedBookings'." << std::endl;
			ss << e.what();
			narcotics::LoggerPool::loggerDomModules.forcedLog( log4cplus::ERROR_LOG_LEVEL, ss.str().c_str(), __FILE__, __LINE__ );
			throw;
		}
		return getNewBookingsACC().getPropertyTable();
	}

	//-------------------------------------------------------------------------------------------------//
	AccessorPropertyTableRef ContributionVoucherDM::getNewInsertedOrderAndSetToInPrint( basar::db::aspect::AccessorPropertyTable_YIterator searchIt )
	{
		if( narcotics::LoggerPool::loggerDomModules.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
		{
			basar::VarString msg;
			msg.format( "ContributionVoucherDM.getNewInsertedOrder");
			narcotics::LoggerPool::loggerDomModules.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__);
		}

		try {
			getOrderACC().execute( "SetToInPrintAndSelNewInsertedBookingsEx", searchIt );
		} 
		catch( basar::Exception& e )
		{
			e;
			std::stringstream ss;
			ss << "ContributionVoucherDM:: Exception caught while executing 'SetToInPrintAndSelNewInsertedBookingsEx'." << std::endl;
			ss << e.what();
			narcotics::LoggerPool::loggerDomModules.forcedLog( log4cplus::ERROR_LOG_LEVEL, ss.str().c_str(), __FILE__, __LINE__ );
			throw;
		}
		return getOrderACC().getPropertyTable();
	}

	//-------------------------------------------------------------------------------------------------//
	//AccessorPropertyTableRef ContributionVoucherDM::getNewInsertedBooking( basar::db::aspect::AccessorPropertyTable_YIterator searchIt )
	//{
	//	if( narcotics::LoggerPool::loggerDomModules.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	//	{
	//		basar::VarString msg;
	//		msg.format( "ContributionVoucherDM.getNewInsertedBookings");
	//		narcotics::LoggerPool::loggerDomModules.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__);
	//	}

	//	try {
	//		getNewBookingsACC().execute( "SelNewInsertedBooking", searchIt );
	//	} 
	//	catch( basar::Exception& e )
	//	{
	//		e;
	//		std::stringstream ss;
	//		ss << "ContributionVoucherDM:: Exception caught while executing 'SelNewInsertedBookings'." << std::endl;
	//		ss << e.what();
	//		narcotics::LoggerPool::loggerDomModules.forcedLog( log4cplus::ERROR_LOG_LEVEL, ss.str().c_str(), __FILE__, __LINE__ );
	//		throw;
	//	}
	//	return getNewBookingsACC().getPropertyTable();
	//}

	//-------------------------------------------------------------------------------------------------//
    void ContributionVoucherDM::resetNotPrintedBookings( basar::db::aspect::AccessorPropertyTable_YIterator resetNotPrintedBookingsIt )
    {
		//need two yits here, one for selecting all datasets, the other for perfoming update
        //to make resetNotPrintedBookings order-based, a not exists query is needed
        if( narcotics::LoggerPool::loggerDomModules.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) ){
			basar::VarString msg;
			msg.format( "ContributionVoucherDM.resetNotPrintedBookings");
			narcotics::LoggerPool::loggerDomModules.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__);
		}
		try {
			getNewBookingsACC().execute( "ResetNotYetPrintedBooking", resetNotPrintedBookingsIt );
		} catch( basar::Exception& e ){
			e;
			std::stringstream ss;
			ss << "ContributionVoucherDM:: Exception caught while executing 'ResetNotYetPrintedBooking'." << std::endl;
			ss << e.what();
			narcotics::LoggerPool::loggerDomModules.forcedLog( log4cplus::ERROR_LOG_LEVEL, ss.str().c_str(), __FILE__, __LINE__ );
			throw;
		}
    }

	//-------------------------------------------------------------------------------------------------//
    bool ContributionVoucherDM::areBookingsInPrint( basar::db::aspect::AccessorPropertyTable_YIterator checkForInPrintBookings )
    {
        if( narcotics::LoggerPool::loggerDomModules.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
		{
			basar::VarString msg;
			msg.format( "ContributionVoucherDM.checkForInPrintBookings");
			narcotics::LoggerPool::loggerDomModules.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__);
		}

        bool retVal = false;

		try 
		{
			basar::db::aspect::ExecuteResultInfo ret = getNewBookingsACC().execute( "CheckForInPrintBookings", checkForInPrintBookings );      

            if( !getNewBookingsACC().getPropertyTable().empty() )
            {
                basar::db::aspect::AccessorPropertyTable_YIterator numberOfInPrintBookingsYit = getNewBookingsACC().getPropertyTable().begin(); 
                
                if( numberOfInPrintBookingsYit.getDecimal("numberofinprintbookings") != basar::Decimal(0.0) ){
                    retVal = true;
                }
            } 
		} 
		catch( basar::Exception& e )
		{
			e;
			std::stringstream ss;
			ss << "ContributionVoucherDM:: Exception caught while executing 'CheckForInPrintBookings'." << std::endl;
			ss << e.what();
			narcotics::LoggerPool::loggerDomModules.forcedLog( log4cplus::ERROR_LOG_LEVEL, ss.str().c_str(), __FILE__, __LINE__ );
			throw;
		}

        return retVal;
    }

	//-------------------------------------------------------------------------------------------------//
    bool ContributionVoucherDM::isContributionVoucherStillInPrint( basar::db::aspect::AccessorPropertyTable_YIterator checkForInPrintVoucher )
    {
        if( narcotics::LoggerPool::loggerDomModules.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
		{
			basar::VarString msg;
			msg.format( "ContributionVoucherDM.isContributionVoucherStillInPrint");
			narcotics::LoggerPool::loggerDomModules.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__);
		}

        bool retVal = false;

        try 
		{
			basar::db::aspect::ExecuteResultInfo ret = getNewBookingsACC().execute( "CheckForInPrintBookings", checkForInPrintVoucher );

			if( !getNewBookingsACC().getPropertyTable().empty() )
            {
                basar::db::aspect::AccessorPropertyTable_YIterator numberOfInPrintBookingsYit = getNewBookingsACC().getPropertyTable().begin(); 
                
                if( numberOfInPrintBookingsYit.getDecimal("numberofinprintbookings") != basar::Decimal(0.0) ){
                    retVal = true;
                }
            }
		} 
		catch( basar::Exception& e )
		{
			e;
			std::stringstream ss;
			ss << "ContributionVoucherDM:: Exception caught while executing 'isContributionVoucherStillInPrint'." << std::endl;
			ss << e.what();
			narcotics::LoggerPool::loggerDomModules.forcedLog( log4cplus::ERROR_LOG_LEVEL, ss.str().c_str(), __FILE__, __LINE__ );
			throw;
		}

        return retVal;
    }

	//-------------------------------------------------------------------------------------------------//
    bool ContributionVoucherDM::hasContributionVoucherAlreadyBeenReceived( basar::VarString scannedContributionVoucherNo )
    {
        BLOG_TRACE_METHOD(narcotics::LoggerPool::loggerDomModules, "ContributionVoucherDM::hasContributionVoucherAlreadyBeenReceived()");
        
        basar::db::aspect::ExecuteResultInfo res;

        basar::Int32 contributionVoucherNo = 0;
        basar::Int32 contributionVoucherTransactionDate = 0;
        
        splitScannedVoucherNo(scannedContributionVoucherNo, contributionVoucherTransactionDate, contributionVoucherNo);

        //todo: get yit

        getContributionVoucherACC().getPropertyTable().clear();
        basar::db::aspect::AccessorPropertyTable_YIterator yit = getContributionVoucherACC().getPropertyTable().insert(basar::FOR_UNKNOWN);

        yit.setInt16( constants::BRANCHNO, m_BranchNo );
        yit.setInt32( constants::TRANSACTIONDATE, contributionVoucherTransactionDate );
        yit.setInt32( constants::CONTRIBUTIONVOUCHERNO, contributionVoucherNo );
        yit.setInt16( constants::ACKNOWLEDGEMENTSTATUS, constants::ACKNOWLEDGEMENT_RECEIVED );

        try
        {
            res = getContributionVoucherACC().execute( "SelAcknowledgementStatus", yit );			
		} 
        catch( const basar::Exception& e) 
		{
			std::stringstream ss;
			ss << "ContributionVoucherDM:: Exception caught while executing 'hasContributionVoucherAlreadyBeenReceived'." << std::endl;
			ss << e.what();
			narcotics::LoggerPool::loggerDomModules.forcedLog( log4cplus::ERROR_LOG_LEVEL, ss.str().c_str(), __FILE__, __LINE__ );
            throw basar::Exception(basar::ExceptInfo("ContributionVoucherDM::hasContributionVoucherAlreadyBeenReceived", e.what(), __FILE__, __LINE__), "SelAcknowledgementStatusException");
        }

        basar::db::aspect::AccessorPropertyTable_YIterator yitAckStatus = getContributionVoucherACC().getPropertyTable().begin();
        
        bool ret = true;

        if( yitAckStatus.isNull() || yitAckStatus.isEnd() ){
            ret = false;
        }
        else
        {
            for( ; !yitAckStatus.isEnd(); ++yitAckStatus )
            {
                basar::Int16 ackStatus = yitAckStatus.getInt16( constants::ACKNOWLEDGEMENTSTATUS );
                ret &= (ackStatus > constants::PRINTED && ackStatus < constants::IN_PRINT);
            }
        }

        return ret;
    }

	//-------------------------------------------------------------------------------------------------//
    bool ContributionVoucherDM::checkUserHasPrintLock( basar::Int16 branchNo, const basar::VarString& userName )
    {
        if( narcotics::LoggerPool::loggerDomModules.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
		{
			basar::VarString msg;
			msg.format( "ContributionVoucherDM.checkUserHasPrintLock");
			narcotics::LoggerPool::loggerDomModules.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__);
		}

		try 
		{
            basar::VarString temp;
            temp.format( "branchno=%d; idbusinessobject=%d; businesstype=%s", branchNo, constants::PRINTLOCK_BUSINESSOBJECT_ID, constants::PRINTLOCK);
			basar::db::aspect::AccessorPropertyTable_YIterator searchIt = 
			basar::db::aspect::Manager::getInstance().createAccessorYIterator(static_cast<basar::I18nString>(temp));

            getSearchACC().execute( "FindPrintLockByBranch", searchIt );            
		} 
		catch( basar::Exception& e )
		{
			e;
			std::stringstream ss;
			ss << "ContributionVoucherDM:: Exception caught while executing 'checkUserHasPrintLock'." << std::endl;
			ss << e.what();
			narcotics::LoggerPool::loggerDomModules.forcedLog( log4cplus::ERROR_LOG_LEVEL, ss.str().c_str(), __FILE__, __LINE__ );
			throw;
		}

        basar::db::aspect::AccessorPropertyTable_YIterator yit = getSearchACC().getPropertyTable().begin();

        bool retVal = false;

        if( !yit.isNull() && !yit.isEnd() )
        {
            if( yit.contains(constants::USERNAME) )
            {
                if( yit.getString(constants::USERNAME) == userName ){
                    retVal = true;
                }
            }
        }

        return retVal;
    }

	//-------------------------------------------------------------------------------------------------//
    AccessorPropertyTableRef ContributionVoucherDM::findBookingsInPrint( basar::Int16 branchNo )
    {
        if( narcotics::LoggerPool::loggerDomModules.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
		{
			basar::VarString msg;
			msg.format( "ContributionVoucherDM.findBookingsInPrint");
			narcotics::LoggerPool::loggerDomModules.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__);
		}

		try 
		{
            basar::VarString temp;
            temp.format( "branchno=%d; acknowledgementstatus=%d;", branchNo, constants::IN_PRINT );
			basar::db::aspect::AccessorPropertyTable_YIterator searchIt = 
			basar::db::aspect::Manager::getInstance().createAccessorYIterator(static_cast<basar::I18nString>(temp));

            getSearchACC().execute( "FindBookingsInPrint", searchIt );            
		} 
		catch( basar::Exception& e )
		{
			e;
			std::stringstream ss;
			ss << "ContributionVoucherDM:: Exception caught while executing 'CheckForInPrintBookings'." << std::endl;
			ss << e.what();
			narcotics::LoggerPool::loggerDomModules.forcedLog( log4cplus::ERROR_LOG_LEVEL, ss.str().c_str(), __FILE__, __LINE__ );
			throw;
		}
        return getSearchACC().getPropertyTable();
    }

	//-------------------------------------------------------------------------------------------------
	basar::db::aspect::AccessorPropertyTable_YIterator ContributionVoucherDM::findPersonalDataByID( basar::db::aspect::AccessorPropertyTable_YIterator yit )
	{
		if( narcotics::LoggerPool::loggerDomModules.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
		{
			basar::ConstString msg = "TransferDM.findPersonalDataByID()";
			narcotics::LoggerPool::loggerDomModules.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
		}

		basar::db::aspect::AccessorPropertyTable_YIterator retYit;
		if( yit.getString(constants::TRANSACTIONTYPE) == TransactionType::GOODSIN_DELIVERY || 
			yit.getString(constants::TRANSACTIONTYPE) == TransactionType::SUPPLIER_RETURNS ) 
		{
			if( 0 <= getCustomerSupplierACC().execute( "SelSupplierForPrint", yit, true, false ).getAffectedRows() ){
					retYit = getCustomerSupplierACC().getPropertyTable().begin();
			}
		}
		else if( yit.getString(constants::TRANSACTIONTYPE) == TransactionType::CUSTOMER_ORDER || 
			yit.getString(constants::TRANSACTIONTYPE) == TransactionType::CUSTOMER_RETURNS ) 
		{
			if( 0 <= getCustomerSupplierACC().execute( "SelCustomerForPrint", yit, true, false ).getAffectedRows()){
					retYit = getCustomerSupplierACC().getPropertyTable().begin();
			}
		}

		if( narcotics::LoggerPool::loggerDomModules.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
		{
			basar::ConstString msg = "leave TransferDM.findPersonalDataByID()";
			narcotics::LoggerPool::loggerDomModules.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
		}
			return retYit;
	}

	//-------------------------------------------------------------------------------------------------
	basar::db::aspect::ExecuteResultInfo ContributionVoucherDM::setContributionVoucherNo( basar::db::aspect::AccessorPropertyTable_YIterator voucherNo )
	{
		if( narcotics::LoggerPool::loggerDomModules.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
		{
			basar::VarString msg;
			msg.format( "TransferDM.setContributionVoucherNo" );
			narcotics::LoggerPool::loggerDomModules.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__);
		}
		
		LOG_YIT("",narcotics::LoggerPool::loggerNarcotics,voucherNo);
		
		return getContributionVoucherACC().execute( "UpdContributionVoucherNo", voucherNo );
	}

	//-------------------------------------------------------------------------------------------------
	// get Contributionvoucher propertytable
	//-------------------------------------------------------------------------------------------------
    AccessorPropertyTableRef ContributionVoucherDM::getAccContributionVoucherPropTab()
    {
		return getContributionVoucherACC().getPropertyTable();
    }

	//-------------------------------------------------------------------------------------------------
	// get manual Contributionvoucher propertytable
	//-------------------------------------------------------------------------------------------------
    AccessorPropertyTableRef ContributionVoucherDM::getAccManualContributionVoucherPropTab()
    {
		return getManualContributionVoucherACC().getPropertyTable();
    }

	//-------------------------------------------------------------------------------------------------
	// clear Contributionvoucher propertytable
	//-------------------------------------------------------------------------------------------------
    void ContributionVoucherDM::clearContributionVoucherPropTab()
    {
		getContributionVoucherACC().getPropertyTable().clear();
    }

	//-------------------------------------------------------------------------------------------------
	// clear manual Contributionvoucher propertytable
	//-------------------------------------------------------------------------------------------------
    void ContributionVoucherDM::clearManualContributionVoucherPropTab()
    {
		getManualContributionVoucherACC().getPropertyTable().clear();
    }

	//-------------------------------------------------------------------------------------------------
	// get DeliveryNoteCopy propertytable
	//-------------------------------------------------------------------------------------------------
  //  AccessorPropertyTableRef ContributionVoucherDM::getAccDeliveryNoteCopyPropTab()
  //  {
		//return getDeliveryNoteCopyACC().getPropertyTable();
  //  }

	//-------------------------------------------------------------------------------------------------
	// clear DeliveryNoteCopy propertytable
	//-------------------------------------------------------------------------------------------------
  //  void ContributionVoucherDM::clearDeliveryNoteCopyPropTab()
  //  {
		//getDeliveryNoteCopyACC().getPropertyTable().clear();
  //  }

	//-------------------------------------------------------------------------------------------------
	// get NoAcknowledgement propertytable
	//-------------------------------------------------------------------------------------------------
    AccessorPropertyTableRef ContributionVoucherDM::getAccNoAcknowledgementPropTab()
    {
		return getNoAcknowledgementACC().getPropertyTable();
    }

	//-------------------------------------------------------------------------------------------------
	// clear NoAcknowledgement propertytable
	//-------------------------------------------------------------------------------------------------
    void ContributionVoucherDM::clearNoAcknowledgementPropTab()
    {
		getNoAcknowledgementACC().getPropertyTable().clear();
    }

	//-------------------------------------------------------------------------------------------------
	// get order propertytable
	//-------------------------------------------------------------------------------------------------
    AccessorPropertyTableRef ContributionVoucherDM::getAccOrderPropTab()
    {
		return getOrderACC().getPropertyTable();
    }

	//-------------------------------------------------------------------------------------------------
	// clear order propertytable
	//-------------------------------------------------------------------------------------------------
    void ContributionVoucherDM::clearOrderPropTab()
    {
		getOrderACC().getPropertyTable().clear();
    }

	//-------------------------------------------------------------------------------------------------
	// get Search propertytable
	//-------------------------------------------------------------------------------------------------
    AccessorPropertyTableRef ContributionVoucherDM::getAccSearchPropTab()
    {
		return getSearchACC().getPropertyTable();
    }

	//-------------------------------------------------------------------------------------------------
	// get Print Search propertytable
	//-------------------------------------------------------------------------------------------------
    AccessorPropertyTableRef ContributionVoucherDM::getAccPrintSearchPropTab()
    {
		return getPrintSearchACC().getPropertyTable();
    }

	//-------------------------------------------------------------------------------------------------
	// clear Search propertytable
	//-------------------------------------------------------------------------------------------------
    void ContributionVoucherDM::clearSearchPropTab()
    {
		getSearchACC().getPropertyTable().clear();
    }

	//-------------------------------------------------------------------------------------------------
	// get NewBookings propertytable
	//-------------------------------------------------------------------------------------------------
    AccessorPropertyTableRef ContributionVoucherDM::getAccNewBookingsPropTab()
    {
		return getNewBookingsACC().getPropertyTable();
    }

	//-------------------------------------------------------------------------------------------------
	// clear NewBookings propertytable
	//-------------------------------------------------------------------------------------------------
    void ContributionVoucherDM::clearNewBookingsPropTab()
    {
		getNewBookingsACC().getPropertyTable().clear();
    }

} //namespace domMod