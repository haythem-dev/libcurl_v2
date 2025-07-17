//----------------------------------------------------------------------------
/*! \file
 *  \brief  internal interface GroupBoxBehavour
 *  \author Michael Eichenlaub
 *  \date   04.05.2010
 */
//----------------------------------------------------------------------------
#ifndef GUARD_GROUPBOXBEHAVOUR_H
#define GUARD_GROUPBOXBEHAVOUR_H

//----------------------------------------------------------------------------
#include "controlbehavour.h"

//---------------------------------------------------------------------------------------//
class QWidget;
class QGroupBox;

//---------------------------------------------------------------------------------------//
namespace basar {
namespace gui   {
namespace tie   {

//---------------------------------------------------------------------------------------//
/*! \brief  behaviour of checkable GroupBox: un-/checks GroupBox */
class GroupBoxBehavour : public ControlBehavour
{
public:
	/*! \brief	c'tor \n no-throw */
							GroupBoxBehavour	(PGuiPropTbl                   pPt,	//!< pointer to GuiPropertyTable
												 std::list<QWidget*>::iterator it	//!< iterator to widget list
												);
	/*! \brief	d'tor \n no-throw */
	virtual				   ~GroupBoxBehavour	();

	/*! \brief	set mapped widget specific value \n no-throw */
	virtual void			setString			(const I18nString & rValue,			//!< value for displaying
												 Int32              currentRow,		//!< row to set string to
												 Int32              currentColumn	//!< column to set string to
												);
	/*! \brief	return mapped widget value \n no-throw */
	virtual I18nString		getString			(Int32 currentRow,		//!< row to get string from
												 Int32 currentColumn	//!< column to get string from
												) const;

	/*! \brief	reset widget and set to default state \n no-throw */
	virtual void			clear				();

	/*! \brief	empty implementation \n no-throw */
	virtual void			erase				(Int32);

	/*! \brief	iterate over all widgets to find out widget with most hits (shown items) in it. 
				\n no-throw */
	virtual Int32			maxCountOfControls	() const;

protected:
	//! getting basar widget type enum for Qt object \n no-throw 
	virtual WidgetTypes		getWidgetType		() const;

private:
							GroupBoxBehavour	(const GroupBoxBehavour &);	//!< forbidden
	GroupBoxBehavour &		operator =			(const GroupBoxBehavour &);	//!< forbidden

	QGroupBox *				m_pWidget;		//!< Qt widget GroupBox
};

//---------------------------------------------------------------------------------------//
} // tie
} // gui
} // basar

//---------------------------------------------------------------------------------------//
#endif	// GUARD
