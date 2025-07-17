#ifndef UI_RESOLVEPRINTINGERRORSVC_H
#define UI_RESOLVEPRINTINGERRORSVC_H
//-----------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  
 *  \author Marco Köppendörfer
 *  \date   06.09.2012
 */
//-----------------------------------------------------------------------------------------------//
// includes
//-----------------------------------------------------------------------------------------------//
#pragma warning (push)
#pragma warning(disable : 4127 4311 4312 4481 4512 4800 4244)
#include <QtWidgets/qdialog.h>
#include "ui_resolveprintingerrors.h"
#pragma warning (pop)

#include <boost/enable_shared_from_this.hpp>
#include <libbasardbaspect.h>
#include <libbasarappl.h>
#include <libbasarguitie.h>

//-----------------------------------------------------------------------------------------------//
// namespaces
//-----------------------------------------------------------------------------------------------//
namespace narcotics 
{
namespace viewConn 
{

//-----------------------------------------------------------------------------------------------//
// class declaration
//-----------------------------------------------------------------------------------------------//
class ResolvePrintingErrorsVC :	public QDialog, public Ui::resolveprintingerrorsvcClass,
								public boost::enable_shared_from_this<ResolvePrintingErrorsVC>, 
								public basar::gui::tie::IViewConnector
{
	Q_OBJECT

public:
	ResolvePrintingErrorsVC							( QWidget *parent = 0 );
	~ResolvePrintingErrorsVC						();

	void								init		();
	basar::gui::tie::WidgetReturnEnum	show		(); 
	void								hide		(); 
	bool								shutdown	(); 

    void                                setData     ( basar::db::aspect::AccessorPropertyTableRef table ); //!< property table of possible printing errors
// Matchers
	void								matchToWidget(); 
	basar::db::aspect::AccessorPropertyTable_YIterator getCurrentSelection();

private slots:
	void								onBtnShowDetails_clicked();
    void								onBtnSolve_clicked();
	void								onBtnClose_clicked();
	void								onTablewidget_itemSelectionChanged();
	
private:
    void								wireEvents(); 
    void								initGuiCtrls(); 
    void                                colorRows();
    bool                                isInPrintBookingAlreadyTransfered( basar::db::aspect::AccessorPropertyTable_YIterator selectedBooking );
    bool                                hasSelectedBookingAlreadyBeenTransfered();

    const basar::cmnutil::Date          getLastSundaysDate();

	basar::gui::tie::MatcherRef			m_matTableWidget;
};

} //namespace viewConn
} //namespace narcotics

#endif //UI_RESOLVEPRINTINGERRORSVC_H