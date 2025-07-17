//----------------------------------------------------------------------------
/*! \file
 *  \brief  internal interface BasarTimeEditBehaviour
 *  \author Michael Eichenlaub
 *  \date   24.08.2009
 */
//----------------------------------------------------------------------------
#ifndef GUARD_BASARTIMEEDITBEHAVIOUR_H
#define GUARD_BASARTIMEEDITBEHAVIOUR_H

//---------------------------------------------------------------------------------------//
// includes
//---------------------------------------------------------------------------------------//
#include "timeeditbehavour.h"

//---------------------------------------------------------------------------------------//
// forward declaration
//---------------------------------------------------------------------------------------//
class QWidget;
class BasarTimeEdit;


//---------------------------------------------------------------------------------------//
// namespaces
//---------------------------------------------------------------------------------------//
namespace basar {
namespace gui	{
namespace tie	{

//---------------------------------------------------------------------------------------//
/*! \brief  implementing behaviour of gui widget basar time edit

			final class */
//---------------------------------------------------------------------------------------//
class BasarTimeEditBehaviour : public TimeEditBehavour
{
public:
	//! c'tor	\n no-throw
								BasarTimeEditBehaviour	(PGuiPropTbl                   pPt,	//!< pointer to GuiPropertyTable
														 std::list<QWidget*>::iterator it	//!< iterator to QWidget list
														);
	//! d'tor \n no-throw
	virtual					   ~BasarTimeEditBehaviour	();

	//! set time value to control's given column	\n no-throw
	virtual void				setTime					(const Time & rValue,				//!< Time value to set to column
														 Int32        currentColumn			//!< column to set time value to
														);
	//! returns the value the iterator points to as a Time. \n	no-throw
    virtual Time				getTime					(Int32 currentColumn				//!< column to get time value from
														) const;

	//! reset widget and set to invalid value	\n no-throw
	virtual void				clear					();

protected:
	//! getting the basar widget type enum for the Qt object \n no-throw 
	virtual WidgetTypes			getWidgetType			() const;

private:
	
								BasarTimeEditBehaviour	(const BasarTimeEditBehaviour & );	//!< forbidden copy constructor
	BasarTimeEditBehaviour &	operator =				(const BasarTimeEditBehaviour & );	//!< forbidden assignment operator

	BasarTimeEdit *				m_pWidget;		//!< pointer to basar widget (Qt derived)
};

//---------------------------------------------------------------------------------------//
} // namespace tie
} // namespace gui
} // namespace basar

//---------------------------------------------------------------------------------------//
#endif // GUARD
