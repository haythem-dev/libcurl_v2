//-------------------------------------------------------------------------------------------------//
// includes
//-------------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "contributionvoucherscanvc.h"
#include "noreturnmessagebox.h"
#include "loggerpool.h"

#pragma warning (push)
#pragma warning(disable : 4127 4311 4312 4481 4512 4800 4244)
#include <QtGui/QPalette>
#pragma warning (pop)

//-------------------------------------------------------------------------------------------------//
// namespaces
//-------------------------------------------------------------------------------------------------//
namespace narcotics
{
namespace viewConn
{

    ///////////////////////////////////////////////////////////////////////////////////////////////////
    //!	\throw	no-throw */
    ///////////////////////////////////////////////////////////////////////////////////////////////////
    ContributionVoucherScanVC::ContributionVoucherScanVC( QWidget *parent )
    : QDialog( parent )
    {	
	    setupUi(this);
        init();
    }

	//-------------------------------------------------------------------------------------------------//
    ContributionVoucherScanVC::~ContributionVoucherScanVC()
    {
    }

	//-------------------------------------------------------------------------------------------------//
    void ContributionVoucherScanVC::hide()
    {
    }

	//-------------------------------------------------------------------------------------------------//
    void ContributionVoucherScanVC::init()
    {
        initGuiCtrls();
        connectSignals();
    }

	//-------------------------------------------------------------------------------------------------//
    void ContributionVoucherScanVC::initGuiCtrls()
    {
	    if( 0 == scannedContributionVoucherNo->validator() ){
		    scannedContributionVoucherNo->setValidator( new QRegExpValidator( QRegExp("\\d{,16}"), this ) );
	    }

        scannedContributionVoucherNo->setText( "" );
	    scannedContributionVoucherNo->setFocus();
        
        //prevent button from closing form on enter keypress
        btnClose->setAutoDefault( false );
        btnClose->setDefault( false );

        lblVouchersScanned->setText( tr("Scanned Vouchers: 0") );
    }

	//-------------------------------------------------------------------------------------------------//
    void ContributionVoucherScanVC::connectSignals()
    {
		QObject::connect(	btnClose,						SIGNAL( clicked()				),	
							this,							SLOT  (	onBtnClose_clicked()	));

//		Todo FA: Mit Autoconnect funktioniert es. onScannedContributionVoucherNo_returnPressed() wird nur einmal aufgerufen.
//		Mit der connect-Funktion, wird die Funktion 2 mal aufgerufen (egal wie man sie benennt)
//		QObject::connect(	scannedContributionVoucherNo,	SIGNAL( returnPressed()				),	
//							this,							SLOT  (	onScannedContributionVoucherNo_returnPressed()	));
    }

	//-------------------------------------------------------------------------------------------------//
    basar::VarString ContributionVoucherScanVC::getScannedContributionVoucherNo()
    {
        QString dateAndContrVouNo = scannedContributionVoucherNo->text();
        
        basar::VarString ret( dateAndContrVouNo.toLocal8Bit().constData() );

        return ret;
    }

	//-------------------------------------------------------------------------------------------------//
    void ContributionVoucherScanVC::closeEvent( QCloseEvent* event )
    {
        shutdown();
        event->accept();
    }

	//-------------------------------------------------------------------------------------------------//
    bool ContributionVoucherScanVC::shutdown()
    {
        this->close();
        return true;
    }

	//-------------------------------------------------------------------------------------------------//
    basar::gui::tie::WidgetReturnEnum ContributionVoucherScanVC::show()
    {
	    return basar::gui::tie::getWidgetReturnType( exec() );
    }

	//-------------------------------------------------------------------------------------------------//
    void ContributionVoucherScanVC::onBtnClose_clicked()
    {
        close();
    }

	//-------------------------------------------------------------------------------------------------//
    void ContributionVoucherScanVC::on_scannedContributionVoucherNo_returnPressed()
    {
	    scannedContributionVoucherNo->setEnabled( false );

        setCursor( Qt::WaitCursor );	

		bool oldState = scannedContributionVoucherNo->blockSignals( true );
        
	    if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	    {
		    basar::ConstString msg = "ContributionVoucherScanVC::on_scannedContributionVoucherNo_returnPressed()";
		    LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	    }            
        
        QString dateAndContrVouNo = scannedContributionVoucherNo->text();

	    QRegExp combinedDateAndContrVouNo("\\d{16}");
        
        basar::appl::EventReturnStruct ret;

	    if( combinedDateAndContrVouNo.exactMatch( dateAndContrVouNo ) ){
			ret = basar::appl::SystemEventManager::getInstance().fire( "MarkScannedContributionVoucherAsReceived" );
	    }
	    else{
			ret.ret = basar::appl::HANDLER_ERROR;
			ret.message = tr("Entered value has wrong format (16 numbers)!!").toLocal8Bit().constData();
		}

	    setCursor( Qt::ArrowCursor );

		if( ret.ret == basar::appl::HANDLER_INFO ){
			// workaround, so that message box doesn´t disappear with next scanning
			QNoReturnMessageBox msgBox(this);
			msgBox.setIcon(QMessageBox::Information);
			msgBox.setText(QString::fromLocal8Bit(ret.message.c_str()));
			msgBox.exec();
        }
        
		if( ret.ret == basar::appl::HANDLER_ERROR ){
			// workaround, so that message box doesn´t disappear with next scanning
			QNoReturnMessageBox msgBox(this);
			msgBox.setIcon(QMessageBox::Critical);
			msgBox.setText(QString::fromLocal8Bit(ret.message.c_str()));
			msgBox.exec();
		}

        if( ret.ret == basar::appl::HANDLER_OK )
        {
            listWidget->insertItem( 0, dateAndContrVouNo.right( 8 ) );
            QString lblText = tr("Scanned Vouchers: ");
            lblText.append( QString::number( listWidget->count() ) );
            lblVouchersScanned->setText( lblText );
			scannedContributionVoucherNo->clear();
			//! \todo FN logging "Empfangsbestätigung <xyz> gescannt" into narcotics.info.log
        }

        scannedContributionVoucherNo->blockSignals( oldState );
        
        scannedContributionVoucherNo->setEnabled( true );
		scannedContributionVoucherNo->selectAll();
        scannedContributionVoucherNo->setFocus();
    }

} //namespace viewConn
} //namespace narcotics