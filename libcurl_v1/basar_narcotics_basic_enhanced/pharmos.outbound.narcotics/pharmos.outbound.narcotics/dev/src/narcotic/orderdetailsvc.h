#ifndef ORDERDETAILSVC_H
#define ORDERDETAILSVC_H
//-----------------------------------------------------------------------------------------------//
/*! \file   orderdetails.h
 *  \brief  VC for displaying order details, cheap copy of orderoverviewvc
 *  \author Marco Köppendörfer	
 *  \date   16.10.2012
 */
//-----------------------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------------------------//
// includes
//-------------------------------------------------------------------------------------------------//
#include <libbasardbaspect.h>
#include <libbasarappl.h>
#include <libbasarguitie.h>
#include <boost/enable_shared_from_this.hpp>

#pragma warning (push)
#pragma warning(disable : 4127 4311 4312 4481 4512 4800 4244)
#include <QtWidgets/qdialog.h>
#include "ui_orderdetails.h"
#pragma warning (pop)

//-------------------------------------------------------------------------------------------------//
// namespace
//-------------------------------------------------------------------------------------------------//
namespace narcotics 
{
namespace viewConn 
{

	class OrderDetailsVC :  public QDialog, public Ui::OrderDetailsVCClass,
							public boost::enable_shared_from_this<OrderDetailsVC>,
							public basar::gui::tie::IViewConnector
{
	Q_OBJECT

public:
	OrderDetailsVC(QWidget *parent = 0);
	~OrderDetailsVC();

	void								setData		( basar::db::aspect::AccessorPropertyTableRef bookingsCatalog ); 
	basar::gui::tie::WidgetReturnEnum	show		();
	bool								shutdown	(); 
	void								hide		(); 
	
	//---------------------------------------------------------------------------------------------------------------------//
	// match - methods
	//---------------------------------------------------------------------------------------------------------------------//

    void                            matchToData();

private slots:

	void onBtnClose_clicked();

private:
	void wireEvents();

	/*!  \brief	Initializes the gui ctrls. \ no-throw */
	void initGuiCtrls();

	/*! initializes the table widget in clearing its values and set its headings. \ no-throw */
	void initTableWidget();

	/*!  \brief	Switches the order specific ctrls to the given state. \ no-throw */
	void setDisabled_grpOrderData( bool disable );

	/*!  \brief	Switches the narcotic specific ctrls to the given state. \ no-throw */
	void setDisabled_grpNarcoticsBookingsData( bool disable );
	
	QDate m_originalTransactionDate;

	// private members
	basar::gui::tie::MatcherRef     m_matOrderData;				//!< matcher for order data
	basar::gui::tie::MatcherRef		m_matNarcoticBookings;			//!< matcher for narcotic data

	std::list<basar::VarString>		m_OriginalAttributeNames;	//!< list for original attributes used for table widget headers

    void colorRows();
};

} //namespace viewConn 
} //namespace narcotics 

#endif //ORDERDETAILSVC_H