//----------------------------------------------------------------------------
/*! \file
 *  \brief  internal interface TimeEditBehavour
 *  \author Roland Kiefert 
 *  \date   08.09.2006
 */
//----------------------------------------------------------------------------
#ifndef GUARD_TIMEEDITBEHAVOUR_H
#define GUARD_TIMEEDITBEHAVOUR_H

//----------------------------------------------------------------------------
#include "controlbehavour.h"

//---------------------------------------------------------------------------------------//
// forward declaration
//---------------------------------------------------------------------------------------//
class QWidget;
class QTimeEdit;

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
// class TimeEditBehavour
/////////////////////////////////////////////////////////////////////////////////////////
/*! \brief  class Time…ditBehavour. Implementing behaviour of gui controls in time edits.

			This is a final class.     
			Class throws no exceptions. */
//---------------------------------------------------------------------------------------//
// implementation
//---------------------------------------------------------------------------------------//
class TimeEditBehavour : public ControlBehavour
{
public:
	//X-Iterator
	/*! \brief	c'tor with no special action. 
				\n no-throw */
	TimeEditBehavour(PGuiPropTbl                   pPt,		//!< pointer to GuiPropertyTable
					 std::list<QWidget*>::iterator it		//!< iterator to QWidget-list
					);
	/*! \brief	d'tor with no special action. 
				\n no-throw */
	virtual ~TimeEditBehavour();
	/*! \brief	set datetime to control's given row + column. 
				\n		 no-throw */
	virtual void					setTime( const Time& rValue, //!< DateTime-value to set to column
	                                         Int32 currentColumn //!< column to set datetime value to
												);
	/*! \brief	returns the value the iterator points to as a DateTime. 
				\n no-throw */
    virtual Time					getTime(Int32 currentColumnc //!< column to get datetime value from
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
    TimeEditBehavour (const TimeEditBehavour&);
	/*! \brief	forbidden assignment operator.*/
	TimeEditBehavour& operator=(const TimeEditBehavour&);
	
	QTimeEdit*						m_TimeEditWidget; //!< pointer to the qt control qTimeEdit
};


} // tie
} // gui
} // basar


#endif // GUARD_TIMEEDITBEHAVOUR_H

