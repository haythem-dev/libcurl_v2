//----------------------------------------------------------------------------
/*! \file
 *  \brief  class implementation of LabelBehavour
 *  \author Roland Kiefert
 *  \date   08.09.2006
 */
//----------------------------------------------------------------------------


//---------------------------------------------------------------------------------------//
// includes
//---------------------------------------------------------------------------------------//
#include "checkboxbehavour.h"

#include "libbasarguitie_transformer.h"

#pragma warning (push)
#pragma warning (disable: 4231 4244 4800)
#include <QtWidgets/QCheckBox>
#pragma warning (pop)

//---------------------------------------------------------------------------------------//
// namespaces
//---------------------------------------------------------------------------------------//
namespace basar {
namespace gui {
namespace tie {

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
CheckBoxBehavour::CheckBoxBehavour (PGuiPropTbl                   pPt,
								    std::list<QWidget*>::iterator it )
								   : ControlBehavour(pPt, it)
{
	m_CheckBoxWidget = static_cast<QCheckBox*>( (*m_it) );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
CheckBoxBehavour::~CheckBoxBehavour()
{
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void CheckBoxBehavour::setString( const I18nString & rValue, Int32, Int32 )
{
	Qt::CheckState state = Qt::Unchecked;

	if (CheckBoxTransformer::s_STATEON == rValue)
		state = Qt::Checked;
	else
	{
		if (CheckBoxTransformer::s_STATEPART == rValue)
			state = Qt::PartiallyChecked;
	}

	m_CheckBoxWidget->setCheckState(state);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\return I18nString value of the current line in the current ctrl.
	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
I18nString CheckBoxBehavour::getString( Int32, Int32 ) const
{
	switch (m_CheckBoxWidget->checkState())
	{
	case Qt::Checked:
		return CheckBoxTransformer::s_STATEON;

	case Qt::PartiallyChecked:
		return CheckBoxTransformer::s_STATEPART;

	case Qt::Unchecked:
	default:
		return CheckBoxTransformer::s_STATEOFF;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////
//! \throw  no-throw
///////////////////////////////////////////////////////////////////////////////////////////
void CheckBoxBehavour::clear()
{
	m_CheckBoxWidget->setCheckState(Qt::Unchecked);
}
///////////////////////////////////////////////////////////////////////////////////////////
//! \throw	no-throw
///////////////////////////////////////////////////////////////////////////////////////////
void CheckBoxBehavour::erase(Int32 )
{
}
///////////////////////////////////////////////////////////////////////////////////////////
/*! \return Int32:	number of hits that the ctrl with the most hits (shown rows/items) has.
	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////////
Int32 CheckBoxBehavour::maxCountOfControls() const
{
	return 1;
}
///////////////////////////////////////////////////////////////////////////////////////////
//! \throw no-throw
//! \return type of given QObject
///////////////////////////////////////////////////////////////////////////////////////////
WidgetTypes	CheckBoxBehavour::getWidgetType() const
{
	return qCheckBox;
}

}// tie
} // gui
} // basar
