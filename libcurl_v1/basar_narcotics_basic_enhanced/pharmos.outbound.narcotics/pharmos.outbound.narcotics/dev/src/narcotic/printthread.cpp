//--------------------------------------------------------------------------------------------//
/*! \file	printthread.cpp
 *  \brief  A thread class for printing contribution vouchers.
 *  \author Benjamin Endlicher
 *  \date   20.09.2010
 */
//--------------------------------------------------------------------------------------------//

//--------------------------------------------------------------------------------------------//
// include section
//--------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "printthread.h"
#include "loggerpool.h"
#include "numberassignmentdm/numberassignmentdm_definitions.h"
#include "receiptnoticeprintdata.h"
#include "contributionvoucherprintdata.h"
#include "weekcalculator.h"
#include "constants.h"
#include "contributionvoucherftptransfer.h"

//--------------------------------------------------------------------------------------------//
// namespace section
//--------------------------------------------------------------------------------------------//

namespace narcotics
{
	// -------------------------------------------------------------------------
    PrintThread::PrintThread() : QThread(),
		TIMEOUT(5), // 5s - reading only one dataset
        m_Timer(NULL), m_TerminationRequested(false), m_UserInteractionRequiredEmitted(false)
	{
		BLOG_TRACE_METHOD( LoggerPool::loggerUseCases, "PrintThread::PrintThread()" );
	}

	// -------------------------------------------------------------------------
	void PrintThread::init( PrintThreadInitParams& params )
	{
        m_BranchNo = params.branchNo;
        m_NarcoticsUserName = params.narcoticsUserName;
		
		m_FTPhost = params.hostname;
		m_FTPUser = params.user;
		m_FTPPW = params.pw;
		
		m_LocalTmpDirectory = params.localTmpDirectory;
		
		if( m_FTPhost.empty() )
		{
			basar::cmnutil::ExceptionInfoStruct excInfo;
			excInfo.context = "PrintThread::init";
			excInfo .file =__FILE__;
			excInfo.line=__LINE__;
			excInfo.reason = "Parametervalue constants::FTP_CITRIX_HOST is empty!!";
			
			throw basar::cmnutil::BasarInvalidStateException( excInfo );	
		}
		
		if( m_FTPUser.empty() )
		{
			basar::cmnutil::ExceptionInfoStruct excInfo;
			excInfo.context = "PrintThread::init";
			excInfo .file =__FILE__;
			excInfo.line=__LINE__;
			excInfo.reason = "Parametervalue constants::FTP_CITRIX_USER is empty!!";
			
			throw basar::cmnutil::BasarInvalidStateException( excInfo );	
		}
		
		if( m_FTPPW.empty() )
		{
			basar::cmnutil::ExceptionInfoStruct excInfo;
			excInfo.context = "PrintThread::init";
			excInfo .file =__FILE__;
			excInfo.line=__LINE__;
			excInfo.reason = "Parametervalue constants::FTP_CITRIX_PW is empty!!";
			
			throw basar::cmnutil::BasarInvalidStateException( excInfo );	
		}	
		
		if( m_LocalTmpDirectory.empty() )
		{
			basar::cmnutil::ExceptionInfoStruct excInfo;
			excInfo.context = "PrintThread::init";
			excInfo .file =__FILE__;
			excInfo.line=__LINE__;
			excInfo.reason = "Parametervalue constants::FTP_LOCAL_TMP is empty!!";
			
			throw basar::cmnutil::BasarInvalidStateException( excInfo );	
		}	
			
		m_PrinterName = params.printerName;
		
		if( m_PrinterName.empty() )
		{
			basar::cmnutil::ExceptionInfoStruct excInfo;
			excInfo.context = "PrintThread::init";
			excInfo .file =__FILE__;
			excInfo.line=__LINE__;
			excInfo.reason = "Parametervalue constants::PRINTERNAME is empty!! Please configure a printer first!";
			
			throw basar::cmnutil::BasarInvalidStateException( excInfo );	
		}
		
        basar::Date today;

        m_Today = today.getDate();

		m_StartDate = params.startDatetxt;
		m_ContributionVoucherDir = params.directoryname;
		m_InternalAccounts = params.internalaccounts.stoi();

		basar::VarString searchParam;
		searchParam.format( "username=%s %s;signaturelnk=%s.bmp;",
            params.narcoticsUserForeName.c_str(),
			params.narcoticsUserSurName.c_str(),
            m_NarcoticsUserName.c_str() );
        m_YitSignature = basar::db::aspect::Manager::getInstance().createAccessorYIterator( static_cast<basar::I18nString>( searchParam ) );

		searchParam.format( "%s=%s;%s=%s;",
			lit::pdm_numberassignment::NUMBERTYPE.getName().c_str(), pdm_numberassignment::OUTPUT,
            lit::pdm_numberassignment::TABLENAME.getName().c_str(), pdm_numberassignment::NARCNUMBER );
		m_YitSearchSerialOutput = basar::db::aspect::Manager::getInstance().createAccessorYIterator( static_cast<basar::I18nString>( searchParam ) );

		m_Connection.initConnection();

		basar::VarString logMsg;
		logMsg.format(	"PrintThread::init() - Print Thread initialized for branchno < %d >, user < %s >\n"
						"	Using following members:\n"
						"	printername: %s\n"
						"	startDatetxt: %s\n"
						"	directoryname: %s\n"
						"	internalaccounts: %d\n"
						"	FTP hostname: %s\n"
						"	FTP user: %s\n"
						"	localTmpDirectory: %s",
			m_BranchNo, m_NarcoticsUserName.c_str(), m_PrinterName.c_str(), m_StartDate.c_str(),
			m_ContributionVoucherDir.c_str(), m_InternalAccounts, m_FTPhost.c_str(), m_FTPUser.c_str(), m_LocalTmpDirectory.c_str()
			);

		BLOG_INFO( LoggerPool::loggerUseCases, logMsg );
	}

	// -------------------------------------------------------------------------
	PrintThread::~PrintThread()
	{
		BLOG_TRACE_METHOD( LoggerPool::loggerUseCases, "PrintThread::~PrintThread()" );
		
		// disconnect
		m_Connection.disconnect();

		basar::VarString logMsg;
		logMsg.format( "PrintThread::~PrintThread() - Print Thread stopped for branchno < %d >, user < %s >.", m_BranchNo, m_NarcoticsUserName.c_str() );

		BLOG_INFO( LoggerPool::loggerUseCases, logMsg );		
	}

	// -------------------------------------------------------------------------
    void PrintThread::run()
    {
		BLOG_TRACE_METHOD( LoggerPool::loggerUseCases, "PrintThread::run()" );

		on_timer_timeout(); // run once, then start timer

		if( m_Timer == NULL )
		{
			m_Timer = new QTimer();
			m_Timer->setObjectName( "timer" );
			connect( m_Timer, SIGNAL( timeout() ), this, SLOT( on_timer_timeout() ), Qt::DirectConnection );
		}
		m_Timer->setInterval( TIMEOUT*1000 );
		m_Timer->start(); 
		
		exec();
		
		QMutexLocker locker( &m_TerminationMutex );
		
		if( m_Timer != NULL )
		{
			m_Timer->stop();
			delete m_Timer;
			m_Timer = NULL;
        }   
    }

    // -------------------------------------------------------------------------	
    void PrintThread::on_termination_requested()
    {
        m_TerminationRequested = true;
    }

	// -------------------------------------------------------------------------
    bool PrintThread::userStillHasPrintLock()
    {
        return m_ContributionVoucherDM.checkUserHasPrintLock( m_BranchNo, m_NarcoticsUserName );
    }
	
	// -------------------------------------------------------------------------
    void PrintThread::on_timer_timeout()
    {
		BLOG_TRACE_METHOD( LoggerPool::loggerUseCases, "PrintThread::on_timer_timeout()" );

		QMutexLocker locker( &m_TerminationMutex );

		emit heartBeat();

		//only print when not requested to shut down
		if( !m_TerminationRequested )
		{
			printContributionVoucher();
		}
		//exit own event loop
		if( m_TerminationRequested && m_Timer != NULL )
		{
			// clean up
			m_BranchDM.shutdown();
			m_ContributionVoucherDM.shutdown();
			m_NumberAssignmentDM.shutdown();

			m_Timer->stop();
			disconnect( m_Timer, SIGNAL( timeout() ), this, SLOT( on_timer_timeout() ) );
			exit();
		}
    }

	// -------------------------------------------------------------------------
	void PrintThread::printContributionVoucher()
	{
		// build up connection to database

		// don´t open / close connection every 5 seconds !
		// connection remaining open as long print thread is active.
		if( m_Connection.getCurrentConnection().isNull() || !m_Connection.getCurrentConnection().isOpen() )
		{
			m_Connection.connect();

 			m_BranchDM.init( m_BranchNo, m_Connection.getCurrentConnection() );
			m_ContributionVoucherDM.init( m_BranchNo, m_Connection.getCurrentConnection() );
			m_NumberAssignmentDM.init( m_Connection.getCurrentConnection(), m_BranchNo );
		}

        if( !userStillHasPrintLock() )
        {
            //user has no printlock, do nothing
            //nothing to be done -> clean up -> return
			
			// if no lock, also running print thread doesn´t make sense! ==> also terminate print thread, so that user can see, that his/her print thread is running / working anymore!
			emit terminatePrintThread();

		    return;
        }

		//reprint check, part 1: check for not printed (= acknowledgementstatus = IN_PRINT, contributionvoucherno still 0) orders
        //set status to FOR_PRINT
        //need to check for whole order!!!elf
        m_ContributionVoucherDM.getAccPrintSearchPropTab().clear();
        basar::db::aspect::AccessorPropertyTable_YIterator yitResetPrinting = m_ContributionVoucherDM.getAccPrintSearchPropTab().insert( basar::FOR_CLEAN );
        yitResetPrinting.setInt16( constants::BRANCHNO, m_BranchNo );
        yitResetPrinting.setInt16( constants::ACKNOWLEDGEMENTSTATUS, constants::FOR_PRINT );
        yitResetPrinting.setInt16( constants::FORMER_ACKNOWLEDGEMENTSTATUS, constants::IN_PRINT );
        m_ContributionVoucherDM.resetNotPrintedBookings( yitResetPrinting );

		//reprint check, part 2: check for not printed (= acknowledgementstatus = IN_PRINT) orders with contributionvoucherno > 0
        //enable check button in mainwindow if found (via signal, keep enabled until user decides what to do)
        //or via main menu, user needs to be allowed to print vouchers
        //user will have to decide if a c-voucher has been printed (Liegt der Ausdruck der Liefer-/Empfangsbestätigung vor?)
        //if user has a hardcopy, it is checked if there is a .pdf-file for this c-voucher, if not, it will be generated using users signature -> set status to PRINTED
        //if user has no hardcopy, one will be printed, also it is checked if there is a .pdf-file for this c-voucher, if not, it will be generated using users signature -> set status to PRINTED
        //dataset will not get a new contributionvoucherno in any case
        //also use whole order as a basis!!!
        m_ContributionVoucherDM.getAccPrintSearchPropTab().clear();
        basar::db::aspect::AccessorPropertyTable_YIterator yitCheckMaybePrinted = m_ContributionVoucherDM.getAccPrintSearchPropTab().insert( basar::FOR_CLEAN );
        yitCheckMaybePrinted.setInt16( constants::BRANCHNO, m_BranchNo );
        yitCheckMaybePrinted.setInt16( constants::ACKNOWLEDGEMENTSTATUS, constants::IN_PRINT );
        
        bool inPrintBookingsPresent = m_ContributionVoucherDM.areBookingsInPrint( yitCheckMaybePrinted );

        if( inPrintBookingsPresent )
        {
            //hm, one shoting is bad... need better idea...
            m_UserInteractionRequiredEmitted = true;
            emit userInteractionRequired();
        }
        else
        {
            //no action required or all errors resolved
            if( m_UserInteractionRequiredEmitted )
            {
                //we notified, we unnotify
                m_UserInteractionRequiredEmitted = false;
                emit errorsResolved();
            }
        }

        //get criteria for internal accounts
		m_ContributionVoucherDM.getAccPrintSearchPropTab().clear();
		basar::db::aspect::AccessorPropertyTable_YIterator yitSearch = m_ContributionVoucherDM.getAccPrintSearchPropTab().insert( basar::FOR_CLEAN );
		yitSearch.setInt16( constants::BRANCHNO, m_BranchNo );
		yitSearch.setInt32( "internalaccounts", m_InternalAccounts );

        //select all printable orders
        basar::db::aspect::AccessorPropertyTableRef allNewOrders = m_ContributionVoucherDM.getAllNewInsertedOrders( yitSearch );

        if( allNewOrders.isNull() || allNewOrders.empty() )
        {
            //nothing to be done -> clean up -> return
		    return;
        }

        basar::db::aspect::AccessorPropertyTable_YIterator yitCurrentOrder = allNewOrders.begin();

        while( !m_TerminationRequested && !yitCurrentOrder.isEnd() )
        {
            //check first if lock is still present            
            if( !userStillHasPrintLock() )
            {
                //user has no printlock, do nothing
                //nothing to be done -> clean up -> return
		        return;
            }            
                  
            m_ContributionVoucherDM.getAccPrintSearchPropTab().clear();
		    basar::db::aspect::AccessorPropertyTable_YIterator yitSrch = m_ContributionVoucherDM.getAccPrintSearchPropTab().insert( basar::FOR_CLEAN );
            yitSrch.setInt16( constants::BRANCHNO, m_BranchNo );
		    yitSrch.setInt32( "internalaccounts", m_InternalAccounts );
            yitSrch.setInt32( constants::CUSTOMERSUPPLIERNO, yitCurrentOrder.getInt32(constants::CUSTOMERSUPPLIERNO) );
            yitSrch.setInt32( constants::TRANSACTIONDATE, yitCurrentOrder.getInt32(constants::TRANSACTIONDATE) );
            yitSrch.setInt32( constants::PURCHASEORDERNO, yitCurrentOrder.getInt32(constants::PURCHASEORDERNO) );
            yitSrch.setInt16( constants::FORMER_ACKNOWLEDGEMENTSTATUS, constants::FOR_PRINT );
            yitSrch.setInt16( constants::ACKNOWLEDGEMENTSTATUS, constants::IN_PRINT );

            //get next order and mark it as IN_PRINT
            basar::db::aspect::AccessorPropertyTableRef insertedOrder = m_ContributionVoucherDM.getNewInsertedOrderAndSetToInPrint( yitSrch );

            if( !insertedOrder.isNull() && !insertedOrder.empty() )
            {
			    printContributionVoucherEx( insertedOrder );
            }

            ++yitCurrentOrder;
        }
	}

//////////////////////////////////////////////////////////////////////////////////////////////
// Print functionality itself is handled in an independent method
//////////////////////////////////////////////////////////////////////////////////////////////
int PrintThread::printContributionVoucherEx( basar::db::aspect::AccessorPropertyTableRef orderToPrint )
{
	int ret = 0;

    basar::Int32 positionsOfOrderToPrint = static_cast<basar::Int32>( orderToPrint.size() );

	// next contributionvoucherno which has to be appended
	basar::Int32 nextInternalContributionVoucherNo = 0;
	
	// first booking
	basar::db::aspect::AccessorPropertyTable_YIterator currentBooking = orderToPrint.begin();
	// yit for appending contributionvoucherno
	basar::db::aspect::AccessorPropertyTable_YIterator voucherNoYit = orderToPrint.begin();
	// get branch data
	basar::db::aspect::AccessorPropertyTable_YIterator yitBranch = m_BranchDM.findBranch();

	basar::db::aspect::AccessorPropertyTable_YIterator yitCustomerSupplier;
	if( currentBooking.getString( constants::TRANSACTIONTYPE ) != TransactionType::DESTRUCTION )
	{
		yitCustomerSupplier = m_ContributionVoucherDM.findPersonalDataByID( currentBooking );
	}

	if( yitCustomerSupplier.isNull() || yitBranch.isNull() || yitCustomerSupplier.isEnd()|| yitBranch.isEnd() )
	{
		basar::VarString msg = "int PrintThread::printContributionVoucher( basar::db::aspect::AccessorPropertyTableRef insertedBookings ):no customerSupplier or yitBranch found for customer supplierno: ";
		msg.append( currentBooking.getSQLString( constants::CUSTOMERSUPPLIERNO ) );

		basar::VarString msg2;
		msg2.format( " printing order: %d (catelogid: %d  ) skipped", currentBooking.getInt32( constants::PURCHASEORDERNO ), currentBooking.getInt32( constants::CATALOGID ) );
		
		msg.append( msg2.c_str() );
		
		BLOG_ERROR( LoggerPool::loggerUseCases,msg );

		//skip ahead to next order - this one is never marked as done -> set status to "not printable" (todo: when is this solved?)

        setNotPrintable( currentBooking );  

        return ret;
	}

    //printing logic works also for less than 6 entries, see if after next while loop
	basar::Int32 beginOfPrintableBookings = 0;
	basar::Int32 endOfPrintableBookings = 0;

	QTime nowTime = QTime::currentTime();	// dont use basar::DateTime in threads!
	basar::Int32 now = nowTime.hour() * 10000 + nowTime.minute() * 100 + nowTime.second();

	// print and decrease bookings counter
	while( positionsOfOrderToPrint > constants::BOOKINGS_PER_FORM )
	{
		endOfPrintableBookings += constants::BOOKINGS_PER_FORM;
		// append contributionvoucherNo 
		nextInternalContributionVoucherNo = m_NumberAssignmentDM.findAndIncrementNoForContext( m_YitSearchSerialOutput );
		basar::Int32 counter = beginOfPrintableBookings;
		
        //need transaction here? all or nothing?

		while( !voucherNoYit.isEnd() && ( counter != endOfPrintableBookings ) )
		{
			// append contributionvoucherno
			voucherNoYit.setInt32( constants::CONTRIBUTIONVOUCHERNO, nextInternalContributionVoucherNo );
            voucherNoYit.setString( constants::USERNAME, m_NarcoticsUserName );
            voucherNoYit.setInt32( constants::PRINTED_DATE, m_Today );
			voucherNoYit.setInt32( constants::PRINTED_TIME, now );
			// set contributionvoucherno of currentBooking in database
			m_ContributionVoucherDM.setContributionVoucherNo( voucherNoYit );
			++voucherNoYit;
			++counter;
		}
		// print voucher
        bool printError = false;

        try
        {
			printForms( orderToPrint, yitBranch, yitCustomerSupplier, beginOfPrintableBookings, endOfPrintableBookings );
        }
        catch(exception::FTPException& ex)
        {
            basar::VarString msg;
            msg.format( "caught ftp exception from printForms call, exception was: %s", ex.what().c_str() );
            BLOG_ERROR( LoggerPool::loggerDomModules, msg.c_str() );
            printError = true;
        }
        catch(...)
        {
            BLOG_ERROR( LoggerPool::loggerDomModules, "caught generic exception from printForms call" );
            printError = true;
        }
		// decrease bookingsCounter by number of already printed bookings
		positionsOfOrderToPrint -= constants::BOOKINGS_PER_FORM;
		// set new begin for bookings to print
		beginOfPrintableBookings += constants::BOOKINGS_PER_FORM;

        //todo: handle any error
        if( !printError )
        {
			setPrinted( currentBooking, nextInternalContributionVoucherNo );
        }

	}
	//handle remainder bookings	
	if( positionsOfOrderToPrint > 0 )
	{			
		endOfPrintableBookings += positionsOfOrderToPrint;
		// append contributionvoucherNo 
		nextInternalContributionVoucherNo = m_NumberAssignmentDM.findAndIncrementNoForContext( m_YitSearchSerialOutput );
		basar::Int32 counter = beginOfPrintableBookings;
		while( !voucherNoYit.isEnd() && ( counter != endOfPrintableBookings ) )
		{
			// append contributionvoucherno
			voucherNoYit.setInt32( constants::CONTRIBUTIONVOUCHERNO, nextInternalContributionVoucherNo );
            voucherNoYit.setString( constants::USERNAME, m_NarcoticsUserName );
            voucherNoYit.setInt32( constants::PRINTED_DATE, m_Today );
			voucherNoYit.setInt32( constants::PRINTED_TIME, now );
			// set contributionvoucherno of currentBooking in database
			m_ContributionVoucherDM.setContributionVoucherNo( voucherNoYit );
			++voucherNoYit;
			++counter;
		}
		// print voucher

        bool printError = false;
        try
        {
			printForms( orderToPrint, yitBranch, yitCustomerSupplier, beginOfPrintableBookings, endOfPrintableBookings );
        }
        catch( exception::FTPException& ex )
        {
            basar::VarString msg;
            msg.format( "caught ftp exception from printForms call, exception was: %s", ex.what().c_str() );
            BLOG_ERROR( LoggerPool::loggerDomModules, msg.c_str() );                
            printError = true;
        }
		catch( basar::Exception& e )
		{
			basar::VarString msg;
            msg.format( "caught exception from printForms call, exception was: %s", e.what().c_str() );
            BLOG_ERROR( LoggerPool::loggerDomModules, msg.c_str() );                
            printError = true;
		}
        catch(...)
        {
            BLOG_ERROR( LoggerPool::loggerDomModules, "caught generic exception from printForms call" );
            printError = true;
        }

        //todo: handle any error
        if( !printError )
        {
			setPrinted( currentBooking, nextInternalContributionVoucherNo );
        }
    }
	
	return ret;
}

//----------------------------------------------------------------------------
void PrintThread::setInPrint( basar::db::aspect::AccessorPropertyTable_YIterator currentBooking )
{
	currentBooking.setInt16( constants::FORMER_ACKNOWLEDGEMENTSTATUS, constants::FOR_PRINT );
	currentBooking.setInt16( constants::ACKNOWLEDGEMENTSTATUS, constants::IN_PRINT );
	basar::db::aspect::ExecuteResultInfo info = m_ContributionVoucherDM.setAcknowledgementstatusOfOrder( currentBooking );
}

//----------------------------------------------------------------------------
void PrintThread::setNotPrintable( basar::db::aspect::AccessorPropertyTable_YIterator currentBooking )
{
	currentBooking.setInt16( constants::FORMER_ACKNOWLEDGEMENTSTATUS, constants::IN_PRINT );
	currentBooking.setInt16( constants::ACKNOWLEDGEMENTSTATUS, constants::NOT_PRINTABLE );
	basar::db::aspect::ExecuteResultInfo info = m_ContributionVoucherDM.setAcknowledgementstatusOfOrder( currentBooking );
}

//----------------------------------------------------------------------------
void PrintThread::setPrinted( basar::db::aspect::AccessorPropertyTable_YIterator currentBooking, basar::Int32 contributionVoucherNo )
{
	currentBooking.setInt16( constants::FORMER_ACKNOWLEDGEMENTSTATUS, constants::IN_PRINT );
	currentBooking.setInt16( constants::ACKNOWLEDGEMENTSTATUS, constants::PRINTED );
	currentBooking.setInt32( constants::CONTRIBUTIONVOUCHERNO, contributionVoucherNo );
	basar::db::aspect::ExecuteResultInfo info = m_ContributionVoucherDM.setAcknowledgementstatusOfOrder( currentBooking );
}

//////////////////////////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////////////////////////
void PrintThread::printForms( basar::db::aspect::AccessorPropertyTableRef insertedBookings, 
				 basar::db::aspect::AccessorPropertyTable_YIterator yitBranch, 
				 basar::db::aspect::AccessorPropertyTable_YIterator yitCustomer, 
				 basar::Int32 startPosition, 
				 basar::Int32 endPosition )
{
	if( narcotics::LoggerPool::loggerDomModules.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) ){
		basar::ConstString msg = "main::printForms";
		narcotics::LoggerPool::loggerDomModules.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__ );
	}

    //maybe we can switch generation of abgabebeleg and printing of Empfangsbestätigung and Lieferschein?
		
	// print Empfangsbestätigung and Lieferschein

	ReceiptNoticePrintData printEmpfang( insertedBookings, yitBranch, yitCustomer, m_YitSignature, startPosition, endPosition );

	// form saved on printer, only data is transferred over the net
	basar::cmnutil::RawPrint rPrint;

	if( rPrint.print( m_PrinterName, printEmpfang ) != basar::cmnutil::PRINTER_SUCCESS )
	{
		if( narcotics::LoggerPool::loggerDomModules.isEnabledFor( log4cplus::ERROR_LOG_LEVEL ) )
		{
			basar::VarString msg = "";
			msg.format( "error while printing receiptnotice for branchno %d user %s: ", m_BranchNo, m_NarcoticsUserName.c_str() );
			msg.append( printEmpfang.getPostPrintingInformation() );
			narcotics::LoggerPool::loggerDomModules.forcedLog( log4cplus::ERROR_LOG_LEVEL, msg, __FILE__, __LINE__ );
		}
	}

	// create Abgabemeldung as pdf file
	ContributionVoucherPrintData pdfAbgabe ( insertedBookings, yitBranch, yitCustomer, m_YitSignature, startPosition, endPosition, m_BranchNo );

	basar::db::aspect::AccessorPropertyTable_YIterator yitBooking = insertedBookings.begin();
	
	for( int i = 0; i < startPosition; i++ )
	{
		++yitBooking;
	}
	
	basar::Int32 contributionvoucherno, bgano;
	
	contributionvoucherno = yitBooking.getInt32( "contributionvoucherno" );
	
	bgano = yitBranch.getInt32( "bgano" );	
	
	ContributionVoucherFTPTransfer ftpTrans( m_FTPhost, m_FTPUser, m_FTPPW, m_ContributionVoucherDir, m_LocalTmpDirectory );	
	
	if( !ftpTrans.storeFile( pdfAbgabe,bgano,contributionvoucherno,m_StartDate ) )
	{	
		sleep(10);
		
		//needed because printdata provides data only once
		ContributionVoucherPrintData pdfAbgabe2( insertedBookings, yitBranch, yitCustomer, m_YitSignature, startPosition, endPosition, m_BranchNo );

		if( !ftpTrans.storeFile( pdfAbgabe2,bgano,contributionvoucherno,m_StartDate ) )
		{	
			basar::cmnutil::ExceptionInfoStruct excInfo;
			excInfo.context = "storeFile(pdfAbgabe,bgano,contributionvoucherno,m_StartDate)";
			excInfo .file =__FILE__;
			excInfo.line=__LINE__;
			
			basar::VarString msg;
			msg.format( "FTP.storeFile Failed!! contributionvoucherno: %d", contributionvoucherno );

			excInfo.reason = msg;
			
			basar::VarString logMsg = "";
			logMsg.format( "PrintThread::printForms (branchno %d, user %s): FTP TRANSFER FAILED for contributionvoucherno %d", m_BranchNo, m_NarcoticsUserName.c_str(), contributionvoucherno );
			BLOG_ERROR( LoggerPool::loggerDomModules, logMsg );

			throw exception::FTPException( excInfo );									
		}	
	}
}

//----------------------------------------------------------------------------
basar::VarString PrintThread::generateFileName( const basar::Int32 bgano, const basar::Int32 contributionvoucherno )
{
	BLOG_TRACE_METHOD( narcotics::LoggerPool::loggerUseCases, "PrintThread::generateFileName()" );

	basar::VarString directoryname;
	basar::VarString filename;
	basar::Int32 serialWeek;
	basar::Int32 serialFileNo;

	// get serial week
	WeekCalculator calc;
	serialWeek = calc.calculateSerialWeek( m_StartDate );

	// determine target folder (-> if it doesn't already exist, it is created automatically)
	directoryname.format( "%s\\%07d%0*d\\",
		m_ContributionVoucherDir.c_str(),
		bgano,
		constants::SERIAL_LENGTH_FOLDER,
		serialWeek );

	// if folder doesn't already exist it has to be created
	QDir dir( QString::fromLocal8Bit(directoryname.c_str()) );
	
	if( dir.exists() == false )
	{
		dir.mkpath( QString::fromLocal8Bit(directoryname.c_str()) );
		serialFileNo = 0;
	}
	else // determine last directory serial number
	{
		QStringList files;
		files = dir.entryList( QDir::Files, QDir::Name );
		serialFileNo = files.size() + 1;
	}

	// append directoryname by target folder
	filename.format( "%s%d%0*d.pdf",
		directoryname.c_str(),
		contributionvoucherno,
		constants::SERIAL_LENGTH_VOUCHER,
		serialFileNo );

	return filename;
}

} //namespace narcotics