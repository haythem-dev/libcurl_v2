#ifndef NA18DELIVERYNOTECOPYVC_H
#define NA18DELIVERYNOTECOPYVC_H
//-----------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  used to enter rectification note (dt.: Berichtigungsvermerk)
 *  \author Benjamin Endlicher
 *  \date   15.09.2010
 */
//-----------------------------------------------------------------------------------------------//

//-----------------------------------------------------------------------------------------------//
// includes
//-----------------------------------------------------------------------------------------------//
#pragma warning (push)
#pragma warning(disable : 4127 4311 4312 4481 4511 4512 4800 4244)
#include <QtWidgets/qdialog.h>
#include "ui_na18deliverynotecopyvc.h"
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

//-------------------------------------------------------------------------------------------------//
// class declaration
//-------------------------------------------------------------------------------------------------//
/*! \brief  class NA18DeliveryNoteCopyVC for entering rectification notes.

    This is a final class.     
 */
class NA18DeliveryNoteCopyVC : public QDialog, public Ui::NA18DeliveryNoteCopyVCClass,
								public boost::enable_shared_from_this<NA18DeliveryNoteCopyVC>, 
								public basar::gui::tie::IViewConnector
{
    Q_OBJECT

public:
	/*! \brief	D'flt c'tor with no (special) action. \n  no-throw */
    NA18DeliveryNoteCopyVC( QWidget *parent = 0 );
	
	/*! \brief	D'flt d'tor with no (special) action. \n  no-throw */
    ~NA18DeliveryNoteCopyVC();

	/*! \brief	Invokes the init() (without any parameters) implicitly, creates the matcher and 
				transforms the original attribute names to the grid attribute names. \n  no-throw */
	void								init();
	
	/*! \brief	D'flt c'tor with no (special) action. \n  no-throw */
	basar::gui::tie::WidgetReturnEnum	show();
	
	/*! \brief	Resets the matcher, calls the init()(without parameters) and closes the dialog. \n  no-throw */
	bool								shutdown();
	void								hide(); 
	void								matchToRectificationNote(basar::db::aspect::AccessorPropertyTable_YIterator yitNote);
	void								matchToContributionVoucherPos(basar::db::aspect::AccessorPropertyTableRef tabPos, int index = -1);
	void								matchFromContributionVoucherHead(basar::db::aspect::AccessorPropertyTable_YIterator yitHead);
	void								matchFromContributionVoucherPos(basar::db::aspect::AccessorPropertyTableRef tabPos);

private slots:
	void								onBtnCancel_clicked();
	void								onBtnOk_clicked();
	void								textEdit_textChanged();
	void								onReason_toggled(bool);
	void								onPositions_QuantityChanged(int, int);

private:
	void								wireEvents();
	void								createLayout();
	void								setQuantityColumnEditable(bool editable);
	void								resetChangedQuantities();

	static const basar::Int16			COLUMN_INDEX_SELECTION			= 0;
	static const basar::Int16			COLUMN_INDEX_ARTCLENO			= 1;
	static const basar::Int16			COLUMN_INDEX_QUANTITY			= 2;
	static const basar::Int16			COLUMN_INDEX_ORIGINAL_QUANTITY	= 3;
};

} //namespace viewConn
} //namespace narcotics

#endif // NA18DELIVERYNOTECOPYVC_H