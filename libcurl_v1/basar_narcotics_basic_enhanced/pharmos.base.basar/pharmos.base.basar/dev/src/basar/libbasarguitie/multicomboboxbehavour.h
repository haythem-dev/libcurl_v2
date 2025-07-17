//----------------------------------------------------------------------------
/*! \file
 *  \brief  internal interface MultiComboBoxBehavour
 *  \author Roland Kiefert 
 *  \date   08.09.2006
 */
//----------------------------------------------------------------------------
#ifndef GUARD_MULTICOMBOBOXBEHAVOUR_H
#define GUARD_MULTICOMBOBOXBEHAVOUR_H

//----------------------------------------------------------------------------
#include "controlbehavour.h"

//---------------------------------------------------------------------------------------//
// forward declaration
//---------------------------------------------------------------------------------------//
class QWidget;
class MultiComboBox;

//---------------------------------------------------------------------------------------//
// namespaces
//---------------------------------------------------------------------------------------//
namespace basar {
namespace gui {
namespace tie {

/////////////////////////////////////////////////////////////////////////////////////////
// class MultiComboBoxBehavour
/////////////////////////////////////////////////////////////////////////////////////////
/*! \brief  class MultiComboBoxBehavour. Implementing behaviour of gui controls in multi-comboboxes.

			This is a final class.     
			Class throws the follwing exceptions: 
			class UnableToReadWriteInLineOfControlException*/
class MultiComboBoxBehavour : public ControlBehavour
{
public:
	//X-Iterator
	/*! \brief	c'tor with no special action. 
				\n no-throw */
	MultiComboBoxBehavour(PGuiPropTbl                   pPt,	//!< pointer to GuiPropertyTable
						  std::list<QWidget*>::iterator it		//!< iterator to QWidget-list
						 );
	/*! \brief	d'tor with no special action. 
				\n no-throw */
	virtual ~MultiComboBoxBehavour();
	/*! \brief	switch forward the iterator (preincrement) 
				\n no-throw */
	virtual bool					isSwitchNextAllowed(Int32 & rCurrentColumn);
	/*! \brief	set string to control's given row + column. 
				\n no-throw */
	virtual void					setString	( const I18nString& rValue, Int32 currentRow, Int32 currentColumn );
	/*! \brief	returns the value the iterator points to as a STRING. 
				\n UnableToReadWriteInLineOfControlException */
	virtual I18nString				getString(Int32 currentRow, Int32 currentColumn) const;
    /*! \brief	returns the position of the iterator. 
				\n no-throw */
	virtual ColumnSize				getIndex(Int32 currentColumn) const;

	//guipropertytable
	/*! \brief	reset the control and set to empty strings. 
				\n no-throw */
	virtual void					clear();
	/*! \brief	remove the row of a multi line ctrl yit is pointing to. 
				If it is the last one the complete mask is cleared. 
				\n no-throw */
	virtual void					erase( Int32 row	//!< row to erase
									);
	/*! \brief	iterate over all ctrls to find out the ctrl 
				with the most hits (shown items) in it. 
				\n no-throw */
	virtual Int32					maxCountOfControls() const;
	/*! \brief	get the absolute index of the current row or item of the gui property table. 
				\n no-throw */
	virtual Int32					getCurrentIndex();

protected:
	//! \brief	getting the basar widget type enum for the Qt object \n no-throw 
	virtual WidgetTypes				getWidgetType		() const;

private:
	/*! \brief	forbidden copy constructor.*/
    MultiComboBoxBehavour (const MultiComboBoxBehavour&);
	/*! \brief	forbidden assignment operator.*/
    MultiComboBoxBehavour& operator=(const MultiComboBoxBehavour&);
	
	MultiComboBox*					m_MultiComboBoxWidget; //!< pointer to the qt control MultiComboBox
};


} // tie
} // gui
} // basar


#endif // GUARD_MULTICOMBOBOXBEHAVOUR_H

