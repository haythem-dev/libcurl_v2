#ifndef CUSTOMERMAPPINGDETAILSVC_H
#define CUSTOMERMAPPINGDETAILSVC_H
//-----------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  
 *  \author Thomas Hörath
 *  \date   15.03.2010
 */
//-----------------------------------------------------------------------------------------------//

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
#include "ui_customermappingdetailsvc.h"
#pragma warning (pop)

//-------------------------------------------------------------------------------------------------//
// namespace
//-------------------------------------------------------------------------------------------------//
namespace narcotics 
{
namespace viewConn 
{
	
class CustomerMappingDetailsVC : public QDialog, public Ui::CustomerMappingDetailsClass,
						public boost::enable_shared_from_this<CustomerMappingDetailsVC>,
						public basar::gui::tie::IViewConnector
{
	Q_OBJECT

public:
	CustomerMappingDetailsVC(QWidget *parent = 0);
	~CustomerMappingDetailsVC();
												
	void			init						( basar::db::aspect::AccessorPropertyTableRef customerData, const bool isCreation ); 

	basar::gui::tie::WidgetReturnEnum	show		();
	bool								shutdown	(); 
	void								hide		(); 

	//---------------------------------------------------------------------------------------------------------------------//
	// match - methods
	//---------------------------------------------------------------------------------------------------------------------//
	/*! \brief	Enter article and narcotic data into detailed dialog. \n no-throw */
	void							matchToCustomerData( basar::db::aspect::AccessorPropertyTable_YIterator yit );
	
	/*! \brief	Narcotic specific order data of the dialog will be matched into appropriate AccessorPropertytable. \n no-throw */
	void							matchFromCustomerData( basar::db::aspect::AccessorPropertyTable_YIterator yit );

	void							matchFromCustExtern (basar::db::aspect::AccessorPropertyTable_YIterator searchIt);
	void							matchFromCustIntern (basar::db::aspect::AccessorPropertyTable_YIterator searchIt);

	
private slots:
	/*! \brief	receives changes on customersupplierno and therefore adapts appropriate 
				\n customersupplierlocation as well as customersuppliername and enables button SaveOrderChanges. 
				\n no-throw. */
	void onCustomernoExtern_editingFinished();
	void onCustomernoIntern_editingFinished();

	void onBtnSwitch_clicked();
	
	void onBtnClose_clicked();

	void onBtnSave_clicked();

	
private:
	void wireEvents();
	
	/*!  \brief	Initializes the gui ctrls (comboBox, search criteria text fields, 
				article and narcotic specific frame, the narcotics booking grid an buttons). \ no-throw */
	void initGuiCtrls(  );

	void switchAdress();

	
	// private members
	basar::gui::tie::MatcherRef		m_matCustomer;
	bool							m_isCreation;
	bool							m_SwitchAdress;
};

} //namespace viewConn 
} //namespace narcotics 

#endif // CUSTOMERMAPPINGDETAILSVC_H