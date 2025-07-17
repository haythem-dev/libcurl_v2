//----------------------------------------------------------------------------
/*! \file
 *  \brief  internal interface BasarDateEditBehaviour
 *  \author Michael Eichenlaub
 *  \date   24.08.2009
 */
//----------------------------------------------------------------------------
#ifndef GUARD_BASARDATEEDITBEHAVIOUR_H
#define GUARD_BASARDATEEDITBEHAVIOUR_H

//---------------------------------------------------------------------------------------//
// includes
//---------------------------------------------------------------------------------------//
#include "dateeditbehavour.h"

//---------------------------------------------------------------------------------------//
// forward declaration
//---------------------------------------------------------------------------------------//
class QWidget;
class BasarDateEdit;


//---------------------------------------------------------------------------------------//
// namespaces
//---------------------------------------------------------------------------------------//
namespace basar {
namespace gui	{
namespace tie	{

//---------------------------------------------------------------------------------------//
/*! \brief  implementing behaviour of gui widget basar date edit

			final class */
//---------------------------------------------------------------------------------------//
class BasarDateEditBehaviour : public DateEditBehavour
{
public:
	//! c'tor	\n no-throw
								BasarDateEditBehaviour	(PGuiPropTbl                   pPt,	//!< pointer to GuiPropertyTable
														 std::list<QWidget*>::iterator it	//!< iterator to QWidget list
														);
	//! d'tor \n no-throw
	virtual					   ~BasarDateEditBehaviour	();

	//! set date value to control's given column	\n no-throw
	virtual void				setDate					(const Date & rValue,				//!< Date value to set to column
														 Int32        currentColumn			//!< column to set date value to
														);
	//! returns the value the iterator points to as a Date. \n	no-throw
    virtual Date				getDate					(Int32 currentColumn				//!< column to get date value from
														) const;

	//! reset widget and set to invalid value	\n no-throw
	virtual void				clear					();

protected:
	//! getting the basar widget type enum for the Qt object \n no-throw 
	virtual WidgetTypes			getWidgetType			() const;

private:
	
								BasarDateEditBehaviour	(const BasarDateEditBehaviour & );	//!< forbidden copy constructor
	BasarDateEditBehaviour &	operator =				(const BasarDateEditBehaviour & );	//!< forbidden assignment operator

	BasarDateEdit *				m_pWidget;		//!< pointer to basar widget (Qt derived)
};

//---------------------------------------------------------------------------------------//
} // namespace tie
} // namespace gui
} // namespace basar

//---------------------------------------------------------------------------------------//
#endif // GUARD
