//----------------------------------------------------------------------------
/*! \file
 *  \brief  internal interface ComboBoxBehavour
 *  \author Roland Kiefert 
 *  \date   08.09.2006
 */
//----------------------------------------------------------------------------
#ifndef GUARD_COMBOBOXBEHAVOUR_H
#define GUARD_COMBOBOXBEHAVOUR_H

//----------------------------------------------------------------------------
#include "controlbehavour.h"

//---------------------------------------------------------------------------------------//
// forward declaration
//---------------------------------------------------------------------------------------//
class QWidget;
class QComboBox;

//---------------------------------------------------------------------------------------//
// namespaces
//---------------------------------------------------------------------------------------//
namespace basar {
namespace gui {
namespace tie {

/////////////////////////////////////////////////////////////////////////////////////////
// class ComboBoxBehavour
/////////////////////////////////////////////////////////////////////////////////////////
/*! \brief  class ComboBoxBehavour. Implementing behaviour combo boxes.

			This is a final class.     
			Class throws the follwing exceptions: 
			class UnableToReadWriteInLineOfControlException. */
class ComboBoxBehavour : public ControlBehavour
{
public:
	/*! \brief	c'tor \n no-throw */
						ComboBoxBehavour(PGuiPropTbl                   pPt,	//!< pointer to GuiPropertyTable
										 std::list<QWidget*>::iterator it	//!< iterator to widget list
										);
	/*! \brief	d'tor \n no-throw */
	virtual			   ~ComboBoxBehavour();

	/*! \brief	set string to widget's given row + column. 
				\n UnableToReadWriteInLineOfControlException */
	virtual void		setString		(const I18nString & rValue, 
										 Int32              currentRow, 
										 Int32              currentColumn 
										);
	/*! \brief	set string to widget's current row and given column.
				\n no-throw */
	virtual void		setCurrentString(const I18nString & rValue,			//!< string value to set to column
										 Int32              currentColumn	//!< column to set current row's string to
										);
	/*! \brief	returns the value the iterator points to as a STRING. 
				\n throw UnableToReadWriteInLineOfControlException */
	virtual I18nString	getString		(Int32 currentRow, 		//!< row to get string value from
										 Int32 currentColumn	//!< column to get string value from
										) const;
	/*! \brief	returns the value the iterator points to as a STRING. 
				\n no-throw */
	virtual I18nString	getCurrentString(Int32 currentColumn	//!< column to get current row's string from
										) const;

	/*! \brief	reset the widget and set to empty strings. 
				\n no-throw */
	virtual void		clear			();
	/*! \brief	remove the row of a multi line ctrl yit is pointing to. 
				If it is the last one the complete mask is cleared. 
				\n no-throw */
	virtual void		erase			(Int32 row	//!< row to erase
										);
	/*! \brief	iterate over all ctrls to find out the ctrl 
				with the most hits (shown items) in it. 
				\n no-throw */
	virtual Int32		maxCountOfControls() const;
	/*! \brief	get the absolute index of the current row or item of the gui property table. 
				\n no-throw */
	virtual Int32		getCurrentIndex	();

protected:
	//! \brief getting the basar widget type enum for the Qt object \n no-throw 
	virtual WidgetTypes	getWidgetType	() const;

private:
						ComboBoxBehavour(const ComboBoxBehavour &);	//!< forbidden copy constructor
    ComboBoxBehavour &	operator =		(const ComboBoxBehavour &);	//!< forbidden assignment operator

	QComboBox *	m_ComboBoxWidget;	//!< widget that behaviour is defined
};


} // tie
} // gui
} // basar


#endif // GUARD_COMBOBOXBEHAVOUR_H

