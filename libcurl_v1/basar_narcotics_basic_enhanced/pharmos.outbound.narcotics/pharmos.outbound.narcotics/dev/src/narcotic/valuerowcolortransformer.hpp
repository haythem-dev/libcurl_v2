//----------------------------------------------------------------------------
/*! \file
 *  \brief
 *  \author
 *  \date
 */
//----------------------------------------------------------------------------

#ifndef GUARD_VALUEROWCOLORTRANSFORMER_H
#define GUARD_VALUEROWCOLORTRANSFORMER_H

//-------------------------------------------------------------------------------------------------//

#include <libbasarguitie.h>

class QTableWidget;

//-------------------------------------------------------------------------------------------------//
// namespace
//-------------------------------------------------------------------------------------------------//

namespace narcotics {
namespace viewConn {

//-------------------------------------------------------------------------------------------------//
// class declaration
//-------------------------------------------------------------------------------------------------//
/*! \brief  class transforming values in row colors in table widget.
 *			This is a final class.  
 *	\throw no-throw
 */
template <class T> class ValueRowColorTransformer : public basar::gui::tie::Transformer
{
public:
	/*! \brief constructor for ValueRowColorTransformer.
	 *	\throw no-throw
	 */
	ValueRowColorTransformer(
		const basar::VarString propertyName,			//!< name of property
		std::map<T, QColor> valueColorMap,				//!< assignment value - color
		QTableWidget* tw								//!< pointer to table widget
		)
	{
		m_PropertyName		= propertyName;
		m_ValueColorMap		= valueColorMap;
		m_TableWidget		= tw;
	}

	/*! \brief destructor.
	 *	\throw no-throw
	 */
	virtual ~ValueRowColorTransformer()
	{
	}


	/*! \brief transform values from right to left iterator
		\throw no-throw */
	virtual void RightToLeft(basar::gui::tie::GuiPropertyTable_YIterator	          guiIt,	//!< iterator to GuiPropertyTable row
							 const basar::db::aspect::AccessorPropertyTable_YIterator accIt)	//!< iterator to property table/cache controller row
	{
		T								value;
		basar::Int32					rowIndex;
		std::map<T, QColor>::iterator	it;
		QTableWidgetItem*				item;

		if (false == accIt.isContainedAndSet(m_PropertyName))
			return;

		value = getValueFromAcc<T>(accIt);

		it = m_ValueColorMap.find(value);
		if (it != m_ValueColorMap.end())
		{
			rowIndex = guiIt.getPosition();
			for (int colIndex = 0; colIndex < m_TableWidget->columnCount(); ++ colIndex)
			{
				item = m_TableWidget->item(rowIndex, colIndex);
				if (item)
					item->setBackground(QBrush((*it).second));
			}
		}
	}

	/*! \brief transform values from left to right iterator
		\n no-throw */
	virtual void LeftToRight(const basar::gui::tie::GuiPropertyTable_YIterator  guiIt,	//!< iterator to GuiPropertyTable row
							 basar::db::aspect::AccessorPropertyTable_YIterator accIt)	//!< iterator to property table/cache controller row
	{
	}

	/*! \brief creates cloned instance
		\n no-throw */
	 virtual boost::shared_ptr<basar::gui::tie::Transformer> create()
	 {
		 return boost::shared_ptr<basar::gui::tie::Transformer>(new ValueRowColorTransformer(*this));
	 }


protected:
	template <class T> T getValueFromAcc(const basar::db::aspect::AccessorPropertyTable_YIterator accIt);

	template <> basar::VarString getValueFromAcc<basar::VarString>(const basar::db::aspect::AccessorPropertyTable_YIterator accIt)
	{
		return accIt.getString(m_PropertyName);
	}

	template <> basar::Int16 getValueFromAcc<basar::Int16>(const basar::db::aspect::AccessorPropertyTable_YIterator accIt)
	{
		return accIt.getInt16(m_PropertyName);
	}

	template <> basar::Int32 getValueFromAcc<basar::Int32>(const basar::db::aspect::AccessorPropertyTable_YIterator accIt)
	{
		return accIt.getInt32(m_PropertyName);
	}

	basar::VarString	m_PropertyName;
	std::map<T, QColor>	m_ValueColorMap;
	QTableWidget*		m_TableWidget;
};

//-------------------------------------------------------------------------------------------------//

} // viewConn
} // narcotics

//-------------------------------------------------------------------------------------------------//

#endif 

//-------------------------------------------------------------------------------------------------//
