//----------------------------------------------------------------------------
/*! \file
 *  \brief  internal interface LabelBehavour
 *  \author Roland Kiefert 
 *  \date   08.09.2006
 */
//----------------------------------------------------------------------------
#ifndef GUARD_LABELBEHAVOUR_H
#define GUARD_LABELBEHAVOUR_H

//----------------------------------------------------------------------------
#include "controlbehavour.h"

//---------------------------------------------------------------------------------------//
// forward declaration
//---------------------------------------------------------------------------------------//
class QWidget;
class QLabel;


//---------------------------------------------------------------------------------------//
// namespaces
//---------------------------------------------------------------------------------------//
namespace basar {
namespace gui {
namespace tie {

/////////////////////////////////////////////////////////////////////////////////////////
// class LabelBehavour
/////////////////////////////////////////////////////////////////////////////////////////
/*! \brief  class LabelBehavour. Implementing behaviour of gui controls in labels.

			This is a final class.     
			Class throws no exceptions. */
class LabelBehavour : public ControlBehavour
{
public:
	//X-Iterator
	/*! \brief	c'tor with no special action. 
				\n no-throw */
	LabelBehavour(PGuiPropTbl                   pPt,	//!< pointer to GuiPropertyTable
				  std::list<QWidget*>::iterator it		//!< iterator to QWidget-list
				 );
	/*! \brief	d'tor with no special action. 
				\n no-throw */
	virtual ~LabelBehavour();
	/*! \brief	set string to control's given row + column. 
				\n no-throw */
	virtual void					setString	( const I18nString& rValue, //!< string-value to set to column
													Int32 currentRow,	//!< row to set string to
													Int32 currentColumn //!< column to set string to
												);
	/*! \brief	returns the value the iterator points to as a STRING. 
				\n no-throw */
	virtual I18nString				getString(Int32 currentRow,		//!< row to get string from
											  Int32 currentColumn	//!< column to get string from
											  ) const;

	//guipropertytable
	/*! \brief	reset the control and set to empty strings. 
				\n no-throw */
	virtual void					clear();
	/*! \brief	remove the row of a multi line ctrl yit is pointing to. 
				If it is the last one the complete mask is cleared. 
				\n no-throw */
	virtual void					erase( Int32  row	//!< row to erase
									);
	/*! \brief	iterate over all ctrls to find out the ctrl 
				with the most hits (shown items) in it. 
				\n no-throw */
	virtual Int32					maxCountOfControls() const;

protected:
	//! \brief getting the basar widget type enum for the Qt object \n no-throw 
	virtual WidgetTypes				getWidgetType		() const;

private:
	/*! \brief	forbidden copy constructor.*/
    LabelBehavour (const LabelBehavour&);
	/*! \brief	forbidden assignment operator.*/
    LabelBehavour& operator=(const LabelBehavour&);
	
	QLabel*							m_labelwidget; //!< pointer to the qt control qLabel
};


} // tie
} // gui
} // basar


#endif // GUARD_LABELBEHAVOUR_H

