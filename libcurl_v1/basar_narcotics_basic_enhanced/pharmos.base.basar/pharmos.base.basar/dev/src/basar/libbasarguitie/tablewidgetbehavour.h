//----------------------------------------------------------------------------
/*! \file
 *  \brief  internal interface TableWidgetBehavour
 *  \author Roland Kiefert 
 *  \date   08.09.2006
 */
//----------------------------------------------------------------------------
#ifndef GUARD_TABLEWIDGETBEHAVOUR_H
#define GUARD_TABLEWIDGETBEHAVOUR_H

//----------------------------------------------------------------------------
#include "controlbehavour.h"

//---------------------------------------------------------------------------------------//
// forward declaration
//---------------------------------------------------------------------------------------//
class QWidget;
class QTableWidget;
class QTableWidgetItem;

//---------------------------------------------------------------------------------------//
// namespaces
//---------------------------------------------------------------------------------------//
namespace basar {
namespace gui {
namespace tie {

//---------------------------------------------------------------------------------------//
// defines
//---------------------------------------------------------------------------------------//

/////////////////////////////////////////////////////////////////////////////////////////
// class TableWidgetBehavour
/////////////////////////////////////////////////////////////////////////////////////////
/*! \brief  class TableWidgetBehavour. Implementing behaviour of gui controls in table widgets.

			This is a final class.     
			Class throws the follwing exceptions: 
			class NoValidMemoryPolicyException,
			class UnableToReadWriteInLineOfControlException. */
//---------------------------------------------------------------------------------------//
// implementation
//---------------------------------------------------------------------------------------//
class TableWidgetBehavour : public ControlBehavour
{
public:
	//X-Iterator
	/*! \brief	c'tor with no special action. 
				\n no-throw */
	TableWidgetBehavour(PGuiPropTbl pPt,					//!< pointer to GuiPropertyTable
						std::list<QWidget*>::iterator it	//!< iterator to QWidget-list
					   );
	/*! \brief	d'tor with no special action. 
				\n no-throw */
	virtual ~TableWidgetBehavour();
	/*! \brief	switch forward the iterator (preincrement) 
				\n no-throw */
	virtual bool		isSwitchNextAllowed(Int32 & rCurrentColumn);
	/*! \brief	set string to control's given row + column. 
				\n NoValidMemoryPolicyException */
	virtual void					setString	( const I18nString& rValue, //!< string-value to set to column
												  Int32 currentRow,		//!< row to set string to
												  Int32 currentColumn	//!< column to set string to
												  );
	/*! \brief	returns the value the iterator points to as a STRING. 
				\n UnableToReadWriteInLineOfControlException */
	virtual I18nString				getString(Int32 currentRow,			//!< row to get string from
											  Int32 currentColumn		//!< column to get string from
											  ) const;
    /*! \brief	returns the position of the iterator. 
				\n no-throw */
	virtual ColumnSize				getIndex(Int32 currentColumn) const;
	/*! \brief	returns true if the value for this index exists
				\n no-throw  */
	virtual bool					isValueExist( Int32 currentRow, Int32 currentColumn ) const;

	//guipropertytable
	/*! \brief	reset the control and set to empty strings. 
				\n no-throw */
	virtual void					clear();
	/*! \brief	remove the row of a multi line ctrl yit is pointing to. 
				If it is the last one the complete mask is cleared. 
				\n NoValidMemoryPolicyException */
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
	virtual void					getSelectedIndices(std::vector<Int32>&);

	//EventHandlerComposite
	/*! \brief eventhandling for the Control. 
		\n	   no-throw */
	virtual bool					eventFilterCtr(QObject *pObject, QEvent * pEvent);

	/*! \brief empty function for resizing Columns and Rows, unued and
	*		   overwritten in BasarTableWidget
		\n	   no-throw */
    virtual void                    checkResize(QTableWidget * tableWidget		//!< pointer to Qt tablewidget
											   );

	/*  \brief neccessary as the method setItem is not virtual in  QTableWidget
	 *         or in BasarTableWidget
	*		   overwritten in BasarTableWidget
	* @param row Row
	* @param column column
	* @param item item 
		\n	   no-throw */
	virtual void                    setItem(int row, int column, QTableWidgetItem * item);

protected:
	//! \brief getting the basar widget type enum for the Qt object \n no-throw 
	virtual WidgetTypes				getWidgetType		() const;

	//!  \brief helperfunctions for one PageDown (no signals)
	bool doPageUp(QTableWidget * tableWidget	//!< pointer to Qt tablewidget
		         );

	//!  \brief helperfunctions for one PageDown (no signals)
	bool doPageDown(QTableWidget * tableWidget	//!< pointer to Qt tablewidget
		           );

	//!  \brief helperfunctions for one PageDown (no signals)
	bool doRowUp(QTableWidget * tableWidget	//!< pointer to Qt tablewidget
		        );

	//!  \brief helperfunctions for one PageDown (no signals)
	bool doRowDown(QTableWidget * tableWidget	//!< pointer to Qt tablewidget
		           );

private:
	/*! \brief	forbidden copy constructor.*/
    TableWidgetBehavour (const TableWidgetBehavour&);
	/*! \brief	forbidden assignment operator.*/
	TableWidgetBehavour& operator=(const TableWidgetBehavour&);
	
	QTableWidget*					m_tableWidget; //!< pointer to the qt control qTableWidget

};


} // tie
} // gui
} // basar


#endif // GUARD_TABLEWIDGETBEHAVOUR_H

