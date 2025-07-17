#ifndef CREATEPROCESSNARCOTICSBOOKING_H
#define CREATEPROCESSNARCOTICSBOOKING_H
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
#include "ui_createprocessnarcoticsbooking.h"
#pragma warning (pop)

#include <boost/enable_shared_from_this.hpp>
#include <libbasardbaspect.h>
#include <libbasarappl.h>
#include <libbasarguitie.h>

//-----------------------------------------------------------------------------------------------//
// forward declarations
//-----------------------------------------------------------------------------------------------//
namespace basar {
	namespace db {
		namespace aspect {
			class AccessorPropertyTable_Yiterator;
		}
	}
}
class MultiComboBox;

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
class CreateProcessNarcoticsBookingVC : public QDialog, public Ui::CreateProcessNarcoticsBookingClass,
										public boost::enable_shared_from_this<CreateProcessNarcoticsBookingVC>, 
										public basar::gui::tie::IViewConnector
{
    Q_OBJECT

public:
    CreateProcessNarcoticsBookingVC( QWidget *parent = 0 );
	~CreateProcessNarcoticsBookingVC();
	basar::gui::tie::WidgetReturnEnum					show();
	void												hide();
	bool												shutdown();

	void												init();
	void												initGuiCtrls();
	void												setDisabled_grpNarcoticData( bool disable );
	void												setDisabled_grpArticleData( bool disable );
		
	void												matchToSuppliers					( basar::db::aspect::AccessorPropertyTableRef aptNarcSupp );
	void												matchToArticleNarcoticData			( basar::db::aspect::AccessorPropertyTable_YIterator narcoticArticleData );
	void												matchToBookingData					( basar::db::aspect::AccessorPropertyTable_YIterator currentBooking );
	void												matchFromCurrentBooking				( basar::db::aspect::AccessorPropertyTable_YIterator currentBooking );
	void												matchToNewBooking					( basar::db::aspect::AccessorPropertyTable_YIterator m_currentBooking );
	void												matchFromNarcoticCustomerSupplierNo	( basar::db::aspect::AccessorPropertyTable_YIterator searchIt );
	void												matchToNarcoticCustomerSupplierName	( basar::db::aspect::AccessorPropertyTable_YIterator supplier );
	void												matchCurrentBgaNo					( basar::Int32& bgano );
	void												matchToBranch						( basar::db::aspect::AccessorPropertyTable_YIterator branch );	
	void												matchToCustomer						( basar::db::aspect::AccessorPropertyTable_YIterator customer );
	void												matchToNarcoticData					( basar::db::aspect::AccessorPropertyTable_YIterator currentBooking );
//	as the Gui field packageunit is matched twice with the values of two different database fields having been both renamed
//	to packageunit, this mistake has to be corrected here by creating a new matchMethod in order to rematch the field with 
//	the correct value manually afterwards
	void												matchToPackageunit					( basar::db::aspect::AccessorPropertyTable_YIterator narcoticDataIt );

	bool												m_DatasetSaved; 

private:
	// methods
	void												wireEvents();
	void												chooseLineCodeDescription( const basar::VarString& LINE_CODE );
	void												loadCombo();
	basar::Int32										getStartInternalBookingsNo();
	MultiComboBox*										m_cboLC;
	bool												m_LoadedCompleted;
	bool												m_LoadedCboLineCode;
	basar::db::aspect::AccessorPropertyTable_YIterator	getBranch();
	void												toStream( std::ostringstream& oss );
	bool												savingAllowed();

private slots:
	void												loadCustomer();
	void												onCustomersupplierno_textChanged( const QString& );
	void												currentIndexChangedOfLineCode( int );
	void												currentIndexChangedOfCustomerSupplier( int );
	void												onBtnClose_clicked();
	void												onBtnSave_clicked();
	void												onCustomersupplierno_editingFinished();
	void												onCustomersuppliername_textChanged( const QString& );
    void												onM_cboLC2_currentIndexChanged( int index );
	void												onQtytransaction_textChanged( const QString& );
	void												onPurchaseorderno_textChanged( const QString& );
	void												onTransactiondate_dateChanged(const QDate&);
	void												onTransactiontime_timeChanged(const QTime&);

private:
	bool												m_bBranchLoaded;
	bool												m_bCboLoaded;
	bool												m_bInit;
	basar::db::aspect::AccessorPropertyTable_YIterator	m_Branch;
	basar::db::aspect::AccessorPropertyTableRef			m_Suppliers;
	basar::gui::tie::MatcherRef							m_matSupplier;
	std::list<basar::VarString>							m_OriginalAttributeNames;
	// member used in order to state if matching has already been done
	bool												m_MatchingDone;
};

} //namespace viewConn
} //namespace narcotics

#endif //CREATEPROCESSNARCOTICSBOOKING_H