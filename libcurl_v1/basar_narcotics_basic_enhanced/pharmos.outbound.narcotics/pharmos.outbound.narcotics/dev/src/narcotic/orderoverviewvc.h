#ifndef ORDEROVERVIEWVC_H
#define ORDEROVERVIEWVC_H
//-----------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  
 *  \author Benjamin Endlicher	
 *  \date   15.03.2010
 */
//-----------------------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------------------------//
// includes
//-------------------------------------------------------------------------------------------------//
#include <libbasardbaspect.h>
#include <libbasarappl.h>
#include <libbasarguitie.h>

// the following include is necessary to create an instance of detailVC
#include "createprocessnarcoticsbooking.h"

#pragma warning (push)
#pragma warning(disable : 4127 4311 4312 4481 4512 4800 4244)
#include <QtWidgets/qdialog.h>
#include "ui_orderoverview.h"
#pragma warning (pop)

//-------------------------------------------------------------------------------------------------//
// namespace
//-------------------------------------------------------------------------------------------------//
namespace narcotics 
{
namespace viewConn 
{
	struct BgaNoData
	{
		basar::Int32 bgaNo;
		basar::VarString orderType;
		basar::VarString name;
		basar::VarString location;
	};

	enum TransactionTypeComboIndex
	{
		INDEX_SELLOFF    = 0,
		INDEX_GI,
		INDEX_CUST_RETURN,
		INDEX_SUPPL_RETURN,
		INDEX_DESTRUCTION
	};

class OrderOverviewVC : public QDialog, public Ui::OrderOverviewVCClass,
						public boost::enable_shared_from_this<OrderOverviewVC>,
						public basar::gui::tie::IViewConnector
{
	Q_OBJECT

public:
	OrderOverviewVC(QWidget *parent = 0);
	~OrderOverviewVC();
												// propertyTables of AccBookingCatalog and AccNarcoticGroup
	void			init						( basar::db::aspect::AccessorPropertyTableRef bookingsCatalog, 
												  basar::db::aspect::AccessorPropertyTableRef narcoticData ); 

	basar::gui::tie::WidgetReturnEnum	show		();
	bool								shutdown	(); 
	void								hide		(); 

	/*! \brief	Returns the current Iterator of the AccessorPropertyTableRef. \n no-throw */
	basar::db::aspect::AccessorPropertyTable_YIterator getCurrentAccessorIterator();

	basar::VarString					getCurrentLineCode();
	//basar::db::aspect::AccessorPropertyTable_YIterator getOrder();

	bool								orderHeaderDataChanged();
	
	//---------------------------------------------------------------------------------------------------------------------//
	// match - methods
	//---------------------------------------------------------------------------------------------------------------------//
	/*! \brief	Enter article and narcotic data into detailed dialog. \n no-throw */
	void							matchToArticleNarcoticData( basar::db::aspect::AccessorPropertyTable_YIterator yit );
	
	/*! \brief	All selected narcotic specific data of the AccessorPropertytable will be displayed in the dialog. \n no-throw */
	void							matchToNarcoticBookings( basar::Int32 tableWidgetRows );

	/*! \brief	Narcotic specific order data of the AccessorPropertytable will be displayed in the dialog. \n no-throw */
	void							matchToOrderData( basar::db::aspect::AccessorPropertyTable_YIterator orderDataIt );

	/*! \brief	Narcotic specific order data of the dialog will be matched into appropriate AccessorPropertytable. \n no-throw */
	void							matchFromOrderData( basar::db::aspect::AccessorPropertyTable_YIterator orderDataIt );

	/*! \brief	Search criteria will set from the dialog to the passed iterator. \n no-throw */
	void							matchFromSelection( basar::db::aspect::AccessorPropertyTable_YIterator searchIt );

	/*! \brief	Assigns dialog values to the passed AccessorPropertyTable_YIterator. 
				Here it will be differentiate if master or detailed dialog is concerned. \n no-throw */
	void							matchFromCurrentBooking( basar::db::aspect::AccessorPropertyTable_YIterator currentBooking );
	
	/*! \returns dialog value customersupplierno.  \n no-throw */
	basar::Int32					matchFromCustomerSupplierNo();

		/*! \returns dialog value transactiontime.  \n no-throw */
	basar::Int32					matchFromTransactionTime();

	basar::Int32					matchFromTransactionDate();

	void							matchFromNarcoticBookings();

	BgaNoData						matchFromCustomerSuppliers();

	/*! \match found customer data - which means customerno, customername and customerlocation - into corresponding Gui lineEdits.  \n no-throw */
	void							matchToCustomerData( basar::db::aspect::AccessorPropertyTable_YIterator customerData );

	/*! \match found supplier data - which means narcoticssupplierno, suppliername and customerlocation - into corresponding Gui lineEdits.  \n no-throw */
	void							matchToSupplierData( basar::db::aspect::AccessorPropertyTable_YIterator supplierData );

	void							matchToCustomerSuppliers( basar::db::aspect::AccessorPropertyTable_YIterator supplierData );

	void							matchToCustomerSuppliers( basar::db::aspect::AccessorPropertyTableRef supplierData );

	/*! \match customer data into corresponding lineEdit of detaildialog.  \n no-throw */
	void							matchToCustomer( basar::db::aspect::AccessorPropertyTable_YIterator customer );

	/*! \match narcoticssupplier into corresponding combobox of detaildialog.  \n no-throw */
	void							matchToSuppliers( basar::db::aspect::AccessorPropertyTableRef aptNarcSupp );
	
	/*! \brief	delegates the passed AccessorPropertytable (branch data) to a sub dialog's member. \n no-throw */
	void							matchToBranch( basar::db::aspect::AccessorPropertyTable_YIterator branch );

	/*! \clears and disables table widget.  \n no-throw */
	void							clearTableWidget();

protected:
	bool eventFilter( QObject *obj, QEvent *event );

private slots:
	/*! \brief	receives changes on customersupplierno and therefore adapts appropriate 
				\n customersupplierlocation as well as customersuppliername and enables button SaveOrderChanges. 
				\n no-throw. */
	void onCustomersupplierno_editingFinished();

	/*! \brief	receives changes on transactiontime and delegates the decision whether button SaveOrderChanges 
				\n has to be enabled or not to CheckTimeChanges. 
				\n no-throw. */
	void onTransactiontime_changed();

	void onTransactiondate_changed( const QDate& newTransactionDate );
	
	void onTransactiontype_changed( int index );

	void onType_changed( int index );
	/*! \brief	Opens the detailed dialog for manipulating the passed, highlighted narcotic booking of the master dialog. 
				\n no-throw. */
	//! \brief	Activates the edit- and delete button if a narcotic booking is selected in the table widget. \n no-throw. */
	void onTableWidget_itemSelectionChanged();

	void onBtnEdit_clicked();
	
	void onBtnSearch_clicked();
	
	void onBtnClose_clicked();

	void currentIndexOfBgaNoChanged( int i );

	void currentIndexOfLineCodeChanged( int i );
	
	//! \brief	Saves changes made to a whole order by using grpOrderData. \n no-throw. */
	void onBtnSaveOrderChanges_clicked();

	void onBtnDelete_clicked();

	//! \brief	Marks the selected (in the table widget) narcotic booking positions as deleted. \n no-throw. */
	void onBtnCancelOrderPositions_clicked();

	//! \brief	Cancels the complete booking and deletes the contribution voucher \n no-throw. */
	void onBtnCancelFullOrder_clicked();
	
	void onBtnPrintDisolveReceipt_clicked();
	
	/*! \brief	Depending upon an valid input in the order number text field, the search button will be enabled or disabled. 
				\n no-throw. */
	void onOrderNoSearch_textChanged( const QString & );

private:
	void wireEvents();
	void initCboLineCode();
	void initCboBgaNo();

	void selectTransactionTypeInCboLineCode(basar::VarString lineType);
	
	void setLineCodeToSelectedTransactionType(int index);
	void setLineCodeToSellOf();
	void setLineCodeToSupplReturn();

	/*!  \brief	Initializes the gui ctrls (comboBox, search criteria text fields, 
				article and narcotic specific frame, the narcotics booking grid an buttons). \ no-throw */
	void initGuiCtrls( bool withSearchCriteria = true );

	/*! initializes the table widget in clearing its values and set its headings. \ no-throw */
	void initTableWidget();

	/*!  \brief	Switches the order specific ctrls to the given state. \ no-throw */
	void setDisabled_grpOrderData( bool disable );

	/*!  \brief	Switches the narcotic specific ctrls to the given state. \ no-throw */
	void setDisabled_grpNarcoticsBookingsData( bool disable );
	
	/*! \brief	enables button SaveOrderChanges. 
				\n no-throw. */
	void enableSaveOrderButton();

	/*! \brief	disables button SaveOrderChanges. 
				\n no-throw. */
	void disableSaveOrderButton();

	// private members
	basar::gui::tie::MatcherRef		m_matNarcoticBookings;		//!< matcher for all selected narcotic bookings
	basar::gui::tie::MatcherRef     m_matOrderData;				//!< matcher for order data
	basar::gui::tie::MatcherRef		m_matNarcoticData;			//!< matcher for narcotic data
	basar::gui::tie::MatcherRef		m_matSupplier;

	std::list<basar::VarString>		m_BgaOriginalAttributeNames;

	std::list<basar::VarString>		m_OriginalAttributeNames;	//!< list for original attributes used for table widget headers
	std::list<basar::VarString>		m_lineCodes;
	bool							m_DetailVCShown;			//!< saves the state if the detailed dialog is shown at the time.
	QString							m_BackupNo;					//!< saves old customersupplierno if rollback is needed

	bool							m_orderHeadDataChanged;

	basar::gui::tie::ViewConnPtr<CreateProcessNarcoticsBookingVC>	m_RecordEditVC;			//!< detailed dialog

	QDate							m_originalTransactionDate;

	int								m_currentLineBgaNo;
	int								m_currentLineLineCode;

	basar::Int16					m_CurrentAckStatus;
};

} //namespace viewConn 
} //namespace narcotics 

#endif // ORDEROVERVIEWVC_H