//----------------------------------------------------------------------------
/*! \file
 *  \brief  Vieconnector interface for input date
 *  \author Roland Kiefert
 *  \date   22.03.2006
 */
//----------------------------------------------------------------------------
#ifndef GUARD_INPUTDATEVC_H
#define GUARD_INPUTDATEVC_H

#include <boost/enable_shared_from_this.hpp>
#include <libbasardbaspect.h>
#include <libbasarappl.h>
#include <libbasarguitie.h>

#pragma warning (push)
#pragma warning(disable : 4127 4311 4312 4481 4512 4800 4244)
#include <QtWidgets/qdialog.h>
#include "ui_inputdatevc.h"
#pragma warning (pop)

//-------------------------------------------------------------------------------------------------//
// namespace
//-------------------------------------------------------------------------------------------------//
namespace narcotics 
{
namespace viewConn 
{

class InputDateVc : public QDialog,
					public boost::enable_shared_from_this<InputDateVc>,
					public basar::gui::tie::IViewConnector
{
    Q_OBJECT

public:
					InputDateVc					( QWidget *parent = 0 );
					~InputDateVc				();
	void			init						(); 
	basar::gui::tie::WidgetReturnEnum	show	(); 
	void			hide						(); 
	bool			shutdown					(); 
	void			setDate						( basar::cmnutil::DateTime dt );
	void			matchFromSelection			( basar::db::aspect::AccessorPropertyTable_YIterator searchIt//!< group id from search parameter
												);

private slots:
	void closeDialog();
	void onBtnClose_clicked();

private:
	void wireEvents();
    void initGuiCtrls();
	Ui::InputDateVcClass ui;
};

} //namespace viewConn
} //namespace narcotics

#endif //GUARD_INPUTDATEVC_H