//----------------------------------------------------------------------------
/*! \file
 *  \brief  Viewconnector interface for incoming goods overview
 *  \author Roland Kiefert
 *  \date   17.02.2006
 */
//----------------------------------------------------------------------------
#ifndef GUARD_NA02INCOMINGRETURNGOODSVC_H
#define GUARD_NA02INCOMINGRETURNGOODSVC_H

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
#include "ui_incomingreturngoods.h"
#pragma warning (pop)

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
class IncomingReturnGoodsVC :	public QDialog, public Ui::IncomingReturnGoodsClass,
								public boost::enable_shared_from_this<IncomingReturnGoodsVC>,
								public basar::gui::tie::IViewConnector
{
    Q_OBJECT

public:
    IncomingReturnGoodsVC(QWidget *parent = 0);
    ~IncomingReturnGoodsVC();
	void			init						( basar::db::aspect::AccessorPropertyTableRef supplierTable	);
	basar::gui::tie::WidgetReturnEnum	show	();
	void			hide						();
	bool			shutdown					();
	void			matchToBooking				( basar::db::aspect::AccessorPropertyTable_YIterator newBooking );
	void			matchFromBooking			( basar::db::aspect::AccessorPropertyTable_YIterator newBooking );
	void			clearDialog					();
	void			enableButtonStateNext		( bool enable );

private slots:
	void 			change_cb_transactiontype(int);
	void 			change_articleno();
	void 			change_customersupplierno();
	void 			change_cb_customersupplierno(int);

	void			onBtnNext_clicked();
	void 			onBtnCancel_clicked();
	void 			onBtnClose_clicked();
	void 			onPurchaseorderno_textChanged( const QString& );
	void 			onArticlename_textChanged( const QString& );
	void 			onPackageunit_textChanged( const QString& );
	void 			onQtytransaction_textChanged( const QString& );
	void 			onPharmaform_textChanged( const QString& );
	void 			onTransactiondate_dateChanged();
	void 			onTransactiontime_timeChanged();

private:
	void			wireEvents();
	void 			initGuiCtrls();
	void			clearContentsArticleData();
	void			clearContentsCustomerData();
	void			enableSaveButtons();

	basar::gui::tie::MatcherRef m_matSupplier;
	std::list<basar::VarString>	m_OrigHeaderAttrList;	//!< list of the original attributes used for combo box headers

	bool m_existingBooking;
};

} //namespace viewConn
} //namespace narcotics

#endif //GUARD_NA02INCOMINGRETURNGOODSVC_H