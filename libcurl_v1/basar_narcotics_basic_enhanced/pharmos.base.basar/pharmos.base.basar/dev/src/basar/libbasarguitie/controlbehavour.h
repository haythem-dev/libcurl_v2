//----------------------------------------------------------------------------
/*! \file
 *  \brief  internal interface ControlBehavour
 *  \author Roland Kiefert 
 *  \date   08.09.2006
 */
//----------------------------------------------------------------------------
#ifndef GUARD_CONTROLBEHAVOUR_H
#define GUARD_CONTROLBEHAVOUR_H

//---------------------------------------------------------------------------------------//
// includes
//---------------------------------------------------------------------------------------//
#pragma warning (push)
#pragma warning (disable: 4127 4512)
#include <QtCore/QObject>
#pragma warning (pop)

#include "libbasar_definitions.h"
#include "libbasarguitie_propertytable.h"

//---------------------------------------------------------------------------------------//
// forward declaration
//---------------------------------------------------------------------------------------//
class QEvent;
class QWidget;

//---------------------------------------------------------------------------------------//
// namespaces
//---------------------------------------------------------------------------------------//
namespace basar {
namespace gui {
namespace tie {

//---------------------------------------------------------------------------------------//
// defines
//---------------------------------------------------------------------------------------//
typedef GuiPropertyTable * PGuiPropTbl;		//!< pointer to gui property table

/////////////////////////////////////////////////////////////////////////////////////////
// class ControlBehavour
/////////////////////////////////////////////////////////////////////////////////////////
/*! \brief  class ControlBehavour. Base class, implementing behaviour of all gui controls,
			which are not specialized by a sub-class.

			This is a base class.     
			Class throws the follwing exceptions: 
			class GuiControlNotSupportedException */
//---------------------------------------------------------------------------------------//
// implementation
//---------------------------------------------------------------------------------------//
class ControlBehavour : public QObject
{
public:
	/*! \brief	c'tor with no special action. 
				\n no-throw */
						ControlBehavour		(PGuiPropTbl                   pPt,	//!< pointer to GuiPropertyTable
											 std::list<QWidget*>::iterator it	//!< iterator to QWidget-list
											);
	/*! \brief	d'tor with no special action. 
				\n no-throw */
	virtual			   ~ControlBehavour		();
	/*! \brief	check if switching to next column is possible (preincrement iterator) 
				\n no-throw */
	virtual bool		isSwitchNextAllowed	(Int32 & rCurrentColumn	//!< index of current column
											);
	/*! \brief	set string to control's given row + column. 
				\n GuiControlNotSupportedException */
	virtual void		setString			(const I18nString & rValue, 
											 Int32              currentRow, 
											 Int32              currentColumn 
											 );
	/*! \brief	set string to control's current row and given column.
				\n GuiControlNotSupportedException */
	virtual void		setCurrentString	(const I18nString & rValue, 
											 Int32              currentColumn );
	/*! \brief	set datetime-value to control's given column. 
				\n	GuiControlNotSupportedException*/
	virtual void		setDateTime			(const DateTime & rValue, 
											 Int32            currentColumn 
											);
	/*! \brief	set date-value to control's given column. 
				\n	GuiControlNotSupportedException*/
	virtual void		setDate				(const Date & rValue, 
											 Int32        currentColumn 
											);
	/*! \brief	set time-value to control's given column. 
				\n	GuiControlNotSupportedException*/
	virtual void		setTime				(const Time & rValue, 
											 Int32        currentColumn 
											);
	/*! \brief	returns the value the iterator points to as a STRING. 
				\n GuiControlNotSupportedException */
	virtual I18nString	getString			(Int32 currentRow, 
											 Int32 currentColumn
											) const;
	/*! \brief	returns the value the iterator points to as a STRING. 
				\n GuiControlNotSupportedException */
	virtual I18nString	getCurrentString	(Int32 currentColumn) const;
	/*! \brief	returns the value the iterator points to as a DateTime. 
				\n GuiControlNotSupportedException */
    virtual DateTime	getDateTime			(Int32 currentColumn) const;
	/*! \brief	returns the value the iterator points to as a Date. 
				\n GuiControlNotSupportedException */
    virtual Date		getDate				(Int32 currentColumn) const;
	/*! \brief	returns the value the iterator points to as a Time. 
				\n GuiControlNotSupportedException */
    virtual Time		getTime				(Int32 currentColumn) const;
    /*! \brief	returns the position of the iterator. 
				\n no-throw */
	virtual ColumnSize	getIndex			(Int32 currentColumn	//!< widget position (0 indexed) in widget list
											) const;
	/*! \brief	returns true if the value for this index exists
				\n no-throw  */
	virtual bool		isValueExist		(Int32 currentRow,		//!< row that is searched for value
											 Int32 currentColumn	//!< column that is searched for value
											) const;
	/*! \brief	returns current widget name
				\n no-throw */
	VarString			getObjectName		() const;

	//guipropertytable
	/*! \brief	reset the control and set to empty strings. 
				\n GuiControlNotSupportedException */
	virtual void		clear				();
	/*! \brief	remove the row of a multi line ctrl where yit is pointing to. 
				If it is the last one, the complete mask is cleared. 
				\n GuiControlNotSupportedException */
	virtual void		erase				(Int32 row	//!< row to erase
											);
	/*! \brief	iterate over all ctrls to find out the ctrl 
				with the most hits (shown items) in it. 
				\n GuiControlNotSupportedException */
	virtual Int32		maxCountOfControls	() const;
	/*! \brief	get the absolute index of the current row or item of the gui property table. 
				\n no-throw */
	virtual Int32		getCurrentIndex		();
	/*! \brief	get the absolute indices of the selected rows or items of the gui property table. 
				\n no-throw */
	virtual void		getSelectedIndices	(std::vector<Int32> & coll //!< collection filled with absolute 
																	   //!< (regardless of the page shown), 
																	   //!< 0-based indexes of selected rows 
																	   //!< or items of multiline widget 
											);
	/*! \brief set the matcher. 
		\n	   no-throw */
	void				setMatcher			(boost::shared_ptr<Matcher> pMatcher);
	void				removeMatcher		();

    virtual void        notifyMatchDone();

    void                setSelf(boost::shared_ptr<ControlBehavour> self);

    virtual void        registerForNotification();

protected:
	//EventHandlerComposite
	/*! \brief eventhandling for the Control. 
		\n	   no-throw */
	bool				eventFilter			(QObject * pObject, 
											 QEvent  * pEvent
											);
	/*! \brief eventhandling for the Control. 
		\n	   no-throw */
	virtual bool		eventFilterCtr		(QObject * pObject,	//!< object the event belongs to.
											 QEvent  * pEvent	//!< event to throw.
											);

	/*! \brief  getting the basar widget type enum for the Qt object \n no-throw */
	virtual WidgetTypes	getWidgetType		() const;

	/*! \brief	stores the shared pointer to the implementation object.*/
    PGuiPropTbl							m_pPt;		//!< pointer holding propertytable
	std::list <QWidget * >::iterator	m_it;		//!< iterator to widget list
	boost::weak_ptr < Matcher >			m_pMatcher;	//!< matcher the handle belongs to
    boost::weak_ptr<ControlBehavour>    m_Self;
private:
						ControlBehavour		(const ControlBehavour &);	//!<forbidden copy constructor
    ControlBehavour &	operator =			(const ControlBehavour &);	//!<forbidden copy constructor
};

//---------------------------------------------------------------------------------------//
} // tie
} // gui
} // basar

//---------------------------------------------------------------------------------------//
#endif // GUARD_CONTROLBEHAVOUR_H
