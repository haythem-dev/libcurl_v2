//----------------------------------------------------------------------------
/*! \file
 *  \brief  internal interface BasarDateEditBehaviour
 *  \author Michael Eichenlaub
 *  \date   24.08.2009
 */
//----------------------------------------------------------------------------

//---------------------------------------------------------------------------------------//
// includes
//---------------------------------------------------------------------------------------//
#include "basardateeditbehaviour.h"

#pragma warning (push)
#pragma warning (disable: 4231 4244 4481 4512 4800)
#include "libbasarqtwidget_dateedit.h"
#pragma warning (pop)

//---------------------------------------------------------------------------------------//
// namespaces
//---------------------------------------------------------------------------------------//
namespace basar {
namespace gui	{
namespace tie	{

//---------------------------------------------------------------------------------------//
//!	\throw no-throw
//!
BasarDateEditBehaviour::BasarDateEditBehaviour ( PGuiPropTbl                   pPt,
												 std::list<QWidget*>::iterator it)
											   : DateEditBehavour(pPt, it)
{
    m_pWidget = static_cast < BasarDateEdit * > ( *m_it );
}

//---------------------------------------------------------------------------------------//
//!	\throw no-throw
//!
BasarDateEditBehaviour::~BasarDateEditBehaviour()
{
}

//---------------------------------------------------------------------------------------//
//!	\throw no-throw
//!
void BasarDateEditBehaviour::setDate (const Date & rValue, Int32 )
{
	QDate val;

    if (!rValue.isValid())
		val = m_pWidget->minimumDate();
	else
		val.setDate(rValue.getYear(), rValue.getMonth (), rValue.getDay());

    m_pWidget->setDate(val);	
}

//---------------------------------------------------------------------------------------//
//!	\throw no-throw
//!	\return Date value of current line in current widget
//!
Date BasarDateEditBehaviour::getDate(Int32) const
{
	QDate qVal = m_pWidget->date();
    Date  bVal;

	if (m_pWidget->minimumDate() == qVal)
		bVal.setInvalid();
	else
		bVal.setDate(qVal.year(), qVal.month(), qVal.day());

    return bVal;
}

//---------------------------------------------------------------------------------------//
//!	\throw  no-throw
//!
void BasarDateEditBehaviour::clear()
{
	Date null;
	null.setInvalid();

    setDate(null, 0);
}

//---------------------------------------------------------------------------------------//
//!	\throw  no-throw
//! \return type of given QObject
//!
WidgetTypes	BasarDateEditBehaviour::getWidgetType() const
{
    return basarDateEdit;
}

//---------------------------------------------------------------------------------------//
}	// namespace tie
}	// namespace gui
}	// namespace basar
