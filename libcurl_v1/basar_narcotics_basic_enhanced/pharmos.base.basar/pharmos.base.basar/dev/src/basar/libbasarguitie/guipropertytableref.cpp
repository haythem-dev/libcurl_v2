//----------------------------------------------------------------------------
/*! \file
 *  \brief  reference to all functions of guipropertytable
 *  \author Roland Kiefert
 *  \date   21.09.2005
 */
//----------------------------------------------------------------------------

//---------------------------------------------------------------------------------------//
// includes
//---------------------------------------------------------------------------------------//

#include "libbasarguitie_propertytable.h"

#include <libbasarproperty_propertydescriptionlistref.h>
#include "guipropertytable.h"

//---------------------------------------------------------------------------------------//
// namespaces
//---------------------------------------------------------------------------------------//
namespace basar {
namespace gui {
namespace tie {


//---------------------------------------------------------------------------------------//
// implementation
//---------------------------------------------------------------------------------------//
//! \return	reference to property description list
//! \throw  OutOfRangeIteratorException
property::PropertyDescriptionListRef GuiPropertyTableRef::getPropertyDescriptionList() const
{
	return getImpl()->getPropertyDescriptionList();
}
    
//! \return size of completly read resultset / propertytable
//! \throw  InvalidValueException, OutOfRangeIteratorException
UInt32 GuiPropertyTableRef::size() const
{
	return getImpl()->size();
}

//! \retval true if resultset and propertytable are empty 
//! \return otherwise returns false
//! \throw  OutOfRangeIteratorException
bool GuiPropertyTableRef::empty() const
{
	return getImpl()->empty();
}

//! \throw  OutOfRangeIteratorException
void GuiPropertyTableRef::clear()
{
	getImpl()->clear();
}

//! \return empty yiterator to new row
//! \throw  OutOfRangeIteratorException
GuiPropertyTable_YIterator GuiPropertyTableRef::insert()
{
	return getImpl()->insert();	
}

//! \throw  OutOfRangeIteratorException
//! \param	yit iterator to line which is deleted at next execute
void GuiPropertyTableRef::erase(GuiPropertyTable_YIterator yit)
{
	getImpl()->erase(yit);
}

//! \return yiterator to first row
//! \throw  OutOfRangeIteratorException
GuiPropertyTable_YIterator GuiPropertyTableRef::begin() const
{
	return getImpl()->begin();
}

//! \return yiterator to last row
//! \throw  OutOfRangeIteratorException
GuiPropertyTable_YIterator GuiPropertyTableRef::end() const
{
	return getImpl()->end(); 
}

/*!	\throw	no-throw
	\param	policy policy for the global event handler. */
void GuiPropertyTableRef::setEventPolicy(UInt32 policy)
{
	getImpl()->setEventPolicy(policy); 
}

/*!	\throw no-throw
	\param	rEventHandler event handler that will be set.
	\param	objectTyp widget type the event handler works for.
	\return event handler's ID */
IdEventHandler GuiPropertyTableRef::setEventHandler(boost::shared_ptr<QObject> rEventHandler, WidgetTypes objectTyp)
{
	return getImpl()->setEventHandler(rEventHandler, objectTyp); 
}

/*!	\throw no-throw
	\param	id event handler's ID */
void GuiPropertyTableRef::deleteEventHandler(IdEventHandler id)
{
	getImpl()->deleteEventHandler(id); 
}

/*!	\throw no-throw
	\return the absolute index of the first shown row of gui property table */
Int32 GuiPropertyTableRef::getAbsoluteIndex()
{
	return getImpl()->getAbsoluteIndex(); 
}

/*!	\throw no-throw
	\return the absolute index of the first shown row of gui property table */
std::vector<Int32> GuiPropertyTableRef::getSelectedIndices()
{
	return getImpl()->getSelectedIndices(); 
}

/*!	\throw no-throw
	\return the absolute index of the current row of gui property table */
Int32 GuiPropertyTableRef::getCurrentIndex()
{
	return getImpl()->getCurrentIndex(); 
}

/*! \return if any multi line controls exist in the property table 
/*! \throw	no-throw */
bool GuiPropertyTableRef::isMultiLinePropertyTable()
{
	return getImpl()->isMultiLinePropertyTable();
}

//!	\param	memoryPolicy police for the guiporpertytable WITHOUT_MEMORY or WITH_MEMORY. */
//! \throw  NoValidMemoryPolicyException
void GuiPropertyTableRef::setMemoryPolicy( MemoryPolicyEnum memoryPolicy )
{
	getImpl()->setMemoryPolicy(memoryPolicy);
}

//----------------------------------------------------------------------------
/*!	\throw	no-throw
	\param	pMatcher matcher the event handler belongs to. */
void GuiPropertyTableRef::setMatcher(boost::shared_ptr<Matcher> pMatcher)
{
	getImpl()->setMatcher(pMatcher);
}	
//----------------------------------------------------------------------------
/*!	\throw	no-throw */
void GuiPropertyTableRef::removeMatcher()
{
	getImpl()->removeMatcher();
}

/*!	\throw no-throw */
void GuiPropertyTableRef::reset()
{
	m_pGuiPropertyTable.reset();
}

/*!	\throw no-throw
	\retval true if shared pointer to implementation object is null
	\return otherwise returns false */
bool GuiPropertyTableRef::isNull() const
{
	return (0 == m_pGuiPropertyTable.get());
}

/*!	\throw  NullReferenceException
	\return pointer to GuiPropertyTable */
const boost::shared_ptr<GuiPropertyTable>& GuiPropertyTableRef::getImpl() const
{
	if (m_pGuiPropertyTable.get() == 0)
	{
		ExceptInfo sInfo("basar.gui.tie.GuiPropertyTableRef.getImpl()", "Reference of GuiPropertyTable - no pointer to impl-class", __FILE__, __LINE__);
		throw NullReferenceException(sInfo);
	}
	return m_pGuiPropertyTable;
}

/*!	\throw  no-throw
	\return pointer to Reference of GuiPropertyTable 
	\param  pGuiPropertyTable shared pointer to GuiPropertyTable*/
GuiPropertyTableRef::GuiPropertyTableRef(boost::shared_ptr<GuiPropertyTable> pGuiPropertyTable) : m_pGuiPropertyTable(pGuiPropertyTable)
{
}

/*!	\throw  no-throw
	\return pointer to Reference of CacheController */
GuiPropertyTableRef::GuiPropertyTableRef()
{
}

bool GuiPropertyTableRef::operator==(const GuiPropertyTableRef & a) const
{
   return ( m_pGuiPropertyTable.get() == a.m_pGuiPropertyTable.get());
}
} // tie
} // gui
} // basar
