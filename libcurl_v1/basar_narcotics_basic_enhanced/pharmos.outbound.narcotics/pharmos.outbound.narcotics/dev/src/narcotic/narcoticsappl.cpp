//-------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  public functions narcotics component (singleton)
 *  \author Thomas Hörath
 *  \date   14.02.2006
 */
//-------------------------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------------------------//
// includes
//-------------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "narcoticsappl.h"
#include "printthread.h"
#include "loggerpool.h"
#include "definitions.h"
#include "prerequisiteschecker.h"
#include "iprerequisitedatasource.h"
#include "postscriptfilesource.h"
#include "printfolderlocator.h"
#include "contributionvoucherhelper.h"
#include <lockmanager/branchlockmanager.h>

//usecases
#include "resolveprintingerrors.h"
#include "contributionvoucher.h"
#include "contributionvoucherscan.h"

#include "ftpservice.h"

//exeptions
#include "missingdependencyinjectionexception.h"

#include <direct.h> // for getting current working directory for loading translation-file


//-------------------------------------------------------------------------------------------------//
// singleton
//-------------------------------------------------------------------------------------------------//
namespace basar {
namespace cmnutil {

using narcotics::NarcoticsApplBase;
template <> typename Singleton<NarcoticsApplBase>::InstancePtr& Singleton<NarcoticsApplBase>::inst()
{
	static InstancePtr s_inst; 
	return s_inst;
}

} // namespace cmnutil
} // namespace basar

//-------------------------------------------------------------------------------------------------//
// namespace
//-------------------------------------------------------------------------------------------------//
namespace narcotics
{
	using narcotic::useCase::services::IPrerequisiteDataSource;
	using narcotic::useCase::services::PrerequisitesCheckerSV;
	using narcotic::dataAccess::PostScriptFileSource;

/////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw  no-throw
	\return pointer to LoginImpl itself */
/////////////////////////////////////////////////////////////////////////////////////////////////////
NarcoticsApplBase::NarcoticsApplSharedPtr NarcoticsApplBase::create()
{
	NarcoticsApplSharedPtr t = NarcoticsApplSharedPtr( new NarcoticsApplBase );
	return t;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw  no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////
NarcoticsApplBase::NarcoticsApplBase() :
	m_PrintThread(NULL)
{
	m_Initialized = false;
	m_Relogin = false;
	m_ElectronicContributionVoucher = false;
	m_NarcoticsDM = boost::shared_ptr<domMod::NarcoticsDM>( new domMod::NarcoticsDM );
	m_NarcoticsGroupDM = boost::shared_ptr<domMod::NarcoticsGroupDM>( new domMod::NarcoticsGroupDM );
	m_CustomerDM = boost::shared_ptr<domMod::CustomerDM>( new domMod::CustomerDM );
	m_SupplierDM = boost::shared_ptr<domMod::SupplierDM>( new domMod::SupplierDM );
	m_ContributionVoucherDM = boost::shared_ptr<domMod::ContributionVoucherDM>( new domMod::ContributionVoucherDM );
	m_SearchDM = boost::shared_ptr<domMod::SearchDM>( new domMod::SearchDM );
	m_OrderDM = boost::shared_ptr<domMod::OrderDM>( new domMod::OrderDM );
    m_BranchDM = boost::shared_ptr<domMod::BranchDM>( new domMod::BranchDM );
    m_LogDM = boost::shared_ptr<domMod::LogDM>( new domMod::LogDM );
	m_PrinterName = "";
	m_HasPrintLock = false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw EVENT_HANDLERS_REGISTRATION no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_HANDLERS_REGISTRATION(NarcoticsApplBase)
	SYSTEM_EVENT_HANDLER_REGISTRATION( NarcoticsApplBase, Relogin )
END_HANDLERS_REGISTRATION()

//----------------------------------------------------------------------------
boost::shared_ptr<domMod::OrderDM> NarcoticsApplBase::getOrderDM()
{
	//poor programmer's singleton :)
	if( !m_OrderDM->isInitialized() )
	{
		basar::Int16 branchno = basar::login::Manager::getInstance().getAreaID();
		
		m_LogDM->init( m_Connection.getCurrentConnection(), 
										basar::login::Manager::getInstance().getAreaID(),
										basar::login::Manager::getInstance().getUserName() );

		m_OrderDM->init(branchno,m_Connection.getCurrentConnection(), m_LogDM, basar::login::Manager::getInstance().getUserName());
	}
	return m_OrderDM;
}

//----------------------------------------------------------------------------
boost::shared_ptr<domMod::LogDM> NarcoticsApplBase::getLogDM()
{
	if( !m_LogDM->isInitialized() )
    {
        m_LogDM->init( m_Connection.getCurrentConnection(), 
										    basar::login::Manager::getInstance().getAreaID(),
        									basar::login::Manager::getInstance().getUserName() );
    }
	return m_LogDM;
}

//----------------------------------------------------------------------------
bool NarcoticsApplBase::isNewNarcoticsProcess( basar::VarString branchnotxt )
{
	basar::I18nString status;

	bool ret = false;

	basar::login::Manager::getInstance().getParaValue( branchnotxt, UADM_NEW_NARCOTIC_PROCESS, status );

	if( !status.empty() )
	{
		status.trim( basar::cmnutil::BString::ALL );

		if( status == "1" ){
			ret = true;
		}
	}	
	return ret;
}

//----------------------------------------------------------------------------
const bool NarcoticsApplBase::isNewNarcoticsProcess()const
{
	basar::I18nString status;

	bool ret = false;
	
	basar::login::LoginBase loginBase = basar::login::Manager::getInstance();

	std::stringstream ss;
	ss << loginBase.getAreaID();
	const basar::I18nString currentBranchnoTxt( ss.str().c_str() );
		
	loginBase.getParaValue( currentBranchnoTxt, UADM_NEW_NARCOTIC_PROCESS, status );

	if( !status.empty() ){
		status.trim( basar::cmnutil::BString::ALL );

		if( status == "1" ){
			ret = true;
		}
	}	
	return ret;
}

//----------------------------------------------------------------------------
bool NarcoticsApplBase::areElectronicDeliveryNotePrerequisitesFullfilled()
{	
	basar::Int16 branchno = basar::login::Manager::getInstance().getAreaID();

	return m_PrerequisitesCheckerSV->areElectronicDeliveryNotePrerequisitesFullfilled( basar::login::Manager::getInstance().getUserName(), branchno );	
}

//----------------------------------------------------------------------------
bool NarcoticsApplBase::arePrerequisitesFullfilled()
{
	return false;
}

//----------------------------------------------------------------------------
const bool NarcoticsApplBase::hasLockForPrinting() const
{
	return m_HasPrintLock;
}

//----------------------------------------------------------------------------
void NarcoticsApplBase::releaseAllPrintLocks()
{
	if( m_LockManager != NULL )
	{		
		std::ostringstream os;
		os	<< QApplication::translate(	"NarcoticsAppl", 
			"Are you sure to release all printing locks for your branch?" ).toLocal8Bit().data()
			<< std::endl;
		basar::gui::tie::MessageBoxButtonEnum answer = basar::gui::tie::questionMsgBox( 0, os.str().c_str() );

		if( answer == basar::gui::tie::ButtonYes )
		{	
			if( m_LockManager->releaseAllLocksByBusinessType( constants::PRINTLOCK ) )
			{
				m_HasPrintLock = false;

				basar::VarString logMsg;
				logMsg.format( "ALL Print Thread locks for branchno < %d > released by user < %s >.", 
					basar::login::Manager::getInstance().getAreaID(), basar::login::Manager::getInstance().getUserName().c_str() );

				BLOG_INFO( LoggerPool::loggerUseCases, logMsg );
			}
		}
	}
}

//----------------------------------------------------------------------------
void NarcoticsApplBase::releaseTransferLocks()
{
	if( m_LockManager != NULL )
	{		
		std::ostringstream os;
		os	<< QApplication::translate(	"NarcoticsAppl", 
			"Are you sure to release all article locks for your branch?").toLocal8Bit().data()
			<< std::endl;
		basar::gui::tie::MessageBoxButtonEnum answer = basar::gui::tie::questionMsgBox(0, os.str().c_str() );

		if( answer == basar::gui::tie::ButtonYes ){
			m_LockManager->releaseAllLocksByBusinessTypeSpecialUserExcluded( constants::TRANSFER_BUSINESS_TYPE );
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw  no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////
bool NarcoticsApplBase::init( bool checkDifferenceQtyInStock /*=true*/ )
{
	// init lockmanager
	if( m_Initialized ){ return true; }

	registerEventHandlers(true);	//!< register = true / deregister = false

	basar::I18nString appl = "NARCOTICS";
	
	narcotic::useCase::services::PrintFolderLocator locator;

	basar::VarString path = locator.getPrintFolderPath();

	if( path.empty() )
	{
		basar::cmnutil::ExceptionInfoStruct excInfo;
		excInfo.context = "NarcoticsApplBase::init( bool checkDifferenceQtyInStock /*=true*/ )";
		excInfo .file =__FILE__;
		excInfo.line=__LINE__;
		excInfo.reason = basar::VarString("folder not found!: ").append( constants::PRINTFOLDERNAME );
		
		throw basar::cmnutil::BasarRuntimeException( excInfo );		
	}
	
	basar::Int16 branchno = basar::login::Manager::getInstance().getAreaID();

	infrastructure::contributionVoucher::ContributionVoucherHelper contributionVoucherHelper( branchno );
	
	path.append("/").append(contributionVoucherHelper.buildFileName( constants::CONTRIBUTIONVOUCHERBASEFILENAME ) );

	boost::shared_ptr<IPrerequisiteDataSource> dataSource( new PostScriptFileSource( path ) );

	m_PrerequisitesCheckerSV = boost::shared_ptr<PrerequisitesCheckerSV>( new PrerequisitesCheckerSV( dataSource ) );

	if( basar::login::Manager::getInstance().nonInteractiveLogin(	appl,
															basar::login::Manager::getInstance().getUserName(), 
															basar::login::Manager::getInstance().getCryptPW(),
															basar::login::Manager::getInstance().getAreaID() ) 
		!= true )
	{
		return false;
	}
	
	m_Connection.initConnection();
	m_Connection.connect();

	loadTranslators();
	
	// get branch specific default printer from UADM
	basar::I18nString branchnotxt;
	branchnotxt.itos( basar::login::Manager::getInstance().getAreaID() );
	basar::login::Manager::getInstance().getParaValue( branchnotxt, UADM_NARCOTICSPRINTER, m_PrinterName );

	basar::I18nString status;

	m_LockManager = boost::shared_ptr<libnarcotics::lockmanager::BranchLockManager>(
		new libnarcotics::lockmanager::BranchLockManager(
			basar::login::Manager::getInstance().getAreaID(),
			m_Connection.getCurrentConnection(),
			basar::login::Manager::getInstance().getUserName(),
			constants::PRINTLOCK_TABLE_NAME,
			LoggerPool::loggerDomModules));

	if( isNewNarcoticsProcess( branchnotxt ) )
	{
		m_ElectronicContributionVoucher = true;		
	}

	// nur auf stock differenes prüfen wenn auch berechtigt			
	if( basar::login::Manager::getInstance().isLegitimated( UADM_MODULE_MAIN, UADM_RIGHT_GENERAL ) )
	{
		m_NarcoticsDM->init( basar::login::Manager::getInstance().getAreaID(), 
			m_Connection.getCurrentConnection(), 
			basar::login::Manager::getInstance().getUserName() );
		
		if( checkDifferenceQtyInStock && m_NarcoticsDM->checkExistDiffQtyInStock() )
		{
			std::ostringstream os;
			os	<< QApplication::translate(	"NarcoticsAppl", 
				"There are differences between the quantity in stock of the original data "
				"and the catalog data.").toLocal8Bit().data()
				<< std::endl 
				<< QApplication::translate(	"NarcoticsAppl", "Should the differences be displayed?").toLocal8Bit().data()
				<< std::endl;
			basar::gui::tie::MessageBoxButtonEnum answer = basar::gui::tie::questionMsgBox(0, os.str().c_str());

			if( answer == basar::gui::tie::ButtonYes ){
				m_DiffQtyInStockUC.run();
			}
		}
		m_NarcoticsDM->shutdown();
	}
    
    basar::Int16 branchNo = basar::login::Manager::getInstance().getAreaID();

    m_ServiceLocator = infrastructure::ServiceLocatorPtr( new infrastructure::ServiceLocator( branchNo, 
								m_Connection.getCurrentConnection() ) );

	m_BranchDM->init( branchNo, m_Connection.getCurrentConnection() );
											
	// get branch data
    basar::db::aspect::AccessorPropertyTable_YIterator yitBranch = m_BranchDM->findBranch();

    basar::Int32 bgano = -1;
    if( !yitBranch.isNull() && !yitBranch.isEnd() ){
        bgano = yitBranch.getInt32( constants::BGANO );
    }

    m_BranchDM->shutdown(); //???

    basar::I18nString startDatetxt;
    basar::login::Manager::getInstance().getParaValue( branchnotxt, UADM_STARTDATE, startDatetxt );

    m_ServiceLocator->injectFtpService( boost::shared_ptr<infrastructure::FtpService>( new infrastructure::FtpService( bgano, startDatetxt, getFtpParameters() ) ) );
    m_ServiceLocator->injectBranchDM( m_BranchDM );
    m_ServiceLocator->injectContributionVoucherDM( m_ContributionVoucherDM );
    m_ServiceLocator->injectSearchDM( m_SearchDM );
    m_ServiceLocator->injectCustomerDM( m_CustomerDM );
	m_ServiceLocator->injectSupplierDM( m_SupplierDM );

	m_Initialized = true;
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw  no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////
NarcoticsApplBase::~NarcoticsApplBase()
{	
	if( isActive() ){
		shutdown();
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw  no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////
bool NarcoticsApplBase::loadTranslators()
{
	basar::I18nString language = basar::login::Manager::getInstance().getLanguageID();
	language.lower();

	//----------- Translator for preview / about (guiqt) ----------------------------------------------
	QString file = "libbasarguiqt_";
	file.append( language.data() );

	char buffer[_MAX_PATH];
	_getcwd( buffer, _MAX_PATH ); // current working directory
	QString cwd = buffer;
	cwd += "/../multilang";

	m_trGuiQt.load( file, cwd );
	QApplication::instance()->installTranslator( &m_trGuiQt );

	file = "qt_";
	file.append( language.data() );
	m_trQt.load( file, cwd );
	QApplication::instance()->installTranslator( &m_trQt );

	//----------- Translator for narcotic.exe (main menu) ----------------------------------------------
	file = "narcotic_";
	file.append( language.data() );
	QString path = QCoreApplication::applicationDirPath();
	path += "/multilang";

	m_trNarcotic.load( file, path );
	QApplication::instance()->installTranslator( &m_trNarcotic );
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw  no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////
bool NarcoticsApplBase::checkUserRight( basar::ConstString rightName )
{
	if( !( basar::login::Manager::getInstance().isLegitimated( UADM_MODULE_MAIN, rightName ) ) )
	{
		basar::ConstString noRight = QApplication::translate( "NarcoticsAppl", 
			"There is no userright granted to your account\n"
			"which would allow you to work with this part of the program!" ).toLocal8Bit().constData();

		basar::gui::tie::infoMsgBox(0, noRight, QApplication::translate("NarcoticsAppl", "Narcotics").toLocal8Bit().constData());
		return false;
	}
	return true;
}

//-------------------------------------------------------------------------------------------------//
// to show the dialogs
//-------------------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw  no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////
void NarcoticsApplBase::startIncomingGoods(QWidget * mw)
{
	try 
	{
		init();

		if( !( checkUserRight( UADM_RIGHT_INCOME ) ) ){
			return;
		}

		m_IncomingGoodsOverviewVC.setParent(mw);
		
		m_IncomingGoodsUC.run();
	} 
	catch( basar::Exception& e )
	{
		basar::gui::tie::criticalMsgBox( 0, getExceptionDefaultText( e.what() ).c_str(), QApplication::translate("NarcoticsAppl", "Narcotics").toLocal8Bit().constData() );
	}
	catch( const std::exception & )
	{
		basar::gui::tie::warningMsgBox( 0, "=== std exception caught ====", QApplication::translate("NarcoticsAppl", "Narcotics").toLocal8Bit().constData() );
	}
    catch(...)
	{
		basar::gui::tie::warningMsgBox( 0, "=== unknown exception caught ====", QApplication::translate("NarcoticsAppl", "Narcotics").toLocal8Bit().constData() );
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//!	\throw  no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////
void NarcoticsApplBase::startNarcoticsBook(QWidget * mw)
{
	try 
	{	
		init();

		if( !( checkUserRight( UADM_RIGHT_GENERAL ) ) ){
			return;
		}

		m_NarcoticBookVC.setParent(mw);

		m_BookingCatalogUC.run();
	} 
	catch( basar::Exception& e )
	{
		basar::gui::tie::criticalMsgBox( 0, getExceptionDefaultText( e.what() ).c_str(), QApplication::translate("NarcoticsAppl", "Narcotics").toLocal8Bit().constData() );
	}
	catch( const std::exception & )
	{
		basar::gui::tie::warningMsgBox( 0, "=== std exception caught ====", QApplication::translate("NarcoticsAppl", "Narcotics").toLocal8Bit().constData() );
	}
    catch(...)
	{
		basar::gui::tie::warningMsgBox( 0, "=== unknown exception caught ====", QApplication::translate("NarcoticsAppl", "Narcotics").toLocal8Bit().constData() );
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw  no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////
void NarcoticsApplBase::startNarcoticsCategory(QWidget * mw)
{
	try 
	{
		init();

		if( !( checkUserRight( UADM_RIGHT_GENERAL ) ) ){
			return;
		}

		m_NarcoticCategoryVC.setParent(mw);

		m_NarcoticsCategoryUC.run();
	} 
	catch( basar::Exception& e )
	{
		basar::gui::tie::criticalMsgBox( 0, getExceptionDefaultText( e.what() ).c_str(), QApplication::translate("NarcoticsAppl", "Narcotics").toLocal8Bit().constData() );
	}
	catch( const std::exception & )
	{
		basar::gui::tie::warningMsgBox( 0, "=== std exception caught ====", QApplication::translate("NarcoticsAppl", "Narcotics").toLocal8Bit().constData() );
	}
    catch(...)
	{
		basar::gui::tie::warningMsgBox( 0, "=== unknown exception caught ====", QApplication::translate("NarcoticsAppl", "Narcotics").toLocal8Bit().constData() );
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//!	\throw  no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////
void NarcoticsApplBase::startCompareNarcoticsOriginal(QWidget * mw)
{
	try 
	{
		init();

		if( !( checkUserRight( UADM_RIGHT_GENERAL ) ) ){
			return;
		}

		m_CompareBookingsVC.setParent(mw);

		m_CompareBookingsUC.run();
	} 
	catch( basar::Exception& e )
	{
		basar::gui::tie::criticalMsgBox( 0, getExceptionDefaultText( e.what() ).c_str(), QApplication::translate("NarcoticsAppl", "Narcotics").toLocal8Bit().constData() );
	}
	catch( const std::exception & )
	{
		basar::gui::tie::warningMsgBox( 0, "=== std exception caught ====", QApplication::translate("NarcoticsAppl", "Narcotics").toLocal8Bit().constData() );
	}
    catch(...)
	{
		basar::gui::tie::warningMsgBox( 0, "=== unknown exception caught ====", QApplication::translate("NarcoticsAppl", "Narcotics").toLocal8Bit().constData() );
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//!	\throw  no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////
void NarcoticsApplBase::startNarcoticsProtocol(QWidget * mw)
{
	try 
	{
		init();

		if( !( checkUserRight( UADM_RIGHT_PROTOCOL ) ) ){
			return;
		}

		m_NarcoticsProtocolVC.setParent(mw);
		
		m_NarcoticsProtocolUC.run();
	} 
	catch( basar::Exception& e )
	{
		basar::gui::tie::criticalMsgBox( 0, getExceptionDefaultText( e.what() ).c_str(), QApplication::translate("NarcoticsAppl", "Narcotics").toLocal8Bit().constData() );
	}
	catch( const std::exception & )
	{
		basar::gui::tie::warningMsgBox( 0, "=== std exception caught ====", QApplication::translate("NarcoticsAppl", "Narcotics").toLocal8Bit().constData() );
	}
    catch(...)
	{
		basar::gui::tie::warningMsgBox( 0, "=== unknown exception caught ====", QApplication::translate("NarcoticsAppl", "Narcotics").toLocal8Bit().constData() );
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw  no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////
void NarcoticsApplBase::startHalfYearlyNotification(QWidget * mw)
{
	try 
	{
		init();

		if( !( checkUserRight( UADM_RIGHT_GENERAL ) ) ){
			return;
		}

		m_HalfYearlyReportVC.setParent(mw);

		m_HalfYearlyReportUC.run();
	} 
	catch( basar::Exception& e )
	{
		basar::gui::tie::criticalMsgBox( 0, getExceptionDefaultText( e.what() ).c_str(), QApplication::translate("NarcoticsAppl", "Narcotics").toLocal8Bit().constData() );
	}
	catch( const std::exception & )
	{
		basar::gui::tie::warningMsgBox( 0, "=== std exception caught ====", QApplication::translate("NarcoticsAppl", "Narcotics").toLocal8Bit().constData() );
	}
    catch(...)
	{
		basar::gui::tie::warningMsgBox( 0, "=== unknown exception caught ====", QApplication::translate("NarcoticsAppl", "Narcotics").toLocal8Bit().constData() );
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw  no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////
void NarcoticsApplBase::startDiffQtyInStock(QWidget * mw)
{
	try 
	{
		init( false );

		if( !( checkUserRight( UADM_RIGHT_GENERAL ) ) ){
			return;
		}

		m_DiffQtyInStockVC.setParent(mw);

		m_DiffQtyInStockUC.run();
	} 
	catch( basar::Exception& e )
	{
		basar::gui::tie::criticalMsgBox( 0,  getExceptionDefaultText( e.what() ).c_str(), QApplication::translate("NarcoticsAppl", "Narcotics").toLocal8Bit().constData() );
	}
	catch( const std::exception & )
	{
		basar::gui::tie::warningMsgBox( 0, "=== std exception caught ====", QApplication::translate("NarcoticsAppl", "Narcotics").toLocal8Bit().constData() );
	}
    catch(...)
	{
		basar::gui::tie::warningMsgBox( 0, "=== unknown exception caught ====", QApplication::translate("NarcoticsAppl", "Narcotics").toLocal8Bit().constData() );
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw  no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////
void NarcoticsApplBase::startCustomerMapping(QWidget * mw)
{
	try
	{
		init();

		if( !( checkUserRight( UADM_RIGHT_GENERAL ) ) ){
			return;
		}

		m_CustomerMappingVC.setParent(mw);
			
		m_CustomerMappingUC.run();	
	} 
	catch( basar::Exception& e )
	{
	    basar::gui::tie::criticalMsgBox( 0, getExceptionDefaultText( e.what() ).c_str(), QApplication::translate("NarcoticsAppl", "Narcotics").toLocal8Bit().constData() );
	}
	catch( const std::exception & )
	{
		basar::gui::tie::warningMsgBox( 0, "=== std exception caught ====", QApplication::translate("NarcoticsAppl", "Narcotics").toLocal8Bit().constData() );
	}
    catch(...)
	{
		basar::gui::tie::warningMsgBox( 0, "=== unknown exception caught ====", QApplication::translate("NarcoticsAppl", "Narcotics").toLocal8Bit().constData() );
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw  no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////
void NarcoticsApplBase::startTransactionData(QWidget * mw)
{
	try
	{
		init();

		if( !( checkUserRight( UADM_RIGHT_GENERAL ) ) ){
			return;
		}

		m_TransactionDataVC.setParent(mw);

		m_GoodsTransactionUC.run();	
	} 
	catch( basar::Exception& e )
	{
	    basar::gui::tie::criticalMsgBox( 0, getExceptionDefaultText( e.what() ).c_str(), QApplication::translate("NarcoticsAppl", "Narcotics").toLocal8Bit().constData() );
	}
	catch( const std::exception & )
	{
		basar::gui::tie::warningMsgBox( 0, "=== std exception caught ====", QApplication::translate("NarcoticsAppl", "Narcotics").toLocal8Bit().constData() );
	}
    catch(...)
	{
		basar::gui::tie::warningMsgBox( 0, "=== unknown exception caught ====", QApplication::translate("NarcoticsAppl", "Narcotics").toLocal8Bit().constData() );
	}
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw  no-throw */
//-------------------------------------------------------------------------------------------------//
void NarcoticsApplBase::startStockList(QWidget * mw)
{
	try 
	{
		init();

		if( !( checkUserRight( UADM_RIGHT_GENERAL ) ) ){
			return;
		}
	
		m_NarcoticsDM->init( basar::login::Manager::getInstance().getAreaID(), 
							 m_Connection.getCurrentConnection(), 
							 basar::login::Manager::getInstance().getUserName()
							);

		m_InputDateVc.setParent(mw);
		
		basar::appl::EventReturnType ret = 
			basar::appl::SystemEventManager::getInstance().fire( "PrintStockList" ); // already called here, because no own VC
		
		if( ret.ret == basar::appl::HANDLER_INFO ){
			basar::gui::tie::infoMsgBox( 0, ret.message.c_str(), QApplication::translate("NarcoticsAppl", "Narcotics").toLocal8Bit().constData() );
		}
	} 
	catch( basar::Exception& e )
	{
		basar::gui::tie::criticalMsgBox( 0, getExceptionDefaultText( e.what() ).c_str(), QApplication::translate("NarcoticsAppl", "Narcotics").toLocal8Bit().constData() );
	}
	catch( const std::exception & )
	{
		basar::gui::tie::warningMsgBox( 0, "=== std exception caught ====", QApplication::translate("NarcoticsAppl", "Narcotics").toLocal8Bit().constData() );
	}
    catch(...)
	{
		basar::gui::tie::warningMsgBox( 0, "=== unknown exception caught ====", QApplication::translate("NarcoticsAppl", "Narcotics").toLocal8Bit().constData() );
	}
	m_NarcoticsDM->shutdown();
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw  no-throw */
//-------------------------------------------------------------------------------------------------//
void NarcoticsApplBase::startStockListQuick(QWidget * mw)
{
	try 
	{
		init();

		if ( !( checkUserRight( UADM_RIGHT_GENERAL ) ) ){
			return;
		}
	
		m_NarcoticsDM->init( basar::login::Manager::getInstance().getAreaID(), 
							 m_Connection.getCurrentConnection(), 
							 basar::login::Manager::getInstance().getUserName()
							);

		m_InputDateVc.setParent(mw);
		
		basar::appl::EventReturnType ret = 
			basar::appl::SystemEventManager::getInstance().fire( "PrintStockListQuick" ); // already called here, because no own VC
		
		if( ret.ret == basar::appl::HANDLER_INFO )
		{
			basar::gui::tie::infoMsgBox( 0, ret.message.c_str(), QApplication::translate("NarcoticsAppl", "Narcotics").toLocal8Bit().constData() );
		}
	} 
	catch( basar::Exception& e )
	{
		basar::gui::tie::criticalMsgBox( 0, getExceptionDefaultText( e.what() ).c_str(), QApplication::translate("NarcoticsAppl", "Narcotics").toLocal8Bit().constData() );
	}
	catch( const std::exception & )
	{
		basar::gui::tie::warningMsgBox( 0, "=== std exception caught ====", QApplication::translate("NarcoticsAppl", "Narcotics").toLocal8Bit().constData() );
	}
    catch(...)
	{
		basar::gui::tie::warningMsgBox( 0, "=== unknown exception caught ====", QApplication::translate("NarcoticsAppl", "Narcotics").toLocal8Bit().constData() );
	}
	m_NarcoticsDM->shutdown();
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw  no-throw */
//-------------------------------------------------------------------------------------------------//
void NarcoticsApplBase::startCountList(QWidget * mw)
{
	try 
	{
		init();

		if( !( checkUserRight( UADM_RIGHT_GENERAL ) ) ){
			return;
		}
		
		m_NarcoticsDM->init( basar::login::Manager::getInstance().getAreaID(), 
							 m_Connection.getCurrentConnection(), 
							 basar::login::Manager::getInstance().getUserName()
							);

		m_InputDateVc.setParent(mw);
		
		basar::appl::EventReturnType ret = 
			basar::appl::SystemEventManager::getInstance().fire( "PrintCountList" ); // already called here, because no own VC
		
		if( ret.ret == basar::appl::HANDLER_INFO )
		{
			basar::gui::tie::infoMsgBox( 0, ret.message.c_str(), QApplication::translate("NarcoticsAppl", "Narcotics").toLocal8Bit().constData() );
		}
	} 
	catch( basar::Exception& e )
	{
		basar::gui::tie::criticalMsgBox( 0, getExceptionDefaultText( e.what() ).c_str(), QApplication::translate("NarcoticsAppl", "Narcotics").toLocal8Bit().constData() );
	}
	catch( const std::exception & )
	{
		basar::gui::tie::warningMsgBox( 0, "=== std exception caught ====", QApplication::translate("NarcoticsAppl", "Narcotics").toLocal8Bit().constData() );
	}
    catch(...)
	{
		basar::gui::tie::warningMsgBox ( 0, "=== unknown exception caught ====", QApplication::translate("NarcoticsAppl", "Narcotics").toLocal8Bit().constData() );
	}
	m_NarcoticsDM->shutdown();
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw  no-throw */
//-------------------------------------------------------------------------------------------------//
void NarcoticsApplBase::startDissolveReceipt(QWidget * mw)
{
	try 
	{
		init();

		if( !( checkUserRight( UADM_RIGHT_GENERAL ) ) ){
			return;
		}
		
		m_NarcoticsDM->init( basar::login::Manager::getInstance().getAreaID(), 
							 m_Connection.getCurrentConnection(), 
							 basar::login::Manager::getInstance().getUserName()
							);

		m_InputDateVc.setParent(mw);
		
		basar::appl::EventReturnType ret = 
			basar::appl::SystemEventManager::getInstance().fire( "PrintDissolveReceipt" ); // already called here, because no own VC
		
		if( ret.ret == basar::appl::HANDLER_INFO )
		{
			basar::gui::tie::infoMsgBox( 0, ret.message.c_str(), QApplication::translate("NarcoticsAppl", "Narcotics").toLocal8Bit().constData() );
		}
	} 
	catch( basar::Exception& e )
	{
		basar::gui::tie::criticalMsgBox( 0, getExceptionDefaultText( e.what() ).c_str(), QApplication::translate("NarcoticsAppl", "Narcotics").toLocal8Bit().constData() );
	}
	catch( const std::exception & )
	{
		basar::gui::tie::warningMsgBox( 0, "=== std exception caught ====", QApplication::translate("NarcoticsAppl", "Narcotics").toLocal8Bit().constData() );
	}
    catch(...)
	{
		basar::gui::tie::warningMsgBox( 0, "=== unknown exception caught ====", QApplication::translate("NarcoticsAppl", "Narcotics").toLocal8Bit().constData() );
	}
	m_NarcoticsDM->shutdown();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw  no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////
void NarcoticsApplBase::startOrderOverview(QWidget * mw)
{
	try {
		init();

		if( !( checkUserRight( UADM_RIGHT_INCOME ) ) ){
			return;
		}

		m_OrderOverviewVC.setParent(mw);
		
		m_OrderOverviewUC.run();
	} 
	catch( basar::Exception& e )
	{
		basar::gui::tie::criticalMsgBox( 0, getExceptionDefaultText( e.what() ).c_str(), QApplication::translate("NarcoticsAppl", "Narcotics").toLocal8Bit().constData() );
	}
	catch( const std::exception & )
	{
		basar::gui::tie::warningMsgBox( 0, "=== std exception caught ====", QApplication::translate("NarcoticsAppl", "Narcotics").toLocal8Bit().constData() );
	}
    catch(...)
	{
		basar::gui::tie::warningMsgBox( 0, "=== unknown exception caught ====", QApplication::translate("NarcoticsAppl", "Narcotics").toLocal8Bit().constData() );
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw  no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////
void NarcoticsApplBase::startContributionVoucher(QWidget * mw)
{
	try {
		init();

		if( !( checkUserRight( UADM_RIGHT_GENERAL ) ) ){
			return;
        }
		useCase::ContributionVoucherUC cvUC;
        
        //inits
        m_LogDM->init( m_Connection.getCurrentConnection(), basar::login::Manager::getInstance().getAreaID(), basar::login::Manager::getInstance().getUserName() );
        m_ContributionVoucherDM->init( basar::login::Manager::getInstance().getAreaID(), m_Connection.getCurrentConnection() );

        m_ContributionVoucherDM->injectLogDM( m_LogDM );

        m_NarcoticsDM->init( basar::login::Manager::getInstance().getAreaID(), m_Connection.getCurrentConnection(), basar::login::Manager::getInstance().getUserName() );
        m_CustomerDM->init( basar::login::Manager::getInstance().getAreaID(), m_Connection.getCurrentConnection() );
		m_SupplierDM->init( basar::login::Manager::getInstance().getAreaID(), m_Connection.getCurrentConnection() );
        m_SearchDM->init( m_Connection.getCurrentConnection() );
        m_BranchDM->init( basar::login::Manager::getInstance().getAreaID(), m_Connection.getCurrentConnection() );

        //injects
        cvUC.injectContributionVoucherDM( m_ContributionVoucherDM );
        cvUC.injectNarcoticsDM( m_NarcoticsDM );
        cvUC.injectCustomerDM( m_CustomerDM );
        cvUC.injectSearchDM( m_SearchDM );
        cvUC.injectBranchDM( m_BranchDM );

        cvUC.injectServiceLocator( m_ServiceLocator );

		cvUC.injectParentWindow(mw);

        cvUC.run();        
	} 
	catch( basar::Exception& e )
	{
		basar::gui::tie::criticalMsgBox( 0, getExceptionDefaultText( e.what() ).c_str(), QApplication::translate("NarcoticsAppl", "Narcotics").toLocal8Bit().constData() );
	}
	catch( const std::exception & )
	{
		basar::gui::tie::warningMsgBox( 0, "=== std exception caught ====", QApplication::translate("NarcoticsAppl", "Narcotics").toLocal8Bit().constData() );
	}
    catch(...)
	{
		basar::gui::tie::warningMsgBox( 0, "=== unknown exception caught ====", QApplication::translate("NarcoticsAppl", "Narcotics").toLocal8Bit().constData() );
	}

    //shutdowns
    m_ContributionVoucherDM->shutdown();
    m_LogDM->shutdown();
    m_NarcoticsDM->shutdown();
    m_CustomerDM->shutdown();
    m_SearchDM->shutdown();
    m_BranchDM->shutdown();
}

//----------------------------------------------------------------------------
PrintThread* NarcoticsApplBase::getPrintThread()
{
	return m_PrintThread;
}

//----------------------------------------------------------------------------
void NarcoticsApplBase::fillPrintThreadInitParams( PrintThreadInitParams& p )
{
    //get parameters for printing
    basar::I18nString branchnotxt;
	basar::I18nString printerName;
	basar::I18nString startDatetxt;
    basar::I18nString directoryname;
	basar::I18nString internalaccounts;

	basar::I18nString hostname;
	basar::I18nString user;
	basar::I18nString pw;
	basar::I18nString localTmpDirectory;

    p.branchNo = basar::login::Manager::getInstance().getAreaID();
    p.narcoticsUserName = basar::login::Manager::getInstance().getUserName();
    p.narcoticsUserSurName = basar::login::Manager::getInstance().getSurName();
    p.narcoticsUserForeName = basar::login::Manager::getInstance().getForeName();

    branchnotxt.itos(basar::login::Manager::getInstance().getAreaID());
	basar::login::Manager::getInstance().getParaValue( branchnotxt, UADM_NARCOTICSPRINTER, printerName );
	// name of directory that shall contain Abgabemeldungen (-> fully qualified name is needed)
	basar::login::Manager::getInstance().getParaValue( UADM_CONTRIBUTIONVOUCHERDIR, directoryname );
	basar::login::Manager::getInstance().getParaValue( branchnotxt, UADM_STARTDATE, startDatetxt );
	basar::login::Manager::getInstance().getParaValue( UADM_START_INTERNAL_BOOKINGS_NO, internalaccounts );

    basar::login::Manager::getInstance().getParaValue( UADM_FTP_CITRIX_HOST, hostname );
    basar::login::Manager::getInstance().getParaValue( UADM_FTP_CITRIX_USER, user );
    basar::login::Manager::getInstance().getParaValue( UADM_FTP_CITRIX_PW, pw );
    basar::login::Manager::getInstance().getParaValue( UADM_FTP_LOCAL_TMP, localTmpDirectory );

	//todo: getFtpParameters here

    p.printerName = printerName;
    p.directoryname = directoryname;
    p.startDatetxt = startDatetxt;
    p.internalaccounts = internalaccounts;
    p.hostname = hostname;
    p.user = user;
    p.pw = pw;
    p.localTmpDirectory = localTmpDirectory;
}

//----------------------------------------------------------------------------
infrastructure::FtpParameters NarcoticsApplBase::getFtpParameters()
{
    basar::I18nString hostname;
	basar::I18nString user;
	basar::I18nString pw;
	basar::I18nString localTmpDirectory;
 
    basar::I18nString directoryname;
    
    basar::login::Manager::getInstance().getParaValue( UADM_FTP_CITRIX_HOST, hostname );
    basar::login::Manager::getInstance().getParaValue( UADM_FTP_CITRIX_USER, user );
    basar::login::Manager::getInstance().getParaValue( UADM_FTP_CITRIX_PW, pw );
    basar::login::Manager::getInstance().getParaValue( UADM_FTP_LOCAL_TMP, localTmpDirectory );
    
    basar::login::Manager::getInstance().getParaValue( UADM_CONTRIBUTIONVOUCHERDIR, directoryname );

    infrastructure::FtpParameters ftpParams;

    ftpParams.hostname = hostname;
    ftpParams.username = user;
    ftpParams.password = pw;
    ftpParams.localTmp = localTmpDirectory;
    ftpParams.saveDirectory = directoryname;
 
    return ftpParams;
}

//----------------------------------------------------------------------------
void NarcoticsApplBase::startResolvePrintingErrors(QWidget * mw)
{
    //instance new resolveprintingerrorsuc
    try
    {
        useCase::ResolvePrintingErrors rpeUC( basar::login::Manager::getInstance().getAreaID() );
        m_ContributionVoucherDM->init( basar::login::Manager::getInstance().getAreaID(), m_Connection.getCurrentConnection() );
        m_NarcoticsDM->init( basar::login::Manager::getInstance().getAreaID(), m_Connection.getCurrentConnection(), basar::login::Manager::getInstance().getUserName() );
        rpeUC.injectNarcoticsDM( m_NarcoticsDM );
        rpeUC.injectContributionVoucherDM( m_ContributionVoucherDM );
        rpeUC.injectSerivceLocator( m_ServiceLocator );

		rpeUC.injectParentWindow( mw );
        
        rpeUC.run();
    }
    catch( basar::Exception& e )
	{
		basar::gui::tie::criticalMsgBox( 0, getExceptionDefaultText( e.what() ).c_str(), QApplication::translate("NarcoticsAppl", "Narcotics").toLocal8Bit().constData() );
	}
	catch( const std::exception & )
	{
		basar::gui::tie::warningMsgBox( 0, "=== std exception caught ====", QApplication::translate("NarcoticsAppl", "Narcotics").toLocal8Bit().constData() );
	}
    catch(...)
	{
		basar::gui::tie::warningMsgBox( 0, "=== unknown exception caught ====", QApplication::translate("NarcoticsAppl", "Narcotics").toLocal8Bit().constData() );
	}

    m_NarcoticsDM->shutdown();
    m_ContributionVoucherDM->shutdown();
}

//----------------------------------------------------------------------------
void NarcoticsApplBase::startContributionVoucherScan(QWidget * mw)
{
    //instance new ContributionVoucherScan
    try
    {
        useCase::ContributionVoucherScan cvsUC( basar::login::Manager::getInstance().getAreaID() );
        m_ContributionVoucherDM->init( basar::login::Manager::getInstance().getAreaID(), m_Connection.getCurrentConnection() );

        cvsUC.injectContributionVoucherDM( m_ContributionVoucherDM );

		cvsUC.injectParentWindow( mw );

        cvsUC.run();
    }
    catch( basar::Exception& e )
	{
		basar::gui::tie::criticalMsgBox( 0, getExceptionDefaultText( e.what() ).c_str(), QApplication::translate("NarcoticsAppl", "Narcotics").toLocal8Bit().constData() );
	}
	catch( const std::exception & )
	{
		basar::gui::tie::warningMsgBox( 0, "=== std exception caught ====", QApplication::translate("NarcoticsAppl", "Narcotics").toLocal8Bit().constData() );
	}
    catch(...)
	{
		basar::gui::tie::warningMsgBox( 0, "=== unknown exception caught ====", QApplication::translate("NarcoticsAppl", "Narcotics").toLocal8Bit().constData() );
	}
    m_ContributionVoucherDM->shutdown();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw  no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////
void NarcoticsApplBase::startPrint()
{
	if ( m_ElectronicContributionVoucher == true  )
	{
		if( areElectronicDeliveryNotePrerequisitesFullfilled() )
		{
			// set parameters for locking
			if( m_LockManager->requestLock( constants::PRINTLOCK,
										    constants::PRINTLOCK_BUSINESSOBJECT_ID,
										    constants::PRINTTHREADS_PER_BRANCH
										   ) )
			{
				m_HasPrintLock = true;

				if( m_PrintThread == 0 ){
					m_PrintThread = new PrintThread();				
				}
				
				if( !m_PrintThread->isRunning() )
				{
                    PrintThreadInitParams p;
                    fillPrintThreadInitParams( p );
					m_PrintThread->init( p );
					m_PrintThread->start();
				}
			}
			else
			{
				QMessageBox::critical( 0, QApplication::translate( "NarcoticsApplBase","Warning" ),QApplication::translate( "NarcoticsApplBase","There is already another print process running for your branch." ),QMessageBox::Ok );

				basar::VarString logMsg;
				logMsg.format( "User < %s > tried to start Print Thread for branchno < %d > although it has been started before.", 
					basar::login::Manager::getInstance().getUserName().c_str(), basar::login::Manager::getInstance().getAreaID() );

				BLOG_INFO( LoggerPool::loggerUseCases, logMsg );
			}
		}
		else
		{
			QMessageBox::critical( 0, QApplication::translate( "NarcoticsApplBase","Warning" ),QApplication::translate( "NarcoticsApplBase","The prerequisites are not fullfilled! Maybe there is no signature for you?" ),QMessageBox::Ok );
		}
	}
	return;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw  no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////
bool NarcoticsApplBase::startLogin(QWidget * mw)
{
	// initialize return value for relogin
	bool retRelogin = false;
	try {
		init();

		if( !( checkUserRight( UADM_RIGHT_GENERAL ) ) ){
			return retRelogin;
		}

		// start loginVC
		m_LoginVC->init();
		m_LoginVC.setParent(mw);
		m_LoginVC->matchCurrentSession( basar::login::Manager::getInstance().getUserID(),
										basar::login::Manager::getInstance().getUserName() );
		m_LoginVC->show();
		m_LoginVC->shutdown();
		// restart application with new login
		if( m_Relogin )
		{
			// set return value to Relogin
			retRelogin = true;
			// reset m_Relogin
			m_Relogin = false;
			// shutdown applicationctrl
			shutdown();
		}
	} 
	catch( basar::Exception& e )
	{
		basar::gui::tie::criticalMsgBox( 0, getExceptionDefaultText( e.what() ).c_str(), QApplication::translate("NarcoticsAppl", "Narcotics").toLocal8Bit().constData() );
	}
	catch( const std::exception & )
	{
		basar::gui::tie::warningMsgBox( 0, "=== std exception caught ====", QApplication::translate("NarcoticsAppl", "Narcotics").toLocal8Bit().constData() );
	}
    catch(...)
	{
		basar::gui::tie::warningMsgBox( 0, "=== unknown exception caught ====", QApplication::translate("NarcoticsAppl", "Narcotics").toLocal8Bit().constData() );
	}
	return retRelogin;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw  no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////
basar::VarString NarcoticsApplBase::getExceptionDefaultText( const basar::VarString& txt )
{
	std::ostringstream os;
    os	<< "Following Exception has been occurred: " 
	    << std::endl 
	    << std::endl 
	    << txt.c_str() 
	    << std::endl 
	    << std::endl
	    << "Please apply to the contact person of the electronic data processing system!"
	    << std::endl;
    
    return os.str().c_str();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw  no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////
bool NarcoticsApplBase::isActive()
{
	// m_Connection is last call of shutdown. Therefore if shutdown has been called, activeConn must have been closed
	if( m_Connection.getCurrentConnection().isOpen() ){
		return true;
	}
	else{
		return false;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw  no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////
void NarcoticsApplBase::stopElectronicContributionVoucherPrint()
{	
	if( m_PrintThread != NULL )
	{
		//wait till thread terminates it's event loop
		m_PrintThread->wait();
		delete m_PrintThread;
		m_PrintThread = NULL;
	}

	if( m_HasPrintLock && m_LockManager != NULL )
	{
		m_LockManager->releaseLock( constants::PRINTLOCK, constants::PRINTLOCK_BUSINESSOBJECT_ID );
		m_HasPrintLock = false;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw  no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////
void NarcoticsApplBase::startCheckPrinterConnection()
{
	try
	{
		init(false);

		//TODO DZ do we need? 
		if (!(checkUserRight(UADM_RIGHT_GENERAL))) 
		{
			return;
		}

		useCase::CheckPrinterConnectionUC uc;
		uc.injectPrinterIP(getPrinterName());
		uc.run();
	}
	catch (basar::Exception& e)
	{
		basar::gui::tie::criticalMsgBox(0, getExceptionDefaultText(e.what()).c_str(), QApplication::translate("NarcoticsAppl", "Narcotics").toLocal8Bit().constData());
	}
	catch (const std::exception&)
	{
		basar::gui::tie::warningMsgBox(0, "=== std exception caught ====", QApplication::translate("NarcoticsAppl", "Narcotics").toLocal8Bit().constData());
	}
	catch (...)
	{
		basar::gui::tie::warningMsgBox(0, "=== unknown exception caught ====", QApplication::translate("NarcoticsAppl", "Narcotics").toLocal8Bit().constData());
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////////////////
void NarcoticsApplBase::shutdown()
{
	// new narcotics process: stop print thread
	// stopping print thread also releases print locking - do before closing m_Connection
	stopElectronicContributionVoucherPrint();

    //shut servicelocation down, also shuts down all aggregated services
    m_ServiceLocator->shutdown();

	// reset m_Initialized
	m_Initialized = false;
	// disconnect
	registerEventHandlers(false);

	//shutdown needs database connection, dammit
	m_OrderDM->shutdown();
    m_LogDM->shutdown();
	
	m_Connection.disconnect();

	QApplication::instance()->removeTranslator( &m_trQt );
	QApplication::instance()->removeTranslator( &m_trGuiQt );
	QApplication::instance()->removeTranslator( &m_trNarcotic );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//! \ret	void
/*!	\throw	no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////
SYSTEM_EVENT_HANDLER_DEFINITION( NarcoticsApplBase, Relogin )
{
	rSource = rSource; //if source not needed otherwise (e.g. for comparing); to avoid warnings
	
	// set relogin state
	m_Relogin = true;
	return basar::appl::EventReturnStruct();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw  no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////
const basar::I18nString & NarcoticsApplBase::getPrinterName() const
{
	return m_PrinterName;
}

} //namespace narcotics