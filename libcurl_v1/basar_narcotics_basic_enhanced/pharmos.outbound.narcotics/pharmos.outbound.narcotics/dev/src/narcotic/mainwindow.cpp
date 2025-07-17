//-------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  narcotics main window
 *  \author	Thomas Hörath
 *  \date   28.06.2006
 */
//-----------------------------------------------------------------------------------------------//
// includes
//-----------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "mainwindow.h"
#include "narcoticsappl.h"
#include "definitions.h"
#include "version.h"
#include "printthread.h"

//-------------------------------------------------------------------------------------------------//
// 
//-------------------------------------------------------------------------------------------------//
mainwindow::mainwindow( QWidget *parent ) 
: QMainWindow( parent ), m_Relogin( false )
{
	ui.setupUi(this);

	QAction * pExitAct = new QAction(tr("E&xit"), this);
	pExitAct->setShortcut(tr("Ctrl+Q"));
    pExitAct->setStatusTip(tr("Leave the application"));
    this->connect(pExitAct, SIGNAL(triggered()), this, SLOT(close()));

	m_pReleaseTransferLocksForBranch = new QAction(tr("Unlock articles"), this);
	m_pReleaseTransferLocksForBranch->setStatusTip(tr("Releases all article locks for this branch"));
	this->connect(m_pReleaseTransferLocksForBranch, SIGNAL(triggered()), this, SLOT(releaseTransferLocks()));

	m_pReleaseAllPrintLocksForBranch = new QAction(tr("Unlock printing"), this);
	m_pReleaseAllPrintLocksForBranch->setStatusTip(tr("Releases all printing locks for this branch"));
	m_pReleaseAllPrintLocksForBranch->setEnabled(false);
	this->connect(m_pReleaseAllPrintLocksForBranch, SIGNAL(triggered()), this, SLOT(releaseAllPrintLocks()));

    m_pSolvePrintError = new QAction(tr("Solve printing error - NA19"), this);
    m_pSolvePrintError->setStatusTip(tr("Solve printing error"));
    m_pSolvePrintError->setEnabled(false);
    this->connect(m_pSolvePrintError, SIGNAL(triggered()), this, SLOT(resolvePrintingErrors()));

	QAction* pCheckPrinter = new QAction(tr("Check printer connection"), this);
	pCheckPrinter->setStatusTip(tr("Check connection with current printer"));
	this->connect(pCheckPrinter, SIGNAL(triggered()), this, SLOT(checkPrinterConnection()));

    QMenu * pFileMenu = this->menuBar()->addMenu(tr("&File"));
	pFileMenu->addAction(m_pReleaseTransferLocksForBranch);
    pFileMenu->addSeparator();
    pFileMenu->addAction(m_pReleaseAllPrintLocksForBranch);
    pFileMenu->addAction(m_pSolvePrintError);
    pFileMenu->addSeparator();
	pFileMenu->addAction(pCheckPrinter);
	pFileMenu->addSeparator();
    pFileMenu->addAction(pExitAct);
    
// new menu
	QMenu *pNarcoticsMenu;
	QMenu *pReportsMenu;

	QAction* pIncomingGoodsAct = new QAction(tr("Incoming &Delivery - NA01"), this);
	pIncomingGoodsAct->setShortcut(tr("Ctrl+I"));
    pIncomingGoodsAct->setStatusTip(tr("Show Incoming Delivery"));
    this->connect(pIncomingGoodsAct, SIGNAL(triggered()), this, SLOT(incomingGoods())); 

	QAction* pCatalogAct = new QAction(tr("&Narcotic Book - NA03"), this);
	pCatalogAct->setShortcut(tr("Ctrl+B"));
    pCatalogAct->setStatusTip(tr("Show Narcotic Book"));
    this->connect(pCatalogAct, SIGNAL(triggered()), this, SLOT(catalog())); 

	QAction* pAnalysisAct = new QAction(tr("&Investigation - NA08"), this);
	pAnalysisAct->setShortcut(tr("Ctrl+R"));
    pAnalysisAct->setStatusTip(tr("Show Investigation"));
    this->connect(pAnalysisAct, SIGNAL(triggered()), this, SLOT(analysis()));

	QAction* pHalfYearReportAct = new QAction(tr("&Half-yearly Notification"), this);
	pHalfYearReportAct->setShortcut(tr("Ctrl+H"));
    pHalfYearReportAct->setStatusTip(tr("Show half-yearly Notification"));
    this->connect(pHalfYearReportAct, SIGNAL(triggered()), this, SLOT(halfYearReport()));

	QAction* pCountListAct = new QAction(tr("&Counting List"), this);
	pCountListAct->setShortcut(tr("Ctrl+Z"));
    pCountListAct->setStatusTip(tr("Show Counting List"));
    this->connect(pCountListAct, SIGNAL(triggered()), this, SLOT(countList()));

	QAction* pStockListAct = new QAction(tr("&Stock List"), this);
	//pStockListAct->setShortcut(tr("Ctrl+K"));
    pStockListAct->setStatusTip(tr("Show Stock List"));
    this->connect(pStockListAct, SIGNAL(triggered()), this, SLOT(stockList()));
	
    QAction* pStockListQuickAct = new QAction(tr("Stock List (Fast)"), this);
    pStockListQuickAct->setShortcut(tr("Ctrl+K"));
    pStockListQuickAct->setStatusTip(tr("Show Stock List (Fast version)"));
    this->connect(pStockListQuickAct, SIGNAL(triggered()), this, SLOT(stockListQuick()));

	QAction* pDestroyPaperAct = new QAction(tr("&Destruction Voucher"), this);
	pDestroyPaperAct->setShortcut(tr("Ctrl+V"));
    pDestroyPaperAct->setStatusTip(tr("Show Destruction Voucher"));
    this->connect(pDestroyPaperAct, SIGNAL(triggered()), this, SLOT(destroyPaper()));

	QAction* pWssComparison = new QAction(tr("&Pharmos-Comparison - NA05"), this);
	pWssComparison->setShortcut(tr("Ctrl+C"));
    pWssComparison->setStatusTip(tr("Show Pharmos-Comparison"));
    this->connect(pWssComparison, SIGNAL(triggered()), this, SLOT(wssComparison()));

	QAction* pNarcGroup = new QAction(tr("Narcotic-&Group - NA06"), this);
	pNarcGroup->setShortcut(tr("Ctrl+G"));
    pNarcGroup->setStatusTip(tr("Show Narcotic Groups"));
    this->connect(pNarcGroup, SIGNAL(triggered()), this, SLOT(narcGroup()));

	QAction* pNarcProtocol = new QAction(tr("&Protocol - NA07"), this);
	pNarcProtocol->setShortcut(tr("Ctrl+P"));
    pNarcProtocol->setStatusTip(tr("Show Protocol"));
    this->connect(pNarcProtocol, SIGNAL(triggered()), this, SLOT(protocol()));

	QAction* pDiffQtyInStock = new QAction(tr("Di&fference stock - NA09"), this);
	pDiffQtyInStock->setShortcut(tr("Ctrl+F"));
    pDiffQtyInStock->setStatusTip(tr("Show difference stock between narcotics and PHARMOS"));
    this->connect(pDiffQtyInStock, SIGNAL(triggered()), this, SLOT(diffQtyInStock()));

	QAction* pOrderOverview = new QAction(tr("Order &Overview - NA12"), this);
	pOrderOverview->setShortcut(tr("Ctrl+O"));
    pOrderOverview->setStatusTip(tr("Show the aricle for an order"));
    this->connect(pOrderOverview, SIGNAL(triggered()), this, SLOT(orderOverview())); 

	QAction* pContributionVoucher = new QAction(tr("Contribution &Voucher - NA17"), this);
	pContributionVoucher->setShortcut(tr("Ctrl+A"));
    pContributionVoucher->setStatusTip(tr("Show missing acknowledgements and create delivery note copies"));
    this->connect(pContributionVoucher, SIGNAL(triggered()), this, SLOT(contributionVoucher()));

    QAction* pContributionVoucherScan = new QAction(tr("Contribution Voucher Scan - NA21"), this);
    this->connect(pContributionVoucherScan, SIGNAL(triggered()), this, SLOT(contributionVoucherScan())); 

	QAction* pCustMapAct = new QAction(tr("&Customer Mapping - NA22"), this);
	pCustMapAct->setShortcut(tr("Ctrl+M"));
    pCustMapAct->setStatusTip(tr("Show mapping of customers"));
    this->connect(pCustMapAct, SIGNAL(triggered()), this, SLOT(customerMapping()));

	QAction* pLogin = new QAction(tr("Login - NA16"), this);
	pLogin->setShortcut(tr("Ctrl+L"));
    pLogin->setStatusTip(tr("Show Login information"));
    this->connect(pLogin, SIGNAL(triggered()), this, SLOT(login()));

	pNarcoticsMenu = this->menuBar()->addMenu(tr("&Narcotics"));
	pNarcoticsMenu->addAction(pIncomingGoodsAct);
	pNarcoticsMenu->addAction(pCatalogAct);
	pNarcoticsMenu->addAction(pAnalysisAct);
	
	pReportsMenu = pNarcoticsMenu->addMenu(tr("Repor&ts"));
	pReportsMenu->addAction(pHalfYearReportAct);
	// REFACTORING remove code behind these buttons: pReportsMenu->addAction(pDestroyPaperAct); --> Vernichtungsbeleg nur noch über NA12 im Rahmen eines Auftrags
	pReportsMenu->addAction(pCountListAct);
	// REFACTORING remove code behind these buttons: pReportsMenu->addAction(pStockListAct);
    pReportsMenu->addAction(pStockListQuickAct);
	
	if( !narcotics::NarcoticsAppl::getInstance().isNewNarcoticsProcess() ){
		pContributionVoucher->setDisabled(true);
	}

	if( !basar::login::Manager::getInstance().isLegitimated(UADM_MODULE_MAIN, UADM_RIGHT_GENERAL) )
	{
		pReportsMenu->setDisabled(true);
		pWssComparison->setDisabled(true);
		pNarcGroup->setDisabled(true);
		pDiffQtyInStock->setDisabled(true);
		pAnalysisAct->setDisabled(true);
		pCustMapAct->setDisabled(true);
		pCatalogAct->setDisabled(true);
		pOrderOverview->setDisabled(true);
		pContributionVoucher->setDisabled(true);
		pLogin->setDisabled(true);
		m_pReleaseTransferLocksForBranch->setDisabled(true);
	}

	if( !basar::login::Manager::getInstance().isLegitimated(UADM_MODULE_MAIN, UADM_RIGHT_PROTOCOL) ){
		pNarcProtocol->setDisabled(true);
	}

	if( !basar::login::Manager::getInstance().isLegitimated(UADM_MODULE_MAIN, UADM_RIGHT_INCOME) ){
		pIncomingGoodsAct->setDisabled(true);
	}

	pNarcoticsMenu->addAction(pWssComparison);
	pNarcoticsMenu->addAction(pNarcGroup);
	pNarcoticsMenu->addAction(pNarcProtocol);
	pNarcoticsMenu->addAction(pDiffQtyInStock);
	pNarcoticsMenu->addAction(pOrderOverview); // new menu orderOverview added
	pNarcoticsMenu->addAction(pContributionVoucher); // new menu contributionVoucher added
    pNarcoticsMenu->addAction(pContributionVoucherScan);
	pNarcoticsMenu->addAction(pCustMapAct);
	pNarcoticsMenu->addAction(pLogin); // new menu login added

// --- About ---
	QAction * pAboutAct = new QAction(tr("?"), this);
    pAboutAct->setShortcut(tr("Ctrl+H"));
    pAboutAct->setStatusTip(tr("Show the application's About box"));
    this->connect(pAboutAct, SIGNAL(triggered()), this, SLOT(about()));

	QMenu * pAboutMenu = this->menuBar()->addMenu(tr("&Help"));
    pAboutMenu->addSeparator();
    pAboutMenu->addAction(pAboutAct);
	
// statusline
	m_Statusline.init(this);
	basar::VarString value(basar::login::Manager::getInstance().getUserName());
	m_Statusline.setUser(value);
	
	value = basar::gui::qt::Manager::getInstance().getInit().host;
	value += " - ";
	value += basar::gui::qt::Manager::getInstance().getInit().database;
	m_Statusline.setConnection(value);

// background
	m_Background.init(tr("Narcotics").toLocal8Bit().data(), VERSION_NUMBER, basar::gui::qt::MainBackground::OUTBOUND, basar::gui::qt::MainBackground::PHOENIXGROUP );
	QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(&m_Background);

//create printprocess buttons
	QGroupBox* grpElectronicContributionVoucher = new QGroupBox(tr("Electronic Contribution Voucher"));

	QHBoxLayout* horizontalLayout = new QHBoxLayout();
	
	QVBoxLayout* verticalLayout = new QVBoxLayout();

	horizontalLayout->addLayout(verticalLayout);

	QVBoxLayout* verticalLayout1 = new QVBoxLayout();

	m_heartBeatVisualizer = new QProgressBar();

	m_heartBeatVisualizer->setRange(0,4);
	m_heartBeatVisualizer->setValue(0);
	m_heartBeatVisualizer->setTextVisible(false);

	verticalLayout1->addWidget(m_heartBeatVisualizer);

	QGroupBox *grpPrintProcessRunning = new QGroupBox(tr("Printthread running"));
	grpPrintProcessRunning->setLayout(verticalLayout1);

	horizontalLayout->addWidget(grpPrintProcessRunning);

	m_btnStartPrintThread = new QPushButton(tr("Start Printing"));
	m_btnStopPrintThread = new QPushButton(tr("Stop Printing"));

	m_btnStartPrintThread->setEnabled(false);
	m_btnStopPrintThread->setEnabled(false);

	this->connect(m_btnStartPrintThread,SIGNAL(clicked()), this, SLOT(onStartPrintThread()));
	this->connect(m_btnStopPrintThread,SIGNAL(clicked()), this, SLOT(onStopPrintThread()));

	verticalLayout->addWidget(m_btnStartPrintThread);
	verticalLayout->addWidget(m_btnStopPrintThread);

	grpElectronicContributionVoucher->setLayout(horizontalLayout);

	if( !narcotics::NarcoticsAppl::getInstance().isNewNarcoticsProcess() ){
		grpElectronicContributionVoucher->setDisabled(true);
	}

	mainLayout->addWidget(grpElectronicContributionVoucher);

	ui.centralWidget->setLayout(mainLayout);	
}

//-----------------------------------------------------------------------------------------------//
void mainwindow::resetContributionVoucherPrintProcess()
{
	tryEnablePrintButton();

	m_btnStopPrintThread->setEnabled(false);

	resetHeartBeatVisualizer();
}

//-----------------------------------------------------------------------------------------------//
bool mainwindow::areElectronicDeliveryNotePrerequisitesFullfilled()
{
	return narcotics::NarcoticsAppl::getInstance().areElectronicDeliveryNotePrerequisitesFullfilled();
}

//-------------------------------------------------------------------------------------------------//
mainwindow::~mainwindow()
{
	m_Statusline.clear();
}

//-------------------------------------------------------------------------------------------------//
bool mainwindow::isToRelogin()
{
	return m_Relogin;
}

//-------------------------------------------------------------------------------------------------//
void mainwindow::resetRelogin()
{
	m_Relogin = false;
}

//-------------------------------------------------------------------------------------------------//
void mainwindow::setStatusline()
{
	basar::VarString value(basar::login::Manager::getInstance().getUserName());
	m_Statusline.setUser(value);
	
	value = basar::gui::qt::Manager::getInstance().getInit().host;
	value += " - ";
	value += basar::gui::qt::Manager::getInstance().getInit().database;
	m_Statusline.setConnection(value);
}

//-------------------------------------------------------------------------------------------------//
void mainwindow::about()
{
	basar::gui::qt::About dlg;
	setCursor( Qt::WaitCursor );
	dlg.show(this);
	setCursor( Qt::ArrowCursor ); 
}

//-------------------------------------------------------------------------------------------------//
void mainwindow::incomingGoods()
{
	setCursor( Qt::WaitCursor );
	narcotics::NarcoticsAppl::getInstance().startIncomingGoods(this);
	setCursor( Qt::ArrowCursor ); 
}

//-------------------------------------------------------------------------------------------------//
void mainwindow::catalog()
{
	setCursor( Qt::WaitCursor );
	narcotics::NarcoticsAppl::getInstance().startNarcoticsBook(this);
	setCursor( Qt::ArrowCursor ); 
}

//-------------------------------------------------------------------------------------------------//
void mainwindow::analysis()
{
	setCursor( Qt::WaitCursor );
	narcotics::NarcoticsAppl::getInstance().startTransactionData(this);
	setCursor( Qt::ArrowCursor );
}

//-------------------------------------------------------------------------------------------------//
void mainwindow::customerMapping()
{
	setCursor( Qt::WaitCursor );
	narcotics::NarcoticsAppl::getInstance().startCustomerMapping(this);
	setCursor( Qt::ArrowCursor );
}

//-------------------------------------------------------------------------------------------------//
void mainwindow::halfYearReport()
{
	setCursor( Qt::WaitCursor );
	narcotics::NarcoticsAppl::getInstance().startHalfYearlyNotification(this);
	setCursor( Qt::ArrowCursor );
}

//-------------------------------------------------------------------------------------------------//
void mainwindow::destroyPaper()
{
	setCursor( Qt::WaitCursor );
	narcotics::NarcoticsAppl::getInstance().startDissolveReceipt(this);
	setCursor( Qt::ArrowCursor );
}

//-------------------------------------------------------------------------------------------------//
void mainwindow::countList()
{
	setCursor( Qt::WaitCursor );
	narcotics::NarcoticsAppl::getInstance().startCountList(this);
	setCursor( Qt::ArrowCursor );
}

//-------------------------------------------------------------------------------------------------//
void mainwindow::stockList()
{
	setCursor( Qt::WaitCursor );
	narcotics::NarcoticsAppl::getInstance().startStockList(this);
	setCursor( Qt::ArrowCursor );
}

//-------------------------------------------------------------------------------------------------//
void mainwindow::stockListQuick()
{
	setCursor( Qt::WaitCursor );
	narcotics::NarcoticsAppl::getInstance().startStockListQuick(this);
	setCursor( Qt::ArrowCursor );
}

//-------------------------------------------------------------------------------------------------//
void mainwindow::wssComparison()
{
	setCursor( Qt::WaitCursor );
	narcotics::NarcoticsAppl::getInstance().startCompareNarcoticsOriginal(this);
	setCursor( Qt::ArrowCursor );
}

//-------------------------------------------------------------------------------------------------//
void mainwindow::narcGroup()
{
	setCursor( Qt::WaitCursor );
	narcotics::NarcoticsAppl::getInstance().startNarcoticsCategory(this);
	setCursor( Qt::ArrowCursor );
}

//-------------------------------------------------------------------------------------------------//
void mainwindow::protocol()
{
	setCursor( Qt::WaitCursor );
	narcotics::NarcoticsAppl::getInstance().startNarcoticsProtocol(this);
	setCursor( Qt::ArrowCursor );
}

//-------------------------------------------------------------------------------------------------//
void mainwindow::diffQtyInStock()
{
	setCursor( Qt::WaitCursor );
	narcotics::NarcoticsAppl::getInstance().startDiffQtyInStock(this);
	setCursor( Qt::ArrowCursor );
}

//-------------------------------------------------------------------------------------------------//
void mainwindow::orderOverview()
{
	setCursor( Qt::WaitCursor );
	narcotics::NarcoticsAppl::getInstance().startOrderOverview(this);
	setCursor( Qt::ArrowCursor ); 
}

//-------------------------------------------------------------------------------------------------//
void mainwindow::contributionVoucher()
{
	setCursor( Qt::WaitCursor );
	narcotics::NarcoticsAppl::getInstance().startContributionVoucher(this);
	setCursor( Qt::ArrowCursor ); 
}

//-------------------------------------------------------------------------------------------------//
void mainwindow::contributionVoucherScan()
{
	setCursor( Qt::WaitCursor );
	narcotics::NarcoticsAppl::getInstance().startContributionVoucherScan(this);
	setCursor( Qt::ArrowCursor ); 
}

//-------------------------------------------------------------------------------------------------//
void mainwindow::login()
{
	setCursor( Qt::WaitCursor );
	m_Relogin = narcotics::NarcoticsAppl::getInstance().startLogin(this);
	setCursor( Qt::ArrowCursor ); 
	
	if( m_Relogin == true ){		
		this->close();
	}
}

//-----------------------------------------------------------------------------------------------//
/*!	\throw  no-throw 
	\brief static function to initalize listbox of components in about-dialog */
//-----------------------------------------------------------------------------------------------//
bool about_define()
{
	basar::DefinitionStruct def;
	def.copyright = "copyright by PHOENIX GmbH & Co. KG";
	def.programDescription = "Narcotics";
	def.version = VERSION_NUMBER;
	basar::gui::qt::Manager::getInstance().init(def);

	basar::DllStruct dll;
	
	dll.name = "libbasarcmnutil.dll";
	dll.pkgVersion = basar::cmnutil::getVersion();
	basar::gui::qt::Manager::getInstance().initDll(dll);

	dll.name = "libbasardbsql.dll";
	dll.pkgVersion = basar::db::sql::getVersion();
	basar::gui::qt::Manager::getInstance().initDll(dll);

	dll.name = "libbasarproperty.dll";
	dll.pkgVersion = basar::property::getVersion();
	basar::gui::qt::Manager::getInstance().initDll(dll);

	dll.name = "libbasardbaspect.dll";
	dll.pkgVersion = basar::db::aspect::getVersion();
	basar::gui::qt::Manager::getInstance().initDll(dll);

	dll.name = "libbasarappl.dll";
	dll.pkgVersion = basar::appl::getVersion();
	basar::gui::qt::Manager::getInstance().initDll(dll);

	dll.name = "libbasarlogin.dll";
	dll.pkgVersion = basar::login::getVersion();
	basar::gui::qt::Manager::getInstance().initDll(dll);
	
	dll.name = "libbasarguitie.dll";
	dll.pkgVersion = basar::gui::tie::getVersion();
	basar::gui::qt::Manager::getInstance().initDll(dll);

	dll.name = "libbasarqtwidget.dll";
	dll.pkgVersion = basar::gui::qt::getVersion();
	basar::gui::qt::Manager::getInstance().initDll(dll);
	
	dll.name = "libbasarguiqt.dll";
	dll.pkgVersion = basar::gui::qt::getVersion();
	basar::gui::qt::Manager::getInstance().initDll(dll);

	return true;
}

//-----------------------------------------------------------------------------------------------//
bool dummy = about_define();  //!< explicit call for about-dlg´s definition

//-----------------------------------------------------------------------------------------------//
bool mainwindow::tryEnablePrintButton()
{
	if( areElectronicDeliveryNotePrerequisitesFullfilled() && narcotics::NarcoticsAppl::getInstance().isNewNarcoticsProcess() )
	{
		m_pReleaseAllPrintLocksForBranch->setEnabled(true);
        m_pSolvePrintError->setEnabled(true);
		m_btnStartPrintThread->setEnabled(true);
	
		return true;
	}
	else
	{
		m_pReleaseAllPrintLocksForBranch->setEnabled(false);
        m_pSolvePrintError->setEnabled(false);
		m_btnStartPrintThread->setEnabled(false);

		return false;
	}
}

//-----------------------------------------------------------------------------------------------//
void mainwindow::onStartPrintThread()
{
	startPrintProcess();

	if( narcotics::NarcoticsAppl::getInstance().hasLockForPrinting() )
	{
		m_btnStartPrintThread->setEnabled(false);
		m_btnStopPrintThread->setEnabled(true);
	}
}

//-----------------------------------------------------------------------------------------------//
void mainwindow::onStopPrintThread()
{
	setCursor( Qt::WaitCursor );
	
    emit requestPrintThreadTermination();

	m_btnStopPrintThread->setEnabled(false);

	stopPrintProcess();
	tryEnablePrintButton();
	resetHeartBeatVisualizer();

	setCursor( Qt::ArrowCursor );
}

//-----------------------------------------------------------------------------------------------//
void mainwindow::onPrintThreadHeartBeat()
{
	int currentValue = m_heartBeatVisualizer->value();
    
    currentValue %= 4;

	m_heartBeatVisualizer->setValue( (currentValue + 1 ) % 5 );
}

//-----------------------------------------------------------------------------------------------//
void mainwindow::onUserInteractionRequired()
{
    m_heartBeatVisualizer->setStyleSheet("QProgressBar::chunk {background: qlineargradient(x1: 0, y1: 0.5, x2: 1, y2: 0.5, stop: 0 red, stop: 1 orange); }");

	m_heartBeatVisualizer->update();
	m_heartBeatVisualizer->show();
}

//-----------------------------------------------------------------------------------------------//
void mainwindow::onErrorsResolved()
{
    m_heartBeatVisualizer->setStyleSheet("QProgressBar::chunk {background: qlineargradient(x1: 0, y1: 0.5, x2: 1, y2: 0.5, stop: 0 blue, stop: 1 darkblue); }");

	m_heartBeatVisualizer->update();
	m_heartBeatVisualizer->show();
}

//-----------------------------------------------------------------------------------------------//
void mainwindow::onPrintThreadStopped()
{
	tryEnablePrintButton();
	resetHeartBeatVisualizer();
	m_btnStopPrintThread->setEnabled(false);
}

//-----------------------------------------------------------------------------------------------//
void mainwindow::releaseAllPrintLocks()
{
	narcotics::NarcoticsAppl::getInstance().releaseAllPrintLocks();
	tryEnablePrintButton();
	m_btnStopPrintThread->setEnabled(false);
}

//-----------------------------------------------------------------------------------------------//
void mainwindow::releaseTransferLocks()
{
	narcotics::NarcoticsAppl::getInstance().releaseTransferLocks();
}

//-----------------------------------------------------------------------------------------------//
void mainwindow::resolvePrintingErrors()
{
	setCursor( Qt::WaitCursor );
	narcotics::NarcoticsAppl::getInstance().startResolvePrintingErrors(this);
	setCursor( Qt::ArrowCursor ); 
}

//-----------------------------------------------------------------------------------------------//
void mainwindow::connectToPrintThread()
{
	const narcotics::PrintThread *printThread = narcotics::NarcoticsAppl::getInstance().getPrintThread();

	//connect to heartbeat 
	this->connect(printThread,SIGNAL(heartBeat()),this,SLOT(onPrintThreadHeartBeat()));

	//connect to terminated for enabling the start print process button again
	this->connect(printThread,SIGNAL(terminated()),this,SLOT(onPrintThreadStopped()));
	this->connect(printThread,SIGNAL(finished()),this,SLOT(onPrintThreadStopped()));

	//connect to userInteractionRequired, sets printthread heartbeatvisualizer red
	this->connect(printThread,SIGNAL(userInteractionRequired()),this,SLOT(onUserInteractionRequired()));

	//connect to errorsResolved, sets printthread heartbeatvisualizer blue
	this->connect(printThread,SIGNAL(errorsResolved()),this,SLOT(onErrorsResolved()));

	this->connect(this,SIGNAL(requestPrintThreadTermination()),printThread,SLOT(on_termination_requested()));
	this->connect(printThread,SIGNAL(terminatePrintThread()),this,SLOT(onStopPrintThread()));
}

//-----------------------------------------------------------------------------------------------//
void mainwindow::disconnectFromPrintThread()
{
	const narcotics::PrintThread *printThread = narcotics::NarcoticsAppl::getInstance().getPrintThread();

	if (printThread != NULL)
	{
		this->disconnect(printThread,SIGNAL(heartBeat()),this,SLOT(onPrintThreadHeartBeat()));
		this->disconnect(printThread,SIGNAL(userInteractionRequired()),this,SLOT(onUserInteractionRequired()));
		this->disconnect(printThread,SIGNAL(errorsResolved()),this,SLOT(onErrorsResolved()));
	}
}

//-----------------------------------------------------------------------------------------------//
void mainwindow::startPrintProcess()
{
	m_heartBeatVisualizer->setStyleSheet("QProgressBar::chunk {background: qlineargradient(x1: 0, y1: 0.5, x2: 1, y2: 0.5, stop: 0 blue, stop: 1 darkblue); }");

	m_heartBeatVisualizer->update();
	m_heartBeatVisualizer->show();

	narcotics::NarcoticsAppl::getInstance().startPrint();

	if( narcotics::NarcoticsAppl::getInstance().hasLockForPrinting() ){
		connectToPrintThread();
	}
}

//-----------------------------------------------------------------------------------------------//
void mainwindow::resetHeartBeatVisualizer()
{
	m_heartBeatVisualizer->setValue(0);
}

//-----------------------------------------------------------------------------------------------//
void mainwindow::stopPrintProcess()
{
	disconnectFromPrintThread();
	narcotics::NarcoticsAppl::getInstance().stopElectronicContributionVoucherPrint();
}

//-----------------------------------------------------------------------------------------------//
void mainwindow::closeEvent(QCloseEvent* event)
{
    emit requestPrintThreadTermination();
    stopPrintProcess();
    event->accept();
}

//-------------------------------------------------------------------------------------------------//
void mainwindow::checkPrinterConnection()
{
	setCursor(Qt::WaitCursor);
	narcotics::NarcoticsAppl::getInstance().startCheckPrinterConnection();
	setCursor(Qt::ArrowCursor);
}