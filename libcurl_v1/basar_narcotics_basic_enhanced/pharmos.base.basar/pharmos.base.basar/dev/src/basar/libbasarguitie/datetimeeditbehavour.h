//----------------------------------------------------------------------------
/*! \file
 *  \brief  internal interface DateTimeEditBehavour
 *  \author Roland Kiefert 
 *  \date   08.09.2006
 */
//----------------------------------------------------------------------------
#ifndef GUARD_DATETIMEEDITBEHAVOUR_H
#define GUARD_DATETIMEEDITBEHAVOUR_H

//----------------------------------------------------------------------------
#include "controlbehavour.h"

//---------------------------------------------------------------------------------------//
// forward declaration
//---------------------------------------------------------------------------------------//
class QWidget;
class QDateTimeEdit;

//---------------------------------------------------------------------------------------//
// namespaces
//---------------------------------------------------------------------------------------//
namespace basar {
namespace gui {
namespace tie {

//---------------------------------------------------------------------------------------//
// defines
//---------------------------------------------------------------------------------------//

/////////////////////////////////////////////////////////////////////////////////////////
// class DateTimeEditBehavour
/////////////////////////////////////////////////////////////////////////////////////////
/*! \brief  class DateTimeEditBehavour. Implementing behaviour of gui controls in datetime edits.

			This is a final class.     
			Class throws no exceptions. */
//---------------------------------------------------------------------------------------//
// implementation
//---------------------------------------------------------------------------------------//
class DateTimeEditBehavour : public ControlBehavour
{
public:
	//X-Iterator
	/*! \brief	c'tor with no special action. 
				\n no-throw */
	DateTimeEditBehavour(PGuiPropTbl                   pPt,	//!< pointer to GuiPropertyTable
					     std::list<QWidget*>::iterator it	//!< iterator to QWidget-list
				        );
	/*! \brief	d'tor with no special action. 
				\n no-throw */
	virtual ~DateTimeEditBehavour();
	/*! \brief	set datetime-value to control's given column. 
				\n		no-throw */
	virtual void					setDateTime( const DateTime& rValue, //!< DateTime-value to set to column
	                                                   Int32 currentColumn //!< column to set datetime value to
												);
	/*! \brief	returns the value the iterator points to as a DateTime. 
				\n		no-throw */
    virtual DateTime				getDateTime(Int32 currentColumn //!< column to get datetime value from
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
    DateTimeEditBehavour (const DateTimeEditBehavour&);
	/*! \brief	forbidden assignment operator.*/
    DateTimeEditBehavour& operator=(const DateTimeEditBehavour&);

	QDateTimeEdit*					m_DateTimeEditWidget; //!< pointer to the qt control qDateTimeEdit
};


} // tie
} // gui
} // basar


#endif // GUARD_DATETIMEEDITBEHAVOUR_H

