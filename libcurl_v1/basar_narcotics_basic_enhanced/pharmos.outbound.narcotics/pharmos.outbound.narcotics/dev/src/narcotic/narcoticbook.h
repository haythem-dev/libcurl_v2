#ifndef GUARD_NARCOTICBOOK_H
#define GUARD_NARCOTICBOOK_H
//-----------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  
 *  \author Bjoern Bischof
 *  \date   17.02.2006
 */
//-----------------------------------------------------------------------------------------------//

//-----------------------------------------------------------------------------------------------//
// includes
//-----------------------------------------------------------------------------------------------//
#pragma warning (push)
#pragma warning(disable : 4127 4311 4312 4481 4512 4800 4244)
#include <QtWidgets/qdialog.h>
#include "ui_narcoticbook.h"
#pragma warning (pop)

#include <boost/enable_shared_from_this.hpp>
#include <libbasardbaspect.h>
#include <libbasarappl.h>
#include <libbasarguitie.h>

#include "createprocessnarcoticsbooking.h"

//-----------------------------------------------------------------------------------------------//
// forward declarations
//-----------------------------------------------------------------------------------------------//
namespace basar {
	namespace db {
		namespace aspect {
			class AccessorPropertyTableRef;
		}
	}
}

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
class NarcoticBookVC :	public QDialog, public Ui::NarcoticBookClass,
						public boost::enable_shared_from_this<NarcoticBookVC>,
						public basar::gui::tie::IViewConnector
{
    Q_OBJECT

public:
    NarcoticBookVC( QWidget *parent = 0 );
    ~NarcoticBookVC();

	/*! \brief	The second init()-method next to the private one. Here the dialog is initialized with two
				AccessorPropertyTableRefs important to create two matchers. \n no-throw */
	void							init(	basar::db::aspect::AccessorPropertyTableRef articleNarcoticsData, 
											basar::db::aspect::AccessorPropertyTableRef narcoticsBookings, 
											basar::db::aspect::AccessorPropertyTableRef initialInventory );	
	
	/*! \brief	The dialog will be shown modal ( QDialog::exec() ). \n no-throw */
	basar::gui::tie::WidgetReturnEnum	show		();
	bool								shutdown	(); 
	void								hide		(); 

	/*! \brief	Returns the current Iterator of the AccessorPropertyTableRef. \n no-throw */
	basar::db::aspect::AccessorPropertyTable_YIterator getCurrentAccessorIterator();

	//---------------------------------------------------------------------------------------------------------------------//
	// match - methods
	//---------------------------------------------------------------------------------------------------------------------//
	/*! \brief	delegates the passed AccessorPropertytable (branch data) to a sub dialog's member. \n no-throw */
	void							matchToBranch( basar::db::aspect::AccessorPropertyTable_YIterator branch );

	/*! \brief	Narcotic specific data of the AccessorPropertytable will be displayed in the dialog. \n no-throw */
	void							matchToNarcoticArticleData4InitialInventory();

	/*! \brief	Narcotic specific data of the AccessorPropertytable will be displayed in the dialog. \n no-throw */
	void							matchToNarcoticArticleData();

	/*! \brief	All selected narcotic specific data of the AccessorPropertytable will be displayed in the dialog. \n no-throw */
	void							matchToNarcoticBookings();

	/*! \brief	Delegates the passed AccessorPropertyTable_YIterator to the homonymous method of the sub (detailed) dialog. \n no-throw */
	void							matchToNewBooking( basar::db::aspect::AccessorPropertyTable_YIterator currentBooking );

	/*! \brief	Delegates the passed AccessorPropertyTable_YIterator to the homonymous method of the sub (detailed) dialog. \n no-throw */
	void							matchToNarcoticCustomerSupplierName( basar::db::aspect::AccessorPropertyTable_YIterator supplier );

	/*! \brief	Search criteria will set from the dialog to the passed iterator. \n no-throw */
	void							matchFromSelection( basar::db::aspect::AccessorPropertyTable_YIterator searchIt );

	/*! \brief	Assigns dialog values to the passed AccessorPropertyTable_YIterator. 
				Here it will be differentiate if master or detailed dialog is concerned. \n no-throw */
	void							matchFromCurrentBooking( basar::db::aspect::AccessorPropertyTable_YIterator currentBooking );

	void							matchToCurrentBooking( basar::db::aspect::AccessorPropertyTable_YIterator currentBooking );

	/*! \brief	Assigns detailed dialog values to the passed AccessorPropertyTable_YIterator. \n no-throw */
	void							matchFromNarcoticSupplierNo( basar::db::aspect::AccessorPropertyTable_YIterator searchIt );
	
	void							matchToSuppliers	( basar::db::aspect::AccessorPropertyTableRef aptNarcSupp );
	void							matchCurrentBgaNo	( basar::Int32& bgano );
	void							matchToCustomer		( basar::db::aspect::AccessorPropertyTable_YIterator customer );
	void							matchToNarcoticData	( basar::db::aspect::AccessorPropertyTable_YIterator currentBooking );
	void							matchToArticleNo	( basar::I18nString articleNoValue);
	
	/*! \brief	Signalizes if the user has executed the search and the result is displayed. 
				Important to decide if the search result has to be reselected if the sub dialog is closed.
				Because via the sub dialog a dataset can be manipulated. So in the master dialog latest data should be displayed only. 
				\n no-throw */
	bool							isSearchDone();

private slots:
	/*! \brief	Prints narcotic bookings. Indepent upon a executed search. 
				It will take into consideration the given search criteria. \n no-throw. */
	void							onBtnPrintList_clicked();
	/*! \brief	Prints delivery receipt for selected booking 
				\n no-throw. */
	void							onBtnPrintDeliveryReceipt_clicked();
	/*! \brief	Prints dissolve receipt for selected booking (if transactiontype of selected booking = 'z')
				\n no-throw. */
	void							onBtnPrintDissolveReceipt_clicked();
	//! \brief	Opens a new dialog where deleted narcotics bookings can be undone. \n no-throw. */
	void							onBtnRestore_clicked();
	//! \brief	Opens the detailed dialog to create a new narcotic booking. \n no-throw. */
	void							onBtnCreate_clicked();
	//! \brief	Erases the highlighted (in the table widget) narcotic booking. \n no-throw. */
	void							onBtnDelete_clicked();
	//! \brief	Cancels the highlighted (in the table widget) narcotic booking. Removes pdf from bfarmfolder\n no-throw. */
	void							onBtnCancellation_clicked();	
	/*! \brief	Opens the detailed dialog for manipulating the passed, highlighted narcotic booking of the master dialog. 
				\n no-throw. */
	void							onBtnEdit_clicked();
	//! \brief	Activates the edit- and delete button if a narcotic booking is selected in the table widget. \n no-throw. */
	void							onTableWidget_itemSelectionChanged();

	/*! \brief	Regardless of the query result the table widget will be cleared and 
				all internal (special bgano) narcotics booking will be displayed. \n no-throw. */
	void							onBtnSearchInternalAccounts_clicked();

	/*! \brief	Regardless of the query result the table widget will be cleared and 
				all incompleted narcotics booking will be displayed. \n no-throw. */
	void							onBtnSearchIncomplete_clicked();

    /*! \brief	Regardless of the query result the table widget will be cleared and 
				all double booked narcotics bookings will be displayed. \n no-throw. */
    void                            onBtnSearchDoubleBookings_clicked();

	/*! \brief	Depending upon an valid input in the article text field, the search button will be enabled or disabled. 
				\n no-throw. */
	void							onTxtArticleNoSearch_textChanged( const QString & );
	/*! \brief	Depending upon an input in the article text field, the search button will be enabled or disabled. 
				\n no-throw. */
	void							onTxtArticleNameSearch_textChanged( const QString& );
	//! \brief	Query will be started with the search criteria input. \n no-throw. */
	void							onBtnSearch_clicked();
	//! \brief	Closes the dialog. \n no-throw. */
	void							onBtnClose_clicked();

	/*! \brief	*/
	void							onTableWidget_itemPressed( QTableWidgetItem* item );

	/*!  \brief	Switches between search by articlename and search by articleno. \ no-throw */
	void							buttonGroupStateChanged();

private:
	/*!  \brief	Initializes the gui ctrls (comboBox, search criteria text fields, 
				article and narcotic specific frame, the narcotics booking grid an buttons). \ no-throw */
	void							wireEvents();
	void							createLayout();
	void							initGuiCtrls();
	
	//! initializes the table widget in clearing its values and set its headings
	void							initTableWidget();

	bool							isNewProcess();

	void							checkButtonsActivation();
	void 							checkIfDeleteButtonAllowedToBeActive();
	void 							checkIfCancellationButtonAllowedToBeActive();
	void 							checkIfEditButtonAllowedToBeActive();
	void 							checkIfPrintDissolveReceiptButtonAllowedToBeActive();
	void 							checkIfPrintDeliveryReceiptButtonAllowedToBeActive();

	// private members
	basar::gui::tie::MatcherRef		m_matNarcoticArticleBook;	//!< matcher for the article and narcotic specific data
	basar::gui::tie::MatcherRef		m_matNarcoticBookings;		//!< matcher for all selected narcotic bookings
	
	basar::gui::tie::MatcherRef		m_matInitialInventory;		//!< matcher for all selected narcotic bookings

	std::list<basar::VarString>		m_OriginalAttributeNames;	//!< list for original attributes used for table widget headers

	bool							m_DetailVCShown;			//!< saves the state if the detailed dialog is shown at the time.
	enum enumDisplayStatus { 
		NORMAL,													//!< The displayed list is from the search criterien come.
		INCOMPLETE_BOOKINGS,										//!< The displayed list show all incomplete bookings.
		INTERNAL_ACCOUNTS,										//!< The displayed list show all bookings with internal accounts.
		DOUBLE_BOOKINGS
	} ;
	enumDisplayStatus				m_DisplayStatus;			//!< is the actual display status in the dialog

public:
	basar::gui::tie::ViewConnPtr<CreateProcessNarcoticsBookingVC>	m_RecordEditVC;			//!< detailed dialog
	
	basar::gui::tie::ViewConnPtr<CreateProcessNarcoticsBookingVC> getRecordEditVC();

};

} //namespace viewConn
} //namespace narcotics

#endif //GUARD_NARCOTICBOOK_H