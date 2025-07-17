//----------------------------------------------------------------------------
/*! \file
 *  \brief  Viewconnector interface for incoming goods overview
 *  \author Roland Kiefert
 *  \date   17.02.2006
 */
//----------------------------------------------------------------------------
#ifndef GUARD_NA01INCOMINGGOODSOVERVIEWVC_H
#define GUARD_NA01INCOMINGGOODSOVERVIEWVC_H

//-------------------------------------------------------------------------------------------------//
// includes
//-------------------------------------------------------------------------------------------------//
#include <boost/enable_shared_from_this.hpp>
#include <libbasardbaspect.h>
#include <libbasarappl.h>
#include <libbasarguitie.h>

// disable warning
#pragma warning (push)
#pragma warning(disable : 4127 4311 4312 4481 4512 4800 4244)
#include <QtWidgets/qdialog.h>
#include "ui_incominggoodsoverview.h"
#pragma warning (pop)

#include "na02incomingreturngoodsvc.h"

//-------------------------------------------------------------------------------------------------//
// namespace
//-------------------------------------------------------------------------------------------------//
namespace narcotics
{
namespace viewConn
{

//-------------------------------------------------------------------------------------------------//
// class declaration
//-------------------------------------------------------------------------------------------------//
class IncomingGoodsOverviewVC : public QDialog, public Ui::IncomingGoodsOverviewClass,
								public boost::enable_shared_from_this<IncomingGoodsOverviewVC>,
								public basar::gui::tie::IViewConnector
{
    Q_OBJECT

public:
    IncomingGoodsOverviewVC						( QWidget *parent = 0  );
    ~IncomingGoodsOverviewVC					();
	void			init						( basar::db::aspect::AccessorPropertyTableRef incomingGoodsTable, //!< property table incoming goods
												  basar::db::aspect::AccessorPropertyTableRef supplierTable, //!< property table narcotics supplier
												  basar::db::aspect::AccessorPropertyTableRef noIncomingGoodsTable, //!< property table number incoming goods
												  basar::db::aspect::AccessorPropertyTableRef noReturnGoodsTable //!< property table number returned goods
												);
	basar::gui::tie::WidgetReturnEnum	show	();
	void			hide						();
	bool			shutdown					();
	void			matchToSelection			( basar::db::aspect::AccessorPropertyTable_YIterator searchIterator );
	void			matchFromSupplier			();
	void			matchFromIncomingGoods		();
	void			matchToIncomingGoods		();
	basar::db::aspect::AccessorPropertyTable_YIterator getCurrentIncomingGood();

	void			getQtyFromCurrentRow		( basar::Int32 & currentQty //!< number of aritcle of current row
												);
	void			setQtyFromCurrentRow		( basar::Int32 currentQty //!< number of aritcle of current row
												);

    basar::db::aspect::AccessorPropertyTable_YIterator getCurrentAlternativeSupplier();

	void			matchFromNoIncomingGoods    ();
	void			matchToNewBooking			( basar::db::aspect::AccessorPropertyTable_YIterator newBooking );
	void			matchFromNewBooking			( basar::db::aspect::AccessorPropertyTable_YIterator newBooking );

	/*! \brief	Fill field articleNo with given value and disable, as well as clear field articleName. \n  no-throw */
	void			matchToArticleNo( basar::I18nString articleNoValue );

	/*! \brief	Fill field supplierNo with given value and disable, as well as clear field supplierName. \n  no-throw */
	void			matchToSupplierNo( basar::I18nString supplierNoValue );

    void            switchSupplierReplacement( const bool );

	void			switchNextBookingsCreation( bool enable );

private slots:
	void activateSearchForArticleName();
	void activateSearchForSupplierName();
    void buttonGroupStateChanged();

    void onBtnRecordItem_clicked();
    void onBtnShowIncomingGoods_clicked();
    void onBtnShowReturnGoods_clicked();
    void onBtnShowAllIncomingGoods_clicked();

	void onTblIncomingGoods_itemSelectionChanged();
    void onTblIncomingGoods_cellPressed(int,int); //sk
    void onTblIncomingGoods_itemActivated(QTableWidgetItem* p);//sk
    void onTblIncomingGoods_cellDoubleClicked(int,int);
	void onTblIncomingGoods_itemChanged(QTableWidgetItem*);
//	void onTblIncomingGoods_closeEditor(QWidget* editor); 

    void onBtnSearch_clicked();

	void onBtnIncomingGoodsRefresh_clicked();
	void onBtnIncomingGoodsDelete_clicked();

	void onBtnProcessAllVisibleIncomingGoods_clicked();

	void onBtnUpdateSupplier_clicked();

	void onOrderNumber_textChanged( const QString& );
	void onArticleNumber_textChanged( const QString& );
	void onArticleName_textChanged( const QString& );
	void onSupplierNo_textChanged( const QString& );
	void onSupplierName_textChanged( const QString& );

private:
	void initGuiCtrls();
    void initSearchOptions();

    void activateOrderSearchCriteria    ( const bool );
    void activateOrderNumberCriterion   ( const bool );
    void activateArticleNumberCriterion ( const bool );
    void activateArticleNameCriterion   ( const bool );

    void activateSupplierSearchCriteria ( const bool );
    void activateSupplierNumberCriterion( const bool );
    void activateSupplierNameCriterion  ( const bool );

    void activateIncomingGoodsDisplay   ( const bool );

    void activateSupplierDisplay( const bool );
	void clearWidget();
    void initSupplierDisplay();
    void initIncomingGoodsDisplay();
    void initSearchCriteria();

    void activateNumberOfGoodsDisplay( const bool );

    const bool isAlternativeSupplierDisplayed();
    const bool isAlternativeSupplierSelected();

    const bool isIncomingGoodDisplayed();
    const bool isIncomingGoodSelected();
    void selectFirstIncomingGood();

	/*! \brief	Checks if the search may be started (supplier no. respectively suppliername, article no or article name or order no have to be set). \n  no-throw */
	const bool isSearchAllowed();
    void evaluateSearchExecution();
    const bool isOneIncomingGoodItemSelected();

    void connectEvents();

	basar::gui::tie::MatcherRef m_matIncomingGoods;
	basar::gui::tie::MatcherRef m_matSupplier;
	basar::gui::tie::MatcherRef m_matNoIncomingGoods;
	basar::gui::tie::MatcherRef m_matNoReturnGoods;

	basar::gui::tie::ViewConnPtr<IncomingReturnGoodsVC> m_ManualDialog;

	std::list<basar::VarString>	m_OriginalAttributeNames;	//!< list for original attributes used for combo box headers

	bool m_IsFirstCall;
    QTableWidgetItem *pCurrTblWidg;
    //bool bAktBtnClicked;
    bool bCellPressed;
    int curRow;
};

} //namespace viewConn
} //namespace narcotics

#endif //GUARD_NA01INCOMINGGOODSOVERVIEWVC_H