//----------------------------------------------------------------------------
/*! \file
 *  \brief  Vieconnector interface for deleted bookings
 *  \author Roland Kiefert
 *  \date   22.03.2006
 */
//----------------------------------------------------------------------------
#ifndef GUARD_DELETEDBOOKINGVC_H
#define GUARD_DELETEDBOOKINGVC_H

//-------------------------------------------------------------------------------------------------//
// includes
//-------------------------------------------------------------------------------------------------//
#include <boost/enable_shared_from_this.hpp>
#include <libbasardbaspect.h>
#include <libbasarappl.h>
#include <libbasarguitie.h>

#pragma warning (push)
#pragma warning(disable : 4127 4311 4312 4481 4512 4800 4244)
#include <QtWidgets/qdialog.h>
#include "ui_deletedbookingvc.h"
#pragma warning (pop)

//-------------------------------------------------------------------------------------------------//
// namespace
//-------------------------------------------------------------------------------------------------//
namespace narcotics 
{
namespace viewConn 
{

class DeletedBookingVC : public QDialog, public Ui::DeletedBookingClass,
						 public boost::enable_shared_from_this<DeletedBookingVC>,
						 public basar::gui::tie::IViewConnector
{
    Q_OBJECT

public:
    DeletedBookingVC(QWidget *parent = 0);
    ~DeletedBookingVC();

	void												init						( basar::db::aspect::AccessorPropertyTableRef deletedBookings ); 
	basar::gui::tie::WidgetReturnEnum					show						(); 
	void												hide						(); 
	bool												shutdown					(); 
	void												matchFromSelection			( basar::db::aspect::AccessorPropertyTable_YIterator searchIt );
	void												matchToDeletedBookings		();
	basar::db::aspect::AccessorPropertyTable_YIterator	getCurrentAccessorIterator	( basar::Int32 position );
	basar::db::aspect::AccessorPropertyTable_YIterator	getSelectedFirstIterator	();
	basar::db::aspect::AccessorPropertyTable_YIterator	getSelectedNextAccIterator	();

private slots:
	void												onArticleNoSearch_textChanged( const QString & );
	void												onBtnSearch_clicked			();
	void												onBtnClose_clicked			();
	void												onBtnRestore_clicked		();
	void												onTableWidget_itemSelectionChanged();

private:
	void												wireEvents					();
	void												createLayout				();
	void												initGuiCtrls				();

	basar::gui::tie::MatcherRef							m_matDeletedBookings;
};

} //namespace viewConn
} //namespace narcotics

#endif //GUARD_DELETEDBOOKINGVC_H