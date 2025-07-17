//----------------------------------------------------------------------------
/*! \file
 *  \brief  internal interface GroupBoxBehavour
 *  \author Michael Eichenlaub
 *  \date   04.05.2010
 */
//----------------------------------------------------------------------------

#include "groupboxbehavour.h"

#include "libbasarguitie_transformer.h"

#pragma warning (push)
#pragma warning (disable: 4231 4244 4800)
#include <QtWidgets/QGroupBox>
#pragma warning (pop)

//---------------------------------------------------------------------------------------//
namespace basar {
namespace gui   {
namespace tie   {

//---------------------------------------------------------------------------------------//
/*!	\throw no-throw */
GroupBoxBehavour::GroupBoxBehavour (PGuiPropTbl                   pPt,
								    std::list<QWidget*>::iterator it )
								   : ControlBehavour(pPt, it)
{
	m_pWidget = static_cast<QGroupBox *>( (*m_it) );
}

//---------------------------------------------------------------------------------------//
/*!	\throw no-throw */
GroupBoxBehavour::~GroupBoxBehavour()
{
}

//---------------------------------------------------------------------------------------//
/*!	\throw no-throw */
void GroupBoxBehavour::setString( const I18nString & rValue, Int32, Int32 )
{
	if ( ! m_pWidget->isCheckable() )
		return;

	bool state = false;

	if (CheckBoxTransformer::s_STATEON == rValue)
		state = true;

	m_pWidget->setChecked(state);
}

//---------------------------------------------------------------------------------------//
/*!	\return mapped string for widget's state
	\throw	no-throw */
I18nString GroupBoxBehavour::getString( Int32, Int32 ) const
{
	if ( ! m_pWidget->isCheckable() )
		return "";

	return (m_pWidget->isChecked() ? CheckBoxTransformer::s_STATEON
		                           : CheckBoxTransformer::s_STATEOFF);
}

//---------------------------------------------------------------------------------------//
//! \throw no-throw
void GroupBoxBehavour::clear()
{
	if ( ! m_pWidget->isCheckable() )
		return ;

	m_pWidget->setChecked(false);
}

//---------------------------------------------------------------------------------------//
//! \throw	no-throw
void GroupBoxBehavour::erase(Int32 )
{
}

//---------------------------------------------------------------------------------------//
/*! \return number of hits that widget with most hits (shown rows/items) has.
	\throw	no-throw */
Int32 GroupBoxBehavour::maxCountOfControls() const
{
	return 1;
}

//---------------------------------------------------------------------------------------//
//! \throw  no-throw
//! \return basar type of given widget
WidgetTypes	GroupBoxBehavour::getWidgetType() const
{
	return qGroupBox;
}

//---------------------------------------------------------------------------------------//
}// tie
} // gui
} // basar
