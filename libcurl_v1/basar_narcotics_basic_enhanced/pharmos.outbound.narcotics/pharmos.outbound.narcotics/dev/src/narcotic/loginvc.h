#ifndef LOGINVC_H
#define LOGINVC_H
//-----------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  
 *  \author Benjamin Endlicher
 *  \date   01.09.2010
 */
//-----------------------------------------------------------------------------------------------//
// includes
//-----------------------------------------------------------------------------------------------//
#pragma warning (push)
#pragma warning(disable : 4127 4311 4312 4481 4512 4800 4244)
#include <QtWidgets/qdialog.h>
#include "ui_loginvc.h"
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
class LoginVC :		public QDialog, public Ui::loginvcClass,
					public boost::enable_shared_from_this<LoginVC>, 
					public basar::gui::tie::IViewConnector
{
	Q_OBJECT

public:
	LoginVC								( QWidget *parent = 0 );
	~LoginVC							();

	void								init		(); 
	basar::gui::tie::WidgetReturnEnum	show		(); 
	void								hide		(); 
	bool								shutdown	(); 

	void								matchCurrentSession( basar::Int32 userID, basar::I18nString userName ); 

private slots:
	void								clearSessionDetails();
	void								onBtnLogout_clicked();
	void								onBtnCancel_clicked();

private:
	void								wireEvents();
};

} //namespace viewConn
} //namespace narcotics

#endif //LOGINVC_H