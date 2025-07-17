//--------------------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  all function implementation matcher guitie
 *  \author Roland Kiefert
 *  \date   20.09.2005
 */
//--------------------------------------------------------------------------------------------------------------//

//--------------------------------------------------------------------------------------------------------------//

#include "matcher.h"

#include <set>
#include "libbasarproperty_propertytable_xiterator.h"
#include "libbasarguitie_exceptions.h"
#include "loggerpool.h"
#include "helper.h"
#include "libbasardbaspect_accessorpropertytable_yiterator.h"

//--------------------------------------------------------------------------------------------------------------//
// namespace
//--------------------------------------------------------------------------------------------------------------//
namespace basar {
namespace gui {
namespace tie {

//--------------------------------------------------------------------------------------------------------------//
void Matcher::clear()
{
	removeMatcherEvents();
	m_AccPropTable.reset();
	m_GuiPropTable.reset();
}

//--------------------------------------------------------------------------------------------------------------//
void Matcher::removeMatcherEvents()
{
   m_GuiPropTable.removeMatcher();
}

//--------------------------------------------------------------------------------------------------------------//
/*! \return reference to AccessorPropertyTable
	\throw	no-throw */
Matcher::AccPropTblRef Matcher::getRight()
{
	return m_AccPropTable;
}

//--------------------------------------------------------------------------------------------------------------//
/*! \return reference to GuiPropertyTableRef
	\throw	no-throw */
GuiPropertyTableRef Matcher::getLeft()
{
	return m_GuiPropTable;
}

//--------------------------------------------------------------------------------------------------------------//
/*! \param  policy the policy of the matcher
	\throw	no-throw */
void Matcher::setMatchingPolicy( const MatcherPolicyType policy )
{
    if (m_Policy != policy)
         m_IndexTable.clear();

	m_Policy = policy;
}

//--------------------------------------------------------------------------------------------------------------//
/*!	\throw	no-throw 
 *  \retval returns the number of matched rows
 */
Int32 Matcher::RightToLeft()
{
    Int32                      retval = 0;
	GuiPropertyTable_YIterator guiIt  = getLeft().begin();

	for (AccPropTblYIterator accIt = getRight().begin(); 
		 accIt != getRight().end(); 
		 ++accIt)
	{
		if (checkPolicy(DISREGARD_DELETED))
		{
			if (accIt.getState().contains(basar::SS_DELETE))
				continue;
		}

		if (guiIt == getLeft().end())
		{
			if (checkPolicy(INSERT))
				guiIt = getLeft().insert();
			else
				break;
		}

		innerRightToLeft(guiIt, accIt);

		++guiIt;
        ++retval;
	}

    guiIt.notifyMatchDone();

	return retval;
}

//--------------------------------------------------------------------------------------------------------------//
/*! \param  guiIt Iterator to GuiPropertyTable where data are matched to
    \param  accIt Iterator to AccessorPropertyTable where data come from
    \throw	UnknownPropertyTypeException, UnknownColoumnException */
void Matcher::RightToLeft(GuiPropertyTable_YIterator guiIt, 
                          const AccPropTblYIterator  accIt)
{

    RightToLeftInternal(guiIt, accIt);

    guiIt.notifyMatchDone();
}

//--------------------------------------------------------------------------------------------------------------//
/*! \param  guiIt Iterator to GuiPropertyTable where data are matched to
	\param  accIt Iterator to AccessorPropertyTable where data come from
	\param  policy the policy of the matcher
	\attention transforming is not executed 
	\throw	UnknownPropertyTypeException, UnknownColoumnException */
void Matcher::RightToLeft(	GuiPropertyTable_YIterator guiIt, 
							const AccPropTblYIterator  accIt, 
							const MatcherPolicyType    policy)
{
	BLOG_TRACE( LoggerPool::matcher(), "RightToLeft - static method with policy." );

	if (accIt.isEnd()) 
	{
		BLOG_TRACE(LoggerPool::matcher(), "RightToLeft - AccessorPropertyTable_YIterator is at end()");
	}
	else
	{
        std::list<IndexStruct> helplist;

        fillIndexStruct (helplist, guiIt, accIt, policy);
        setGuiPropTblYIt(helplist, guiIt, accIt);

        guiIt.notifyMatchDone();

	}

	BLOG_TRACE(LoggerPool::matcher(), "RightToLeft - matching completed.");
}

//--------------------------------------------------------------------------------------------------------------//
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
	\throw	UnknownIndexException 
*/
Int32 Matcher::RightToLeft(const Int32 index, const Int32 count)
{
    if (index == 0 && count == 0)
        return RightToLeft();

    if (index < 0)
    {
        basar::VarString msg;
        msg.format("row count <%d> + index <%d> are unknown in AccessorPropertyTable", 
			       count, 
				   index);
        ExceptInfo sInfo("basar.gui.tie.Matcher.RightToLeft()", msg, __FILE__, __LINE__);
        throw UnknownIndexException(sInfo);
    }

	Int32                      retval = 0;
    AccPropTblYIterator        accIt  = getRight().begin();
    GuiPropertyTable_YIterator guiIt  = getLeft ().begin();

    if (step2Idx4Right2Left(index, guiIt, accIt))
    {
        if (count > 0)
        {
            for (Int32 j = 0; j < count && accIt != getRight().end(); ++j, ++accIt, ++guiIt)
            {
                if (checkPolicy(DISREGARD_DELETED )) 
                {
                    while (!accIt.isEnd() && accIt.getState().contains(basar::SS_DELETE)) 
                    {
                        ++accIt;
                    }

					// no more lines in Accessor, so we stop:
                    if (accIt.isEnd())
                        break ;
                }

                if (guiIt == getLeft().end())
                {
                    if (checkPolicy(INSERT))
                        guiIt = getLeft().insert();
                    else
                        // no more lines in Gui, so we stop:
                        break;
                }

                RightToLeftInternal(guiIt, accIt);
                ++retval;
            }
        }
        else
        {
            for (Int32 j = 0; j > count; --accIt, --guiIt)
            {
				if ( !skipDeletedBackward(accIt) )
                {
                    // we are at the beginning
                    //break;
                    basar::VarString msg;
                    msg.format("row count <%d> + index <%d> are unknown in AccessorPropertyTable", 
								count, 
								index);
                    ExceptInfo sInfo("basar.gui.tie.Matcher.RightToLeft()", msg, __FILE__, __LINE__);
                    throw UnknownIndexException(sInfo); 
                }

                RightToLeftInternal(guiIt, accIt);
                ++retval;
                --j;

                // break, befor we go beyond the first, throw an Exception if there is count left
                if (checkIsBegin(guiIt, accIt, (count < j), "basar.gui.tie.Matcher.RightToLeft()", index, count))
                    break;
            }
        }
    }

    guiIt.notifyMatchDone();

    return retval;
}

//--------------------------------------------------------------------------------------------------------------//
/*! \throw	no-throw
 *  \retval returns the number of matched rows
 */
Int32 Matcher::LeftToRight()
{
    Int32                      retval = 0;
	AccPropTblYIterator        accIt  = getRight().begin();
	GuiPropertyTable_YIterator guiIt  = getLeft ().begin();

	for (Int32 i = 0; i < guiIt.getAbsolutePosition(); ++i, ++accIt)
	{
		if (checkPolicy(DISREGARD_DELETED))
		{
			if (accIt.getState().contains(basar::SS_DELETE))
				continue;
		}

		if (accIt != getRight().end())
		{
			if (checkPolicy( INSERT))
				accIt = getRight().insert(basar::FOR_INSERT);
			else
				break;
		}
	}

	for (; guiIt != getLeft().end(); ++guiIt)
	{
        skipDeletedForward(accIt);

		if (accIt == getRight().end())
		{
			if (checkPolicy(INSERT))
				accIt = getRight().insert(basar::FOR_INSERT);
			else
				break;
		}

		LeftToRight(guiIt, accIt);
		++accIt;
        ++retval;
	}

    return retval;
}

//--------------------------------------------------------------------------------------------------------------//
/*! \param  guiIt Iterator to GuiPropertyTable where data come from
	\param  accIt Iterator to AccessorPropertyTable where data are matched to
	\throw	UnknownPropertyTypeException, UnknownColoumnException, TypecastFailedException */
void Matcher::LeftToRight(	const GuiPropertyTable_YIterator guiIt, 
							const AccPropTblYIterator        accIt)
{
	VarString errorMsg;

	if (accIt.isEnd()) 
    {
		BLOG_TRACE(LoggerPool::matcher(), "LeftToRight - AccessorPropertyTable_YIterator is at end()");
	}
    else if (accIt.getPropertyTable() == m_AccPropTable  && 
             guiIt.getPropertyTable() == m_GuiPropTable     )
    {
        innerLeftToRight(guiIt, accIt);
    }
    else  
    { 
        BLOG_TRACE( LoggerPool::matcher(), "LeftToRight - AccessorPropertyTable_YIterator different PropTab." );

        std::list<IndexStruct> helplist;

        fillIndexStruct    (helplist, guiIt, accIt, m_Policy);
        setAccPropTblYIt   (helplist, guiIt, accIt);
        transformLeft2Right(guiIt, accIt);
    }
}

//--------------------------------------------------------------------------------------------------------------//
/*! \param  guiIt Iterator to GuiPropertyTable where data come from
	\param  accIt Iterator to AccessorPropertyTable where data are matched to
	\param  policy the policy of the matcher
	\attention transforming is not executed 
	\throw	UnknownPropertyTypeException, UnknownColoumnException, TypecastFailedException */
void Matcher::LeftToRight(const GuiPropertyTable_YIterator guiIt, 
						  const AccPropTblYIterator        accIt, 
						  const MatcherPolicyType          policy)
{
	BLOG_TRACE( LoggerPool::matcher(), "LeftToRight - static method with policy." );

	if (accIt.isEnd()) 
    {
		BLOG_TRACE(LoggerPool::matcher(), "LeftToRight - AccessorPropertyTable_YIterator is at end()");
	}
    else  
    { 
        std::list<IndexStruct> helplist;
        fillIndexStruct (helplist, guiIt, accIt, policy);
        setAccPropTblYIt(helplist, guiIt, accIt);
    }

	BLOG_TRACE(LoggerPool::matcher(), "LeftToRight - matching completed.");
}

//--------------------------------------------------------------------------------------------------------------//
/*! LeftToRight for an array of rows, from index count number of rows.
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
			 <li> Is the index grater than the "last possible index" an 
			      Exception is thrown </li>
	      </ul>
	   </li>
	</ul>

	\param  index position where matching is started.
	\param  count number of rows to match.
	\retval returns the number of matched rows
	\throw	UnknownIndexException 
*/
Int32 Matcher::LeftToRight(const Int32 index, const Int32 count)
{
    if (index == 0 && count == 0)
        return LeftToRight();

    if (index < 0)
    {
        basar::VarString msg;
        msg.format("row count <%d> + index <%d> are unknown in AccessorPropertyTable", 
			       count, 
				   index);
        ExceptInfo sInfo("basar.gui.tie.Matcher.LeftToRight()", msg, __FILE__, __LINE__);
        throw UnknownIndexException(sInfo);
    }

	Int32                      retval = 0;
    AccPropTblYIterator        accIt  = getRight().begin();
    GuiPropertyTable_YIterator guiIt  = getLeft ().begin();

    if (step2Idx4Left2Right(index, guiIt, accIt))
    {
        if (count > 0)
        {
            for (Int32 j = 0; j < count; ++j, ++accIt, ++guiIt)
            {
				skipDeletedForward(accIt);

				if (accIt == getRight().end() && ! checkPolicy(INSERT))
					break;

				if (guiIt == getLeft().end())
                {
                    basar::VarString msg;
                    msg.format(" index <%d> + row count<%d> are unknown in GuiPropertyTable", 
							    index, 
								count);
                    ExceptInfo sInfo("basar.gui.tie.Matcher.LeftToRight()", msg, __FILE__, __LINE__);
                    throw UnknownIndexException(sInfo); 
                }

                if (accIt == getRight().end())
                {
                    if (checkPolicy(INSERT))
                        accIt = getRight().insert(basar::FOR_INSERT);
                    else
                        break;
                }

                LeftToRight(guiIt, accIt);
                ++retval;
            }
        }
        else
        {
            for (Int32 j = 0; j > count; --accIt, --guiIt)
            {
				if ( !skipDeletedBackward(accIt) )
                {
                    // we are at the beginning
                    //break;
                    basar::VarString msg;
                    msg.format("row count <%d> + index <%d> are unknown in AccessorPropertyTable", 
								count, 
								index);
                    ExceptInfo sInfo("basar.gui.tie.Matcher.LeftToRight()", msg, __FILE__, __LINE__);
                    throw UnknownIndexException(sInfo); 
                }
                
                LeftToRight(guiIt, accIt);
                ++retval;
                --j;

                // break, befor we go beyond the first, throw an Exception if there is count left
                if (checkIsBegin(guiIt, accIt, (count < j), "basar.gui.tie.Matcher.LeftToRight()", index, count))
                    break;
            }
        }
    }

    return retval;
}

//--------------------------------------------------------------------------------------------------------------//
/*! \throw no-throw 
	\return yiterator to the first selected line. if no line selected return end iterator */
GuiPropertyTable_YIterator Matcher::getCurrentLeft()
{
    GuiPropertyTable_YIterator it = getLeft().end();

    if (!(getLeft().isMultiLinePropertyTable()))
    {
        it = getLeft().begin();

        if (LoggerPool::matcher().isEnabledFor(log4cplus::TRACE_LOG_LEVEL))
		{
            LoggerPool::matcher().forcedLog(
				log4cplus::TRACE_LOG_LEVEL, 
				(getLeft().end() == it) ? "getCurrentLeft(): no valid iterator"
										: "getCurrentLeft() - returns begin()",
				__FILE__, __LINE__);
		}
    }
    else
    {
        Int32 index = getLeft().getCurrentIndex();
        if (index < 0)
        {
            BLOG_TRACE(LoggerPool::matcher(), "getCurrentLeft(): gui index is not valid.");
            it =  getLeft().end();
        }
        else
        {
            for (it = getLeft().begin(); it != getLeft().end(); ++it)
            {
                if (it.getAbsolutePosition() == index)
                    break;
            }

            if (LoggerPool::matcher().isEnabledFor(log4cplus::TRACE_LOG_LEVEL))
            {
                VarString msg;
                msg.format("getCurrentLeft() - current absolute gui index is <%d>", index);
                LoggerPool::matcher().forcedLog(log4cplus::TRACE_LOG_LEVEL, 
					                                msg,
													__FILE__, __LINE__);
            }
        }
    }

    return it;
}

//--------------------------------------------------------------------------------------------------------------//
/*! \throw no-throw 
	\return yiterator to the first selected line. if no line selected return end iterator */
Matcher::AccPropTblYIterator Matcher::getCurrentRight()
{
	if (!(getLeft().isMultiLinePropertyTable()))
		return getRight().begin();

	Int32 index = getLeft().getCurrentIndex();

	if (index < 0)
	{
		BLOG_TRACE(LoggerPool::matcher(), "getCurrentRight(): accessor index is not valid");
		return getRight().end();
	}

	AccPropTblYIterator it = getRight().begin();

	for (basar::Int32 i = 0; it != getRight().end(); ++it, ++i)
	{
		skipDeletedForward(it);

		if (i == index)
			break;
	}
	
	if (LoggerPool::matcher().isEnabledFor(log4cplus::TRACE_LOG_LEVEL))
	{
		VarString msg;
		msg.format("getCurrentRight() - current absolute accessor index is <%d>", index);
		LoggerPool::matcher().forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	return it;
}

//--------------------------------------------------------------------------------------------------------------//
/*! \throw no-throw 
	\return yiterator to the first selected line. if no line selected return end iterator */
GuiPropertyTable_YIterator Matcher::getSelectedLeft()
{
	m_LeftIndices = getLeft().getSelectedIndices();

	if (m_LeftIndices.empty())
	{
		BLOG_TRACE(LoggerPool::matcher(), "getSelectedLeft(): no gui rows selected.");
		return getLeft().end();
	}

	m_LeftIndex = m_LeftIndices.begin();

	GuiPropertyTable_YIterator yIt = getLeft().begin();

	for (; yIt != getLeft().end(); ++yIt)
	{
		if (yIt.getAbsolutePosition() == *(m_LeftIndex))
			break;
	}

	return yIt;
}

//--------------------------------------------------------------------------------------------------------------//
/*! \throw no-throw 
	\return yiterator to the next selected line. if no line selected return end iterator */
GuiPropertyTable_YIterator Matcher::getNextSelectedLeft()
{
	if (m_LeftIndex == m_LeftIndices.end()) 
		return getLeft().end();

	++m_LeftIndex;

	if (m_LeftIndex == m_LeftIndices.end()) 
		return getLeft().end();

	GuiPropertyTable_YIterator yIt = getLeft().begin();

	for (; yIt != getLeft().end(); ++yIt)
	{
		if (yIt.getAbsolutePosition() == *(m_LeftIndex))
				break;
	}

	return yIt;
}

//--------------------------------------------------------------------------------------------------------------//
/*! \throw no-throw 
	\return yiterator to the first selected line. if no line selected return end iterator */
Matcher::AccPropTblYIterator Matcher::getSelectedRight()
{
	m_RightIndices = getLeft().getSelectedIndices();

	if (m_RightIndices.empty())
	{
		BLOG_TRACE(LoggerPool::matcher(), "getSelectedRight(): no gui rows selected.");
		m_RightIndex = m_RightIndices.end();
		return getRight().end();
	}

	AccPropTblYIterator	yIt = getRight().begin();

	m_RightIndex = m_RightIndices.begin();

	for (Int32 index = 0; yIt != getRight().end(); ++index,++yIt)
	{
		skipDeletedForward(yIt);

		if (index == *(m_RightIndex))
			break;
	}

	return yIt;
}

//--------------------------------------------------------------------------------------------------------------//
/*! \throw no-throw 
	\return yiterator to the next selected line. if no line selected return end iterator */
Matcher::AccPropTblYIterator Matcher::getNextSelectedRight()
{
	if (m_RightIndex == m_RightIndices.end()) 
		return getRight().end();

	++m_RightIndex;

	if (m_RightIndex == m_RightIndices.end()) 
		return getRight().end();

	AccPropTblYIterator	yIt = getRight().begin();

	for (Int32 index = 0; yIt != getRight().end(); ++yIt,++index)
	{
		skipDeletedForward(yIt);

		if (index == *(m_RightIndex))
			break;
	}

	return yIt;
}

//--------------------------------------------------------------------------------------------------------------//
/*!	\throw  no-throw
	\param	guiPropTable reference to gui property table
	\param	accPropTable reference to accessor property table
	\return pointer to matcher itself */
Matcher::MatcherSharedPtr Matcher::create(const GuiPropertyTableRef guiPropTable, 
										  const AccPropTblRef       accPropTable)
{
	MatcherSharedPtr t = MatcherSharedPtr(new Matcher(guiPropTable, accPropTable));
	t->setWeakPtr(t);

	return t;
}

//--------------------------------------------------------------------------------------------------------------//
/*! \return (only one group of) Matcher shared pointer 
	\throw  no-throw */
const Matcher::MatcherSharedPtr Matcher::getThis() const 
{
	return MatcherSharedPtr(m_WeakPtr);
}

//--------------------------------------------------------------------------------------------------------------//
/*! \throw  no-throw
	\param weakPtr weak pointer to the class Matcher */
void Matcher::setWeakPtr( const MatcherWeakPtr weakPtr )
{
	m_WeakPtr = weakPtr;
}

//--------------------------------------------------------------------------------------------------------------//
/*! \param  guiPropTable reference to a gui property table
	\param  accPropTable reference to an accessor property table
	\throw  no-throw */
Matcher::Matcher (const GuiPropertyTableRef guiPropTable, 
                  AccPropTblRef             accPropTable)
                 : m_GuiPropTable(guiPropTable), 
                   m_AccPropTable(accPropTable), 
                   m_Policy      (CONVERTTYPE | INSERT | SKIP)
{
    BLOG_TRACE(LoggerPool::matcher(), "Matcher constructed");
}

//--------------------------------------------------------------------------------------------------------------//
//! \throw	no-throw
//!
Matcher::~Matcher()
{
}

//--------------------------------------------------------------------------------------------------------------//
//! \return	void
//! \param  headerLabels represents all column header labels of the table widget
//! \param  originalAttributeNames represents all original (database specific) attribute names
//! \throw	no-throw 
//!
//--------------------------------------------------------------------------------------------------------------//
void Matcher::transformProperties(const std::list<VarString> & headerLabels,  
								  const std::list<VarString> & originalAttributeNames )
{
	if( headerLabels.size() != originalAttributeNames.size() )
	{
		throw cmnutil::BasarInvalidParameterException(ExceptInfo("basar.gui.tie.Matcher.transformProperties()", 
																 "number of horizontal header labels "
																 "of widget table does not match "
																 "number of properties which have "
																 "to be transformed!",
																 __FILE__, __LINE__ ));
	}
	
	std::ostringstream transformedProperties;

	for (std::list<VarString>::const_iterator it_header_labels  = headerLabels          .begin(), 
		                                      it_original_names = originalAttributeNames.begin();
		 it_header_labels != headerLabels.end(); 
		 ++it_header_labels, ++it_original_names )
	{
		transformedProperties << (*it_original_names) << " = " << (*it_header_labels) << ";";
	}

	PropertyNameTransformer propertyNameTransformer( transformedProperties.str().c_str() );
	push_back( propertyNameTransformer );

	if (LoggerPool::matcher().isEnabledFor(log4cplus::TRACE_LOG_LEVEL))
	{
		VarString msg;
		msg.format("transformProperties() - transformed properties: <%s>",
					transformedProperties.str().c_str());
		LoggerPool::matcher().forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}
}

//--------------------------------------------------------------------------------------------------------------//
/*! \param transformer transformer to add to the matcher
	\throw no-throw 
	\return index of the transformer in the matcher */
//--------------------------------------------------------------------------------------------------------------//
const TransformerIndex Matcher::push_back(Transformer & transformer)
{
	Transformer::TransformSharedPtr t(transformer.create());
	m_TransformerList.push_back(t);

	if (LoggerPool::matcher().isEnabledFor(log4cplus::TRACE_LOG_LEVEL))
	{
		VarString msg;
		msg.format("new transformer push backed at index <%d>", m_TransformerList.size() - 1);
		LoggerPool::matcher().forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	return static_cast<TransformerIndex>(m_TransformerList.size()-1);
}

//--------------------------------------------------------------------------------------------------------------//
/*! internal function for RightToLeft */
inline void Matcher::innerRightToLeft(GuiPropertyTable_YIterator guiIt, 
                               const AccPropTblYIterator  accIt)
{
    if (accIt.isEnd())
    {
        BLOG_TRACE( LoggerPool::matcher(), "innerRightToLeft - AccessorPropertyTable_YIterator is at end()" );
    }
    else
    {
        if (m_IndexTable.empty())
        {
            BLOG_TRACE(LoggerPool::matcher(), "innerRightToLeft - index table empty");

            try
            {
				fillIndexStruct(m_IndexTable, guiIt, accIt, m_Policy);
            }
            catch (basar::Exception & )
            {
				m_IndexTable.clear();
				throw;
            }
        }
        else
        {
            BLOG_TRACE(LoggerPool::matcher(), "innerRightToLeft - index table NOT empty.");
        }

        setGuiPropTblYIt   (m_IndexTable, guiIt, accIt);
        transformRight2Left(guiIt, accIt);
    }
}

//--------------------------------------------------------------------------------------------------------------//
/*! internal function for LeftToRight
 *  @param guiIt The GuiPropertyTable_YIterator, the values are gotten from
 *  @param accIt The AccessorPropertyTable_YIterator, the values are written to
 */
inline void Matcher::innerLeftToRight(const GuiPropertyTable_YIterator guiIt, 
                               AccPropTblYIterator              accIt )
{
    if (accIt.isEnd())
    {
        BLOG_TRACE( LoggerPool::matcher(), "innerLeftToRight - AccessorPropertyTable_YIterator is at end()" );
    }
    else
    {
        if (m_IndexTable.empty())
        {
            BLOG_TRACE(LoggerPool::matcher(), "innerLeftToRight - index table empty");

            try
            {
				fillIndexStruct(m_IndexTable, guiIt, accIt, m_Policy);
            }
            catch (basar::Exception & )
            {
				m_IndexTable.clear();
				throw;
            }
        }
        else
        {
            BLOG_TRACE(LoggerPool::matcher(), "innerLeftToRight - index table NOT empty");
        }

        setAccPropTblYIt   (m_IndexTable, guiIt, accIt);
        transformLeft2Right(guiIt, accIt);
    }
}

//--------------------------------------------------------------------------------------------------------------//
/*! internal function: use of Transformer for LeftToRight
 *  @param accIt The AccessorPropertyTable_YIterator, the values are gotten from
 *  @param guiIt The GuiPropertyTable_YIterator, the values are written to
 */
void Matcher::transformRight2Left(GuiPropertyTable_YIterator guiIt, 
                                  const AccPropTblYIterator  accIt )
{
	std::list<Transformer::TransformSharedPtr>::const_iterator itEnd = m_TransformerList.end();

    for (std::list<Transformer::TransformSharedPtr>::iterator itCur = m_TransformerList.begin(); 
		 itCur != itEnd; 
		 ++itCur)
    {
        (*itCur)->RightToLeft(guiIt, accIt);
    }

    BLOG_TRACE(LoggerPool::matcher(), "transformRight2Left - transforming completed.");
}

//--------------------------------------------------------------------------------------------------------------//
/*! used transformer on LeftToRight
 *  @param guiIt The GuiPropertyTable_YIterator the values are gotten from
 *  @param accIt The AccessorPropertyTable_YIterator the values are written to
 */
void  Matcher::transformLeft2Right(const GuiPropertyTable_YIterator guiIt, 
                                   AccPropTblYIterator              accIt )
{
	std::list<Transformer::TransformSharedPtr>::const_iterator itEnd = m_TransformerList.end();
    
	for (std::list<Transformer::TransformSharedPtr>::iterator itCur = m_TransformerList.begin(); 
		 itCur != itEnd; 
		 ++itCur)
    {
        (*itCur)->LeftToRight(guiIt, accIt);
    }

    BLOG_TRACE(LoggerPool::matcher(), "transformLeft2Right - transforming completed.");
}

//--------------------------------------------------------------------------------------------------------------//
/*! writes one value to GuiPropertyTable_YIterator (defined by index)
 *  @param index defines the interconnection between the AccessorPropertyTable_YIterator
 *               and the GuiPropertyTable_YIterator for one key (columnname) as
 *               indices.
 *  @param accIt The AccessorPropertyTable_YIterator, the values are gotten from
 *  @param guiIt The GuiPropertyTable_YIterator, the values are written to 
 */
void Matcher::setGuiPropTblYIt(IndexStruct              & index, 
							   GuiPropertyTable_YIterator guiIt,
							   const AccPropTblYIterator  accIt)
{
	switch (index.guiType.getType())
	{
	case basar::STRING:
		{
			I18nString tempString;

			if (!Helper::getStringFromAcc(accIt, index.accProperty, tempString, index.accType))
			{
				// ignore empty string.
				if (tempString != "")
				{
					basar::VarString msg;
					msg.format("accessor type <%d> is unknown", index.accType);
					ExceptInfo sInfo("basar.gui.tie.Matcher.setGuiPropTblYIt()", msg, __FILE__, __LINE__);
					throw UnknownPropertyTypeException(sInfo);
				}
			}
			else
				guiIt.setString(index.guiProperty, tempString);
		}
		break;

	case basar::TIME:
		{
			Time tempTime;

			if (!Helper::getTimeFromAcc(accIt, index.accProperty, tempTime, index.accType))
			{
				basar::VarString msg;
				msg.format("accessor type <%d> can not be casted to guiType TIME", index.accType);
				ExceptInfo sInfo("basar.gui.tie.Matcher.setGuiPropTblYIt()", msg, __FILE__, __LINE__);
				throw UnknownPropertyTypeException(sInfo);
			}

			guiIt.setTime(index.guiProperty, tempTime);
		}
		break;

	case basar::DATE:
		{
			Date tempDate;

			if (!Helper::getDateFromAcc(accIt, index.accProperty, tempDate, index.accType))
			{
				basar::VarString msg;
				msg.format("accessor type <%d> can not be casted to guiType DATE", index.accType);
				ExceptInfo sInfo("basar.gui.tie.Matcher.setGuiPropTblYIt()", msg, __FILE__, __LINE__);
				throw UnknownPropertyTypeException(sInfo);
			}
		
			guiIt.setDate(index.guiProperty, tempDate);
		}
		break;

	case basar::DATETIME:
		{
			DateTime tempDateTime;

			if (!Helper::getDateTimeFromAcc(accIt, index.accProperty, tempDateTime, index.accType))
			{
				basar::VarString msg;
				msg.format("accessor type <%d> can not be casted to guiType DATETIME", index.accType);
				ExceptInfo sInfo("basar.gui.tie.Matcher.setGuiPropTblYIt()", msg, __FILE__, __LINE__);
				throw UnknownPropertyTypeException(sInfo);
			}

			guiIt.setDateTime(index.guiProperty, tempDateTime);
		}
		break;

	default:
		{
			basar::VarString msg;
			msg.format("gui type <%d> is unknown", index.guiType);
			ExceptInfo sInfo("basar.gui.tie.Matcher.setGuiPropTblYIt()", msg, __FILE__, __LINE__);
			throw UnknownPropertyTypeException(sInfo);
		}
	}
}

//--------------------------------------------------------------------------------------------------------------//
/*! writes  values to GuiPropertyTable_YIterator (defined by index)
 *  @param indexlist defines the interconnection between the AccessorPropertyTable_YIterator
 *                   and the GuiPropertyTable_YIterator for all columns as indices.
 *  @param accIt The AccessorPropertyTable_YIterator, the values are gotten from
 *  @param guiIt The GuiPropertyTable_YIterator, the values are written to 
 */
void Matcher::setGuiPropTblYIt(std::list<IndexStruct>   & indexlist, 
							   GuiPropertyTable_YIterator guiIt,
							   const AccPropTblYIterator  accIt)
{
    for (std::list<IndexStruct>::iterator iterIndex = indexlist.begin(); 
		 iterIndex != indexlist.end(); 
		 ++iterIndex)
    {
        setGuiPropTblYIt(*iterIndex, guiIt, accIt);
    }

    BLOG_TRACE(LoggerPool::matcher(), "setGuiPropTblYIt - matching completed;.");
}

//--------------------------------------------------------------------------------------------------------------//
/*! fills an Index Struct with corresponding indices between GuiPropertyTable_YIterator and 
    AccessorPropertyTable_YIterator; takes the policy into account
 *  @param indexList defines the interconnection between the AccessorPropertyTable_YIterator
 *                   and the GuiPropertyTable_YIterator for all columns as indices.
 *  @param accIt The AccessorPropertyTable_YIterator, the values are gotten from
 *  @param guiIt The GuiPropertyTable_YIterator, the values are written to 
 *  @param policy The matching policy, f.ex. SKIP
 */
void Matcher::fillIndexStruct(std::list<IndexStruct>   & indexList, 
                              GuiPropertyTable_YIterator guiIt, 
                              const AccPropTblYIterator  accIt ,
                              const MatcherPolicyType    policy)
{
    bool existColumn;

    for (property::PropertyTable_XIterator xAccIt = accIt.begin(); xAccIt != accIt.end(); ++xAccIt)
    {
        if (checkPolicy(policy, CONVERTTYPE))
            existColumn = guiIt.contains(xAccIt.getName());
        else 
            existColumn = guiIt.contains(xAccIt.getName(),xAccIt.getType());

        if (!existColumn)
        {
            if (!checkPolicy(policy, SKIP))
            {
                basar::VarString msg;
                msg.format("column <%s> of AccessorPropertyTable_YIterator "
					       "is unknown in GuiProperty_YIterator", 
						   xAccIt.getName().c_str());
                ExceptInfo sInfo("basar.gui.tie.Matcher.fillIndexStruct()", msg, __FILE__, __LINE__);
                throw UnknownColumnException(sInfo); 
            }

            if (LoggerPool::matcher().isEnabledFor(log4cplus::TRACE_LOG_LEVEL))
            {
                VarString msg;
                msg.format("RightToLeft - accessor column <%s> does not exist "
					       "in GuiProperty_YIterator -> skipped",
							xAccIt.getName().c_str());
                LoggerPool::matcher().forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
            }
        }
        else
        {
            if (LoggerPool::matcher().isEnabledFor(log4cplus::TRACE_LOG_LEVEL))
            {
                VarString msg;
                msg.format("RightToLeft - accessor column <%s> is known in GuiProperty_YIterator",
							xAccIt.getName().c_str());
                LoggerPool::matcher().forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
            }

            IndexStruct index;

            index.accProperty = xAccIt.getIndex();
            index.accType     = xAccIt.getType ();
            index.guiProperty = guiIt .getIndex(xAccIt.getName());
            index.guiType     = guiIt .getType (xAccIt.getName());

            indexList.push_back(index);
        }
    }

    if (!checkPolicy(policy, SKIP) )
    {
        if (accIt.getNumberOfColumns() != guiIt.getNumberOfColumns())
        {
            std::set<basar::property::ColumnIndex>  knownIndices;

            for (std::list<IndexStruct>::const_iterator iter = indexList.begin(); 
				 iter != indexList.end(); 
				 ++iter)
            {
                knownIndices.insert(iter->guiProperty);
            }

			for (property::ColumnIndex i = 0; i < guiIt.getNumberOfColumns(); ++i)
            {
                if (knownIndices.find(i) == knownIndices.end())
                {
                    basar::VarString msg;
                    msg.format("column <%s> of GuiPropertyTable_YIterator is unknown in "
						       "AccessorProperty_YIterator", 
								guiIt.getName(i).c_str());
					ExceptInfo sInfo("basar.gui.tie.Matcher.fillIndexStruct()", msg, __FILE__, __LINE__);
                    throw UnknownColumnException(sInfo); 
                }
            }
        }
    }
}

//--------------------------------------------------------------------------------------------------------------//
/*!
 * writes  one value to AccessorPropertyTable_YIterator (defined by index)
 * @param index defines the interconnection between the AccessorPropertyTable_YIterator
 *                  and the GuiPropertyTable_YIterator for one column as indices.
 * @param guiIt The GuiPropertyTable_YIterator, the values are gotten from
 * @param accIt The AccessorPropertyTable_YIterator, the values are written to 
 */
void Matcher::setAccPropTblYIt(IndexStruct                    & index, 
							   const GuiPropertyTable_YIterator guiIt,
							   AccPropTblYIterator              accIt)
{
    if ( !guiIt.isValueExist(index.guiProperty))
		return;

    VarString errorMsg;
    bool      set = true;

	switch (index.guiType.getType())
	{
	case basar::STRING:
        if (guiIt.getString(index.guiProperty).find("TRANSFORMER"))
            set = Helper::setStringToAcc(accIt,
										 index.accProperty, 
										 guiIt.getString(index.guiProperty), 
										 index.accType, 
										 errorMsg);
		break;
    
	case basar::TIME:
        set = Helper::setTimeToAcc(accIt,	
								   index.accProperty, 
								   guiIt.getTime(index.guiProperty), 
								   index.accType, 
								   errorMsg);
		break;
    
	case basar::DATE:
        set = Helper::setDateToAcc(accIt,
								   index.accProperty, 
								   guiIt.getDate(index.guiProperty), 
								   index.accType, 
								   errorMsg);
		break;
    
	case basar::DATETIME:
        set = Helper::setDateTimeToAcc(accIt,
									   index.accProperty, 
									   guiIt.getDateTime(index.guiProperty), 
									   index.accType, 
									   errorMsg);
		break;
    
	default:
		{
			basar::VarString msg;
			msg.format("type <%d> is unknown in gui", index.guiType);
			ExceptInfo sInfo("basar.gui.tie.Matcher.setAccPropTblYIt()", 
								msg,
								__FILE__, __LINE__);
			throw UnknownPropertyTypeException(sInfo);
		}
	}

    if (!set)
    {
        if (errorMsg != "")
        {
            ExceptInfo sInfo("basar.gui.tie.Matcher.setAccPropTblYIt()", 
								errorMsg, 
								__FILE__, __LINE__);
            throw TypecastFailedException(sInfo);
        }
    }
}

//--------------------------------------------------------------------------------------------------------------//
/*!
 * writes values to AccessorPropertyTable_YIterator (defined by index)
 * @param index the list of indices that define the interconnection (id to id)
 *        between the given GuiPropertyTable_YIterator and
 *        AccessorPropertyTable_YIterator
 * @param guiIt The GuiPropertyTable_YIterator the values are gotten from
 * @param accIt The AccessorPropertyTable_YIterator the values are written to
 */
void Matcher::setAccPropTblYIt(std::list<IndexStruct>         & index, 
							   const GuiPropertyTable_YIterator guiIt,
							   AccPropTblYIterator              accIt)
{
    for (std::list<IndexStruct>::iterator iterIndex = index.begin(); 
		 iterIndex != index.end(); 
		 ++iterIndex)
    {
        setAccPropTblYIt(*iterIndex, guiIt, accIt);
    }

    BLOG_TRACE(LoggerPool::matcher(), "setAccPropTblYIt - matching completed;.");
}

//--------------------------------------------------------------------------------------------------------------//
/*!
 * steps up to index, returns false if the index can't be reached.
 * If the policy includes DISREGARD_DELETED those lines are ignored
 * If the policy includes INSERT GuiPropertyTable_YIterator lines
 * are inserted
 * @param index The index value the YIterators step up to
 * @param guiIt The GuiPropertyTable_YIterator that is stepped up
 * @param accIt The AccessorPropertyTable_YIterator that is stepped up
 * @return false if index can't be reached, true otherwise
 */
bool Matcher::step2Idx4Right2Left(const Int32                  index, 
								  GuiPropertyTable_YIterator & guiIt, 
								  AccPropTblYIterator        & accIt )
{
    bool retval = true;

    for (Int32 i = 0, count = index; 
		 i < count; 
		 ++i, ++accIt, ++guiIt)
    {
        if (!skipDeletedForward(accIt))
        {
            basar::VarString msg;
            msg.format("The Index <%d> is unknown in AccessorPropertyTable", index);
            ExceptInfo sInfo("basar.gui.tie.Matcher.step2Idx4Right2Left()", 
							 msg, 
							 __FILE__, __LINE__);
            throw UnknownIndexException(sInfo); 
        }

        if ( guiIt.isEnd() )
        {
            if (checkPolicy(INSERT))
            {
                // if this is uped for non internal guit, then this has to change to
                // guiIt = guiIt.getGuiPropertyTable().insert();
                // and guiIt.getGuiPropertyTable() has to be changed to
                // return a non const GuiPropertyTable
                guiIt = getLeft().insert();
            }
            else
            {
                retval = false;
                break;
            }
        }
    }

    return retval;
}

//--------------------------------------------------------------------------------------------------------------//
/*!
* steps up to index, returns false if the index can't be reached.
* If the policy includes DISREGARD_DELETED those lines are ignored
* If the policy includes INSERT AccessorPropertyTable_YIterator lines
* are inserted
 * @param index The index value the YIterators step up to
 * @param guiIt The GuiPropertyTable_YIterator that is stepped up
 * @param accIt The AccessorPropertyTable_YIterator that is stepped up
 * @return false if index can't be reached, true otherwise
*/
bool Matcher::step2Idx4Left2Right(const Int32                  index, 
								  GuiPropertyTable_YIterator & guiIt, 
								  AccPropTblYIterator        & accIt)
{
	if (index < guiIt.getAbsolutePosition())
	{
		basar::VarString msg;
		msg.format("index <%d> is unknown in GuiPropertyTable", index);
		ExceptInfo sInfo("basar.gui.tie.Matcher.step2Idx4Left2Right()", 
						 msg, 
						 __FILE__, __LINE__);
		throw UnknownIndexException(sInfo); 
	}

	bool  retval = true;
	Int32 i;

	for (i = 0; i < guiIt.getAbsolutePosition(); ++i, ++accIt)
	{
		skipDeletedForward(accIt);

		if (accIt != getRight().end())
		{
			if (checkPolicy(INSERT))
			{
				accIt = getRight().insert(basar::FOR_INSERT);
			}
			else
			{
				retval = false;
				break;
			}
		}
	}

	if (retval)
	{
		for (; i < index; ++i, ++accIt, ++guiIt)
		{
			if (guiIt == getLeft().end())
			{
				basar::VarString msg;
				msg.format("index <%d> is unknown in GuiPropertyTable", index);
				ExceptInfo sInfo("basar.gui.tie.Matcher.step2Idx4Left2Right()", 
								 msg,
								 __FILE__, __LINE__);
				throw UnknownIndexException(sInfo); 
			}

			skipDeletedForward(accIt);

			if (accIt == getRight().end())
			{
				if (checkPolicy(INSERT))
				{
					accIt = getRight().insert(basar::FOR_INSERT);
				}
				else
				{
					retval = false;
					break;
				}
			}
		}
	}

	return retval;
}

//--------------------------------------------------------------------------------------------------------------//
/*! 
 * checks if the AccessorPropertyTable_YIterator or GuiPropertyTable_YIterator
 * is isBegin(). If throws is true an Exception is thrown, if the returnvalue 
 * would be true. Internal Function, that is also used to throw Exceptions
 * at begin() for (index, negative count) Matching
 * \throws UnknownIndexException, if throws is true
 * \return true if one of guiIt or accIt are isBegin(), false if otherwise
 */
bool  Matcher::checkIsBegin(GuiPropertyTable_YIterator & guiIt,		//!< Gui Iterator, to be checked
							AccPropTblYIterator        & accIt,		//!< Accessor Iterator, to be checked
							bool                         throws,    //!< Parameter, that indicates if begin is valid or an exception has to be thrown
							ConstString                  errortext, //!< ErrorText in case of an Exception, to be thrown
							Int32                        index,     //!< Value for ErrorText
							Int32                        count      //!< Value for ErrorText
						   )
{
	bool retval = guiIt.isBegin() || accIt.isBegin();

	if (retval && throws)
	{
		if (accIt == getRight().begin())
		{
			basar::VarString msg;
			msg.format("index <%d> + row count <%d> are unknown in AccessorPropertyTable", 
						index, 
						count);
			ExceptInfo sInfo(errortext, msg, __FILE__, __LINE__);
			throw UnknownIndexException(sInfo); 
		}

		if (guiIt == getLeft().begin())
		{
			basar::VarString msg;
			msg.format("index <%d> + row count <%d> are unknown in GuiPropertyTable", 
					   index,
					   count);
			ExceptInfo sInfo(errortext, msg, __FILE__, __LINE__);
			throw UnknownIndexException(sInfo); 
		} 
	}

	return retval;
}

//--------------------------------------------------------------------------------------------------------------//
/*! skips all deleted rows at accIt++; 
 *  @param accIt The AccessorPropertyTable_YIterator, that is forwarded to the next undeleted row.
 *  @return true if PropertyTable contains at least one non-deleted row (from accIt++ towards end()), false otherwise
*/
bool Matcher::skipDeletedForward(AccPropTblYIterator & accIt )
{
	if (checkPolicy(DISREGARD_DELETED)) 
	{
		while (!accIt.isEnd() && accIt.getState().contains(basar::SS_DELETE)) 
		{
			++accIt;
		}
	}

	return (!accIt.isEnd());
}

//--------------------------------------------------------------------------------------------------------------//
/*! skips all deleted rows at accIt--;
*   @param accIt iterator to be decreased
*   @return true if PropertyTable contains at least one non-deleted row (from accIt-- towards begin()), false otherwise
*/
bool Matcher::skipDeletedBackward(AccPropTblYIterator & accIt)
{
	bool retval = true;

	if (checkPolicy(DISREGARD_DELETED)) 
	{
		while (!accIt.isBegin() && accIt.getState().contains(basar::SS_DELETE))
		{
			accIt--;
		}

		if (accIt.isBegin() && accIt.getState().contains(basar::SS_DELETE))
		{
			retval = false;
		}
	}

	return retval;
}

inline void Matcher::RightToLeftInternal(GuiPropertyTable_YIterator guiIt, const AccPropTblYIterator accIt)
{
    if (accIt.isEnd())
    {
        BLOG_TRACE(LoggerPool::matcher(), "RightToLeft - AccessorPropertyTable_YIterator is at end()");
        //nothing to do
        return;
    }
    else if (accIt.getPropertyTable() == m_AccPropTable  &&
        guiIt.getPropertyTable() == m_GuiPropTable)
    {
        innerRightToLeft(guiIt, accIt);
    }
    else
    {
        BLOG_TRACE(LoggerPool::matcher(), "RightToLeft - AccessorPropertyTable_YIterator different PropTab");

        std::list<IndexStruct> helplist;

        fillIndexStruct(helplist, guiIt, accIt, m_Policy);
        setGuiPropTblYIt(helplist, guiIt, accIt);
        transformRight2Left(guiIt, accIt);
    }
}

//--------------------------------------------------------------------------------------------------------------//
/*! checks is the MatcherPolicyType policy includes the MatcherPolicyEnum type
    \return true if policy includes type, false otherwise
 */
bool Matcher::checkPolicy(const MatcherPolicyType & policy,
						  const MatcherPolicyEnum & type  )
{
	return ( (policy & type) == type); 
}

//--------------------------------------------------------------------------------------------------------------//
/*! checks is the policy  includes the MatcherPolicyEnum type 
    \return true if internals policy includes type, false otherwise
*/
bool Matcher::checkPolicy(const MatcherPolicyEnum & type)
{
	return ((m_Policy & type) == type); 
}

//--------------------------------------------------------------------------------------------------------------//
} // tie
} // gui
} // basar
