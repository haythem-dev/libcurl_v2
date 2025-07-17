//----------------------------------------------------------------------------
/*! \file
 *  \brief  reference to all functions of AccessorProperyTableRef 
 *  \author Roland Kiefert
 *  \date   18.08.2005
 */
//----------------------------------------------------------------------------

//---------------------------------------------------------------------------------------//
#include "libbasardbaspect_accessorpropertytable.h"

#include "libbasardbaspect_accessorpropertytable_yiterator.h"
#include "cachecontroller.h"
#include "libbasarproperty_propertydescriptionlistref.h"
#include "libbasardebug.h"
#include "libbasarproperty_propertytable_xiterator.h"
#include "libbasarproperty_propertystate.h"
using namespace basar::property;

//---------------------------------------------------------------------------------------//
namespace basar	 {
namespace db	 {
namespace aspect {

//----------------------------------------------------------------------------
void  dumpAPTRef(size_t addr)
{
    dumpAPTRefRows(addr, 100);
}

void dumpAPTRefRows(size_t addr, int numRows)
{
    if (0 == addr)
        return;

    AccessorPropertyTableRef obj = *(reinterpret_cast<AccessorPropertyTableRef *>(addr));

    if (obj.isNull())
        return;

    std::ostringstream outStr;

    obj.toStream(outStr, numRows);

    debug::printDbgMsg(outStr.str().c_str());
}


ConstBuffer formatStatus(SupportedStateEnum state)
{
    switch (state)
    {
    case SS_UNKNOWN:
        return "UNKNOWN";
    case SS_CLEAN:
        return "CLEAN";
    case SS_UNSET:
        return "UNSET";
    case SS_INSERT:
        return "INSERT";
    case SS_UPDATE:
        return "UPDATE";
    case SS_DELETE:
        return "DELETE";
    default:
        return "ERROR";
    }
}

void CharRepl(std::ostream& strm, const char ch, const int count)
{
    for (int i = 0; i < count; i++)
    {
        strm << ch;
    }
}

void AccessorPropertyTableRef::toStream(std::ostream& strm /*= std::cout*/, int maxRows) const
{
    int MAX_COLS = 10;
    const int NAME_WIDTH = 12;
    const int TYPE_WIDTH = 8;
    const int NR_WIDTH = 3;
    const int ROW_SHIFT_WIDTH = 10;

    AccessorPropertyTable_YIterator yit = begin();
    if (yit.isEnd())
    {
        strm << "Property table is empty!" << std::endl;
        return;
    }

    int numCols;
    int numHeaderRows = 0;
    strm << " # ";
    PropertyTable_XIterator xit = yit.begin();
    while (!xit.isEnd())
    {
        CharRepl(strm, ' ', numHeaderRows * ROW_SHIFT_WIDTH);
        numCols = 0;
        for (; !xit.isEnd() && numCols < MAX_COLS; ++xit, ++numCols)
        {
            VarString header;
            header.format("%12.12s[%6.6s]", xit.getName().c_str(), xit.getType().toString().c_str());
            strm << header;
        }
        strm << std::endl;
        ++numHeaderRows;
    }

    // print separator
    CharRepl(strm, '_', MAX_COLS*(NAME_WIDTH + TYPE_WIDTH) + NR_WIDTH + ROW_SHIFT_WIDTH * (numHeaderRows - 1));
    strm << std::endl;

    // print rows
    int numRows = 0;
    for (; !yit.isEnd() && numRows < maxRows; ++yit, ++numRows)
    {
        // print row's number
        VarString valueStr;
        valueStr.format("%-3d", numRows);
        strm << valueStr;
        // print row's property ine by one
        numCols = 0;
        numHeaderRows = 0;
        PropertyTable_XIterator xit2 = yit.begin();
        while (!xit2.isEnd())
        {
            CharRepl(strm, ' ', numHeaderRows * ROW_SHIFT_WIDTH);
            numCols = 0;
            for (; !xit2.isEnd() && numCols < MAX_COLS; ++xit2, ++numCols)
            {
                VarString value;
                if (xit.getState().getState() != SS_UNSET)
                    value = xit2.getSQLString().c_str();

                valueStr.format("%12.12s[%6.6s]", value.c_str(), formatStatus(xit2.getState().getState()));
                strm << valueStr;
            }
            strm << std::endl;
            ++numHeaderRows;
        }
        strm << std::endl;
    }
    if (!yit.isEnd())
    {
        strm << "There are more rows in the property table, they are not shown!" << std::endl;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw  no-throw
	\return pointer to Reference of CacheController 
	\param  pCacheController shared pointer to cachecontroller*/
///////////////////////////////////////////////////////////////////////////////////////////
AccessorPropertyTableRef::AccessorPropertyTableRef( boost::shared_ptr<CacheController> pCacheController ) 
: m_pCacheController( pCacheController )
{
}
///////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw  no-throw
	\return pointer to Reference of CacheController */
///////////////////////////////////////////////////////////////////////////////////////////
AccessorPropertyTableRef::AccessorPropertyTableRef()
{
}
///////////////////////////////////////////////////////////////////////////////////////////
//! \return	reference to property description list
//! \throw  OutOfRangeIteratorException
///////////////////////////////////////////////////////////////////////////////////////////
const PropertyDescriptionListRef AccessorPropertyTableRef::getPropertyDescriptionList() const
{
	return getImpl()->getPropertyDescriptionList();
}
///////////////////////////////////////////////////////////////////////////////////////////
//! \return actual size of the propertytable
//! \throw   OutOfRangeIteratorException
///////////////////////////////////////////////////////////////////////////////////////////
NumberRows AccessorPropertyTableRef::size() const
{
	return getImpl()->size();
}
///////////////////////////////////////////////////////////////////////////////////////////
//! \return size of   read resultset  
//! \throw  InvalidValueException, OutOfRangeIteratorException
///////////////////////////////////////////////////////////////////////////////////////////
NumberRows AccessorPropertyTableRef::getNumberOfFetchedRows() const
{
	return getImpl()->getNumberOfFetchedRows();
}
///////////////////////////////////////////////////////////////////////////////////////////
//! \retval true if resultset and propertytable are empty 
//! \return otherwise returns false
//! \throw no-throw
///////////////////////////////////////////////////////////////////////////////////////////
bool AccessorPropertyTableRef::empty() const
{
	return getImpl()->empty();
}
///////////////////////////////////////////////////////////////////////////////////////////
//! \retval true if resultset and propertytable are empty 
//! \return otherwise returns false
//! \throw  no-throw
///////////////////////////////////////////////////////////////////////////////////////////
bool AccessorPropertyTableRef::isEmpty() const
{
	return getImpl()->empty();
}
///////////////////////////////////////////////////////////////////////////////////////////
//! \return maximum size, limited by propertytable
//! \throw  OutOfRangeIteratorException
///////////////////////////////////////////////////////////////////////////////////////////
MaxSizeRows AccessorPropertyTableRef::max_size() const
{
	return getImpl()->max_size();
}
///////////////////////////////////////////////////////////////////////////////////////////
//! \return yiterator to first row with given state in propertystate
//! \throw  OutOfRangeIteratorException
//! \param  state state which first returned line should be in
///////////////////////////////////////////////////////////////////////////////////////////
AccessorPropertyTable_YIterator AccessorPropertyTableRef::begin( const PropertyStateSet& state ) const
{
	return getImpl()->begin( state );
}
///////////////////////////////////////////////////////////////////////////////////////////
//! \return empty yiterator to new row
//! \throw  OutOfRangeIteratorException
//! \param  eInsertTypeEnum type which new inserted row should have
///////////////////////////////////////////////////////////////////////////////////////////
AccessorPropertyTable_YIterator AccessorPropertyTableRef::insert( const InsertTypeEnum eInsertTypeEnum )
{
	return getImpl()->insert( eInsertTypeEnum );	
}
///////////////////////////////////////////////////////////////////////////////////////////
//! \throw  OutOfRangeIteratorException
//! \param	yit iterator to line which is deleted at next execute
///////////////////////////////////////////////////////////////////////////////////////////
void AccessorPropertyTableRef::markForDelete( AccessorPropertyTable_YIterator yit )
{
	getImpl()->markForDelete(yit);
}
///////////////////////////////////////////////////////////////////////////////////////////
//! \throw  OutOfRangeIteratorException
//! \param	yit iterator pointing to the row whose states should be set to SS_UNKNOWN
///////////////////////////////////////////////////////////////////////////////////////////
void AccessorPropertyTableRef::setStateUnknown( AccessorPropertyTable_YIterator yit )
{
	getImpl()->setStateUnknown(yit);
}
///////////////////////////////////////////////////////////////////////////////////////////
/*! \throw  OutOfRangeIteratorException
	\param	yItBegin iterator to line where delete starts
	\param	yItEnd   iterator to line where delete ends (position just beyond last element 
					 removed from container)
	\return	AccessorPropertyTable_YIterator is the iterator pointing to the successor 
			element of the erased ones. */
///////////////////////////////////////////////////////////////////////////////////////////
AccessorPropertyTable_YIterator 
AccessorPropertyTableRef::erase (AccessorPropertyTable_YIterator yItBegin, 
								 AccessorPropertyTable_YIterator yItEnd )
{
	return getImpl()->erase(yItBegin, yItEnd);
}
///////////////////////////////////////////////////////////////////////////////////////////
/*! \throw  OutOfRangeIteratorException
	\param	yit iterator to line which is deleted from propertytable (not database)
	\return	AccessorPropertyTable_YIterator is the iterator pointing to the successor 
			element of the erased one. */
///////////////////////////////////////////////////////////////////////////////////////////
AccessorPropertyTable_YIterator AccessorPropertyTableRef::erase( AccessorPropertyTable_YIterator yit )
{
	return getImpl()->erase(yit);
}

///////////////////////////////////////////////////////////////////////////////////////////
/*! \throw  no-throw
	\param	state defining which lines are to be deleted
*/
///////////////////////////////////////////////////////////////////////////////////////////
void AccessorPropertyTableRef::erase( const PropertyStateSet& state )
{
    return getImpl()->erase( state );
}

///////////////////////////////////////////////////////////////////////////////////////////
//! \return yiterator to first row
//! \throw  OutOfRangeIteratorException
///////////////////////////////////////////////////////////////////////////////////////////
AccessorPropertyTable_YIterator AccessorPropertyTableRef::begin() const
{
	return getImpl()->begin();
}
///////////////////////////////////////////////////////////////////////////////////////////
//! \return yiterator to last row
//! \throw  OutOfRangeIteratorException
///////////////////////////////////////////////////////////////////////////////////////////
AccessorPropertyTable_YIterator AccessorPropertyTableRef::end() const
{
	return getImpl()->end(); 
}

///////////////////////////////////////////////////////////////////////////////////////////
//! \return returning iterator pointing to "at" row
//! \param index Position in PropertyTable to return Yit to
//! \throw  OutOfRangeIteratorException
///////////////////////////////////////////////////////////////////////////////////////////
AccessorPropertyTable_YIterator AccessorPropertyTableRef::at( const basar::Int32 index )
{
    return getImpl()->at( index );
}

///////////////////////////////////////////////////////////////////////////////////////////
//! \throw  OutOfRangeIteratorException
///////////////////////////////////////////////////////////////////////////////////////////
void AccessorPropertyTableRef::clear()
{
	return getImpl()->clear(); 
}
///////////////////////////////////////////////////////////////////////////////////////////
//! \throw  no-throw
//! \param  Statement sql-statement to save as member
///////////////////////////////////////////////////////////////////////////////////////////
void AccessorPropertyTableRef::setStatement( const db::sql::StatementRef Statement )
{
	getImpl()->setStatement(Statement);
}

///////////////////////////////////////////////////////////////////////////////////////////
//! \throw  no-throw
//! \param  Resultset resultset to set as member
///////////////////////////////////////////////////////////////////////////////////////////
void AccessorPropertyTableRef::setResultset( const db::sql::ResultsetRef Resultset )
{
	getImpl()->setResultset(Resultset);
}
///////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw
	\retval true if shared pointer to implementation object is null
	\return otherwise returns false */
///////////////////////////////////////////////////////////////////////////////////////////
bool AccessorPropertyTableRef::isNull() const
{
	return (0 == m_pCacheController.get());
}
///////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw  NullReferenceException */
///////////////////////////////////////////////////////////////////////////////////////////
void AccessorPropertyTableRef::reset()
{
	m_pCacheController.reset();
}
///////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw  NullReferenceException
	\return pointer to CacheController */
///////////////////////////////////////////////////////////////////////////////////////////
const boost::shared_ptr<CacheController>& AccessorPropertyTableRef::getImpl() const
{
	if (m_pCacheController.get() == 0)
	{
		ExceptInfo sInfo( "basar.db.aspect.AccessorPropertyTableRef.getImpl", 
						  "Reference of CacheController - no pointer to impl-class", 
						  __FILE__, __LINE__);
		throw NullReferenceException(sInfo);
	}
	return m_pCacheController;
}
///////////////////////////////////////////////////////////////////////////////////////////
/*! \throw no-throw
	\param yit iterator whose propertystates are set to SS_CLEAN
	\return AccessorPropertyTable_YIterator removed of entries in state SS_DELETE, other are set to SS_CLEAN
*/
///////////////////////////////////////////////////////////////////////////////////////////
AccessorPropertyTable_YIterator 
AccessorPropertyTableRef::executePerformed( AccessorPropertyTable_YIterator yit )
{
	return getImpl()->executePerformed( yit );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! \return const Reference to the created Snapshot of the PropertyTable
//! \throw no-throw
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
const PropertyTableSnapshotRef AccessorPropertyTableRef::createSnapshot() const
{
	return getImpl()->createSnapshot();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! \throw no-throw
//!	\param  snapShot a snapshot of this class
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void AccessorPropertyTableRef::resetToSnapshot( const PropertyTableSnapshotRef snapShot )
{
	return getImpl()->resetToSnapshot( snapShot );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! \return const Enum that contains the current CachingPolicy
//! \throw no-throw
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
CachingPolicyEnum AccessorPropertyTableRef::getCachingPolicy() const
{
	return getImpl()->getCachingPolicy();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! \retval true if the snapshot was created successfully
//! \return otherwise returns false
//! \throw no-throw
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool AccessorPropertyTableRef::createInternalSnapshot()
{
	return getImpl()->createInternalSnapshot();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! \throw no-throw
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void AccessorPropertyTableRef::resetInternalSnapshot()
{
	getImpl()->resetInternalSnapshot();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! \throw no-throw
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void AccessorPropertyTableRef::clearInternalSnapshot()
{
	getImpl()->clearInternalSnapshot();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! \throw no-throw
//! \param toCreate true if internal snapshot should be created
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void AccessorPropertyTableRef::switchCreateInternalSnapshot( const bool toCreate )
{
	getImpl()->switchCreateInternalSnapshot( toCreate );
}

bool AccessorPropertyTableRef::operator==(const AccessorPropertyTableRef & a) const
{
   return ( m_pCacheController.get() == a.m_pCacheController.get());
}

} // aspect
} // db
} // basar
