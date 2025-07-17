//----------------------------------------------------------------------------
/*! \file
 *  \brief  internal interface DateEditBehavour
 *  \author Roland Kiefert 
 *  \date   08.09.2006
 */
//----------------------------------------------------------------------------
#ifndef GUARD_DATEEDITBEHAVOUR_H
#define GUARD_DATEEDITBEHAVOUR_H

//----------------------------------------------------------------------------
#include "controlbehavour.h"

//---------------------------------------------------------------------------------------//
// forward declaration
//---------------------------------------------------------------------------------------//
class QWidget;
class QDateEdit;

//---------------------------------------------------------------------------------------//
// namespaces
//---------------------------------------------------------------------------------------//
namespace basar {
namespace gui {
namespace tie {

/////////////////////////////////////////////////////////////////////////////////////////
// class DateEditBehavour
/////////////////////////////////////////////////////////////////////////////////////////
/*! \brief  class DateEditBehavour. Implementing behaviour of gui controls in date edits.

			This is a final class.     
			Class throws no exceptions. */
class DateEditBehavour : public ControlBehavour
{
public:
	/*! \brief	c'tor with no special action. 
				\n no-throw */
	DateEditBehavour(PGuiPropTbl                   pPt,	//!< pointer to GuiPropertyTable
					 std::list<QWidget*>::iterator it	//!< iterator to QWidget-list
				   );
	/*! \brief	d'tor with no special action. 
				\n no-throw */
	virtual ~DateEditBehavour();
	/*! \brief	set datetime-value to control's given column. 
				\n	no-throw */
	virtual void					setDate( const Date& rValue, //!< Date-value to set to column
	                                               Int32 currentColumn //!< column to set datetime value to
												);
	/*! \brief	returns the value the iterator points to as a DateTime. 
				\n no-throw */
    virtual Date					getDate(Int32 currentColumn	//!< column to get datetime value from
									) const;

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

protected:
	//! \brief getting the basar widget type enum for the Qt object \n no-throw 
	virtual WidgetTypes				getWidgetType		() const;

private:
	/*! \brief	forbidden copy constructor.*/
    DateEditBehavour (const DateEditBehavour&);
	/*! \brief	forbidden assignment operator.*/
    DateEditBehavour& operator=(const DateEditBehavour&);
	
	QDateEdit*						m_DateEditWidget; //!< pointer to the qt control qDateEdit
};


} // tie
} // gui
} // basar


#endif // GUARD_DATEEDITBEHAVOUR_H

