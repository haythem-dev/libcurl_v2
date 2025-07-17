#ifndef UI_RESULTSETOVERVIEWVC_H
#define UI_RESULTSETOVERVIEWVC_H
//-----------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  
 *  \author Benjamin Endlicher
 *  \date   24.03.2010
 */
//-----------------------------------------------------------------------------------------------//
// includes
//-----------------------------------------------------------------------------------------------//
#pragma warning (push)
#pragma warning(disable : 4127 4311 4312 4481 4512 4800 4244)
#include <QtWidgets/qdialog.h>
#include "ui_resultsetoverview.h"
#pragma warning (pop)

#include <boost/enable_shared_from_this.hpp>
#include <libbasardbaspect.h>
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
class ResultSetOverviewVC :	public QDialog, public Ui::resultsetoverviewvcClass,
							public boost::enable_shared_from_this<ResultSetOverviewVC>, 
							public basar::gui::tie::IViewConnector
{
	Q_OBJECT

public:
	ResultSetOverviewVC								( QWidget *parent = 0 );
	~ResultSetOverviewVC							();

	void								init		( basar::db::aspect::AccessorPropertyTableRef table,
													  basar::I18nString attribute1,
													  basar::I18nString attribute2,
													  basar::I18nString headerlabel1, 
													  basar::I18nString headerlabel2 ); //!< property table of narcotics group data
	basar::gui::tie::WidgetReturnEnum	show		(); 
	void								hide		(); 
	bool								shutdown	(); 
// Matchers
	void								matchToWidget(); 
	basar::db::aspect::AccessorPropertyTable_YIterator getCurrentSelection();

private slots:
	void 								onBtnTakeOver_clicked();
	void 								onBtnCancel_clicked();
	void 								onTablewidget_itemDoubleClicked( QTableWidgetItem* );
	void								onTablewidget_itemSelectionChanged();
	
private:
	void								wireEvents();

	basar::gui::tie::MatcherRef			m_matTableWidget;
	bool								m_Cancelled;
};

} //namespace viewConn
} //namespace narcotics

#endif //UI_RESULTSETOVERVIEWVC_H