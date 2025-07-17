//-------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  implemtation of domain module "narcotics"
 *  \author	Roland Kiefert
 *  \date   22.02.2006
 */
//-------------------------------------------------------------------------------------------------//
// includes
//-------------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "narcoticsmodule.h"
#include "loggerpool.h"
#include "halfyearperiodhelper.h"
#include "libbasarcmnutil_logging.h"
//#include "branchlockmanager.h"

//-------------------------------------------------------------------------------------------------//
// namespaces
//-------------------------------------------------------------------------------------------------//
namespace domMod
{

//-------------------------------------------------------------------------------------------------//
// using declarations
//-------------------------------------------------------------------------------------------------//
using basar::db::aspect::AccessorInstanceRef;
using basar::db::aspect::AccessorPropertyTableRef;
using basar::db::aspect::AccessorPropertyTable_YIterator;

//-------------------------------------------------------------------------------------------------//
// static variables
// HINT:	with creating a new static variable for using it with the member 
//			'basar::db::aspect::AccessorPropertyTable_YIterator	m_searchIterator;' 
//			consider the following 3 issues:
//			1. creation of the attribute
//			2. think of adding the new attribute to the yiterator in the method 'init()'
//			3. and the reseting of the same attribute in the method 'clearSearchIterator()'
//-------------------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------------------//
/*!	\throw		no-throw */
//-------------------------------------------------------------------------------------------------//
NarcoticsDM::NarcoticsDM()
:m_Initialized( 0 )
{
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw		no-throw */
//-------------------------------------------------------------------------------------------------//
NarcoticsDM::~NarcoticsDM()
{
	shutdown();
}

//-------------------------------------------------------------------------------------------------//
//! \return		void 
//! \param		branchNo is used in several method so it is set to a member.
//! \param		activeConn important for the creating process of every AccessorInstanceRef.
//! \param		userName is used for the Log modul.
/*!	\throw		no-throw */
//-------------------------------------------------------------------------------------------------//
void NarcoticsDM::init( const basar::Int16 branchNo, const basar::db::aspect::ConnectionRef activeConn, const basar::VarString& userName )
{
	if( narcotics::LoggerPool::loggerDomModules.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::VarString msg;
		msg.format("init of NarcoticsDM. Parameter: branchNo : %d; userName : %s", branchNo, userName.c_str());
		narcotics::LoggerPool::loggerDomModules.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__);
	}

	if( 0 == m_Initialized )
	{
		m_ConnectionRef = activeConn;
		m_BranchNo = branchNo;
		basar::VarString branch;
		branch.format( "%s=%i; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s;",
						constants::BRANCHNO, m_BranchNo, 
						constants::FROMDATE, 
						constants::TODATE, 
						constants::ARTICLENO, 
						constants::ARTICLECODE, 
						constants::ARTICLENAME,
						constants::PURCHASEORDERNO, 
						constants::TRANSACTIONTYPE, 
						constants::DELETEDFLAG, 
						constants::PROCESSEDFLAG, 
						constants::CUSTOMERSUPPLIERNO,
						constants::SUPPLIERNO,
						constants::INTERNAL_ACCOUNTS, 
						constants::CUSTOMER_SUPPLIER_NAME,
						constants::TRANSACTIONDATE,        
						constants::SUPPLIERNAME,
						constants::CUSTOMERNAME,
						constants::NARCOTICNAME,
						constants::GROUP,
						constants::SHOW_LOCKED_CUST);

		m_searchIterator = basar::db::aspect::Manager::getInstance().createAccessorYIterator( static_cast<basar::I18nString>(branch) );
		m_LogDM.init( activeConn, branchNo, userName ); 
		m_LockManager = boost::shared_ptr<libnarcotics::lockmanager::BranchLockManager>(
			new libnarcotics::lockmanager::BranchLockManager(branchNo, activeConn, userName, constants::TRANSFERLOCK_TABLE_NAME, narcotics::LoggerPool::loggerDomModules));
	}
	m_Initialized++;
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
// Lazy Instantiation of Accessors in this Section: Getter-Methods defined here, won't be executed before really being needed
// Consequently, Accessors won't be initialized before being needed
CREATE_ACCESSOR_INSTANCE_DEF( NarcoticsDM, NarCatalog, BookingCatalog, basar::db::aspect::ON_DEMAND_CACHING					);
CREATE_ACCESSOR_INSTANCE_DEF( NarcoticsDM, InitialInventory, BookingCatalog, basar::db::aspect::ON_DEMAND_CACHING			);
CREATE_ACCESSOR_INSTANCE_DEF( NarcoticsDM, UpdQtyInStock, BookingCatalog, basar::db::aspect::ON_DEMAND_CACHING				);
CREATE_ACCESSOR_INSTANCE_DEF( NarcoticsDM, CompareBooking, BookingCatalog, basar::db::aspect::ON_DEMAND_CACHING				);
CREATE_ACCESSOR_INSTANCE_DEF( NarcoticsDM, NarcoticsSupplier, NarcoticsSupplier, basar::db::aspect::FULL_CACHING			);
CREATE_ACCESSOR_INSTANCE_DEF( NarcoticsDM, NumberIncomingGoods, NumberIncomingGoods, basar::db::aspect::ON_DEMAND_CACHING	);
CREATE_ACCESSOR_INSTANCE_DEF( NarcoticsDM, NumberReturnGoods, NumberReturnGoods, basar::db::aspect::ON_DEMAND_CACHING		);
CREATE_ACCESSOR_INSTANCE_DEF( NarcoticsDM, DiffQtyInStock, DiffQtyInStock, basar::db::aspect::ON_DEMAND_CACHING				);
CREATE_ACCESSOR_INSTANCE_DEF( NarcoticsDM, ArticleNosDiffQtyInStock, DiffQtyInStock, basar::db::aspect::ON_DEMAND_CACHING	);
CREATE_ACCESSOR_INSTANCE_DEF( NarcoticsDM, NarcLock, Locks, basar::db::aspect::ON_DEMAND_CACHING							);
CREATE_ACCESSOR_INSTANCE_DEF( NarcoticsDM, InventoryCountData, BookingCatalog, basar::db::aspect::ON_DEMAND_CACHING			);
CREATE_ACCESSOR_INSTANCE_DEF( NarcoticsDM, PrintNarcBook, BookingCatalog, basar::db::aspect::ON_DEMAND_CACHING				);
CREATE_ACCESSOR_INSTANCE_DEF( NarcoticsDM, PrevBooking, BookingCatalog, basar::db::aspect::ON_DEMAND_CACHING				);
CREATE_ACCESSOR_INSTANCE_DEF( NarcoticsDM, NarcArticle, BookingCatalog, basar::db::aspect::ON_DEMAND_CACHING				);
CREATE_ACCESSOR_INSTANCE_DEF( NarcoticsDM, BookingControl, BookingsControl, basar::db::aspect::ON_DEMAND_CACHING			);
CREATE_ACCESSOR_INSTANCE_DEF( NarcoticsDM, TempCatalog, BookingCatalog, basar::db::aspect::ON_DEMAND_CACHING				);

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
AccessorPropertyTable_YIterator NarcoticsDM::getSearchIterator()
{
	return m_searchIterator;
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
void NarcoticsDM::resetSearchIterator()
{
	basar::ConstString fun = "NarcoticsDM::resetSearchIterator()";
    BLOG_TRACE_METHOD( narcotics::LoggerPool::loggerDomModules, fun );

    basar::db::aspect::AccessorPropertyTable_XIterator xit = m_searchIterator.begin();
    for( ; !xit.isEnd(); ++xit )
	{
        if( constants::BRANCHNO == xit.getName() ){ 
			continue; 
		}
        xit.setPropertyState( basar::SS_UNSET );
    }
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
void NarcoticsDM::shutdown()
{
	basar::ConstString fun = "NarcoticsDM::shutdown()";
    BLOG_TRACE_METHOD( narcotics::LoggerPool::loggerDomModules, fun );
    
	if( m_Initialized == 0 ){
		return;
	}
	m_Initialized--;
	if( m_Initialized == 0 )
	{
		FREE_ACCESSOR( m_NarCatalogACC );
		FREE_ACCESSOR( m_InitialInventoryACC );
		FREE_ACCESSOR( m_NumberIncomingGoodsACC );
		FREE_ACCESSOR( m_NumberReturnGoodsACC );
		FREE_ACCESSOR( m_NarcoticsSupplierACC );
		FREE_ACCESSOR( m_UpdQtyInStockACC );
		FREE_ACCESSOR( m_DiffQtyInStockACC );
		FREE_ACCESSOR( m_ArticleNosDiffQtyInStockACC );
		FREE_ACCESSOR( m_NarcLockACC );
		FREE_ACCESSOR( m_CompareBookingACC );
		FREE_ACCESSOR( m_InventoryCountDataACC );
		FREE_ACCESSOR( m_PrintNarcBookACC );
		FREE_ACCESSOR( m_PrevBookingACC );
		FREE_ACCESSOR( m_NarcArticleACC );
		FREE_ACCESSOR( m_BookingControlACC );
		FREE_ACCESSOR( m_TempCatalogACC );

		m_LogDM.shutdown();
	}
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
const bool NarcoticsDM::isSupplierReplaceable()
{
    basar::ConstString fun = "NarcoticsDM::isSupplierReplaceable()";
    BLOG_TRACE_METHOD( narcotics::LoggerPool::loggerDomModules, fun );

    bool replaceable = false;
    if( getNarCatalogACC().getPropertyTable().empty() ){ 
		return replaceable; 
	}

	AccessorPropertyTable_YIterator yitIncomingGood = getNarCatalogACC().getPropertyTable().begin();
	basar::Int32 purchaseorderno = yitIncomingGood.getInt32( constants::PURCHASEORDERNO );
    for( ; !yitIncomingGood.isEnd(); ++yitIncomingGood )
	{
        if( purchaseorderno != yitIncomingGood.getInt32(constants::PURCHASEORDERNO) ){ break; }
        if( TransactionType::GOODSIN_DELIVERY != yitIncomingGood.getString(constants::TRANSACTIONTYPE) ){ break; }
    }

    if( yitIncomingGood.isEnd() ){ 
		replaceable = true; 
	}
    return replaceable;
}

//-------------------------------------------------------------------------------------------------//
//!	\param	yit is the accessorPropertyTable the values of the sql statement
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
AccessorPropertyTableRef NarcoticsDM::findCatalogByPattern( AccessorPropertyTable_YIterator yit )
{
    basar::ConstString fun = "NarcoticsDM::findCatalogByPattern()";
    BLOG_TRACE_METHOD( narcotics::LoggerPool::loggerDomModules, fun );

    try {
		getNarCatalogACC().execute( "SelCatalog", yit, true, true, static_cast<basar::db::aspect::DatabaseHintEnum>(basar::db::aspect::FULL_CACHING) );
    } 
	catch( basar::Exception& e )
	{
        e;
        std::stringstream ss;
        ss << "NarcoticsDM::findCatalogByPattern: Exception caught while executing 'SelCatalog'.";
        narcotics::LoggerPool::loggerDomModules.forcedLog( log4cplus::ERROR_LOG_LEVEL, ss.str().c_str(), __FILE__, __LINE__ );
        throw;
    }
	
    return getNarCatalogACC().getPropertyTable();
}

//-------------------------------------------------------------------------------------------------//
//!	\param	yit is the accessorPropertyTable the values of the sql statement
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
AccessorPropertyTableRef NarcoticsDM::findDoubleBookedBookings( AccessorPropertyTable_YIterator yit )
{
	if( narcotics::LoggerPool::loggerDomModules.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::VarString msg = "findDoubleBookedBookings of NarcoticsDM. Parameter: \n ";
		for( basar::db::aspect::AccessorPropertyTable_XIterator xit = yit.begin(); xit != yit.end(); xit++ )
		{
			if( !(xit.getState() == basar::SS_UNSET) ){
				msg = msg + xit.getName() + ":" + xit.getSQLString() + "\n";
			}
		}
		narcotics::LoggerPool::loggerDomModules.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__);
	}
    getNarCatalogACC().execute( "SelDoubleBookings", yit, true, true, basar::db::aspect::HOLD_RECORDSET );
	return getNarCatalogACC().getPropertyTable();
}

//-------------------------------------------------------------------------------------------------//
//!	\param	yit is the accessorPropertyTable the values of the sql statement
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
AccessorPropertyTableRef NarcoticsDM::findUncompleteBookings( AccessorPropertyTable_YIterator yit )
{
	if( narcotics::LoggerPool::loggerDomModules.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::VarString msg = "findUncompleteBookings of NarcoticsDM. Parameter: \n ";
		for( basar::db::aspect::AccessorPropertyTable_XIterator xit = yit.begin(); xit != yit.end(); xit++ )
		{
			if( !(xit.getState() == basar::SS_UNSET) ){
				msg = msg + xit.getName() + ":" + xit.getSQLString() + "\n";
			}
		}
		narcotics::LoggerPool::loggerDomModules.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__);
	}
	getNarCatalogACC().execute( "SelIncomplete", yit );
	return getNarCatalogACC().getPropertyTable();
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
AccessorPropertyTableRef NarcoticsDM::findTempCatalogByPattern( AccessorPropertyTable_YIterator yit )
{
	if( narcotics::LoggerPool::loggerDomModules.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::VarString msg = "findTempCatalogByPattern of NarcoticsDM. Parameter: \n ";
		for( basar::db::aspect::AccessorPropertyTable_XIterator xit = yit.begin(); xit != yit.end(); xit++ )
		{
			if( !(xit.getState() == basar::SS_UNSET) ){
				msg = msg + xit.getName() + ":" + xit.getSQLString() + "\n";
			}
		}
		narcotics::LoggerPool::loggerDomModules.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__);
	}
	getTempCatalogACC().execute( "SelCatalog", yit );
	return getTempCatalogACC().getPropertyTable();
}

//-------------------------------------------------------------------------------------------------//
//!	\param	yit is the accessorPropertyTable the values of the sql statement
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
AccessorPropertyTableRef NarcoticsDM::findBookingControlByPattern( AccessorPropertyTable_YIterator yit )
{
	if( narcotics::LoggerPool::loggerDomModules.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) ){
		basar::VarString msg = "findBookingControlByPattern of NarcoticsDM. Parameter: \n ";
		for( basar::db::aspect::AccessorPropertyTable_XIterator xit = yit.begin(); xit != yit.end(); ++xit )
		{
			if ( !(xit.getState() == basar::SS_UNSET) ){
				msg = msg + xit.getName() + ":" + xit.getSQLString() + "\n";
			}
		}
		narcotics::LoggerPool::loggerDomModules.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__);
	}
	getBookingControlACC().execute( "SelNarcBookingsForInquiry", yit );
	return getBookingControlACC().getPropertyTable();
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
AccessorPropertyTableRef NarcoticsDM::findNarcoticsSupplier()
{
    basar::ConstString fun = "NarcoticsDM::findNarcoticsSupplier()";
    BLOG_TRACE_METHOD( narcotics::LoggerPool::loggerDomModules, fun );

	basar::VarString temp;	
	temp.format("branchno=%d;",m_BranchNo);
    
	AccessorPropertyTable_YIterator yit = 
        basar::db::aspect::Manager::getInstance().createAccessorYIterator( static_cast<basar::I18nString>(temp) );

    AccessorPropertyTableRef suppliersPropTab;
    basar::db::aspect::ExecuteResultInfo result = getNarcoticsSupplierACC().execute( "SelSupplier", yit );
    if( result.hasError() ){ 
		return suppliersPropTab; 
	}

	result = getNarcoticsSupplierACC().execute( "SelPhoenixSupplier", yit, false );

    if( result.hasError() ){ 
		return suppliersPropTab; 
	}
	
    return getNarcoticsSupplierACC().getPropertyTable();
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
AccessorPropertyTable_YIterator	NarcoticsDM::findNarcoticsSupplierByBgaNo( const basar::Int32 bgaNo )
{
	if( narcotics::LoggerPool::loggerDomModules.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::VarString msg;
		msg.format("findNarcoticsSupplierByBgaNo of NarcoticsDM. Parameter: bgaNo : %d", bgaNo);
		narcotics::LoggerPool::loggerDomModules.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__);
	}

	basar::VarString where;
	where.format( "narcoticssupplierno=%d;", bgaNo );
	basar::db::aspect::AccessorPropertyTable_YIterator yit = 
        basar::db::aspect::Manager::getInstance().createAccessorYIterator( static_cast<basar::I18nString>(where) );
	getNarcoticsSupplierACC().execute( "SelSupplierByBgaNo", yit );

	return getNarcoticsSupplierACC().getPropertyTable().begin();
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
AccessorPropertyTableRef NarcoticsDM::findNarcoticsSupplierByName( const basar::VarString suppliername )
{
	if( narcotics::LoggerPool::loggerDomModules.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::VarString msg;
		msg.format( "findNarcoticsSupplierByName of NarcoticsDM. Parameter: suppliername : %d", suppliername.c_str() );
		narcotics::LoggerPool::loggerDomModules.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__);
	}

	basar::VarString where;
	where.format( "suppliername=%s;", suppliername.c_str() );
	basar::db::aspect::AccessorPropertyTable_YIterator yit = 
        basar::db::aspect::Manager::getInstance().createAccessorYIterator( static_cast<basar::I18nString>(where) );

	getNarcoticsSupplierACC().execute( "SelSupplierByName", yit );

	return getNarcoticsSupplierACC().getPropertyTable();
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
AccessorPropertyTableRef NarcoticsDM::findNoIncomingGoods()
{
    basar::ConstString fun = "IncomingGoodsUC::findNoIncomingGoods()";
    BLOG_TRACE( narcotics::LoggerPool::loggerDomModules, fun );

    basar::VarString temp;	
	temp.format("branchno=%d;",m_BranchNo);
	basar::db::aspect::AccessorPropertyTable_YIterator yit = 
        basar::db::aspect::Manager::getInstance().createAccessorYIterator(static_cast<basar::I18nString>(temp));
	
    AccessorPropertyTableRef incomingGoodsPropTab;
    basar::db::aspect::ExecuteResultInfo result = getNumberIncomingGoodsACC().execute( "SelIncominggoods", yit );
    if( result.hasError() ){ 
		return incomingGoodsPropTab; 
	}
    result = getNumberReturnGoodsACC().execute("SelReturngoods",yit);
    if( result.hasError() ){ 
		return incomingGoodsPropTab; 
	}

    return getNumberReturnGoodsACC().getPropertyTable();
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
AccessorPropertyTableRef NarcoticsDM::findDifferencesToOriginal()
{
	if( narcotics::LoggerPool::loggerDomModules.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "findDifferencesToOriginal of NarcoticsDM.";
		narcotics::LoggerPool::loggerDomModules.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	basar::VarString temp;
	temp.format( "branchno=%d;",m_BranchNo );
	basar::db::aspect::AccessorPropertyTable_YIterator yit = 
			basar::db::aspect::Manager::getInstance().createAccessorYIterator( static_cast<basar::I18nString>(temp) );
	getDiffQtyInStockACC().execute( "SelDiffQtyInStock",yit );
	return getDiffQtyInStockACC().getPropertyTable();
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
AccessorPropertyTableRef NarcoticsDM::findDifferencesToOriginalArticleNosOnly()
{
	if( narcotics::LoggerPool::loggerDomModules.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "findDifferencesToOriginalArticleNosOnly of NarcoticsDM.";
		narcotics::LoggerPool::loggerDomModules.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	basar::VarString temp;
	temp.format( "branchno=%d;",m_BranchNo );
	basar::db::aspect::AccessorPropertyTable_YIterator yit = 
			basar::db::aspect::Manager::getInstance().createAccessorYIterator( static_cast<basar::I18nString>(temp) );
	getArticleNosDiffQtyInStockACC().execute( "SelDiffQtyInStock", yit );
	return getArticleNosDiffQtyInStockACC().getPropertyTable();
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
AccessorPropertyTable_YIterator	NarcoticsDM::findPreviousBooking( basar::db::aspect::AccessorPropertyTable_YIterator yIt )
{
	if( narcotics::LoggerPool::loggerDomModules.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::VarString msg = "findPreviousBooking of NarcoticsDM. Parameter: \n ";
		for( basar::db::aspect::AccessorPropertyTable_XIterator xit = yIt.begin(); xit != yIt.end(); xit++ )
		{
			if( !(xit.getState() == basar::SS_UNSET) ){
				msg = msg + xit.getName() + ":" + xit.getSQLString() + "\n";
			}
		}
		narcotics::LoggerPool::loggerDomModules.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__);
	}
	getPrevBookingACC().execute( "SelQuickPrevForUpdQtyInStock", yIt );
	return getPrevBookingACC().getPropertyTable().begin();
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
AccessorPropertyTableRef NarcoticsDM::findData4InventoryCountPrint()
{
	if( narcotics::LoggerPool::loggerDomModules.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "findData4InventoryCountPrint of NarcoticsDM.";
		narcotics::LoggerPool::loggerDomModules.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	basar::VarString temp;
	temp.format("branchno=%d;",m_BranchNo);
	basar::db::aspect::AccessorPropertyTable_YIterator yit = 
			basar::db::aspect::Manager::getInstance().createAccessorYIterator( static_cast<basar::I18nString>(temp) );

	getInventoryCountDataACC().execute("SelectInventoryCountData", yit);
	
	return getInventoryCountDataACC().getPropertyTable();
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
AccessorPropertyTableRef NarcoticsDM::findData4InventoryStockPrint( AccessorPropertyTable_YIterator yit )
{
	if( narcotics::LoggerPool::loggerDomModules.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "findData4InventoryCountPrint of NarcoticsDM.";
		narcotics::LoggerPool::loggerDomModules.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	getInventoryCountDataACC().execute("SelectInventoryCountDataQuick", yit);
	
	return getInventoryCountDataACC().getPropertyTable();
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
AccessorPropertyTableRef NarcoticsDM::findNarcArticles( AccessorPropertyTable_YIterator yit )
{
	if( narcotics::LoggerPool::loggerDomModules.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::VarString msg = "findNarcArticles of NarcoticsDM. Parameter: \n ";
		for( basar::db::aspect::AccessorPropertyTable_XIterator xit = yit.begin(); xit != yit.end(); xit++ )
		{
			if( !(xit.getState() == basar::SS_UNSET) ){
				msg = msg + xit.getName() + ":" + xit.getSQLString() + "\n";
			}
		}
		narcotics::LoggerPool::loggerDomModules.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__);
	}
	getNarcArticleACC().execute("SelNarcArticle", yit);
	return getNarcArticleACC().getPropertyTable();
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
const bool NarcoticsDM::checkBookingsExistance( basar::db::aspect::AccessorPropertyTable_YIterator yIt )
{
	if( narcotics::LoggerPool::loggerDomModules.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::VarString msg = "checkBookingsExistance of NarcoticsDM. Parameter: \n ";
		for( basar::db::aspect::AccessorPropertyTable_XIterator xit = yIt.begin(); xit != yIt.end(); xit++ )
		{
			if( !(xit.getState() == basar::SS_UNSET) ){
				msg = msg + xit.getName() + ":" + xit.getSQLString() + "\n";
			}
		}
		narcotics::LoggerPool::loggerDomModules.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__);
	}
	getPrintNarcBookACC().execute( "SelCountOfBookings", yIt );
	return ( getPrintNarcBookACC().getPropertyTable().begin().getDecimal("countofbookings") != basar::Decimal(0.0) );	
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
AccessorPropertyTableRef NarcoticsDM::findBookings4PrintCatalogByPattern( AccessorPropertyTable_YIterator yit )
{
	if( narcotics::LoggerPool::loggerDomModules.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::VarString msg = "findBookings4PrintCatalogByPattern of NarcoticsDM. Parameter: \n ";
		for( basar::db::aspect::AccessorPropertyTable_XIterator xit = yit.begin(); xit != yit.end(); xit++ )
		{
			if( !(xit.getState() == basar::SS_UNSET) ){
				msg = msg + xit.getName() + ":" + xit.getSQLString() + "\n";
			}
		}
		narcotics::LoggerPool::loggerDomModules.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__);
	}

	basar::db::aspect::AccessorInstanceRef accPrintBookings	= 
		basar::db::aspect::Manager::getInstance().createAccessorInstance("Acc_PrintBookings", "BookingCatalog", m_ConnectionRef);

	m_accPrintBookingsList.push_back( accPrintBookings );
	accPrintBookings.execute( "SelPrintBookData", yit );
	return accPrintBookings.getPropertyTable();
}

//-------------------------------------------------------------------------------------------------//
/*!	\return	
	\param	articleNo which a lock entry is inserted with.
	\throw	no-throw */
//-------------------------------------------------------------------------------------------------//
const narcotics::ExecuteReturnType NarcoticsDM::lock( const basar::Int32 articleNo )
{
	if( narcotics::LoggerPool::loggerDomModules.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::VarString msg;
		msg.format("lock of NarcoticsDM. Parameter: articleNo : %d", articleNo);
		narcotics::LoggerPool::loggerDomModules.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__);
	}

	basar::db::aspect::ExecuteResultInfo executeResultInfo;

	bool lockGranted = m_LockManager->requestLock( constants::TRANSFER_BUSINESS_TYPE, articleNo, constants::TRANSFER_MAX_LOCKS_PER_BOOKING );

	//fake return value
	if( lockGranted )
	{
		executeResultInfo.m_error = basar::db::aspect::SUCCESS;
		executeResultInfo.m_affectedRows = 1;
	}
	else{
		return narcotics::ExecuteReturnType::ARTICLE_LOCKED;
	}

	return narcotics::ExecuteReturnType( executeResultInfo );
}

//-------------------------------------------------------------------------------------------------//
/*!	\return	
	\param	articleNo that is used for unlocking the article.
	\throw	no-throw */
//-------------------------------------------------------------------------------------------------//
const narcotics::ExecuteReturnType NarcoticsDM::unlock( const basar::Int32 articleNo )
{
	if( narcotics::LoggerPool::loggerDomModules.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::VarString msg;
		msg.format("unlock of NarcoticsDM. Parameter: articleNo : %d", articleNo);
		narcotics::LoggerPool::loggerDomModules.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__);
	}

	basar::db::aspect::ExecuteResultInfo executeResultInfo;

	bool lockReleased = m_LockManager->releaseLock( constants::TRANSFER_BUSINESS_TYPE, articleNo );

	//fake return value
	if( lockReleased )
	{
		executeResultInfo.m_error = basar::db::aspect::SUCCESS;
		executeResultInfo.m_affectedRows = 1;
	}
	else{
		return narcotics::ExecuteReturnType::LOCK_ERROR;
	}

	return narcotics::ExecuteReturnType( executeResultInfo );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\return a iterator of a empty property table line
	\throw	no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////
const AccessorPropertyTable_YIterator NarcoticsDM::deepCopy( basar::db::aspect::AccessorPropertyTable_YIterator yit )
{
	m_YIteratorBackUp = yit.cloneToUnboundedPropertyRow();
	return m_YIteratorBackUp;
}

//-------------------------------------------------------------------------------------------------//
/*!	\retval true ;
	\retval false otherwise
	\throw	no-throw */
//-------------------------------------------------------------------------------------------------//
const bool NarcoticsDM::isDeepCopyEqualToCurrentDatasetOfDB()
{
	AccessorPropertyTable_YIterator yitDB = getCompareBookingACC().getPropertyTable().begin();

	const basar::cmnutil::ParameterList pl(	"catalogid; branchno; articleno; transactiondate; transactiontime; "
											"customersupplierno; transactiontype; purchaseorderno; "
											"qtytransaction; deletedflag; processedflag;" );

	return yitDB.compareSetValues( m_YIteratorBackUp, pl ).first;
}

//-------------------------------------------------------------------------------------------------//
//! \retval	-99 signalizes that the article is locked.
//!	\throw	no-throw
//-------------------------------------------------------------------------------------------------//
const narcotics::ExecuteReturnType NarcoticsDM::queryCurrentDatasetOfDB()
{
	static basar::ConstString fun = "domMod.NarcoticsDM.queryCurrentDatasetOfDB";

	if( narcotics::LoggerPool::loggerDomModules.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::VarString msg;
		msg.format("queryCurrentDatasetOfDB of NarcoticsDM. ");
		narcotics::LoggerPool::loggerDomModules.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__);
	}
	
	AccessorPropertyTable_YIterator yit; 
	getCompareBookingACC().getPropertyTable().clear();
	yit = getCompareBookingACC().getPropertyTable().insert( basar::FOR_CLEAN );
		
	basar::Int32 value;
	value = m_YIteratorBackUp.getInt32( constants::CATALOGID );	
	
	yit.setInt32( constants::CATALOGID, m_YIteratorBackUp.getInt32( constants::CATALOGID ) );
	yit.setInt16( constants::BRANCHNO,  m_YIteratorBackUp.getInt16( constants::BRANCHNO  ) );
	yit.setInt32( constants::ARTICLENO, m_YIteratorBackUp.getInt32( constants::ARTICLENO ) );

	return narcotics::ExecuteReturnType( getCompareBookingACC().execute( "SelCatalog", yit ) );	
}

//-------------------------------------------------------------------------------------------------//
//! \retval	 >0	signalizes the number of datasets are affected in the case of an update or insert. \n
//! \retval	 0	signalizes that nothing was saved. \n
//! \retval	-1	signalizes that a general exception has thrown. \n
//! \retval	-2	signalizes that an error has happended with locking the article. \n
//! \retval	-3	signalizes that the necessary attributes are not set. \n
//! \retval	-4	signalizes that the deep copy (is a copy of the dataset as it has been read out of the database) 
//				is not equal to the current dataset in the database. 
//! \retval -5  signalizes that the stockupdate results in negative quantity in stock
//! \retval	-99 signalizes that the article is locked.
//!	\throw	no-throw
//-------------------------------------------------------------------------------------------------//
const narcotics::ExecuteReturnType NarcoticsDM::saveBooking( AccessorPropertyTable_YIterator yit, bool checkOnly, bool valid )
{
	if( narcotics::LoggerPool::loggerDomModules.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::VarString msg = "saveBooking of NarcoticsDM. Parameter: \n ";
		for( basar::db::aspect::AccessorPropertyTable_XIterator xit = yit.begin(); xit != yit.end(); xit++ )
		{
			if( !(xit.getState() == basar::SS_UNSET) ){
				msg = msg + xit.getName() + ":" + xit.getSQLString() + "\n";
			}
		}
		narcotics::LoggerPool::loggerDomModules.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__);
	}
	bool toSaveBooking				= false;
	bool transactionActive			= false;
	bool toStockUpdate				= false;
	bool stockUpdOnTheBasisOfUpdate = true;
	bool dataSetLocked				= false;
	narcotics::ExecuteReturnType ret;
	narcotics::ExecuteReturnType saveValue;
	basar::db::aspect::ExecuteResultInfo::RowNumber affectedRows = 0;

	basar::Int32 lockedArticleNo = -1;

	try {
		//------------------------------------------------------------------------------------//
		// INSERT
		//------------------------------------------------------------------------------------//
		if( yit.getState().contains(basar::SS_INSERT) )
		{
			toSaveBooking				= true;
			toStockUpdate				= true;
			stockUpdOnTheBasisOfUpdate	= false;
			if( yit.getState(constants::BRANCHNO) == basar::SS_UNSET ){
				yit.setInt16(constants::BRANCHNO, m_BranchNo);
			}

			if( false == yit.arePropertiesSet( "articleno;       transactiondate; transactiontime; customersupplierno; "
											   "transactiontype; purchaseorderno; qtytransaction;  branchno;"          ) )
			{
				return ret = narcotics::ExecuteReturnType::ATTRIBUTES_UNSET;
			}

			// check if the article is locked --> has not to be in the transaction
			if( checkIsArticleLocked( yit.getInt32( constants::ARTICLENO ) ) ){
				return ret = narcotics::ExecuteReturnType::ARTICLE_LOCKED;
			}
		}
		else
		{
			//------------------------------------------------------------------------------------//
			// UPDATE
			//------------------------------------------------------------------------------------//
			//	1. find out if the old and new value distinguishes
			//	2. check if the regarded article is locked
			//	3. lock the article
			//	4. begin of a transaction
			//	5. select db values of the processed dataset at the moment
			//	6. comparing db values with the current one
			//	7. write log
			basar::VarString value, valueBackUP;
			basar::db::aspect::AccessorPropertyTable_XIterator xit = yit.begin( basar::SS_UPDATE );
			for( ; xit != yit.end(); xit++ )
			{
				if (xit.getName() == constants::SELECTION)
					continue;
				value = xit.getSQLString();
				valueBackUP = m_YIteratorBackUp.getSQLString( xit.getName() );

				if( value != valueBackUP )
				{
					if( false == transactionActive )
					{
						const basar::DateTime transactionDate(yit.getInt32(constants::TRANSACTIONDATE), -1);
						BLOG_TRACE(narcotics::LoggerPool::loggerUseCases, transactionDate.toStrDate().c_str());
						// check if date is in the correct timeframe
						if (!halfyearperiodhelper::isDateInCurrentOrPreviousHalfYear(transactionDate))
						{
							return ret = narcotics::ExecuteReturnType::OUTDATED_DATE;
						}

						// check if the article is locked --> has not to be in the transaction
						if( checkIsArticleLocked( yit.getInt32( constants::ARTICLENO ) ) ){
							return ret = narcotics::ExecuteReturnType::ARTICLE_LOCKED;
						}
						// article is not locked therefore it has to be locked.
					// IMPORTANTCHANGE
						//lock-method inserts dataset in lock table, should be called outside of a transaction
						//else entry in lock table will not be visible until commit (furthermore: inserted lock is removed before commit)
						saveValue = lock( yit.getInt32( constants::ARTICLENO ) );
						if( !saveValue.hasError() )
						{
							affectedRows = saveValue.getAffectedRows();
							lockedArticleNo = yit.getInt32( constants::ARTICLENO );
						}

						if( narcotics::ExecuteReturnType::OK_BY_SELECT_BUT_NOTHING_SAVED_BY_MANIPULATION == affectedRows ){
							return ret = narcotics::ExecuteReturnType::LOCK_ERROR;
						}
						dataSetLocked = true;

						getNarCatalogACC().beginTransaction();
						transactionActive = true;

						// query of the current dataset to compare it with the one of the deep copy.
						if( narcotics::ExecuteReturnType::OK_BY_SELECT_BUT_NOTHING_SAVED_BY_MANIPULATION > ( ret = queryCurrentDatasetOfDB() ).getError() )
						{
							getNarCatalogACC().rollbackTransaction();
							unlock(lockedArticleNo);

							return ret;
						}
						// the actual comparison
						if( false == isDeepCopyEqualToCurrentDatasetOfDB() )
						{
							getNarCatalogACC().rollbackTransaction();
							unlock(lockedArticleNo);

							return ret = narcotics::ExecuteReturnType::CURRENT_DATASET_UNEQUAL_DB_DATASET;
						}
						toSaveBooking = true;
					}
					// to log any changes generally
					m_LogDM.saveLogEntryUpdate(yit.getInt32( constants::CATALOGID ), xit.getName(), valueBackUP, value );
				}
				//	do the 'bestandsfortschreibung' if the following attributes are changed
				if( constants::QTYTRANSACTION  == xit.getName() || constants::TRANSACTIONDATE == xit.getName() || 
					constants::TRANSACTIONTIME == xit.getName()	|| constants::TRANSACTIONTYPE == xit.getName() ||
					constants::PROCESSEDFLAG   == xit.getName() || constants::DELETEDFLAG == xit.getName() )
				{
					toStockUpdate = true;
				}
			} // end FOR-Loop (update attributes)
		}

		//-----------------------------------------------------------------------//
		// the properly save process
		//-----------------------------------------------------------------------//
		//	1. if necessary begin transaction 
		//	2. if necessary lock the article
		//	3. saving the article in the db
		//	4. do the stock calculation update
		//	5. unlock the article
		if( true == toSaveBooking )
		{
			if( valid == true ) 
			{
				if( checkValidateBooking(yit).getError() < 0 )
				{
					if( true == transactionActive ){
						getNarCatalogACC().rollbackTransaction();
					}
					
					saveValue = unlock( yit.getInt32( constants::ARTICLENO ) );
					affectedRows = 0;
					
					if( !saveValue.hasError() ){
						affectedRows = saveValue.getAffectedRows();
					}
					
					if( narcotics::ExecuteReturnType::OK_BY_SELECT_BUT_NOTHING_SAVED_BY_MANIPULATION == affectedRows ){
						return ret = narcotics::ExecuteReturnType::LOCK_ERROR;
					}

					return narcotics::ExecuteReturnType::INVALID_VALUE;
				}
			}
			// important for INSERT; for UPDATE it has been set yet for logging of the changes
			if( false == dataSetLocked )
			{
				saveValue = lock( yit.getInt32( constants::ARTICLENO ) );
				affectedRows = 0;
				
				if( !saveValue.hasError() )
				{
					affectedRows = saveValue.getAffectedRows();
					lockedArticleNo = yit.getInt32( constants::ARTICLENO );
				}
				
				if( narcotics::ExecuteReturnType::OK_BY_SELECT_BUT_NOTHING_SAVED_BY_MANIPULATION >= affectedRows )
				{
					if( transactionActive == true ){
						getNarCatalogACC().rollbackTransaction();
					}
					return ret = narcotics::ExecuteReturnType::LOCK_ERROR;
				}
				dataSetLocked = true;
			}			
			if( false == transactionActive )
			{
				getNarCatalogACC().beginTransaction();
				transactionActive = true;
			}
// IMPORTANTCHANGE
			ret = getNarCatalogACC().execute( "SaveBooking", yit );
			affectedRows = 0;
			
			if( !ret.hasError() ){
				affectedRows = ret.getAffectedRows(); //m_affectedRows;
			}
			if( affectedRows == narcotics::ExecuteReturnType::OK_BY_SELECT_BUT_NOTHING_SAVED_BY_MANIPULATION )
			{ 
				getNarCatalogACC().rollbackTransaction();

				if( dataSetLocked == true ){
					unlock( lockedArticleNo );
				}

				return ret;
			}

			// do the 'bestandsfortschreibung' if the attribute 'qtytransaction' is changed
			if( true == toStockUpdate )
			{
				if( narcotics::ExecuteReturnType::ARTICLE_LOCKED == (stockUpdate( yit, checkOnly, stockUpdOnTheBasisOfUpdate ) ).getError() )
				{
					getNarCatalogACC().rollbackTransaction();

					if( dataSetLocked == true ){
						unlock( lockedArticleNo );
					}

					return  ret = narcotics::ExecuteReturnType::NEGATIVE_QTY_IN_STOCK;
				}
			}

			//	unlock the article 
			//	(without testing before if the article is locked, because article has to be locked anyway)
			//	new: if we we are here, we got a lock, so we have to unlock again
			saveValue = unlock( lockedArticleNo );
			
			affectedRows = 0;

			if( !saveValue.hasError() ){
				affectedRows = saveValue.getAffectedRows(); //m_affectedRows;
			}

			if( narcotics::ExecuteReturnType::OK_BY_SELECT_BUT_NOTHING_SAVED_BY_MANIPULATION == affectedRows )
			{
				getNarCatalogACC().rollbackTransaction(); //do nothing if final unlock fails
				return ret = narcotics::ExecuteReturnType::LOCK_ERROR;
			}

			getNarCatalogACC().commitTransaction();

			dataSetLocked = false;
		} // end if-condition (toSaveBooking)
	} 
	catch( const basar::Exception& )
	{
		if( true == transactionActive ){
			getNarCatalogACC().rollbackTransaction();
		}

		if( true == dataSetLocked ){
			unlock( yit.getInt32( lockedArticleNo ) );
		}
		// negativer Rueckgabe-Wert
		ret = narcotics::ExecuteReturnType::BASAR_EXCEPTION_THROWN;
	}

    //here no lock should exist anyway
    m_LockManager->releaseAllLocksByBranchBusinessTypeAndUser( constants::TRANSFER_BUSINESS_TYPE );

	return ret;
}

//-------------------------------------------------------------------------------------------------//
//! \retval	 0	signalizes that nothing was saved. \n
//!	\throw	no-throw
//-------------------------------------------------------------------------------------------------//
const narcotics::ExecuteReturnType NarcoticsDM::saveOrderChanges( basar::db::aspect::AccessorPropertyTableRef probTab, AccessorPropertyTable_YIterator firstPosYit )
{
    //done: need to copy probTab - using more than one accessor
    //todo: handle time changes
    basar::db::aspect::AccessorPropertyTable_YIterator yit = probTab.begin();

    //has to be same for all entries, we are working with a whole order here
    basar::Int32 originalTransactionDate = yit.getInt32( constants::TRANSACTIONDATE );
    basar::Int32 originalTransactionTime = yit.getInt32( constants::TRANSACTIONTIME );

    basar::Int32 newTransationDate = originalTransactionDate;
    basar::Int32 newTransactionTime = originalTransactionTime;
    //check if new date/time is set, else copy it
    if( yit.isContainedAndSet( constants::NEWTRANSACTIONDATE ) ){
        newTransationDate = yit.getInt32( constants::NEWTRANSACTIONDATE );
    }

    if( yit.isContainedAndSet( constants::NEWTRANSACTIONTIME ) ){
        newTransactionTime = yit.getInt32( constants::NEWTRANSACTIONTIME );
    }

	if( narcotics::LoggerPool::loggerDomModules.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
    {
	    basar::VarString msg = "saveOrderChanges of NarcoticsDM. Parameter: \n ";
	    for( basar::db::aspect::AccessorPropertyTable_XIterator xit = yit.begin(); xit != yit.end(); xit++ )
		{
		    if( !(xit.getState() == basar::SS_UNSET) ){
			    msg = msg + xit.getName() + ":" + xit.getSQLString() + "\n";
		    }
	    }
		narcotics::LoggerPool::loggerDomModules.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__);
	}

    bool updateQtyInStockNeeded = false;
    //check date/time changes for update using deltas -> no time change, no need to update qtys in stock
    //also check for type changes -> b to a etc.
    //done: check if set - date & time
    if( originalTransactionDate != newTransationDate || 
        originalTransactionTime != newTransactionTime )
    {
        updateQtyInStockNeeded = true;
    }

	bool transactionActive	= false;
	narcotics::ExecuteReturnType ret;
	basar::db::aspect::ExecuteResultInfo::RowNumber affectedRows = 0;
	
	try 
	{
		if( false == yit.arePropertiesSet("purchaseorderno; customersupplierno; customersuppliername; "
			"customersupplierlocation; transactiondate; transactiontime; transactiontype;" ) )
        {
				return ret = narcotics::ExecuteReturnType::ATTRIBUTES_UNSET;
		}

        //lock articles here if qty in stock update needed
        if( updateQtyInStockNeeded )
        {
            //because of bool and
            bool lockSuccess = true;
            
            //todo: lock all articles in order
            for( ; !yit.isEnd(); yit++ )
			{
                lockSuccess &= m_LockManager->requestLock(constants::TRANSFER_BUSINESS_TYPE, yit.getInt32(constants::ARTICLENO), constants::TRANSFER_MAX_LOCKS_PER_BOOKING );
            }

            if( !lockSuccess )
            {
                ret.LOCK_ERROR;
                m_LockManager->releaseAllLocksByBranchBusinessTypeAndUser(constants::TRANSFER_BUSINESS_TYPE);
                
                return ret;
            }

            //reset yit
            yit = probTab.begin();
        }

		if( false == transactionActive )
		{
			getNarCatalogACC().beginTransaction();
			transactionActive = true;
		}

// ------------------- narcprotocol for order changes ------------------
		basar::VarString value, valueBackUP;
		std::map<basar::VarString, basar::VarString> updCols;
		std::map<basar::VarString, basar::VarString>::iterator it;

		// --> property names aller potentiell änderbaren Werte
		updCols.insert(std::map<basar::VarString, basar::VarString>::value_type(constants::NEWTRANSACTIONTYPE, constants::TRANSACTIONTYPE));
		updCols.insert(std::map<basar::VarString, basar::VarString>::value_type(constants::NEWTRANSACTIONDATE, constants::TRANSACTIONDATE));
		updCols.insert(std::map<basar::VarString, basar::VarString>::value_type(constants::NEWTRANSACTIONTIME, constants::TRANSACTIONTIME));
		updCols.insert(std::map<basar::VarString, basar::VarString>::value_type(constants::CUSTOMER_SUPPLIER_STREET, constants::CUSTOMER_SUPPLIER_STREET));
		updCols.insert(std::map<basar::VarString, basar::VarString>::value_type(constants::CUSTOMERSUPPLIERNO, constants::CUSTOMERSUPPLIERNO));
		updCols.insert(std::map<basar::VarString, basar::VarString>::value_type(constants::CUSTOMER_SUPPLIER_NAME, constants::CUSTOMER_SUPPLIER_NAME));
		updCols.insert(std::map<basar::VarString, basar::VarString>::value_type(constants::CUSTOMER_SUPPLIER_LOCATION, constants::CUSTOMER_SUPPLIER_LOCATION));
		
		for(it=updCols.begin(); it != updCols.end(); ++it)
		{
			yit = probTab.begin();

			if (!yit.isContainedAndSet(it->first) || !firstPosYit.isContainedAndSet(it->second))
				continue;

			value = yit.getSQLString( it->first );
			valueBackUP = firstPosYit.getSQLString( it->second );
			
			if( value != valueBackUP )
			{	
				for( ; !yit.isEnd(); yit++)
				{
					// to log any changes generally
					m_LogDM.saveLogEntryUpdate(yit.getInt32( constants::CATALOGID ), it->second, valueBackUP, value );
				}
			}
		}

		//reset yit
        yit = probTab.begin();
// ------------------- END: narcprotocol for order changes ------------------	

		ret = getNarCatalogACC().execute( "UpdOrderChanges", yit );
		
		if( !ret.hasError() ){
			affectedRows = ret.getAffectedRows(); //m_affectedRows;
		}
		
		if( affectedRows == narcotics::ExecuteReturnType::OK_BY_SELECT_BUT_NOTHING_SAVED_BY_MANIPULATION )
		{ 
			getNarCatalogACC().rollbackTransaction();
            m_LockManager->releaseAllLocksByBranchBusinessTypeAndUser(constants::TRANSFER_BUSINESS_TYPE);
			return ret;
		}

        if( updateQtyInStockNeeded )
        {
            //todo: update qty in stock, use deltas - need: old transactiondate / time, new (if changed) qty etc            

            basar::db::aspect::AccessorInstanceRef accGetPrevQtyInStock = basar::db::aspect::Manager::getInstance().createAccessorInstance( "Acc_NarCatalog", "BookingCatalog", m_ConnectionRef, basar::db::aspect::FULL_CACHING );
            basar::db::aspect::AccessorInstanceRef accUpdateQtyInStockUsingDeltas = basar::db::aspect::Manager::getInstance().createAccessorInstance( "Acc_NarCatalog", "BookingCatalog", m_ConnectionRef, basar::db::aspect::ON_DEMAND_CACHING );
            
            //todo: lock all articles
            //todo: do only if date/time is changed

            yit = probTab.begin();

            for( ; !yit.isNull() && !yit.isEnd(); ++yit )
            {
                basar::Int32 qtyDelta = yit.getInt32( constants::QTYTRANSACTION );

                if( yit.getString( constants::TRANSACTIONTYPE ) == TransactionType::CUSTOMER_ORDER ||
                    yit.getString( constants::TRANSACTIONTYPE ) == TransactionType::DESTRUCTION ||
                    yit.getString( constants::TRANSACTIONTYPE ) == TransactionType::STOCK_DIFFERENCE_DECREASE ||
                    yit.getString( constants::TRANSACTIONTYPE ) == TransactionType::SUPPLIER_RETURNS
                    )
                {
                    qtyDelta *= -1;
                }

                //1) get delta
                //2) get new qty in stock (articles former qtyinstock + delta
                
                accUpdateQtyInStockUsingDeltas.getPropertyTable().clear();
                basar::db::aspect::AccessorPropertyTable_YIterator yitNew = accUpdateQtyInStockUsingDeltas.getPropertyTable().insert( basar::FOR_UNKNOWN );

                yitNew.setInt32( constants::ORIGINALTRANSACTIONDATE, originalTransactionDate );
                yitNew.setInt32( constants::ORIGINALTRANSACTIONTIME, originalTransactionTime );

                yitNew.setInt32( constants::NEWTRANSACTIONDATE, newTransationDate );
                yitNew.setInt32( constants::NEWTRANSACTIONTIME, newTransactionTime );

                yitNew.setInt16( constants::BRANCHNO, yit.getInt16( constants::BRANCHNO ) );
                yitNew.setInt32( constants::ARTICLENO, yit.getInt32( constants::ARTICLENO ) );

                if( originalTransactionDate < newTransationDate || 
					(originalTransactionDate == newTransationDate && originalTransactionTime < newTransactionTime ) )
				{
                    yitNew.setInt32( "qtyDelta", qtyDelta * -1 );
					ret = accUpdateQtyInStockUsingDeltas.execute( "UpdateQtyInStockUsingDeltasFuture", yitNew );
				}
                else
				{
                    yitNew.setInt32( "qtyDelta", qtyDelta );
					ret = accUpdateQtyInStockUsingDeltas.execute( "UpdateQtyInStockUsingDeltasPast", yitNew );
                }

                //do not use affectedRows, if order is moved without moving over an other order affectedRows == 0 is ok!
		        if( ret.hasError() )
		        { 
			        getNarCatalogACC().rollbackTransaction();
                    m_LockManager->releaseAllLocksByBranchBusinessTypeAndUser( constants::TRANSFER_BUSINESS_TYPE );
			        
                    return ret;
		        }

                basar::db::aspect::AccessorPropertyTable_YIterator yitQty = accGetPrevQtyInStock.getPropertyTable().insert( basar::FOR_UNKNOWN );

                yitQty.setInt16( constants::BRANCHNO, yit.getInt16( constants::BRANCHNO ) );
                yitQty.setInt32( constants::ARTICLENO, yit.getInt32( constants::ARTICLENO ) );
                yitQty.setInt32( constants::TRANSACTIONDATE, newTransationDate);
                yitQty.setInt32( constants::TRANSACTIONTIME, newTransactionTime);

                ret = accGetPrevQtyInStock.execute( "SelQuickPrevForUpdQtyInStock", yitQty );

                if( ret.hasError() )
                {
			        getNarCatalogACC().rollbackTransaction();
                    m_LockManager->releaseAllLocksByBranchBusinessTypeAndUser( constants::TRANSFER_BUSINESS_TYPE );
			        
                    return ret;                    
                }

				basar::Int32 qtyInStockBefore;
				
				if (accGetPrevQtyInStock.getPropertyTable().begin().isEnd())
					qtyInStockBefore = 0;
				else
					qtyInStockBefore = accGetPrevQtyInStock.getPropertyTable().begin().getInt32( constants::QTYINSTOCK );

                yit.setInt32( constants::QTYINSTOCK, qtyInStockBefore + qtyDelta );

                ret = accGetPrevQtyInStock.execute( "UpdateQtyInStockNoState", yit );

                if( ret.hasError() )
                {
                    getNarCatalogACC().rollbackTransaction();
                    m_LockManager->releaseAllLocksByBranchBusinessTypeAndUser( constants::TRANSFER_BUSINESS_TYPE );
			        
                    return ret;   
                }
            }
        }

		getNarCatalogACC().commitTransaction();

        //todo: unlock all articles - outside of transaction

		//reinit yit :)
        yit = probTab.begin();

		if( yit.isContainedAndSet( constants::NEWTRANSACTIONDATE ) ){
			yit.setInt32( constants::TRANSACTIONDATE,yit.getInt32( constants::NEWTRANSACTIONDATE ) );
		}

	} 
	catch( const basar::Exception& )
	{
		if( true == transactionActive ){
			getNarCatalogACC().rollbackTransaction();
		}
		// negative return value
		ret = narcotics::ExecuteReturnType::BASAR_EXCEPTION_THROWN;
	}
		
	if (yit.isContainedAndSet(constants::NEWTRANSACTIONDATE))
	{
		yit.setPropertyState(constants::NEWTRANSACTIONDATE, basar::SS_UNSET);
	}

    //before returning make sure any set articlelock is removed

    m_LockManager->releaseAllLocksByBranchBusinessTypeAndUser( constants::TRANSFER_BUSINESS_TYPE );

	return ret;
}

//-------------------------------------------------------------------------------------------------//
//!	\throw no-throw
//-------------------------------------------------------------------------------------------------//
const narcotics::ExecuteReturnType NarcoticsDM::checkValidateBooking( AccessorPropertyTable_YIterator yit )
{
	if (yit.getState("qtytransaction") != basar::SS_UNSET)
		if (yit.getInt32("qtytransaction") < 0) return narcotics::ExecuteReturnType::INVALID_VALUE;
	if (yit.getState(constants::TRANSACTIONTIME) != basar::SS_UNSET)
		if (yit.getInt32(constants::TRANSACTIONTIME) <= 0) return narcotics::ExecuteReturnType::INVALID_VALUE;
	if (yit.getState(constants::TRANSACTIONDATE) != basar::SS_UNSET)
		if (yit.getInt32(constants::TRANSACTIONDATE) <= 0) return narcotics::ExecuteReturnType::INVALID_VALUE;
	if (yit.getState(constants::TRANSACTIONTYPE) != basar::SS_UNSET)
		if (yit.getString(constants::TRANSACTIONTYPE) == "") return narcotics::ExecuteReturnType::INVALID_VALUE;
	if (yit.getState(constants::PROCESSEDFLAG) != basar::SS_UNSET)
		if (yit.getString(constants::PROCESSEDFLAG) != "0" && yit.getString(constants::PROCESSEDFLAG) != "1") return narcotics::ExecuteReturnType::INVALID_VALUE;
	if (yit.getState(constants::DELETEDFLAG) != basar::SS_UNSET)
		if (yit.getString(constants::DELETEDFLAG) != "0" && yit.getString(constants::DELETEDFLAG) != "1") return narcotics::ExecuteReturnType::INVALID_VALUE;
	if (yit.getState(constants::BRANCHNO) != basar::SS_UNSET)
		if (yit.getInt16(constants::BRANCHNO) == 0) return narcotics::ExecuteReturnType::INVALID_VALUE;
	if (yit.getState(constants::ARTICLENO) != basar::SS_UNSET)
		if (yit.getInt32(constants::ARTICLENO) == 0) return narcotics::ExecuteReturnType::INVALID_VALUE;
	if (yit.getState("customersupplierno") != basar::SS_UNSET)
		if (yit.getInt32("customersupplierno") == 0) return narcotics::ExecuteReturnType::INVALID_VALUE;
	if (yit.getState("customersuppliername") != basar::SS_UNSET)
		if (yit.getString("customersuppliername") == "") return narcotics::ExecuteReturnType::INVALID_VALUE;
	if (yit.getState("customersupplierlocation") != basar::SS_UNSET)
		if (yit.getString("customersupplierlocation") == "") return narcotics::ExecuteReturnType::INVALID_VALUE;
	// ToDo: maybe this code line has to be revised again because of logical mistakes 
	return narcotics::ExecuteReturnType::OK_BY_SELECT_BUT_NOTHING_SAVED_BY_MANIPULATION;
}

//-------------------------------------------------------------------------------------------------//
//!	\throw no-throw
//-------------------------------------------------------------------------------------------------//
const basar::db::aspect::ExecuteResultInfo NarcoticsDM::updateDiffQtyInStock()
{
	BLOG_TRACE_METHOD(narcotics::LoggerPool::loggerDomModules, "NarcoticsDM::updateDiffQtyInStock()");

	basar::db::aspect::ExecuteResultInfo res;
	basar::VarString where;

	// search iterator for all statements
	where.format( "branchno=%d; creationdate=%d;", m_BranchNo, basar::cmnutil::DateTime::getCurrent().getDate() );
	basar::db::aspect::AccessorPropertyTable_YIterator yit = 
        basar::db::aspect::Manager::getInstance().createAccessorYIterator( static_cast<basar::I18nString>(where) );

	// 1. delete all branch no specific datasets
	res = getDiffQtyInStockACC().execute( "DelDiffQtyInStock", yit );
	if( res.hasError() ){
		return res;
	}

	// 2. do a new query to insert the qty in stock differences in table 'narcdiffqtyinstock'
	res = getDiffQtyInStockACC().execute( "InsertDiffQtyInStock", yit );
	if( res.hasError() ){
		return res;
	}
	
	res = getDiffQtyInStockACC().execute( "InsertNegativeQtyInStock", yit );
	if( res.hasError() ){
		return res;
	}
	
	return res;
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
const narcotics::ExecuteReturnType NarcoticsDM::strikeOut( AccessorPropertyTable_YIterator yit, bool checkOnly )
{
	basar::ConstString fun = "NarcoticsDM::strikeOut()";
    BLOG_TRACE_METHOD( narcotics::LoggerPool::loggerDomModules, fun );
	
    narcotics::ExecuteReturnType ret;
	narcotics::ExecuteReturnType saveValue;
	basar::db::aspect::ExecuteResultInfo::RowNumber affectedRows = 0;
	bool transactionActive = false;
	bool articleLocked = false;

	basar::Int32 lockedArticleNo = -1;

	try 
	{
		//! \todo magic numbers!
		// check if the article is locked --> has not to be in the transaction
		
 		if( checkIsArticleLocked( yit.getInt32(constants::ARTICLENO) ) ){
			return ret = narcotics::ExecuteReturnType::ARTICLE_LOCKED;
		}
		// article is not locked therefore it has to be locked.
		saveValue = lock( yit.getInt32(constants::ARTICLENO));
		if( !saveValue.hasError() )
		{
			affectedRows = saveValue.getAffectedRows();
			articleLocked = true;
			lockedArticleNo = yit.getInt32(constants::ARTICLENO);
		}

		if( narcotics::ExecuteReturnType::OK_BY_SELECT_BUT_NOTHING_SAVED_BY_MANIPULATION == affectedRows ){
			return ret = narcotics::ExecuteReturnType::LOCK_ERROR;
		}

		getNarCatalogACC().beginTransaction();
		transactionActive = true;

		// query of the current dataset to compare it with the one of the deep copy.
		ret = queryCurrentDatasetOfDB();
		if( narcotics::ExecuteReturnType::OK_BY_SELECT_BUT_NOTHING_SAVED_BY_MANIPULATION > ret.getError() )
		{
			getNarCatalogACC().rollbackTransaction();
			if( articleLocked == true ){
				unlock( lockedArticleNo );
			}
			return ret;
		}

		// the actual comparison
		if( isDeepCopyEqualToCurrentDatasetOfDB() == false )
		{
			getNarCatalogACC().rollbackTransaction();
			if( articleLocked == true ){
				unlock( lockedArticleNo );
			}
			return ret = narcotics::ExecuteReturnType::CURRENT_DATASET_UNEQUAL_DB_DATASET;
		}

		//! \todo to check the return code!
		ret  = getNarCatalogACC().execute( "UpdStrikeOutBook", yit ); //m_affectedRows;
		LOG_YIT( fun, narcotics::LoggerPool::loggerDomModules, yit );
		affectedRows = 0;
		if( !ret.hasError() ){
			affectedRows = ret.getAffectedRows();
		}

		m_LogDM.saveLogEntryDelete( yit.getInt32( constants::CATALOGID ) );

		if( affectedRows > narcotics::ExecuteReturnType::OK_BY_SELECT_BUT_NOTHING_SAVED_BY_MANIPULATION )
		{
			bool onTheBasisOfUpdate = true;
			if( narcotics::ExecuteReturnType::ARTICLE_LOCKED == ( stockUpdate( yit, checkOnly, onTheBasisOfUpdate ) ).getError() )
			{
				getNarCatalogACC().rollbackTransaction();

				if( articleLocked == true ){
					unlock( lockedArticleNo );
				}

				return ret = narcotics::ExecuteReturnType::NEGATIVE_QTY_IN_STOCK;
			}
		}
		
		saveValue = unlock( lockedArticleNo );
		affectedRows = 0;
		if( !saveValue.hasError() ){
			affectedRows = saveValue.getAffectedRows();
		}
		// unlock the article 
		// (without testing before if the article is locked, because article has to be locked anyway)
		if( narcotics::ExecuteReturnType::OK_BY_SELECT_BUT_NOTHING_SAVED_BY_MANIPULATION == affectedRows )
		{
			getNarCatalogACC().rollbackTransaction();
			return ret = narcotics::ExecuteReturnType::LOCK_ERROR;
		}
		getNarCatalogACC().commitTransaction();
	} 
	catch( const basar::Exception ) 
	{
		if( true == transactionActive ){
			getNarCatalogACC().rollbackTransaction();
		}
		if( articleLocked == true && lockedArticleNo != -1 ){
			unlock( lockedArticleNo );
		}
		ret = narcotics::ExecuteReturnType::BASAR_EXCEPTION_THROWN;
	}
	return ret;
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
const narcotics::ExecuteReturnType NarcoticsDM::restoreStrikedOut( AccessorPropertyTable_YIterator yit, bool checkOnly )
{
	if( narcotics::LoggerPool::loggerDomModules.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::VarString msg = "restoreStrikedOut of NarcoticsDM. Parameter: \n ";
		for( basar::db::aspect::AccessorPropertyTable_XIterator xit = yit.begin(); xit != yit.end(); ++xit )
		{
			if( !(xit.getState() == basar::SS_UNSET) ){
				msg = msg + xit.getName() + ":" + xit.getSQLString() + "\n";
			}
		}
		narcotics::LoggerPool::loggerDomModules.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__);
	}
// as m_YIteratorBackUp is resetted at this position due to rollback problems, it has to be refilled again
	deepCopy( yit );

// IMPORTANTCHANGE
	narcotics::ExecuteReturnType ret;
	narcotics::ExecuteReturnType saveValue;
	basar::db::aspect::ExecuteResultInfo::RowNumber affectedRows;
	bool transactionActive = false;
	bool articleLocked = false;

    basar::Int32 articleNo = yit.getInt32( constants::ARTICLENO );

	try 
	{
		// check if the article is locked --> has not to be in the transaction
		if( checkIsArticleLocked( yit.getInt32( constants::ARTICLENO ) ) ){
			return ret = narcotics::ExecuteReturnType::ARTICLE_LOCKED;
		}

		// article is not locked therefore it has to be locked.
		saveValue  = lock( articleNo );
		affectedRows = 0;
		
		if( !saveValue.hasError() )
		{
			affectedRows = saveValue.getAffectedRows();
			articleLocked = true;
		}
		
		if( narcotics::ExecuteReturnType::OK_BY_SELECT_BUT_NOTHING_SAVED_BY_MANIPULATION == affectedRows ){
			return ret = narcotics::ExecuteReturnType::LOCK_ERROR;
		}

		getNarCatalogACC().beginTransaction();
		transactionActive = true;
		// query of the current dataset to compare it with the one of the deep copy.
		ret = queryCurrentDatasetOfDB();
		if( narcotics::ExecuteReturnType::OK_BY_SELECT_BUT_NOTHING_SAVED_BY_MANIPULATION > ( ret.getError() ) )
		{
			getNarCatalogACC().rollbackTransaction();
			if( articleLocked == true ){
				unlock( articleNo );
			}
			return ret;
		}
		// the actual comparison
		if( false == isDeepCopyEqualToCurrentDatasetOfDB() )
		{
			getNarCatalogACC().rollbackTransaction();
			if( articleLocked == true ){
				unlock( yit.getInt32( constants::ARTICLENO ) );
			}
			return ret = narcotics::ExecuteReturnType::CURRENT_DATASET_UNEQUAL_DB_DATASET;
		}
		//! \todo to check the return code!
		ret = getNarCatalogACC().execute( "UpdRestoreBook" ,yit );
		affectedRows = 0; //m_affectedRows;
		if( !ret.hasError() ){
			affectedRows = ret.getAffectedRows();
		}
		m_LogDM.saveLogEntryRestore( yit.getInt32( constants::CATALOGID ) );
		
		if( affectedRows > narcotics::ExecuteReturnType::OK_BY_SELECT_BUT_NOTHING_SAVED_BY_MANIPULATION )
		{
			bool onTheBasisOfUpdate = true;
			
			if( narcotics::ExecuteReturnType::ARTICLE_LOCKED == ( stockUpdate( yit, checkOnly, onTheBasisOfUpdate ) ).getError() )
			{
				getNarCatalogACC().rollbackTransaction();

				if( articleLocked == true ){
					unlock( articleNo );
				}

				return ret = narcotics::ExecuteReturnType::NEGATIVE_QTY_IN_STOCK;
			}
		}	

		//	unlock the article 
		//	(without testing before if the article is locked, because article has to be locked anyway)
		saveValue = unlock( yit.getInt32( constants::ARTICLENO ) );
		affectedRows = 0;
		if( !saveValue.hasError() )
		{
			affectedRows = saveValue.getAffectedRows();
			articleLocked = false;
		}
		
		if( narcotics::ExecuteReturnType::OK_BY_SELECT_BUT_NOTHING_SAVED_BY_MANIPULATION == affectedRows )
		{
			getNarCatalogACC().rollbackTransaction();
			return ret = narcotics::ExecuteReturnType::LOCK_ERROR;
		}
		getNarCatalogACC().commitTransaction();

	} 
	catch( const basar::Exception& )
	{ 
		if( true == transactionActive ){
			getNarCatalogACC().rollbackTransaction();
		}
		if( articleLocked == true ){
			unlock( articleNo );
		}
		ret = narcotics::ExecuteReturnType::BASAR_EXCEPTION_THROWN;
	}
	return ret;
}

//-------------------------------------------------------------------------------------------------//
//! \return	 >0	signalizes the number of datasets are affected in the case of an update. \n
//! \retval	 0	signalizes that nothing was saved. \n
//! \retval	 <0	signalizes that a general exception has thrown. \n
//!	\throw	no-throw
//-------------------------------------------------------------------------------------------------//
const narcotics::ExecuteReturnType NarcoticsDM::saveSupplier( basar::VarString orderNo, basar::Int32 supplierNo )
{
	if( narcotics::LoggerPool::loggerDomModules.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::VarString msg;
		msg.format("saveSupplier of NarcoticsDM. Parameter: orderNo : %d; supplierNo : %d", orderNo.c_str(), supplierNo);
		narcotics::LoggerPool::loggerDomModules.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__);
	}

	basar::VarString temp;
	temp.format("purchaseorderno=%s; customersupplierno=%ld; branchno=%d;", orderNo.c_str(), supplierNo, m_BranchNo);
	basar::db::aspect::AccessorPropertyTable_YIterator yit = 
        basar::db::aspect::Manager::getInstance().createAccessorYIterator( static_cast<basar::I18nString>(temp) );
	return ( getNarCatalogACC().execute( "UpdSupplier", yit ) );
}

//-------------------------------------------------------------------------------------------------//
/*!	\return true if a negative stock for this articlenumber and the branch
	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
const bool NarcoticsDM::checkForNegativeStock( AccessorPropertyTable_YIterator yit )
{
	if( narcotics::LoggerPool::loggerDomModules.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::VarString msg = "checkForNegativeStock of NarcoticsDM. Parameter: \n ";
		for( basar::db::aspect::AccessorPropertyTable_XIterator xit = yit.begin(); xit != yit.end(); ++xit )
		{
			if( !(xit.getState() == basar::SS_UNSET) ){
				msg = msg + xit.getName() + ":" + xit.getSQLString() + "\n";
			}
		}
		narcotics::LoggerPool::loggerDomModules.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__);
	}
	basar::db::aspect::AccessorInstanceRef accTempNarCatalog = 
		basar::db::aspect::Manager::getInstance().createAccessorInstance( 
						"Acc_TempBookingCatalog", "BookingCatalog", getNarCatalogACC().getConnection() );
	accTempNarCatalog.execute("SelNegativeStock",yit);
	return !(accTempNarCatalog.getPropertyTable().empty() );
}

//-------------------------------------------------------------------------------------------------//
/*!	\return true if a negative stock for this articlenumber and the branch
	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
const bool NarcoticsDM::checkExistDiffQtyInStock()
{
	if( narcotics::LoggerPool::loggerDomModules.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "checkExistDiffQtyInStock of NarcoticsDM.";
		narcotics::LoggerPool::loggerDomModules.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	basar::VarString temp;
	temp.format( "branchno=%d;",m_BranchNo );
	basar::db::aspect::AccessorPropertyTable_YIterator yit = 
        basar::db::aspect::Manager::getInstance().createAccessorYIterator( static_cast<basar::I18nString>(temp) );
	getDiffQtyInStockACC().execute( "SelCountDiffQtyInStock", yit );
	return ( getDiffQtyInStockACC().getPropertyTable().begin().getDecimal("countofdiff") != basar::Decimal(0.0) );
}

//-------------------------------------------------------------------------------------------------//
/*!	\retval true if a lock dataset exists for the given article number and the branchno (member variable);
	\retval false otherwise
	\param	articleNo which is looked up for a locked entry with.
	\throw	no-throw */
//-------------------------------------------------------------------------------------------------//
const bool NarcoticsDM::checkIsArticleLocked( const basar::Int32 articleNo )
{
	if( narcotics::LoggerPool::loggerDomModules.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::VarString msg;
		msg.format("checkIsArticleLocked of NarcoticsDM. Parameter: articleNo : %d", articleNo);
		narcotics::LoggerPool::loggerDomModules.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__);
	}

	basar::VarString filterParams;
	filterParams.format( "branchno = %d; idbusinessobject = %d; businesstype = %s", m_BranchNo, articleNo, constants::TRANSFER_BUSINESS_TYPE );
	basar::db::aspect::AccessorPropertyTable_YIterator yit = 
        basar::db::aspect::Manager::getInstance().createAccessorYIterator( static_cast<basar::I18nString>(filterParams) );
	const basar::db::aspect::ExecuteErrorEnum ret = ( getNarcLockACC().execute("SelLock", yit) ).getError(); //m_affectedRows;
	ret;
	return ( getNarcLockACC().getPropertyTable().begin().getDecimal( "cntlocks" ) != basar::Decimal(0.0) );
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
AccessorPropertyTableRef NarcoticsDM::getAccBookingControlPropTab()
{
	return getBookingControlACC().getPropertyTable();
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
AccessorPropertyTableRef NarcoticsDM::getAccNarCatalogPropTab()
{
	return getNarCatalogACC().getPropertyTable();
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
AccessorPropertyTableRef NarcoticsDM::getAccNarSupplierPropTab()
{
	return getNarcoticsSupplierACC().getPropertyTable();
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
AccessorPropertyTableRef NarcoticsDM::getAccNoReturnGoodsPropTab()
{
	return getNumberReturnGoodsACC().getPropertyTable();
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
AccessorPropertyTableRef NarcoticsDM::getAccNoIncomingGoodsPropTab()
{
	return getNumberIncomingGoodsACC().getPropertyTable();
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
AccessorPropertyTableRef NarcoticsDM::getAccDiffQtyInStock()
{
	return getDiffQtyInStockACC().getPropertyTable();
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
AccessorPropertyTableRef NarcoticsDM::getAccArticleNosDiffQtyInStock()
{
	return getArticleNosDiffQtyInStockACC().getPropertyTable();
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
AccessorPropertyTableRef NarcoticsDM::getAccNarInitialInventoryPropTab()
{
	return getInitialInventoryACC().getPropertyTable();
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw 
	\return number of rows where qtyinstock was updated or value -99 if parameter "checkOnly" = true and 
	a negative quantity in stock would be appear at least in one booking.
	parameter is single changed booking which is already written to database,
	locking for this article has to be done before calling this method, there will be no check for lock in here! */
//-------------------------------------------------------------------------------------------------//
const narcotics::ExecuteReturnType NarcoticsDM::stockUpdate( AccessorPropertyTable_YIterator changedBooking, bool checkOnly, bool onTheBasisOfUpdate )
{
	if( narcotics::LoggerPool::loggerDomModules.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::VarString msg = "stockUpdate of NarcoticsDM. Parameter: \n ";
		for( basar::db::aspect::AccessorPropertyTable_XIterator xit = changedBooking.begin(); xit != changedBooking.end(); xit++ )
		{
			if( !(xit.getState() == basar::SS_UNSET) ){
				msg = msg + xit.getName() + ":" + xit.getSQLString() + "\n";
			}
		}
		narcotics::LoggerPool::loggerDomModules.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__);
	}
	//! \todo check if article is locked (especially for batch) !?
	//! \todo transaction necessary!
	
    getUpdQtyInStockACC().getPropertyTable().clear();

	if( onTheBasisOfUpdate )
	{
		basar::Int32 date = m_YIteratorBackUp.getInt32( constants::TRANSACTIONDATE );
		basar::Int32 time = m_YIteratorBackUp.getInt32( constants::TRANSACTIONTIME );
		
		if( date < changedBooking.getInt32( constants::TRANSACTIONDATE ) ){
			changedBooking.setInt32( constants::TRANSACTIONDATE, date );
		}

		if( changedBooking.getInt32(constants::TRANSACTIONDATE) ==  date )
		{
			if( time < changedBooking.getInt32(constants::TRANSACTIONTIME) ){
				changedBooking.setInt32( constants::TRANSACTIONTIME, time );
			}
		}
	}

	getUpdQtyInStockACC().execute( "SelQuickPrevForUpdQtyInStock", changedBooking );
	AccessorPropertyTable_YIterator PrevBooking = getUpdQtyInStockACC().getPropertyTable().begin();
	
	basar::Int32 PrevQtyInStock;
	
	if( PrevBooking.isEnd() ){ //no start stock !?
		PrevQtyInStock = 0;
	} 
	else{
		PrevQtyInStock = PrevBooking.getInt32( "qtyinstock" );
	}
	
	getUpdQtyInStockACC().getPropertyTable().clear();
	getUpdQtyInStockACC().execute( "SelFollowForUpdQtyInStock", changedBooking );

	AccessorPropertyTableRef FollowingBookings		= getUpdQtyInStockACC().getPropertyTable();
	AccessorPropertyTable_YIterator CurrentBooking	= FollowingBookings.begin();
	
	basar::Int16 bookingFactor = 1;
	while( !CurrentBooking.isEnd() )
	{
		// the letter "a" represents selloff (deu: Kundenauftrag)
		// the letter "y" represents a separate debit (deu: gesonderte Abbuchung), 
		// the letter "z" represents a separate posting (deu: gesonderte Abbuchung)
		if( TransactionType::CUSTOMER_ORDER == CurrentBooking.getString(constants::TRANSACTIONTYPE) || 
			TransactionType::SUPPLIER_RETURNS == CurrentBooking.getString(constants::TRANSACTIONTYPE) || 
			TransactionType::STOCK_DIFFERENCE_DECREASE == CurrentBooking.getString(constants::TRANSACTIONTYPE) || 
			TransactionType::DESTRUCTION == CurrentBooking.getString(constants::TRANSACTIONTYPE) )
		{
			bookingFactor = -1;
		} 
		else{
			bookingFactor = 1; 
		}
		if( checkOnly )
		{
			if( PrevQtyInStock + ( bookingFactor * CurrentBooking.getInt32("qtytransaction") ) < 0 ){
				return narcotics::ExecuteReturnType::ARTICLE_LOCKED;
			}
		}
		CurrentBooking.setInt32("qtyinstock", PrevQtyInStock + ( bookingFactor * CurrentBooking.getInt32("qtytransaction")) );
		PrevQtyInStock = CurrentBooking.getInt32("qtyinstock");
		++CurrentBooking;
	}
	return ( getUpdQtyInStockACC().execute( "UpdateQtyInStock", FollowingBookings.begin(), FollowingBookings.end() ) ); //m_affectedRows;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\return a iterator of a empty property table line
	\throw no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////
basar::db::aspect::AccessorPropertyTable_YIterator NarcoticsDM::addEmptyBooking()
{
	if( narcotics::LoggerPool::loggerDomModules.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "addEmptyBooking of NarcoticsDM.";
		narcotics::LoggerPool::loggerDomModules.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	getNarCatalogACC().getPropertyTable().clear();
	getNarCatalogACC().getPropertyTable().insert( basar::FOR_INSERT );
	getNarCatalogACC().getPropertyTable().begin().setString( constants::CUSTOMER_SUPPLIER_LOCATION, " ");
	return getNarCatalogACC().getPropertyTable().begin();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\return a iterator of a empty property table line
	\throw no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////
const basar::db::aspect::AccessorPropertyTable_YIterator NarcoticsDM::getBackUpCurrentIterator() const
{
	return m_YIteratorBackUp;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\return a iterator of a empty property table line
	\throw	no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////
AccessorPropertyTable_YIterator NarcoticsDM::findLatestDiffQtyByArticleNo(	const basar::Int32 articleNo, 
																			const basar::Int16 branchNo )
{
	if( narcotics::LoggerPool::loggerDomModules.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::VarString msg;
		msg.format("findCatalogByArticleNo of NarcoticsDM. Parameter: articleNo : %d; branchNo : %d", articleNo, branchNo);
		narcotics::LoggerPool::loggerDomModules.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__);
	}

	basar::VarString where;
	where.format( "articleno=%d; branchno=%d;", articleNo, branchNo );
	basar::db::aspect::AccessorPropertyTable_YIterator yit = 
        basar::db::aspect::Manager::getInstance().createAccessorYIterator( static_cast<basar::I18nString>(where) );
	getDiffQtyInStockACC().execute( "SelLatestByArticleNo", yit );

	return getDiffQtyInStockACC().getPropertyTable().begin();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\return 
	\throw	no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////
AccessorPropertyTable_YIterator NarcoticsDM::findInitialInventoryByArticleNo( const basar::Int32 articleNo, const basar::Int16 branchNo, const basar::Long32 fromDate )
{
	if( narcotics::LoggerPool::loggerDomModules.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::VarString msg;
		msg.format("findInitialInventoryByArticleNo of NarcoticsDM. Parameter: articleNo : %d; branchNo : %d; fromDate : %d", articleNo, branchNo, fromDate);
		narcotics::LoggerPool::loggerDomModules.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__);
	}

	basar::VarString where;
	where.format( "articleno=%d; branchno=%d; fromdate=%d", articleNo, branchNo, fromDate );
	basar::db::aspect::AccessorPropertyTable_YIterator yit = 
        basar::db::aspect::Manager::getInstance().createAccessorYIterator( static_cast<basar::I18nString>(where) );
	getInitialInventoryACC().execute( "SelectInitialInventory", yit );

	return getInitialInventoryACC().getPropertyTable().begin();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\return 
	\throw	no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////
AccessorPropertyTableRef NarcoticsDM::findArticleNoByName( AccessorPropertyTable_YIterator searchIt )
{
	if( narcotics::LoggerPool::loggerDomModules.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::VarString msg;
		msg.format( "findArticleNoByName of NarcoticsDM." );
		narcotics::LoggerPool::loggerDomModules.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__);
	}

	getNarCatalogACC().execute( "SelectArticleNoByArticleName", searchIt );

	return getNarCatalogACC().getPropertyTable();
}

} //namespace domMod