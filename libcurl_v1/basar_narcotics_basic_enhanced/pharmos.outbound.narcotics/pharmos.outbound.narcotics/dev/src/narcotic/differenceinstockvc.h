//----------------------------------------------------------------------------
/*! \file
 *  \brief  Vieconnector interface for deleted bookings
 *  \author Roland Kiefert
 *  \date   22.03.2006
 */
//----------------------------------------------------------------------------
#ifndef GUARD_DIFFERENCEINSTOCKVC_H
#define GUARD_DIFFERENCEINSTOCKVC_H

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
#include "ui_differenceinstockvc.h"
#pragma warning (pop)

//-------------------------------------------------------------------------------------------------//
// namespace
//-------------------------------------------------------------------------------------------------//
namespace narcotics 
{
namespace viewConn 
{

class DifferenceInStockVC : public QDialog, public Ui::DifferenceInStockClass,
							public boost::enable_shared_from_this<DifferenceInStockVC>,
							public basar::gui::tie::IViewConnector
{
    Q_OBJECT

public:
										DifferenceInStockVC		( QWidget *parent = 0 );
										~DifferenceInStockVC	();
	basar::gui::tie::WidgetReturnEnum	show					(); 
	void								hide					(); 
	bool								shutdown				(); 
	void								init					( basar::db::aspect::AccessorPropertyTableRef diffQtyInStock );
	void								matchToData				();

private:
	void								initGuiCtrls			(); 
	void								wireEvents				();

	basar::gui::tie::MatcherRef			m_matDiffQtyInStock;

private slots:
	void								onBtnUpdateDiffQtyInStock_clicked();
	void								onBtnShowCompareBookings_clicked();
	void								onBtnClose_clicked		();
};

} //namespace viewConn
} //namespace narcotics

#endif //GUARD_DIFFERENCEINSTOCKVC_H