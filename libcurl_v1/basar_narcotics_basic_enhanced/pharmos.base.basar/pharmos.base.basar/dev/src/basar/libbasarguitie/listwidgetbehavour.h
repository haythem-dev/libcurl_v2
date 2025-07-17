//----------------------------------------------------------------------------
/*! \file
 *  \brief  internal interface ListWidgetBehavour
 *  \author Roland Kiefert 
 *  \date   08.09.2006
 */
//----------------------------------------------------------------------------
#ifndef GUARD_LISTWIDGETBEHAVOUR_H
#define GUARD_LISTWIDGETBEHAVOUR_H

//----------------------------------------------------------------------------
#include "controlbehavour.h"

//---------------------------------------------------------------------------------------//
// forward declaration
//---------------------------------------------------------------------------------------//
class QWidget;
class QListWidget;

//---------------------------------------------------------------------------------------//
// namespaces
//---------------------------------------------------------------------------------------//
namespace basar {
namespace gui {
namespace tie {

/////////////////////////////////////////////////////////////////////////////////////////
// class ListWidgetBehavour
/////////////////////////////////////////////////////////////////////////////////////////
/*! \brief  class LineEditBehavour. Implementing behaviour of gui controls in line edits.

			This is a final class.     
			Class throws exception:
			UnableToReadWriteInLineOfControlException */
class ListWidgetBehavour : public ControlBehavour
{
public:
	//X-Iterator
	/*! \brief	c'tor with no special action. 
				\n no-throw */
	ListWidgetBehavour(PGuiPropTbl                   pPt,	//!< pointer to GuiPropertyTable
					   std::list<QWidget*>::iterator it		//!< iterator to QWidget-list
				      );
	/*! \brief	d'tor with no special action. 
				\n no-throw */
	virtual ~ListWidgetBehavour();
	/*! \brief	set string to control's given row + column. 
				\n UnableToReadWriteInLineOfControlException */
	virtual void					setString	( const I18nString& rValue, Int32 currentRow, Int32 currentColumn );
	/*! \brief	set string to control's current row and given column.
				\n no-throw */
	virtual void					setCurrentString( const I18nString& rValue, //!< string-value to set to column
													  Int32 currentColumn //!< column to set current row's string to
													  );
	/*! \brief	returns the value the iterator points to as a STRING. 
				\n UnableToReadWriteInLineOfControlException */
	virtual I18nString				getString(Int32 currentRow, Int32 currentColumn) const;
	/*! \brief	returns the value the iterator points to as a STRING. 
				\n no-throw */
	virtual I18nString				getCurrentString(Int32 currentColumn //!< column to get current row's string from
													) const;

	//guipropertytable
	/*! \brief	reset the control and set to empty strings. 
				\n no-throw */
	virtual void					clear();
	/*! \brief	remove the row of a multi line ctrl yit is pointing to. 
				If it is the last one the complete mask is cleared. 
				\n no-throw */
	virtual void					erase( Int32 row	//!< row to erase
									);
	/*! \brief	iterate over all ctrls to find out the ctrl 
				with the most hits (shown items) in it. 
				\n no-throw */
	virtual Int32					maxCountOfControls() const;
	/*! \brief	get the absolute index of the current row or item of the gui property table. 
				\n no-throw */
	virtual Int32					getCurrentIndex();
	/*! \brief	get the absolute indices of the selected rows or items of the gui property table. 
				\n no-throw */
	virtual void		getSelectedIndices(std::vector<Int32>&);

protected:
	//! \brief getting the basar widget type enum for the Qt object \n no-throw 
	virtual WidgetTypes				getWidgetType		() const;

private:
	/*! \brief	forbidden copy constructor.*/
    ListWidgetBehavour (const ListWidgetBehavour&);
	/*! \brief	forbidden assignment operator.*/
    ListWidgetBehavour& operator=(const ListWidgetBehavour&);
	
	QListWidget*					m_ListWidget; //!< pointer to the qt control qLabel
};


} // tie
} // gui
} // basar


#endif // GUARD_LISTWIDGETBEHAVOUR_H

