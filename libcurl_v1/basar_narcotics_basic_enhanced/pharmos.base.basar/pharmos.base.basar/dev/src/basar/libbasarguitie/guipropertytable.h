//----------------------------------------------------------------------------
/*! \file
 *  \brief  internal interface GuiPropertyTable
 *  \author Roland Kiefert 
 *  \date   18.08.2005
 */
//----------------------------------------------------------------------------
#ifndef GUARD_GUIPROPERTYTABLE_H
#define GUARD_GUIPROPERTYTABLE_H

//---------------------------------------------------------------------------------------//
#include <boost/shared_ptr.hpp>
#include "libbasarproperty_propertydescriptionlistref.h"
#include "libbasarguitie_definitions.h"

#include <set>

//---------------------------------------------------------------------------------------//
// forward declaration
//---------------------------------------------------------------------------------------//
class QWidget;
class QObject;
class QTableWidget;

//---------------------------------------------------------------------------------------//
// namespaces
//---------------------------------------------------------------------------------------//
namespace basar {
namespace gui {
namespace tie {

//---------------------------------------------------------------------------------------//
// namespaces specific forward declaration
//---------------------------------------------------------------------------------------//
class GuiPropertyTable_YIterator;
class EventHandlerComposite;
class Matcher;
class ControlBehavour;

//---------------------------------------------------------------------------------------//
// defines
//---------------------------------------------------------------------------------------//
#define ITERATOR_ENDPOS	-99 //!< unique value to represent the end-iterator's value


/////////////////////////////////////////////////////////////////////////////////////////
// class GuiPropertyTable
/////////////////////////////////////////////////////////////////////////////////////////
/*! \brief  class GuiPropertyTable manages all gui ctrls of a given gui frame
			(dialog, frame or shape, etc.). It can regulate the ctrls which should not be read by a WidgetPolicyEnum.
			It (GuiPropertyTable) will erase or insert rows in multi line ctrls.
			It will take into consideration if the gui ctrls has a memory or not.

			This is a final class.     
			Class throws the follwing exceptions: 
			class InvalidValueException,
			class UnknownPropertyNameException. */
//---------------------------------------------------------------------------------------//
// implementation
//---------------------------------------------------------------------------------------//
class GuiPropertyTable
{
    friend class GuiPropertyTable_XIterator;
    friend class GuiPropertyTable_YIterator;
    friend class ControlBehavour;
    friend class TableWidgetBehavour;

public:

    //!	\brief	shared pointer to GuiPropertyTable
    typedef boost::shared_ptr< GuiPropertyTable >	GuiPropertyTableSharedPtr;

    /*! \brief	simple factory method (static method) to create a new object of this class.
                \n no-throw */
    static GuiPropertyTableSharedPtr				create(const QWidget         * qWidget,
        EventHandlerComposite * eh,
        const WidgetPolicyEnum  eWidgetPolicy);

    /*! \brief	return reference to PropertyDescriptionList
                \n no-throw */
    basar::property::PropertyDescriptionListRef		getPropertyDescriptionList() const;

    /*! \brief	return shared pointer to this by casting weak -> shared
                \n no-throw */
    GuiPropertyTableSharedPtr						thisPtr() const;

    /*! \brief	iterate over all ctrls to find out the ctrl
                with the most hits (shown items) in it.
                \n GuiControlNotSupportedException */
    Int32											maxCountOfControls() const;

    /*! \brief	return the size returned by the method 'maxCountOfControls()'
                plus the member variable 'm_AbsoluteIndex'
                \n GuiControlNotSupportedException */
    UInt32											size() const;

    /*! \brief	still open what to do here . \n no-throw */
    bool											empty() const;

    /*! \brief	reset the complete gui frame to empty strings.
                \n GuiControlNotSupportedException */
    void											clear();

    /*! \brief	return iterator to first row. \n no-throw */
    GuiPropertyTable_YIterator						begin() const;

    /*! \brief	return iterator to row beyond the last one. \n no-throw */
    GuiPropertyTable_YIterator						end() const;

    /*! \brief	set new empty row to guipropertytable and
                return iterator to this row. \n no-throw */
    GuiPropertyTable_YIterator						insert();

    /*! \brief	remove the row of a multi line ctrl yit is pointing to.
                If it is the last one the complete mask is cleared.
                \n NoValidMemoryPolicyException, GuiControlNotSupportedException */
    void											erase(const GuiPropertyTable_YIterator yIt);

    /*! \brief	set the insert strategy for the multi line ctrls. It defines
                if new rows will be appended to the ctrl or will be overwriting
                the existing hits so that the vertical scroll bar will never appear.
                \n NoValidMemoryPolicyException */
    void											setMemoryPolicy(MemoryPolicyEnum memoryPolicy);

    /*! \brief	set an eventhandler for a certain widget type.
                \n no-throw */
    IdEventHandler									setEventHandler(boost::shared_ptr<QObject> rEventHandler,
        WidgetTypes objectTyp = qUnset);
    /*! \brief	delete the eventhandler. \n no-throw */
    void											deleteEventHandler(IdEventHandler id);

    /*! \brief	set the event policy. \n no-throw */
    void											setEventPolicy(UInt32 policy);

    /*! \brief	get the absolute index of the first shown row of the gui property table.
                \n no-throw */
    Int32											getAbsoluteIndex();

    /*! \brief	get the absolute index of the current row or item of the gui property table.
                \n no-throw */
    Int32											getCurrentIndex();

    /*! \brief	get the absolute indices of the selected rows or items of the gui property table.
                \n no-throw */
    std::vector<Int32>								getSelectedIndices();

	/*! \brief	return if there any multi line controls exist in the property table. 
				\n no-throw */
	bool											isMultiLinePropertyTable();

	/*! \brief	destruction of GuiPropertyTable \n no-throw */
	~GuiPropertyTable( );

	/*! \brief set the matcher. 
		\n	   no-throw */
	void setMatcher(boost::shared_ptr<Matcher> pMatcher);

    //! \brief  removes the Matcher
	void removeMatcher();

    void registerForMatchDoneNotification(boost::weak_ptr<ControlBehavour> behaviour);

    void unregisterForMatchDoneNotification(boost::weak_ptr<ControlBehavour> behaviour);

    void notifyMatchDone();

private:
	/*! \brief	construction of GuiPropertyTable called by the static method 
				( create() [simple factory method]); \n no-throw */
	GuiPropertyTable( const QWidget* qWidget, EventHandlerComposite* eh, 
		              const WidgetPolicyEnum eWidgetPolicy = WITH_ALL_CTRLS );

	//! typedef to avoid the repetition of the parameterised class 'GuiPropertyTable'.
	typedef boost::weak_ptr<GuiPropertyTable>		GuiPropertyTableWeakPtr; 
	/*! \brief	set a weak pointer of this to the member 'm_WeakPtr'. \n no-throw */
	void											setWeakPtr( GuiPropertyTableWeakPtr weakPtr	);		

	//! stores a weak pointer of itself (this)
	GuiPropertyTableWeakPtr							m_WeakPtr;

	/*! \brief	container (QList) of all gui ctrls of a gui frame
				the GuiPropertyTable is associated with. */
	std::list<QWidget*>								m_AllCtrls;

	/*! \brief	container (QList) of all gui ctrls of a gui frame
				the GuiPropertyTable is associated with. */
	std::list< boost::shared_ptr< ControlBehavour > > m_Ctrls;

	/*! \brief flag indicates if there are any multi line controls in the list m_AllCtrls */
	bool											m_MultiLineCtrlsExist;

	/*! \brief	returns a reference to the implementation 
				of the PropertyDescriptionList */
	basar::property::PropertyDescriptionListRef		m_PdlRef;
	
	//! stores the settings of the using of a paging mechanism
	MemoryPolicyEnum								m_MemoryPolicy; 
	
	//! stores the settings of the ctrls read in belonging to a certain GuiPropertyTable
	WidgetPolicyEnum								m_WidgetPolicy;
	
	//! stores an event filter which is installed on a table widget ctrl if it is read in in the c'tor.
	EventHandlerComposite*							m_EventHandler;
	
	//! represents the first shown item/row in a multi line crtl.
	//!	It can be greater than item/rows counted in the case of the paging mechanism.
	Int32											m_AbsoluteIndex;

	/*! \brief	container (map) of all event ctrls of a gui frame
				the GuiPropertyTable is associated with. */
	std::map<QWidget * ,  boost::shared_ptr< ControlBehavour > > m_eventFilterMapByQObject;

	/*! \brief	container (map) of all event ctrls of a gui frame
				the GuiPropertyTable is associated with. */
	std::map<boost::shared_ptr< ControlBehavour > , QWidget * >  m_eventFilterMapByBehaviour;

    std::set<boost::weak_ptr<ControlBehavour> >               m_BehavioursToNotify;
};


} // tie
} // gui
} // basar


#endif // GUARD_GUIPROPERTYTABLE_H
