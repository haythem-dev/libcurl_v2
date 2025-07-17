//-------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  Transformer for checkbox in a table widget.
 *  \author Frank Naumann
 *  \date   27.03.2007
 */
//-------------------------------------------------------------------------------------------------//

#ifndef GUARD_LIBUTIL_VIEWCONN_CHECKBOXTABLETRANSFORMER_H
#define GUARD_LIBUTIL_VIEWCONN_CHECKBOXTABLETRANSFORMER_H

//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include <libbasarguitie.h>

//-------------------------------------------------------------------------------------------------//
// forward declaration
//-------------------------------------------------------------------------------------------------//
class QTableWidget;


//-------------------------------------------------------------------------------------------------//
// class declaration section
//-------------------------------------------------------------------------------------------------//
namespace narcotics
{
namespace viewConn 
{
	//-------------------------------------------------------------------------------------------------//
	// class declaration
	//-------------------------------------------------------------------------------------------------//
	/*! \brief Transformer for checkboxes in table widgets.

			Matches one property of the property table to one checkbox column in the table widget.
			Supported property types are STRING, INT16, INT32. */
	class CheckBoxTableTransformer : public basar::gui::tie::Transformer
	{
	public:
	/*! \brief constructor
		\param pTableWidget  The table widget in which the checkbox column shall be displayed
		\param pColumnName   The column name of the table column (header label!)
		\param pPropertyName The name of the property in accessor property table.
		\param pOn           The string representation of "checked state" in the property table.
		\param pOff          The string representation of "unchecked state" in the property table.
		\param pPart         The string representation of "partially checked state" in the property table.
		\throw               no-throw */
	CheckBoxTableTransformer(QTableWidget* pTableWidget, const basar::I18nString pColumnName, 
		const basar::I18nString pPropertyName,
		const basar::I18nString pOn = "On", 
		const basar::I18nString pOff = "Off",
		const basar::I18nString pPart = "Part");

	/*! \brief destructor for checkbox table transformer
		\throw no-throw */
	~CheckBoxTableTransformer();

	/*! \brief transform values from right to left iterator
		\param guiIt iterator to GuiPropertyTable row
		\param accIt iterator to property table/cache controller row
		\throw       UnknownPropertyTypeException */
	virtual void RightToLeft(basar::gui::tie::GuiPropertyTable_YIterator guiIt, const basar::db::aspect::AccessorPropertyTable_YIterator accIt);

	/*! \brief transform values from left to right iterator
		\param guiIt iterator to GuiPropertyTable row
		\param accIt iterator to property table/cache controller row
		\throw       TypecastFailedException */
	virtual void LeftToRight(const basar::gui::tie::GuiPropertyTable_YIterator guiIt, basar::db::aspect::AccessorPropertyTable_YIterator accIt);

	/*! \brief creates cloned instance
		\throw no-throw */
	virtual boost::shared_ptr<basar::gui::tie::Transformer> create();

	private:
		QTableWidget*     m_TableWidget;   //!< The QTableWidget (or alternatively BasarTableWidget) to add a checkbox column. 
		basar::I18nString m_ColumnName;    //!< The GUI property table column name.
		basar::I18nString m_PropertyName;  //!< The accessor property table column name.
		basar::I18nString m_On;            //!< The string representation of "checked state" in the property table.
		basar::I18nString m_Off;           //!< The string representation of "unchecked state" in the property table.
		basar::I18nString m_Part;          //!< The string representation of "partially checked state" in the property table.
		bool              m_ShowText;      //!< Defines whether the text representation of the value will be displayed in cell.
	};

} // viewConn 
} // libutil

#endif//GUARD_LIBUTIL_VIEWCONN_CHECKBOXTABLETRANSFORMER_H