//----------------------------------------------------------------------------
/*! \file
 *  \brief  Vieconnector interface for compare narcotics bookings and PHAMOS bookings
 *  \author Roland Kiefert
 *  \date   17.02.2006
 */
//----------------------------------------------------------------------------
#ifndef GUARD_HALFYEARLYREPORTVC_H
#define GUARD_HALFYEARLYREPORTVC_H

//-------------------------------------------------------------------------------------------------//
// includes
//-------------------------------------------------------------------------------------------------//
#include <boost/enable_shared_from_this.hpp>
#include <libbasardbaspect.h>
#include <libbasarappl.h>
#include <libbasarguitie.h>

#pragma warning (push)
#pragma warning(disable : 4127 4311 4312 4481 4512 4800 4244)
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/qdialog.h>
#include "ui_halfyearlyreportvc.h"
#pragma warning (pop)

//-------------------------------------------------------------------------------------------------//
// namespace
//-------------------------------------------------------------------------------------------------//
namespace narcotics 
{
namespace viewConn 
{

enum ReportTypeEnum
{
	noReportselected = 0,
	C1Report,
	C2Report,
	C4Report
};

//-------------------------------------------------------------------------------------------------//
class HalfYearlyReportVC :  public QDialog, public Ui::HalfYearlyReportClass,
							public boost::enable_shared_from_this<HalfYearlyReportVC>,
							public basar::gui::tie::IViewConnector

{
    Q_OBJECT

public:
    HalfYearlyReportVC							( QWidget *parent = 0 );
    ~HalfYearlyReportVC							();
	basar::gui::tie::WidgetReturnEnum	show	(); 
	void			hide						(); 
	bool			shutdown					(); 
	void			init						( basar::db::aspect::AccessorPropertyTableRef openOrders );
	void			matchFromPeriod				( basar::DateTime& from, basar::DateTime& to, ReportTypeEnum& reportType );
	void			matchToData					();
	basar::db::aspect::AccessorPropertyTable_YIterator getCurrentAccessorIterator	();

private:
	void			wireEvents					();
	void			initGuiCtrls				(); 

	QButtonGroup								m_ReportTypeGroup;
	basar::db::aspect::AccessorPropertyTableRef m_openOrder;
	basar::gui::tie::MatcherRef					m_matData;

private slots:
	void onBtnPrint_clicked		();
	void onBtnSearch_clicked	();
	void onBtnClose_clicked		();
	void onBtnDelete_clicked	();
};

} //namespace viewConn
} //namespace narcotics

#endif //GUARD_HALFYEARLYREPORTVC_H