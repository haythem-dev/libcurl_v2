#ifndef CUSTOMERMAPPINGVC_H
#define CUSTOMERMAPPINGVC_H
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
#include "ui_customermappingvc.h"	
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
class CustomerMappingVC :	public QDialog, public Ui::CustomerMappingsClass,
							public boost::enable_shared_from_this<CustomerMappingVC>,
							public basar::gui::tie::IViewConnector
{
    Q_OBJECT

public:
    CustomerMappingVC( QWidget *parent = 0 );
    ~CustomerMappingVC();

	void								init		( basar::db::aspect::AccessorPropertyTableRef customerMappings );
	basar::gui::tie::WidgetReturnEnum	show		();
	bool								shutdown	();
	void								hide		(); 
	
	/*! \brief	Assigns the search criteria of the dialog to the corresponding attributes 
				of the passed accessor propertytable yiterator. \n  no-throw */
	void								matchFromSelection( basar::db::aspect::AccessorPropertyTable_YIterator searchIt );
	
	/*! \brief	Write back data of the dialog (guipropertytabel) to the accessorpropertytable. \n  no-throw */
	void								matchToCustomerMappings();

	/*! \brief	Fill field txtBgaNo with given value and disable, as well as clear field customerName. \n  no-throw */
	void								matchToCustomerNo( basar::I18nString customerNoValue );
	
private:
	// methods
	void								wireEvents();
	void								initList();

	/*! \brief	Initialize gui ctrls (set dflt values, set validators, set table widget header labels). \n  no-throw */
	void								initGuiCtrls();

    // members
	/*! \brief	Matcher to dislay data from the accessor propertytable to the gui propertytable and vice versa. \n  no-throw */
	basar::gui::tie::MatcherRef							m_matCustomerMappings;
	std::list<basar::VarString>							m_lstCaptionsCustomerMappings;
	std::list<basar::VarString>							m_OriginalAttributeNamesCustomerMappings;

private slots:
	void												onBtnClose_clicked();
	void												onBtnSearch_clicked();
	void												onBtnEdit_clicked();
	void												onBtnNew_clicked();
	void												onBtnDelete_clicked();
	void 												onTablewidget_itemDoubleClicked( QTableWidgetItem* );
	void												onTablewidget_itemSelectionChanged();
};

} //namespace viewConn
} //namespace narcotics

#endif //CUSTOMERMAPPINGVC_H