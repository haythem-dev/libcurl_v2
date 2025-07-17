//-----------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  simple ui for scanning returned contribution vouchers
 *  \author Marco Köppendörfer
 *  \date   08.01.2013
 */
//-----------------------------------------------------------------------------------------------//
#ifndef GUARD_NARCOTICS_CONTRIBUTIONVOUCHERSCANVC_H
#define GUARD_NARCOTICS_CONTRIBUTIONVOUCHERSCANVC_H

//-----------------------------------------------------------------------------------------------//
// includes
//-----------------------------------------------------------------------------------------------//
#pragma warning (push)
#pragma warning(disable : 4127 4311 4312 4481 4512 4800 4244)
#include <QtWidgets/qdialog.h>
#include "ui_contributionvoucherscanvc.h"	
#pragma warning (pop)

#include <boost/enable_shared_from_this.hpp>
#include <libbasardbaspect.h>
#include <libbasarappl.h>
#include <libbasarguitie.h>

//-------------------------------------------------------------------------------------------------//
// namespace
//-------------------------------------------------------------------------------------------------//

namespace narcotics
{
namespace viewConn
{

class ContributionVoucherScanVC : public QDialog, public Ui::contributionvoucherscanvcClass,
								  public boost::enable_shared_from_this<ContributionVoucherScanVC>, 
								  public basar::gui::tie::IViewConnector
{
    Q_OBJECT

    public:
        ContributionVoucherScanVC						( QWidget *parent = 0 );          										
        ~ContributionVoucherScanVC						();

        void											closeEvent( QCloseEvent *event );

            /*! \brief	Invokes the init() (without any parameters) implicitly, creates the matcher and 
			transforms the original attribute names to the grid attribute names. \n  no-throw */
	    void											init();
	    basar::gui::tie::WidgetReturnEnum				show();
    	void            								hide();
	    bool											shutdown();

        basar::VarString                                getScannedContributionVoucherNo();
    
    private:
        void initGuiCtrls();
        void connectSignals();

    private slots:
        void onBtnClose_clicked();
        void on_scannedContributionVoucherNo_returnPressed();
};

} //namespace viewConn
} //namespace narcotics

#endif //GUARD_NARCOTICS_CONTRIBUTIONVOUCHERSCANVC_H