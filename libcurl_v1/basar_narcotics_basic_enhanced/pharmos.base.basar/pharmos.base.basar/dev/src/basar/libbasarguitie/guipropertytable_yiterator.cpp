//----------------------------------------------------------------------------
/*! \file
 *  \brief  class implementation of y-iterator to guipropertytable
 *  \author Roland Kiefert
 *  \date   24.08.2005
 */
//----------------------------------------------------------------------------

//---------------------------------------------------------------------------------------//
// includes
//---------------------------------------------------------------------------------------//

#include "libbasarguitie_propertytable.h"

#include "guipropertytable.h"
#include "../macro.h"
#include "libbasarproperty_propertytype.h"

//---------------------------------------------------------------------------------------//
// namespace
//---------------------------------------------------------------------------------------//
namespace basar {
namespace gui {
namespace tie {

//////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw 
	\param pGuiPropertyTable pointer to gui property table
	\param row position in resultset */
//////////////////////////////////////////////////////////////////////////////////////////////////////////
GuiPropertyTable_YIterator::GuiPropertyTable_YIterator( boost::shared_ptr<GuiPropertyTable> pGuiPropertyTable, 
													    Int32                               row /* = -1 */   ) 
													  : m_Pt        ( pGuiPropertyTable ), 
													    m_CurrentRow( row )
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw */
//////////////////////////////////////////////////////////////////////////////////////////////////////////
GuiPropertyTable_YIterator::GuiPropertyTable_YIterator () : m_CurrentRow( -1 )
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////			
/*!	\retval true if equal
	\return otherwise returns false
	\param  rhs yiterator tthat is checked for equality 
	\throw	DifferentContainersIteratorException */
//////////////////////////////////////////////////////////////////////////////////////////////////////////
bool GuiPropertyTable_YIterator::operator == ( const GuiPropertyTable_YIterator& rhs ) const
{
	ConstString fun = "basar.gui.tie.GuiPropertyTable_YIterator.operator==()";	

	checkValid();

	// comparison: same container and same iterator
	if( m_Pt != rhs.m_Pt )
	{
		throw DifferentContainersIteratorException(ExceptInfo ( fun, 
																"Iterator of different container.", 
																__FILE__, __LINE__ ));
			  
	}
	
	return m_CurrentRow == rhs.m_CurrentRow;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\retval true if not equal
	\return otherwise returns false
	\param  rhs yiterator that is checked for unequality
	\throw	no-throw */
//////////////////////////////////////////////////////////////////////////////////////////////////////////
bool GuiPropertyTable_YIterator::operator != ( const GuiPropertyTable_YIterator& rhs ) const
{
	return !(operator == (rhs));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////	
/*!	\return	GuiPropertyTable_XIterator pointing to the first gui ctrl of the read in ones
	\throw	no-throw */
//////////////////////////////////////////////////////////////////////////////////////////////////////////
GuiPropertyTable_XIterator GuiPropertyTable_YIterator::begin() const
{
	checkValid();
	return GuiPropertyTable_XIterator( m_Pt, m_Pt->m_Ctrls.begin(), m_CurrentRow );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\return	GuiPropertyTable_XIterator pointing to item beyond the last one 
	\throw	no-throw */
//////////////////////////////////////////////////////////////////////////////////////////////////////////
GuiPropertyTable_XIterator GuiPropertyTable_YIterator::end() const
{
	checkValid();
	return GuiPropertyTable_XIterator( m_Pt, m_Pt->m_Ctrls.end(), ITERATOR_ENDPOS );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\return GuiPropertyTable_YIterator set to next element (praecrement)
	\throw OutOfRangeIteratorException */
//////////////////////////////////////////////////////////////////////////////////////////////////////////
GuiPropertyTable_YIterator GuiPropertyTable_YIterator::operator++()
{
	FUNCTION_NAME( basar.gui.tie.GuiPropertyTable_YIterator.operator++ )

	checkValid();
		
	if( m_CurrentRow == ITERATOR_ENDPOS )
	{
        throw OutOfRangeIteratorException( ExceptInfo( fun, 
													   "current position of iterator is end()!", 
													   __FILE__, __LINE__) );
    }

	// compare the highest (item) count with the member of the current row 'm_CurrentRow'
	if( ++m_CurrentRow >= m_Pt->m_AbsoluteIndex + m_Pt->maxCountOfControls() )
	{
		m_CurrentRow = ITERATOR_ENDPOS; // ITERATOR_ENDPOS signals the end of the y-iterator
	}

	return *this;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\return GuiPropertyTable_YIterator set to next element (postcrement)  
	\param i only for postfix
	\throw OutOfRangeIteratorException */
//////////////////////////////////////////////////////////////////////////////////////////////////////////
GuiPropertyTable_YIterator GuiPropertyTable_YIterator::operator++( int )
{
	FUNCTION_NAME( basar.gui.tie.PropertyTable_YIterator.operator++(int) )

	checkValid();
		
	if( m_CurrentRow == ITERATOR_ENDPOS )
	{
        throw OutOfRangeIteratorException( ExceptInfo( fun, 
													   "current position of iterator is end()!", 
													   __FILE__, __LINE__) );
    }

    GuiPropertyTable_YIterator yit( *this );

	// compare the highest (item) count with the member of the current row 'm_CurrentRow'
	if( ++m_CurrentRow >= m_Pt->m_AbsoluteIndex + m_Pt->maxCountOfControls() ){
		m_CurrentRow = ITERATOR_ENDPOS; // ITERATOR_ENDPOS signals the end of the y-iterator
	}

    return yit;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\return GuiPropertyTable_YIterator pointing to the previous one (prae)
	\throw	OutOfRangeIteratorException */
//////////////////////////////////////////////////////////////////////////////////////////////////////////
GuiPropertyTable_YIterator GuiPropertyTable_YIterator::operator--()
{
	FUNCTION_NAME( basar.gui.tie.GuiPropertyTable_YIterator.operator-- )

	checkValid();
	
	//! \todo actually it should exist and stl pendant to 'rend()'
	if( m_CurrentRow == 0 )
	{
        throw OutOfRangeIteratorException( ExceptInfo( fun, 
													   "current position of iterator is begin()!", 
													   __FILE__, __LINE__) );
    }

	--m_CurrentRow;

	return *this;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\return GuiPropertyTable_YIterator pointing to the previous one (post)
	\param	i only for präfix
	\throw	OutOfRangeIteratorException */
//////////////////////////////////////////////////////////////////////////////////////////////////////////
GuiPropertyTable_YIterator GuiPropertyTable_YIterator::operator--( int )
{
	FUNCTION_NAME( basar.gui.tie.GuiPropertyTable_YIterator.operator--(int) )

	checkValid();
		
	if( m_CurrentRow == 0 )
	{
        throw OutOfRangeIteratorException(ExceptInfo( fun, 
													  "current position of iterator is begin()!", 
													  __FILE__, __LINE__) );
    }
	
	GuiPropertyTable_YIterator yit( *this );
	m_CurrentRow--;

    return yit;
}
//------------------------------------------------------------------------------------------------------//
// PROPERTIES METHODS
//------------------------------------------------------------------------------------------------------//
//////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\param	rColName name of column to set string-value to
	\param	rValue string-value to set to column 
	\throw	no-throw */
//////////////////////////////////////////////////////////////////////////////////////////////////////////
void GuiPropertyTable_YIterator::setString( const ColumnName& rColName, const I18nString& rValue )
{
	checkValid();
	setString( getIndex(rColName), rValue );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\param	index of column to set string-value to
	\param	rValue string-value to set to the indexed column 
	\throw	no-throw */
//////////////////////////////////////////////////////////////////////////////////////////////////////////
void GuiPropertyTable_YIterator::setString( ColumnSize index, const I18nString& rValue )
{
	checkValid();
	GuiPropertyTable_XIterator it = getXIterator(index);

	it.setString( rValue );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\return I18nString value of given column
	\param  rColName name of column to get string-value from 
	\throw	no-throw */
//////////////////////////////////////////////////////////////////////////////////////////////////////////
I18nString GuiPropertyTable_YIterator::getString( const ColumnName& rColName ) const
{
	checkValid();
	return getString( getIndex(rColName) );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\return I18nString value of given index
	\param  index representing the position of the column
	\throw	no-throw */
//////////////////////////////////////////////////////////////////////////////////////////////////////////
I18nString GuiPropertyTable_YIterator::getString( const ColumnSize index ) const
{
	checkValid();
	GuiPropertyTable_XIterator it = getXIterator(index);

	return it.getString(); 
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\param	rColName name of column to set DateTime-value to
	\param	rValue datetime-value to set to column 
	\throw	no-throw */
//////////////////////////////////////////////////////////////////////////////////////////////////////////
void GuiPropertyTable_YIterator::setDateTime( const ColumnName& rColName, const DateTime& rValue )
{
	checkValid();
	setDateTime( getIndex(rColName), rValue );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\param	index of column to set DateTime-value to
	\param	rValue datetime-value to set to the indexed column 
	\throw	no-throw */
//////////////////////////////////////////////////////////////////////////////////////////////////////////
void GuiPropertyTable_YIterator::setDateTime( ColumnSize index, const DateTime& rValue )
{
	checkValid();
	GuiPropertyTable_XIterator it = getXIterator(index);

	it.setDateTime( rValue );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\return DateTime value of given column
	\param  rColName name of column to get string-value from 
	\throw	no-throw */
//////////////////////////////////////////////////////////////////////////////////////////////////////////
DateTime GuiPropertyTable_YIterator::getDateTime( const ColumnName& rColName ) const
{
	checkValid();
	return getDateTime( getIndex(rColName) );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\return DateTime value of given index
	\param  index representing the position of the column
	\throw	no-throw */
//////////////////////////////////////////////////////////////////////////////////////////////////////////
DateTime GuiPropertyTable_YIterator::getDateTime( const ColumnSize index ) const
{
	checkValid();
	GuiPropertyTable_XIterator it = getXIterator(index);
	return it.getDateTime(); 
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\param	rColName name of column to set DateTime-value to
	\param	rValue datetime-value to set to column 
	\throw	no-throw */
//////////////////////////////////////////////////////////////////////////////////////////////////////////
void GuiPropertyTable_YIterator::setTime( const ColumnName& rColName, const Time& rValue )
{
	checkValid();
	setTime( getIndex(rColName), rValue );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\param	index of column to set DateTime-value to
	\param	rValue datetime-value to set to the indexed column 
	\throw	no-throw */
//////////////////////////////////////////////////////////////////////////////////////////////////////////
void GuiPropertyTable_YIterator::setTime( ColumnSize index, const Time& rValue )
{
	checkValid();
	GuiPropertyTable_XIterator it = getXIterator(index);
	it.setTime( rValue );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\return DateTime value of given column
	\param  rColName name of column to get string-value from 
	\throw	no-throw */
//////////////////////////////////////////////////////////////////////////////////////////////////////////
Time GuiPropertyTable_YIterator::getTime( const ColumnName& rColName ) const
{
	checkValid();
	return getTime( getIndex(rColName) );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\return DateTime value of given index
	\param  index representing the position of the column
	\throw	no-throw */
//////////////////////////////////////////////////////////////////////////////////////////////////////////
Time GuiPropertyTable_YIterator::getTime( const ColumnSize index ) const
{
	checkValid();
	GuiPropertyTable_XIterator it = getXIterator(index);
	return it.getTime(); 
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\param	rColName name of column to set DateTime-value to
	\param	rValue datetime-value to set to column 
	\throw	no-throw */
//////////////////////////////////////////////////////////////////////////////////////////////////////////
void GuiPropertyTable_YIterator::setDate( const ColumnName& rColName, const Date& rValue )
{
	checkValid();
	setDate( getIndex(rColName), rValue );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\param	index of column to set DateTime-value to
	\param	rValue datetime-value to set to the indexed column 
	\throw	no-throw */
//////////////////////////////////////////////////////////////////////////////////////////////////////////
void GuiPropertyTable_YIterator::setDate( ColumnSize index, const Date& rValue )
{
	checkValid();
	GuiPropertyTable_XIterator it = getXIterator(index);
	it.setDate( rValue );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\return DateTime value of given column
	\param  rColName name of column to get string-value from 
	\throw	no-throw */
//////////////////////////////////////////////////////////////////////////////////////////////////////////
Date GuiPropertyTable_YIterator::getDate( const ColumnName& rColName ) const
{
	checkValid();
	return getDate( getIndex(rColName) );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\return DateTime value of given index
	\param  index representing the position of the column
	\throw	no-throw */
//////////////////////////////////////////////////////////////////////////////////////////////////////////
Date GuiPropertyTable_YIterator::getDate( const ColumnSize index ) const
{
	checkValid();
	GuiPropertyTable_XIterator it = getXIterator(index);
	return it.getDate(); 
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\param	rColName name of column to set string-value to
	\param	rValue string-value to set to column 
	\throw	no-throw */
//////////////////////////////////////////////////////////////////////////////////////////////////////////
void GuiPropertyTable_YIterator::setCurrentString( const ColumnName& rColName, const I18nString& rValue )
{
	checkValid();
	setCurrentString( getIndex(rColName), rValue );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\param	index of column to set string-value to
	\param	rValue string-value to set to the indexed column 
	\throw	no-throw */
//////////////////////////////////////////////////////////////////////////////////////////////////////////
void GuiPropertyTable_YIterator::setCurrentString( ColumnSize index, const I18nString& rValue )
{
	checkValid();
	GuiPropertyTable_XIterator it = getXIterator(index);

	it.setCurrentString( rValue );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\return I18nString value of given column
	\param  rColName name of column to get string-value from 
	\throw	no-throw */
//////////////////////////////////////////////////////////////////////////////////////////////////////////
I18nString GuiPropertyTable_YIterator::getCurrentString( const ColumnName& rColName ) const
{
	checkValid();
	return getCurrentString( getIndex(rColName) );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\return I18nString value of given column
	\param  index representing the position of the column
	\throw	no-throw */
//////////////////////////////////////////////////////////////////////////////////////////////////////////
I18nString GuiPropertyTable_YIterator::getCurrentString( const ColumnSize index ) const
{
	checkValid();
	GuiPropertyTable_XIterator it = getXIterator(index);

	return it.getCurrentString(); 
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////		
/*!	\return property::PropertyType type of passed column 
	\param	rColName column whose type is asked
	\throw	no-throw */
//////////////////////////////////////////////////////////////////////////////////////////////////////////
property::PropertyType GuiPropertyTable_YIterator::getType( const ColumnName& rColName ) const
{
	checkValid();
	return m_Pt->getPropertyDescriptionList().getType( rColName );
}
//------------------------------------------------------------------------------------------------------//
// contains methods
//------------------------------------------------------------------------------------------------------//
//////////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \retval true if the passed column name is found	
	\return otherwise returns false
	\param	rColName is the passed column name to search for.
	\throw	no-throw */
//////////////////////////////////////////////////////////////////////////////////////////////////////////
bool GuiPropertyTable_YIterator::contains( const ColumnName& rColName ) const
{
	return m_Pt->getPropertyDescriptionList().contains( rColName );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \retval true if the passed column name with the given type is found
	\return otherwise returns false
	\param	rColName is the passed column name to search for.
	\param	pt is the passed type to search for.
	\throw	no-throw */
//////////////////////////////////////////////////////////////////////////////////////////////////////////
bool GuiPropertyTable_YIterator::contains(	const ColumnName           & rColName, 
											const property::PropertyType pt      ) const
{
	return m_Pt->getPropertyDescriptionList().contains( rColName, pt );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////		
/*! \retval true if the passed column name with the given 
			type in the form of a pair is found
	\return otherwise returns false
	\param	p is the pair of the column name and the type to search for.
	\throw	no-throw */
//////////////////////////////////////////////////////////////////////////////////////////////////////////		
bool GuiPropertyTable_YIterator::contains( const std::pair<ColumnName, property::PropertyType>& p) const
{
	return m_Pt->getPropertyDescriptionList().contains( p );
}
//------------------------------------------------------------------------------------------------------//
// index based methods
//------------------------------------------------------------------------------------------------------//
//////////////////////////////////////////////////////////////////////////////////////////////////////////		
/*!	\return index of given column 
	\param rColName column whose index is wanted 
	\throw no-throw */
//////////////////////////////////////////////////////////////////////////////////////////////////////////
ColumnSize GuiPropertyTable_YIterator::getIndex( const ColumnName& rColName ) const
{
	checkValid();
	return m_Pt->getPropertyDescriptionList().getIndex( rColName );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////		
/*!	\retval true if a value is found for passed column name
	\return otherwise returns false
	\param rColName column to check for existence 
	\throw no-throw */
//////////////////////////////////////////////////////////////////////////////////////////////////////////
bool GuiPropertyTable_YIterator::isValueExist( const ColumnName& rColName ) const
{
	checkValid();
	return isValueExist( getIndex(rColName) );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////		
/*!	\retval true if a value is found for given index
	\return otherwise returns false
	\param index representing the position of the column 
	\throw no-throw */
//////////////////////////////////////////////////////////////////////////////////////////////////////////
bool GuiPropertyTable_YIterator::isValueExist( const ColumnSize index ) const
{
	checkValid();
	GuiPropertyTable_XIterator it = getXIterator(index);
	return it.isValueExist(); 
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////		
/*!	\return Int32: value representing the current line of the visible items of the concerned gui control.
					But this current line could be the line of the second page of visualized items 
					or beyond it. So the absolute position is the addition of the current line and
					the visible lines of the control (means page).
	\throw no-throw */
//////////////////////////////////////////////////////////////////////////////////////////////////////////		
Int32 GuiPropertyTable_YIterator::getAbsolutePosition() const
{
	checkValid();
	return m_CurrentRow;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////		
/*!	\return Int32: value representing the current line of the visible items of the concerned gui control.
				   This current line is the line of the aktual page of visualized items 
	\throw no-throw */
//////////////////////////////////////////////////////////////////////////////////////////////////////////		
Int32 GuiPropertyTable_YIterator::getPosition() const
{
	checkValid();
	return m_CurrentRow - m_Pt->getAbsoluteIndex();
}
///////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw 
	\return reference to a gui property table */
///////////////////////////////////////////////////////////////////////////////////////
const GuiPropertyTableRef GuiPropertyTable_YIterator::getGuiPropertyTable() const
{
	return m_Pt;
}
//------------------------------------------------------------------------------------------------------//
// initialization check of a object of this class. Important in the case of dflt construction.
//------------------------------------------------------------------------------------------------------//
//////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw InvalidIteratorException */
//////////////////////////////////////////////////////////////////////////////////////////////////////////
void GuiPropertyTable_YIterator::checkValid() const
{
	if( m_Pt.get() != 0 )
		return;

	throw InvalidIteratorException(ExceptInfo("basar.gui.tie.GuiPropertyTable_YIterator.checkValid", 
											  "pointer to GuiPropertyTable not valid", 
												__FILE__, __LINE__) );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////		
/**
 *  \return bool indicating the internal value has been initialized
 */
//////////////////////////////////////////////////////////////////////////////////////////////////////////		
bool GuiPropertyTable_YIterator::isNull() const
{
	return ( m_Pt.get() == 0 );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////		
/**
 * resets the internal Pointer to null
 */
//////////////////////////////////////////////////////////////////////////////////////////////////////////		
void GuiPropertyTable_YIterator::reset()
{
	m_Pt.reset();
	m_CurrentRow = -1;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////		
//! \return bool indicating if Iterator is pointing at the begin of the GuiPropertyTable
//////////////////////////////////////////////////////////////////////////////////////////////////////////		
bool GuiPropertyTable_YIterator::isBegin() const
{
	checkValid();
	return (*this == m_Pt->begin());
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////		
//! \return bool indicating if Iterator is pointing at the end of the GuiPropertyTable
//////////////////////////////////////////////////////////////////////////////////////////////////////////		
bool GuiPropertyTable_YIterator::isEnd() const
{
	checkValid();
	return (*this == m_Pt->end());
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////		
//! \return UInt32 giving the size of the GuiPropertyTable
//////////////////////////////////////////////////////////////////////////////////////////////////////////		
UInt32 GuiPropertyTable_YIterator::size() const
{
	checkValid();
	return ( m_Pt->size());
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////		
/**
 * \return GuiPropertyTable_XIterator for given index
 * \throw  InvalidIteratorException
 * \param  index index of the column
 */
//////////////////////////////////////////////////////////////////////////////////////////////////////////		
GuiPropertyTable_XIterator GuiPropertyTable_YIterator::getXIterator(const ColumnSize index ) const
{
    FUNCTION_NAME( basar.gui.tie.GuiPropertyTable_YIterator.getXIterator )

    checkValid();

	GuiPropertyTable_XIterator it = begin();
	ColumnSize                 i  = 0;

	while (end() != it)		// check valid iterator first!
	{
		if (i == index)		// check index then!
			return it;

		++i;
		++it;
	}

    VarString errMsg;
    errMsg.format("GuiPropertyTable_XIterator at index <%d> not valid, "
		          "available columns <%d>", 
				  index,
				  i);
    throw InvalidIteratorException(ExceptInfo( fun, errMsg, __FILE__, __LINE__) );
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////		
/**
 * \return GuiPropertyTable_XIterator for given column name
 * \param rColName column name
 */
//////////////////////////////////////////////////////////////////////////////////////////////////////////		
GuiPropertyTable_XIterator GuiPropertyTable_YIterator::getXIterator(const ColumnName& rColName  ) const
{
    checkValid();
    return getXIterator( getIndex(rColName) );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////		
/**
 * \return GuiPropertyTableRef referenceing the ProperyTable
 */
//////////////////////////////////////////////////////////////////////////////////////////////////////////		
GuiPropertyTableRef GuiPropertyTable_YIterator::getPropertyTable() const
{
    return GuiPropertyTableRef(m_Pt);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////		
/**
 * \return the coumnname for an index 
 * \param index index of the column
 */
//////////////////////////////////////////////////////////////////////////////////////////////////////////		
ColumnName GuiPropertyTable_YIterator::getName(const ColumnSize index) const
{
     checkValid();
     return m_Pt->getPropertyDescriptionList().getName(index);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////		
/**
 * \return the number of columns
 */
//////////////////////////////////////////////////////////////////////////////////////////////////////////		
ColumnSize GuiPropertyTable_YIterator::getNumberOfColumns() const 
{
    checkValid();
    return m_Pt->getPropertyDescriptionList().size();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////		

 void GuiPropertyTable_YIterator::notifyMatchDone()
 {
     checkValid();

     m_Pt->notifyMatchDone();
 }

} // tie
} // gui
} // basar
