#ifndef TRANSACTIONDATA_H
#define TRANSACTIONDATA_H
//-----------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  
 *  \author Bjoern Bischof
 *  \date   27.04.2006
 */
//-----------------------------------------------------------------------------------------------//

//-----------------------------------------------------------------------------------------------//
// includes
//-----------------------------------------------------------------------------------------------//
#pragma warning (push)
#pragma warning(disable : 4127 4311 4312 4481 4512 4800 4244)
#include <QtWidgets/qdialog.h>
#include "ui_transactiondatavc.h"	
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

//-------------------------------------------------------------------------------------------------//
// class declaration
//-------------------------------------------------------------------------------------------------//
/*! \brief  class TransactionDataVC displays the transaction data (Deu: Bewegungsdaten).

    This is a final class.     
 */
class TransactionDataVC :	public QDialog, public Ui::TransactionDataClass,
							public boost::enable_shared_from_this<TransactionDataVC>,
							public basar::gui::tie::IViewConnector
{
    Q_OBJECT

public:
    TransactionDataVC( QWidget *parent = 0 );
    ~TransactionDataVC();

	void								init		( basar::db::aspect::AccessorPropertyTableRef narcoticsBookings );
	basar::gui::tie::WidgetReturnEnum	show		();
	bool								shutdown	();
	void								hide		(); 
	
	/*! \brief	Assigns the search criteria of the dialog to the corresponding attributes 
				of the passed accessor propertytable yiterator. \n  no-throw */
	void								matchFromSelection( basar::db::aspect::AccessorPropertyTable_YIterator searchIt );
	
	/*! \brief	Write back data of the dialog (guipropertytabel) to the accessorpropertytable. \n  no-throw */
	void								matchToNarcoticBookings();

	/*! \brief	Fill field articleNo with given value and disable, as well as clear field articleName. \n  no-throw */
	void								matchToArticleNo( basar::db::aspect::AccessorPropertyTable_YIterator yit );

	/*! \brief	Fill field txtBgaNo with given value and disable, as well as clear field customerName. \n  no-throw */
	void								matchToCustomerNo( basar::I18nString customerNoValue );
	
	/*! \brief	Fill field txtSupplierNo with given value and disable, as well as clear field supplierName. \n  no-throw */
	void								matchToSupplierNo( basar::I18nString supplierNoValue );

private:
	// methods
	void								wireEvents();
	void								initList();

	/*! \brief	Initialize gui ctrls (set dflt values, set validators, set table widget header labels). \n  no-throw */
	void								initGuiCtrls();

    /*! \brief	Checks if the search may be started (supplier no. respectively customer no, article no or date have to be set). \n  no-throw */
	bool								isSearchAllowed();
	
    /*! \brief	Checks if the print may be started (supplier no. respectively customer no or article no have to be set). \n  no-throw */
	bool								isPrintAllowed();

	/*! \brief	Enables or disables the specific frame including its controls. \n  no-throw */
	void								setDisabled_frameSupplierSearch( bool disable );

	/*! \brief	Enables or disables the specific frame including its controls. \n  no-throw */
	void								setDisabled_frameCustomerSearch( bool disable );

	/*! \brief	Enables or disables the specific frame including its controls. \n  no-throw */
	void								setDisabled_frameArticleSearch( bool disable );

	// members
	/*! \brief	Matcher to dislay data from the accessor propertytable to the gui propertytable and vice versa. \n  no-throw */
	basar::gui::tie::MatcherRef							m_matNarcoticBookings;
	std::list<basar::VarString>							m_lstCaptionsBgaNo;
	std::list<basar::VarString>							m_OriginalAttributeNamesBgaNo;
	std::list<basar::VarString>							m_lstCaptionsPZN;
	std::list<basar::VarString>							m_OriginalAttributeNamesPZN;
	basar::db::aspect::AccessorPropertyTableRef			m_AccessorPropertyTableRef;

private slots:
	void												activateSearchForSupplierName();
	void												activateSearchForCustomerName();
	void												activateSearchForArticleName();
	void												combineSearchCriteria();

	void												onBtnClose_clicked();
	void												onChkWithDate_stateChanged( int );
	void												onTxtArticleCode_textChanged( const QString & );
	void												onTxtArticleName_textChanged( const QString& );
	void												onTxtBgaNo_textChanged( const QString & );
	void												onTxtCustomerName_textChanged( const QString& );
	void												onTxtSupplierNo_textChanged( const QString& );
	void												onTxtSupplierName_textChanged( const QString& );
	void												onBtnSearch_clicked();
	void												onBtnPrint_clicked();
};

} //namespace viewConn
} //namespace narcotics

#endif //TRANSACTIONDATA_H