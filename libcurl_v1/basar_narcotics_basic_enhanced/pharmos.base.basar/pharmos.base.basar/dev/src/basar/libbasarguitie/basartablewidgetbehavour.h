//----------------------------------------------------------------------------
/*! \file
 *  \brief  internal interface BasarTableWidgetBehavour
 *  \author Roland Kiefert 
 *  \date   08.09.2006
 */
//----------------------------------------------------------------------------
#ifndef GUARD_BASARTABLEWIDGETBEHAVOUR_H
#define GUARD_BASARTABLEWIDGETBEHAVOUR_H

//---------------------------------------------------------------------------------------//
#include "tablewidgetbehavour.h"

//---------------------------------------------------------------------------------------//
// forward declaration
//---------------------------------------------------------------------------------------//
class QWidget;
class BasarTableWidget;

//---------------------------------------------------------------------------------------//
// namespaces
//---------------------------------------------------------------------------------------//
namespace basar {
namespace gui {
namespace tie {

/////////////////////////////////////////////////////////////////////////////////////////
// class BasarTableWidgetBehavour
/////////////////////////////////////////////////////////////////////////////////////////
/*! \brief  class BasarTableWidgetBehavour. Implementing behaviour of gui controls in BasarTableWidget.

			This is a final class.     
			Class throws the follwing exceptions: 
			class NoValidMemoryPolicyException,
			class UnableToReadWriteInLineOfControlException. */
class BasarTableWidgetBehavour : public TableWidgetBehavour
{
public:
	//X-Iterator
	/*! \brief	c'tor with no special action. 
				\n no-throw */
	BasarTableWidgetBehavour(PGuiPropTbl                   pPt, //!< pointer to GuiPropertyTable
							 std::list<QWidget*>::iterator it	//!< iterator to QWidget-list
							);
	/*! \brief	d'tor with no special action. 
				\n no-throw */
	virtual ~BasarTableWidgetBehavour();
	
	 
	//guipropertytable
	/*! \brief	reset the control and set to empty strings. 
				\n GuiControlNotSupportedException */
	virtual void					clear();
	

	/*! \brief  resizing Columns and Rows if specified in BasarTableWidget
		\n	   no-throw */
    virtual void                    checkResize(QTableWidget * tableWidget	//!< pointer to Qt tablewidget
											   );

/*  \brief neccessary as the method setItem is not virtual in  QTableWidget
	*         or in BasarTableWidget
	*		   overwritten in BasarTableWidget
	* @param row Row
	* @param column column
	* @param item item 
		\n	   no-throw */
	virtual void                    setItem(int row, int column, QTableWidgetItem * item);

    virtual void                    notifyMatchDone();

    virtual void registerForNotification() /*override*/;

protected:
	//! \brief getting the basar widget type enum for the Qt object \n no-throw 
	virtual WidgetTypes				getWidgetType		() const;

private:
	/*! \brief	forbidden copy constructor.*/
    BasarTableWidgetBehavour (const BasarTableWidgetBehavour&);
	/*! \brief	forbidden assignment operator.*/
    BasarTableWidgetBehavour& operator=(const BasarTableWidgetBehavour&);
	
	BasarTableWidget*				m_basarTableWidget; //!< pointer to the qt control qTableWidget
};


} // tie
} // gui
} // basar


#endif // GUARD_BASARTABLEWIDGETBEHAVOUR_H

