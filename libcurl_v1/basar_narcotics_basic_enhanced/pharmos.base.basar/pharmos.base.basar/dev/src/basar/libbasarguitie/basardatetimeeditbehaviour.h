//----------------------------------------------------------------------------
/*! \file
 *  \brief  internal interface BasarDateTimeEditBehaviour
 *  \author Michael Eichenlaub
 *  \date   24.08.2009
 */
//----------------------------------------------------------------------------
#ifndef GUARD_BASARDATETIMEEDITBEHAVIOUR_H
#define GUARD_BASARDATETIMEEDITBEHAVIOUR_H

//---------------------------------------------------------------------------------------//
// includes
//---------------------------------------------------------------------------------------//
#include "datetimeeditbehavour.h"

//---------------------------------------------------------------------------------------//
// forward declaration
//---------------------------------------------------------------------------------------//
class QWidget;
class BasarDateTimeEdit;

//---------------------------------------------------------------------------------------//
// namespaces
//---------------------------------------------------------------------------------------//
namespace basar {
namespace gui	{
namespace tie	{

//---------------------------------------------------------------------------------------//
/*! \brief  implementing behaviour of gui widget basar datetime edit

			final class */
//---------------------------------------------------------------------------------------//
class BasarDateTimeEditBehaviour : public DateTimeEditBehavour
{
public:
	//! c'tor	\n no-throw
									BasarDateTimeEditBehaviour	(PGuiPropTbl                   pPt,	//!< pointer to GuiPropertyTable
																 std::list<QWidget*>::iterator it	//!< iterator to QWidget list
																);
	//! d'tor \n no-throw
	virtual						   ~BasarDateTimeEditBehaviour	();

	//! set datetime value to control's given column	\n no-throw
	virtual void					setDateTime					(const DateTime & rValue,			//!< DateTime value to set to column
																 Int32            currentColumn		//!< column to set datetime value to
																);
	//! returns the value the iterator points to as a DateTime. \n	no-throw
    virtual DateTime				getDateTime					(Int32 currentColumn				//!< column to get datetime value from
																) const;
	//! reset widget and set to invalid value	\n no-throw
	virtual void					clear						();

protected:
	//! getting the basar widget type enum for the Qt object \n no-throw 
	virtual WidgetTypes				getWidgetType				() const;

private:
	
									BasarDateTimeEditBehaviour	(const BasarDateTimeEditBehaviour & );	//!< forbidden copy constructor
	BasarDateTimeEditBehaviour &	operator =					(const BasarDateTimeEditBehaviour & );	//!< forbidden assignment operator

	//! sets the default date
	void							setDefaultDate				();

	BasarDateTimeEdit *				m_pWidget;		//!< pointer to basar widget (Qt derived)
};

//---------------------------------------------------------------------------------------//
} // namespace tie
} // namespace gui
} // namespace basar

//---------------------------------------------------------------------------------------//
#endif // GUARD
