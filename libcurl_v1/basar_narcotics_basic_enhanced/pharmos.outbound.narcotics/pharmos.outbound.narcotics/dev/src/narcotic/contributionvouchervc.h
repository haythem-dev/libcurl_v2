#ifndef CONTRIBUTIONVOUCHERVC_H
#define CONTRIBUTIONVOUCHERVC_H
//-----------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  shows missing acknowledgements and creates delivery note copies
 *  \author Benjamin Endlicher
 *  \date   14.09.2010
 */
//-----------------------------------------------------------------------------------------------//

//-----------------------------------------------------------------------------------------------//
// includes
//-----------------------------------------------------------------------------------------------//
#pragma warning (push)
#pragma warning(disable : 4127 4311 4312 4481 4512 4800 4244)
#include <QtWidgets/qdialog.h>
#include "ui_contributionvouchervc.h"	
#pragma warning (pop)

#include <boost/enable_shared_from_this.hpp>
#include <libbasardbaspect.h>
#include <libbasarappl.h>
#include <libbasarguitie.h>

#include "acknowledgementstatustransformer.h"

//-----------------------------------------------------------------------------------------------//
// namespaces
//-----------------------------------------------------------------------------------------------//
namespace narcotics 
{
namespace viewConn 
{

//-------------------------------------------------------------------------------------------------//
// class declaration
//-------------------------------------------------------------------------------------------------//
/*! \brief  class ContributionVoucherVC shows missing acknowledgements and creates delivery note copies.

    This is a final class.     
 */
class ContributionVoucherVC : public QDialog, public Ui::contributionvouchervcClass,
							  public boost::enable_shared_from_this<ContributionVoucherVC>, 
							  public basar::gui::tie::IViewConnector
{
    Q_OBJECT

public:
    ContributionVoucherVC( QWidget *parent = 0 );
    ~ContributionVoucherVC();

	void													init( basar::db::aspect::AccessorPropertyTableRef openAcknowledgements );
	basar::gui::tie::WidgetReturnEnum						show		();
	void													closeEvent( QCloseEvent* event );
	bool													shutdown	();
	void													hide		(); 

// matchers
	basar::db::aspect::AccessorPropertyTable_YIterator		getFirstSelectedContributionVoucher();
	basar::db::aspect::AccessorPropertyTable_YIterator		getNextSelectedContributionVoucher();
    basar::db::aspect::AccessorPropertyTableRef             getCurrentDisplayedData() const;

	void													matchFromOpenAcknowledgements();
	void													matchFromSearch();
	void													matchToSearch( basar::db::aspect::AccessorPropertyTable_YIterator searchIt );

	bool													askForRetransmission( basar::Int32 contributionVoucherNo, basar::Int32 purchaseOrderno );

private:
	void													initGuiCtrls();
	void													initTableWidgetAcknowledgement();
	void													initHeaderLables( QStringList& lstLables, std::list<basar::VarString>& lstOriginalAttributes );
    void                                                    initCboAcknowledgementStatus();

    bool                                                    areSearchParametersValid();   
    bool                                                    areSearchTimespanParametersValid();
    bool                                                    areSearchPurchaseOrderParametersValid();
    bool                                                    areSearchContributionVoucherParametersValid();
	
	void													createMatchers( basar::db::aspect::AccessorPropertyTableRef openAcknowledgements );
    void                                                    connectSignals();

    void                                                    toggleGuiCtrls();

    void                                                    toggleGuiCtrlsPurchaseOrderNo( bool state );
    void                                                    toggleGuiCtrlsContributionVoucherNo( bool state );
    void                                                    toggleGuiCtrlsTimespan( bool state );

    void                                                    clearGuiCtrlsPurchaseOrderNo();
    void                                                    clearGuiCtrlsContributionVoucherNo();
    void                                                    clearGuiCtrlsTimespan();
	
	// members
	basar::gui::tie::MatcherRef								m_matAcknowledgements;

	std::list<basar::VarString>								m_OriginalAttributeNames;	//!< list for original attributes used for table widget headers
	QStringList												m_lstLables;				//!< list for table widget headers
	bool													m_Initialized;				//!< bool to declare if header labels have already been initialized
	
private slots:
    //radios
    void                                                    onRdBtnPurchaseOrderNo_toggled();
    void                                                    onRdBtnContributionVoucherNo_toggled();
    void                                                    onRdBtnTimespan_toggled();

    //buttons
    void                                                    onBtnDeliveryNoteCopy_clicked();
	void													onBtnOpenEmptyDeliveryNote_clicked();
	void													onBtnReprintDeliverynote_clicked();
	void													onBtnClose_clicked();
	void													onBtnAcknowledgementReceived_clicked();
	void													onBtnAcknowledgementPrinted_clicked();
	void													onBtnSearch_clicked();
	void													onBtnOpenEmptyContributionVoucher_clicked();
	void													checkButtonState();
    void                                                    onBtnPrintList_clicked();
};

} //namespace viewConn
} //namespace narcotics

#endif //CONTRIBUTIONVOUCHERVC_H