//-------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  Implementation of transformer for checkbox in a table widget
 *  \author Frank Naumann
 *  \date   27.03.2007
 */
//-------------------------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "checkboxtabletransformer.h"
#include "libbasarqtwidget_tablewidget.h"

//-------------------------------------------------------------------------------------------------//
// class definition section
//-------------------------------------------------------------------------------------------------//
namespace narcotics
{
namespace viewConn 
{

//-------------------------------------------------------------------------------------------------//
CheckBoxTableTransformer::CheckBoxTableTransformer(QTableWidget* pTableWidget, const basar::I18nString pColumnName,
	const basar::I18nString pPropertyName, const basar::I18nString pOn, const basar::I18nString pOff,
	const basar::I18nString pPart)
{
	m_TableWidget	= pTableWidget;
	m_ColumnName	= pColumnName;
	m_PropertyName	= pPropertyName;
	m_On			= pOn;
	m_Off			= pOff;
	m_Part			= pPart;
	m_ShowText		= false;
}

//-------------------------------------------------------------------------------------------------//
CheckBoxTableTransformer::~CheckBoxTableTransformer() 
{
	m_TableWidget = NULL;
}

//-------------------------------------------------------------------------------------------------//
void CheckBoxTableTransformer::RightToLeft(basar::gui::tie::GuiPropertyTable_YIterator guiIt,
										const basar::db::aspect::AccessorPropertyTable_YIterator accIt)
{
	basar::Int32					row  = guiIt.getPosition();
	basar::Int32					col  = 0;//static_cast<basar::Int32>(guiIt.getIndex(m_ColumnName));
	basar::db::aspect::PropertyType	type = accIt.getType(m_PropertyName);
	QTableWidgetItem*				item = m_TableWidget->item(row, col);

	basar::I18nString value = m_Off;
	if (accIt.isContainedAndSet(m_PropertyName))
	{
		switch (type.getType())
		{
		case basar::INT16  : value.format("%d", accIt.getInt16(m_PropertyName)); break;
		case basar::INT32  : value.format("%d", accIt.getInt32(m_PropertyName)); break;
		case basar::STRING : value = accIt.getString(m_PropertyName);            break;
		default            :                                                     break;
		}
	}

	m_TableWidget->setItem(row, col, (item = new QTableWidgetItem()));
	item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
	if (m_ShowText == true)
	{
		item->setText(QString::fromLocal8Bit(value.c_str()));
	}

	if (value == m_On)
		item->setCheckState(Qt::Checked);
	else if (value == m_Part)
		item->setCheckState(Qt::PartiallyChecked);
	else
		item->setCheckState(Qt::Unchecked);
}

//-------------------------------------------------------------------------------------------------//
void CheckBoxTableTransformer::LeftToRight(const basar::gui::tie::GuiPropertyTable_YIterator guiIt,
										   basar::db::aspect::AccessorPropertyTable_YIterator accIt)
{
	basar::Int32					row  = guiIt.getPosition();
	basar::Int32					col  = static_cast<basar::Int32>(guiIt.getIndex(m_ColumnName));
	basar::db::aspect::PropertyType	type = accIt.getType(m_PropertyName);
	QTableWidgetItem*				item = m_TableWidget->item(row, col);

	basar::I18nString value = m_Off;
	if (item != NULL)
	{
		switch (item->checkState())
		{
		case Qt::Checked          : value = m_On;   break;
		case Qt::Unchecked        : value = m_Off;  break;
		case Qt::PartiallyChecked : value = m_Part; break;
		default                   :                 break;
		}
	}

	switch (type.getType())
	{
	case basar::INT16  : accIt.setInt16 (m_PropertyName, static_cast<basar::Int16>(value.stoi())); break;
	case basar::INT32  : accIt.setInt32 (m_PropertyName, value.stoi()); break;
	case basar::STRING : accIt.setString(m_PropertyName, value);        break;
	default            :                                                break;
	}
}

//-------------------------------------------------------------------------------------------------//
boost::shared_ptr<basar::gui::tie::Transformer> CheckBoxTableTransformer::create()
{
	return boost::shared_ptr<basar::gui::tie::Transformer>(new CheckBoxTableTransformer(*this));
}


} //namespace viewConn
} //namespace narcotics