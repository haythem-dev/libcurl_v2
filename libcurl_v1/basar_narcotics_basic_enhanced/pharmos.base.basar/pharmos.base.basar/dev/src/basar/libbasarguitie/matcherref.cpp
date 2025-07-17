//------------------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  reference to all functions of matcher guitie 
 *  \author Roland Kiefert
 *  \date   20.09.2005
 */
//------------------------------------------------------------------------------------------------------------//

//------------------------------------------------------------------------------------------------------------//
// includes
//------------------------------------------------------------------------------------------------------------//

#include "libbasarguitie_matcher.h"
#include "matcher.h"
#include "libbasardbaspect_accessorpropertytable_yiterator.h"

//------------------------------------------------------------------------------------------------------------//
// namespaces
//------------------------------------------------------------------------------------------------------------//
namespace basar {
namespace gui {
namespace tie {

//------------------------------------------------------------------------------------------------------------//
void MatcherRef::clear()
{
	getImpl()->clear();
}

//------------------------------------------------------------------------------------------------------------//
void MatcherRef::removeMatcherEvents()
{
	getImpl()->removeMatcherEvents();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return reference to AccessorPropertyTable
	\n no-throw */
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
db::aspect::AccessorPropertyTableRef MatcherRef::getRight() const
{
	return getImpl()->getRight();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return reference to GuiPropertyTable
	\n no-throw */
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
GuiPropertyTableRef MatcherRef::getLeft() const
{
	return getImpl()->getLeft();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \param  policy the policy of the matcher
	\n		no-throw */
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MatcherRef::setMatchingPolicy(const Int32 policy)
{
	return getImpl()->setMatchingPolicy(policy);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*! matches whole PropertyTable from Right To Left (acc to gui) 
    \n no-throw
    \retval number of rows matched
*/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Int32 MatcherRef::RightToLeft() 
{
	return getImpl()->RightToLeft();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*! matches whole PropertyTable from Left To Right (gui to acc) 
   \n \retval number of rows matched
    \n no-throw */
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Int32 MatcherRef::LeftToRight()
{
	return getImpl()->LeftToRight();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*! RightToLeft for an array of rows, from index count number of rows.
    The count value may be negative:
	<ul> 
	   <li> interpretation of the parameters count 
	     <ul>
	        <li> count > 0. as number of rows (If there are less rows than
			     count, the matching stops without an exception) </li>
            <li> count < 0 as number of rows, before the index, if the 
			     possible number of rows is less than abs(count), an Exception
			     is thrown after all the other rows (index to 0)
				 are matched ). </li>
            <li> count = 0 for index > 0 no lines are matched, 
			     when the index is 0 all lines are matched </li>
            <li> If the policy DISREGARD_DELETED is used, only non deleted 
			     rows are counted </li>
	     </ul>
	   </li>
	   <li> interpretation of the parameters index  (start line for matching)
	       <ul>
             <li> If the policy DISREGARD_DELETED is used, only non deleted 
			      rows are counted </li>
	         <li> index < 0 returns an exception </li>
			 <li> index = 0 and count = 0 all lines are matched </li>
			 <li> index >= 0 (start index for matching) </li>
			 <li> Is the index grater than the "last possible index + 1" an 
			      Exception is thrown, if the Index is "last possible index +1"
				  no exception is thrown and no line is matched (This speciality
				  is because of non "FULL_CACHING" policies, that do not know the 
				  last possible index) </li>
             <li> Is the index not possible in the gui Propertylist (not INSERT), 
			      no exception is thrown, and no line is matched (the possiblility 
				  of the index in the Accessor propertyTable is not checked.) </li>
	      </ul>
	   </li>
	</ul>
    \retval returns the number of matched rows
    \param  index position where matching is started.
	\param  count number of rows to match.
	\throw	UnknownIndexException */
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Int32 MatcherRef::RightToLeft(const Int32 index, const Int32 count)
{
	return getImpl()->RightToLeft(index, count);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \param  index position where matching is started.
	\param  count number of rows to match.
	\retval returns the number of matched rows
	\throw	UnknownIndexException */
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Int32 MatcherRef::LeftToRight(const Int32 index, const Int32 count)
{
	return getImpl()->LeftToRight(index, count);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \param  guiIt Iterator to GuiPropertyTable where data are matched to
	\param  accIt Iterator to AccessorPropertyTable where data come from 
	\throw	UnknownPropertyTypeException, UnknownColoumnException */
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MatcherRef::RightToLeft(	const GuiPropertyTable_YIterator                  guiIt, 
								const db::aspect::AccessorPropertyTable_YIterator accIt) const
{
	return getImpl()->RightToLeft(guiIt,accIt);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \param  guiIt Iterator to GuiPropertyTable where data come from
	\param  accIt Iterator to AccessorPropertyTable where data are matched to
	\throw UnknownPropertyTypeException, UnknownColoumnException, TypecastFailedException */
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MatcherRef::LeftToRight(	const GuiPropertyTable_YIterator                  guiIt, 
								const db::aspect::AccessorPropertyTable_YIterator accIt) const
{
	return getImpl()->LeftToRight(guiIt,accIt);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \param     guiIt  Iterator to GuiPropertyTable where data are matched to
	\param     accIt  Iterator to AccessorPropertyTable where data come from
	\param     policy the policy of the matcher
	\attention transforming is not executed 
	\throw	UnknownPropertyTypeException, UnknownColoumnException */
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MatcherRef::RightToLeft(	const GuiPropertyTable_YIterator                  guiIt, 
								const db::aspect::AccessorPropertyTable_YIterator accIt, 
								const Int32 policy)
{
	Matcher::RightToLeft(guiIt,accIt,policy);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \throw no-throw  
	\return yiterator to the current line. if there's no current line return -1 */
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
GuiPropertyTable_YIterator MatcherRef::getCurrentLeft()
{
	return getImpl()->getCurrentLeft();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \throw no-throw  
	\return yiterator to the current line. if there's no current line return -1 */
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
db::aspect::AccessorPropertyTable_YIterator MatcherRef::getCurrentRight()
{
	return getImpl()->getCurrentRight();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \throw no-throw  
	\return yiterator to the first selected line. if there's no line selected return end iterator */
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
GuiPropertyTable_YIterator MatcherRef::getSelectedLeft()
{
	return getImpl()->getSelectedLeft();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \throw no-throw  
	\return yiterator to the next selected line. if there's no line selected return end iterator */
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
GuiPropertyTable_YIterator MatcherRef::getNextSelectedLeft()
{
	return getImpl()->getNextSelectedLeft();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \throw no-throw  
	\return yiterator to the first selected line. if there's no line selected return end iterator */
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
db::aspect::AccessorPropertyTable_YIterator MatcherRef::getSelectedRight()
{
	return getImpl()->getSelectedRight();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \throw no-throw  
	\return yiterator to the next selected line. if there's no line selected return end iterator */
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
db::aspect::AccessorPropertyTable_YIterator MatcherRef::getNextSelectedRight()
{
	return getImpl()->getNextSelectedRight();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \param     guiIt  Iterator to GuiPropertyTable where data come from
	\param     accIt  Iterator to AccessorPropertyTable where data are matched to
	\param     policy the policy of the matcher
	\attention transforming is not executed 
	\throw UnknownPropertyTypeException, UnknownColoumnException, TypecastFailedException */
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MatcherRef::LeftToRight(const GuiPropertyTable_YIterator            guiIt, 
							 db::aspect::AccessorPropertyTable_YIterator accIt, 
							 Int32 policy)
{
	Matcher::LeftToRight(guiIt,accIt,policy);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \param transformer transformer to add to the matcher
	\throw no-throw 
	\return index of the transformer in the matcher */
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TransformerIndex MatcherRef::push_back(Transformer& transformer)
{
	return getImpl()->push_back(transformer);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \param pMatcher pointer to the impl class matcher.
 	\throw no-throw */
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
MatcherRef::MatcherRef(boost::shared_ptr<Matcher> pMatcher) : m_pMatcher(pMatcher)
{
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw */
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
MatcherRef::MatcherRef()
{
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \n no-throw 
	\retval true if shared pointer to implementation class is null
	\return otherwise returns false */
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const bool MatcherRef::isNull() const
{
	return (0 == m_pMatcher.get());
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \throw no-throw */
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MatcherRef::reset()
{
	m_pMatcher.reset();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw  NullReferenceException
	\return pointer to matcher implementation */
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const boost::shared_ptr<Matcher>& MatcherRef::getImpl() const
{
	if (m_pMatcher.get() == 0)
	{
		ExceptInfo sInfo("basar.gui.tie.MatcherRef.getImpl", "Reference of Matcher - no pointer to impl-class", __FILE__, __LINE__);
		throw NullReferenceException(sInfo);
	}
	return m_pMatcher;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \brief connects all column header labels with the original attribute names.\n no-throw */
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MatcherRef::transformProperties( const std::list<VarString>& headerLabels, 
									  const std::list<VarString>& originalAttributeNames )
{
	return getImpl()->transformProperties( headerLabels, originalAttributeNames );
}


} // matcher
} // db
} // basar
