//-------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  narcotics main window
 *  \author	Thomas Hörath
 *  \date   28.06.2006
 */
//-------------------------------------------------------------------------------------------------//
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//-----------------------------------------------------------------------------------------------//
// includes
//-----------------------------------------------------------------------------------------------//
#include <libbasarcmnutil.h> // muss vor  ...guiqt includiert werden!
#include <libbasarguiqt.h>

#pragma warning (push)
#pragma warning(disable: 4127 4244 4311 4312 4481 4512 4800)
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QProgressBar>
#include "ui_mainwindow.h"
#pragma warning (pop)

//-------------------------------------------------------------------------------------------------//
class mainwindow : public QMainWindow
{
    Q_OBJECT

public:
    mainwindow(QWidget *parent = 0);
    ~mainwindow();
	// delegate logout reaction to main
	bool isToRelogin();
	// let member be resetted by main
	void resetRelogin();
	// set Statusline for current user
	void setStatusline();
	
	void resetContributionVoucherPrintProcess();

    void closeEvent ( QCloseEvent * event );

signals:
    void requestPrintThreadTermination();

private slots:
	//slot which is called when printhtread sends heartbeat. increments progressbar
	void onPrintThreadHeartBeat();

    //slot which is called by printthread when user interaction for resolving printing errors is require
    void onUserInteractionRequired();

    void onErrorsResolved();

	//slot which is called when thread is terminated or finished
	void onPrintThreadStopped();

	void onStopPrintThread();
	void onStartPrintThread();
    void about();
	void incomingGoods();
	void catalog();
	void analysis();
	void customerMapping();
	void halfYearReport();
	void destroyPaper();
	void countList();
	void stockList();
    void stockListQuick();
	void wssComparison();
	void narcGroup();
	void protocol();
	void diffQtyInStock();
	void orderOverview();
	void contributionVoucher();
    void contributionVoucherScan();
	void login();
	void startPrintProcess();
	void stopPrintProcess();
	void releaseAllPrintLocks();
	void releaseTransferLocks();
    void resolvePrintingErrors();
	void checkPrinterConnection();

private:
	bool tryEnablePrintButton();

	//sets the value of the progressbar to 0
	void resetHeartBeatVisualizer();
	//registers with printthread signal
	void connectToPrintThread();
	//deregisters printthread signal
	void disconnectFromPrintThread();
	//checks whether signatures are in postscriptfile abgabemeldung.ps by searching for username.bmp entry in ps
	bool areElectronicDeliveryNotePrerequisitesFullfilled();
    Ui::mainwindowClass ui;

	basar::gui::qt::StatusLine							m_Statusline;
	basar::gui::qt::MainBackground						m_Background;
	bool												m_Relogin;				//!< declares value for relogin

	QPushButton *m_btnStartPrintThread;
	QPushButton *m_btnStopPrintThread;
	QAction *m_pReleaseAllPrintLocksForBranch;
	QAction *m_pReleaseTransferLocksForBranch;

    QAction *m_pSolvePrintError;
	QProgressBar *m_heartBeatVisualizer;
};

#endif // MAINWINDOW_H