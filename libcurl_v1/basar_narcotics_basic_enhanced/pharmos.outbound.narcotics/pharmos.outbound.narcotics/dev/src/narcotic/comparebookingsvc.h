//----------------------------------------------------------------------------
/*! \file
 *  \brief  Vieconnector interface for compare narcotics bookings and PHAMOS bookings
 *  \author Roland Kiefert
 *  \date   17.02.2006
 */
//----------------------------------------------------------------------------
#ifndef GUARD_COMPAREBOOKINGSVC_H
#define GUARD_COMPAREBOOKINGSVC_H

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
#include "ui_comparebookingsvc.h"
#pragma warning (pop)

//-------------------------------------------------------------------------------------------------//
// namespace
//-------------------------------------------------------------------------------------------------//
namespace narcotics 
{
namespace viewConn 
{

//-------------------------------------------------------------------------------------------------//
// class
//-------------------------------------------------------------------------------------------------//
class CompareBookingsVC : public QDialog, public Ui::CompareBookingsClass,
						  public boost::enable_shared_from_this<CompareBookingsVC>,
						  public basar::gui::tie::IViewConnector
{
    Q_OBJECT

public:
    CompareBookingsVC( QWidget *parent = 0 );
    ~CompareBookingsVC();
	void			init						(	basar::db::aspect::AccessorPropertyTableRef dataTable,			//!< property table article data and narcotics group
													basar::db::aspect::AccessorPropertyTableRef narcoticsTable,		//!< property table narcotics booking
													basar::db::aspect::AccessorPropertyTableRef originalTable,		//!< property table original booking
													basar::db::aspect::AccessorPropertyTableRef articleDiff,		//!< property table narcdiffqtyinstock
													basar::db::aspect::AccessorPropertyTableRef articleDiffArtNos,	//!< property table narcdiffqtyinstock
													bool invokeContextDiffQtyInStock = false						//!< dialog is called from the dialog-class/ UC DiffQtyInstock
												); 
	basar::gui::tie::WidgetReturnEnum	show	(); 
	void			hide						(); 
	bool			shutdown					(); 

	void			matchFromSelection			( basar::db::aspect::AccessorPropertyTable_YIterator searchIterator );
	void			matchToNarcoticData			();
	void			matchToNarcoticBookings		();
	void			matchToOriginalBookings		();
	void			matchToArticleDifference	();
	void			matchToArticleNosWithDiffQtyInStock();

	basar::db::aspect::AccessorPropertyTable_YIterator getCurrentAccessorIterator	();
	bool			isSelected					();

private:
	//-------------------------------------------------------------------------------------------------//
	// member functions
	//-------------------------------------------------------------------------------------------------//
	void							wireEvents();

	void							initGuiCtrls();
	void							initSearchFrame();
	void							initArticleDataFrame();
	void							initNarcoticDataFrame();
	void							initBasarTableWidgets();

	void							createMatcher(	basar::db::aspect::AccessorPropertyTableRef dataTable,		
													basar::db::aspect::AccessorPropertyTableRef narcoticsTable,
													basar::db::aspect::AccessorPropertyTableRef originalTable,	
													basar::db::aspect::AccessorPropertyTableRef articleDiff,
													basar::db::aspect::AccessorPropertyTableRef articleDiffArtNos );

	void							setResultsInTable( bool invokeContextDiffQtyInStock );

	//-------------------------------------------------------------------------------------------------//
	// member variables
	//-------------------------------------------------------------------------------------------------//
	basar::gui::tie::MatcherRef		m_matNarcoticsData;
	basar::gui::tie::MatcherRef		m_matNarcoticBookings;
	basar::gui::tie::MatcherRef		m_matOriginalBookings;
	basar::gui::tie::MatcherRef		m_matArticleDifference;
	basar::gui::tie::MatcherRef		m_matArticleNosDiffQtyInStock;
	bool							m_invokeContextDiffQtyInStock;

private slots:
	void onCboArticleNos_currentIndexChanged	( const QString & );
	void onTxtArticleNoSearch_textChanged		( const QString & );
	void onBtnClose_clicked						();
	void onBtnDelete_clicked					();
	void onBtnRebuild_clicked					();
	void onBtnSearch_clicked					();
	void closeDialog							();
	void itemSelectedIsChange					();
};

} //namespace viewConn
} //namespace narcotics

#endif //GUARD_COMPAREBOOKINGSVC_H