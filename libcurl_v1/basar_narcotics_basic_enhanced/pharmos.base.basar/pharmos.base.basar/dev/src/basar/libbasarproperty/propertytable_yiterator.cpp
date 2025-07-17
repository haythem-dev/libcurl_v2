//------------------------------------------------------------------------------------//
/*! \file
 *  \brief  class PropertyTable_YIterator represents an iterator pointing 
			to a collection of propertyinternals (whole row/line)
 *  \author Bischof Björn 
 *  \date   23.08.2005
 */
//------------------------------------------------------------------------------------//

//------------------------------------------------------------------------------------//
// includes
//------------------------------------------------------------------------------------//

#include "libbasarproperty_propertytable_yiterator.h"

#include "libbasarcmnutil_config.h"
#include "propertytable.h"
#include "libbasarproperty_propertytable_xiterator.h"
#include "libbasarproperty_propertystate.h"
#include "propertyinternal.h"
#include "libbasarproperty_propertytype.h"
#include "loggerpool.h"

#include "../macro.h"

//------------------------------------------------------------------------------------//
// namespaces
//------------------------------------------------------------------------------------//
namespace basar
{
namespace property
{

////////////////////////////////////////////////////////////////////////////////////////
/*! \throw	no-throw */  
////////////////////////////////////////////////////////////////////////////////////////
PropertyTable_YIterator::PropertyTable_YIterator() : m_It          (), 
													 m_FilteredStep(0)
{
}

////////////////////////////////////////////////////////////////////////////////////////
/*! \param	rhs passed PropertyStateSet object with states that will be set to the 
			PropertyStateSet member m_Pss.
	\throw	no-throw */  
////////////////////////////////////////////////////////////////////////////////////////
PropertyTable_YIterator::PropertyTable_YIterator ( const PropertyStateSet & rhs )
                                                 : m_It          (),
												   m_Pss         (rhs),
												   m_FilteredStep(0)
{
}

////////////////////////////////////////////////////////////////////////////////////////
/*! \param	rhs passed PropertyTable_YIterator object that will be assigned to the new 
			instantiated one with regard to the PropertyStateSet member of the passed rhs.
	\throw	no-throw */  
////////////////////////////////////////////////////////////////////////////////////////
PropertyTable_YIterator::PropertyTable_YIterator( const PropertyTable_YIterator & rhs )
{
	assign( rhs );
}

////////////////////////////////////////////////////////////////////////////////////////
/*! \param	rhs passed PropertyTable_YIterator object that will be assigned to the new 
			instantiated one with regard to the PropertyStateSet member of the passed rhs.
	\param	pss afterwards overwrites the PropertyStateSet member of the new object.
	\throw	no-throw */  
////////////////////////////////////////////////////////////////////////////////////////
PropertyTable_YIterator::PropertyTable_YIterator( const PropertyTable_YIterator & rhs, 
												  const PropertyStateSet        & pss )
{
	assign( rhs );
	
	/*! \todo	a) reflecting if the passed PropertyStateSet object should be 
				   simply assigned to the PropertyStateSet member (as active) or 
				b) should be inserted in the PropertyStateSet member or 
				c) there should be a check if the states of the passed PropertyTable_YIterator
				   are contained in the passed PropertyStateSet object.*/
	m_Pss = pss; // collides with the logic in the method "assign()";
}

////////////////////////////////////////////////////////////////////////////////////////
/*! \param	pt is the shared pointer to class aggregating the container
	\param	it is the pure/real stl-iterator
	\throw	no-throw*/ 
////////////////////////////////////////////////////////////////////////////////////////
PropertyTable_YIterator::PropertyTable_YIterator( const PropertyTableSharedPtr      pt, 
												  const CollPropertyInternalRowIter it )
												: m_Pt          (pt), 
												  m_It          (it), 
												  m_FilteredStep(0 )
{
}

////////////////////////////////////////////////////////////////////////////////////////
/*! \throw	no-throw */  
////////////////////////////////////////////////////////////////////////////////////////
void PropertyTable_YIterator::clear()
{
	if ( !isNull() )
		m_Pt->clear();
	
	m_Pt.reset();
}

////////////////////////////////////////////////////////////////////////////////////////
/*! \throw	no-throw */  
////////////////////////////////////////////////////////////////////////////////////////
void PropertyTable_YIterator::reset()
{
	clear();
}

////////////////////////////////////////////////////////////////////////////////////////
/*! \param	rhs passed PropertyTable_YIterator object that will be assigned to the left one 
			with regard to the PropertyStateSet member of the passed rhs.
	\return PropertyTable_YIterator which has been assigned
	\throw	no-throw */  
////////////////////////////////////////////////////////////////////////////////////////
PropertyTable_YIterator & PropertyTable_YIterator::operator = ( const PropertyTable_YIterator & rhs )
{
	// avoid self assignment
	if( &rhs == this ) 
		return *this; 

	assign( rhs );
	
	return *this; 
}

////////////////////////////////////////////////////////////////////////////////////////
/*! \param	rhs passed PropertyTable_YIterator object whose values are set to this.
	\throw	no-throw */  
////////////////////////////////////////////////////////////////////////////////////////
void PropertyTable_YIterator::assign( const PropertyTable_YIterator & rhs )
{
	m_Pt			= rhs.m_Pt;
    m_It			= rhs.m_It;

	if(    SS_UNKNOWN == rhs.m_Pss.getStates()
		|| SS_UNSET == rhs.m_Pss.getStates() )
	{
		// initialize property state set
		m_Pss = PropertyStateSet();
	}
	else
	{
		m_Pss = rhs.m_Pss;
	}
	
	m_FilteredStep	= rhs.m_FilteredStep;
}

////////////////////////////////////////////////////////////////////////////////////////
/*! \retval true if the two compared objects are equal
	\return otherwise returns false
	\param	rhp is the passed object that should be checked for equality.
			Comparing criteria are the same container and the same iterator
	\throw	InvalidIteratorException, DifferentContainersIteratorException */ 
////////////////////////////////////////////////////////////////////////////////////////
bool PropertyTable_YIterator::operator == ( const PropertyTable_YIterator & rhp ) const
{
	FUNCTION_NAME(basar.property.PropertyTable_YIterator.operator==)

	checkPropTbl();

	if( rhp.m_Pt != m_Pt ){
		ExceptInfo sInfo( fun, "Iterator of different container.", __FILE__, __LINE__ );
		throw DifferentContainersIteratorException(sInfo);
	}

#ifdef _WIN32
	#if _MSC_VER >= 1600
		#ifdef _DEBUG

        //extra code for working around the debug iterator level 2 in vc2010

		if (m_It._Ptr == 0 && rhp.m_It._Ptr == 0)
		{
			return true;
		}

        if ( (m_It._Ptr == rhp.m_It._Ptr) && (rhp.m_It._Getcont() == 0 && m_It._Getcont() == 0) )
        {
            return true;
        }

		if ( rhp.m_It._Getcont() != m_It._Getcont())
		{
			return false;
		}
		#endif
	#endif
#endif
	
	return rhp.m_It == m_It;
}
////////////////////////////////////////////////////////////////////////////////////////
/*! \retval true if the two compared objects are not equal
	\return otherwise returns false
	\param	rhp is the iterator checked for unequality.
			This method negates the method 'operator==()'
	\throw	no-throw */ 
////////////////////////////////////////////////////////////////////////////////////////
bool PropertyTable_YIterator::operator != ( const PropertyTable_YIterator & rhp ) const
{
    return ! ( this->operator == (rhp) );
}

////////////////////////////////////////////////////////////////////////////////////////
/*!	\return PropertyTable_XIterator pointing to first column in propertytable
	\throw	no-throw */ 
////////////////////////////////////////////////////////////////////////////////////////
PropertyTable_XIterator PropertyTable_YIterator::begin() const
{
	//FUNCTION_NAME( basar.property.PropertyTable_YIterator::begin() const )

	/*! \todo	the expression '&(*m_It' is an bald pointer. 
				It collides with our claim to a safe memory management.
				But here it is intended to use a bald pointer. */
	
	// BB: hier raucht er ab!
	/*if( m_It->second.begin() == m_It->second.end() ){
		VarString msg = "current iterator is begin() == end()!";
		throw OutOfRangeIteratorException( 
				ExceptInfo( fun, msg.c_str(), __FILE__, __LINE__) );
	}*/

    checkPropTbl();

	return PropertyTable_XIterator ( m_Pt, &(*m_It), m_It->second.begin() );
}

PropertyTable_XIterator     PropertyTable_YIterator::at          (Difference_Type diff) const
{
	return PropertyTable_XIterator ( m_Pt, &(*m_It), (m_It->second.begin()+diff) );    
}

////////////////////////////////////////////////////////////////////////////////////////
/*!	\return PropertyTable_XIterator pointing to first column in propertytable
	\param	pss contains all possible states that are 
			compared with the state of every single column.
	\throw	InvalidIteratorException */
////////////////////////////////////////////////////////////////////////////////////////
PropertyTable_XIterator PropertyTable_YIterator::begin( const PropertyStateSet & pss )
{
    PropertyTable_XIterator       xit   = begin();
	const PropertyTable_XIterator itEnd = end();

	for(; xit != itEnd; ++xit )
	{
		if( pss.contains( xit.getStateInternal() ) )
			break;
	}

	xit.setFilter( pss ); 

	return xit;
}
////////////////////////////////////////////////////////////////////////////////////////
/*!	\return PropertyTable_XIterator pointing to position beyond the last column
	\throw	no-throw */ 
////////////////////////////////////////////////////////////////////////////////////////
PropertyTable_XIterator PropertyTable_YIterator::end() const
{
	//if( m_It->second.begin() == m_It->second.end() ){
	//	VarString msg = "current iterator is begin() == end()!";
	//	throw OutOfRangeIteratorException( 
	//			ExceptInfo( fun, msg.c_str(), __FILE__, __LINE__) );
	//}
	return PropertyTable_XIterator ( m_Pt, 
									 &(*m_It), 
									 m_It->second.end() );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \retval	true if the iterator (this) points to the iterator the method begin() returns.
	\return	otherwise returns false
	\throw	InvalidIteratorException */
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool PropertyTable_YIterator::isBegin() const
{
	checkPropTbl();
	return (m_Pt->begin() == *this);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \retval	true if the iterator (this) points to the iterator the method end() returns.
	\return	otherwise returns false
	\throw	InvalidIteratorException */
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool PropertyTable_YIterator::isEnd() const
{
	checkPropTbl();
	return (m_Pt->end() == *this);
}

////////////////////////////////////////////////////////////////////////////////////////
NumberColumns   PropertyTable_YIterator::size() const
{
	checkPropTbl();
	return m_Pt->getPropertyDescriptionList().size();
}

////////////////////////////////////////////////////////////////////////////////////////
/*! \return	PropertyTable_YIterator filtered switched via the next real stl-iterator 
			(via precrement) to
	\throw	OutOfRangeIteratorException */ 
////////////////////////////////////////////////////////////////////////////////////////
PropertyTable_YIterator& PropertyTable_YIterator::operator ++ ()
{
    FUNCTION_NAME( basar.property.PropertyTable_YIterator.operator++ )

	checkPropTbl();

	const CollPropertyInternalRowIter itEnd = m_Pt->m_CollPIRow.end();

	if( itEnd == m_It )
	{
        throw OutOfRangeIteratorException(ExceptInfo( fun, 
													  "current iterator position is end()!", 
													  __FILE__, __LINE__) );
    }

    ++m_It;
	m_FilteredStep = 1;

	for( ; itEnd != m_It; ++m_It, ++m_FilteredStep )
	{
		if( m_Pss.contains( getState() ) )
			break;
	}

	return *this;
}
////////////////////////////////////////////////////////////////////////////////////////
/*! \return	PropertyTable_YIterator filtered switched via the next real stl-iterator 
			(via precrement) to
	\param	i is a pseudo parmeter to avoid a compiler warning about an 
			unreferenced variable/parameter
	\throw	OutOfRangeIteratorException */ 
////////////////////////////////////////////////////////////////////////////////////////
PropertyTable_YIterator PropertyTable_YIterator::operator ++ ( int )
{
    FUNCTION_NAME( basar.property.PropertyTable_YIterator.operator++(int) )

	checkPropTbl();

	const CollPropertyInternalRowIter itEnd = m_Pt->m_CollPIRow.end();

	if( itEnd == m_It )
	{
        throw OutOfRangeIteratorException( ExceptInfo( fun, 
													   "current iterator position is end()!", 
													   __FILE__, __LINE__) );
    }

    PropertyTable_YIterator it( *this );
    ++m_It; 
	m_FilteredStep = 1;

	for( ; itEnd != m_It; ++m_It, ++m_FilteredStep )
	{
		if( m_Pss.contains( getState() ) )
			break;
	}

	return it;        
}
////////////////////////////////////////////////////////////////////////////////////////
/*! \return	PropertyTable_YIterator filtered switched via the previous real stl-iterator 
			(via precrement) to
	\throw	OutOfRangeIteratorException */ 
////////////////////////////////////////////////////////////////////////////////////////
PropertyTable_YIterator& PropertyTable_YIterator::operator -- ()
{
    FUNCTION_NAME( basar.property.PropertyTable_YIterator.operator-- )

	checkPropTbl();

	const CollPropertyInternalRowIter itBegin = m_Pt->m_CollPIRow.begin();

	if( itBegin == m_It )
	{
        throw OutOfRangeIteratorException( ExceptInfo( fun, 
													   "current iterator position is begin()!", 
													   __FILE__, __LINE__) );
    }

    --m_It;
	m_FilteredStep = 1;

	for( ; itBegin != m_It; --m_It, ++m_FilteredStep )
	{
		if( m_Pss.contains(getState()) )
			break;
	}

    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////
/*! \return	PropertyTable_YIterator filtered switched via the previous real stl-iterator 
			(via precrement) to
	\param	i is a pseudo parmeter to avoid a compiler warning about an 
			unreferenced variable/parameter
	\throw	OutOfRangeIteratorException */ 
////////////////////////////////////////////////////////////////////////////////////////
PropertyTable_YIterator PropertyTable_YIterator::operator -- ( int i )
{
    FUNCTION_NAME( basar.property.PropertyTable_YIterator.operator--(int) )

	i = i;

	checkPropTbl();

	const CollPropertyInternalRowIter itBegin = m_Pt->m_CollPIRow.begin();

	if( itBegin == m_It )
	{
        throw OutOfRangeIteratorException( ExceptInfo( fun, 
													   "current iterator position is begin()!", 
													   __FILE__, __LINE__) );
    }

    PropertyTable_YIterator it( *this );
    --m_It; 
	m_FilteredStep = 1;

	for( ; itBegin != m_It; --m_It, ++m_FilteredStep )
	{
		if( m_Pss.contains(getState()) )
			break;
	}

    return it;        
}
////////////////////////////////////////////////////////////////////////////////////////
/*!	\return	PropertyStateSet (a set of states) indicates 
			which state/s the row/line is in 
	\throw	no-throw */
////////////////////////////////////////////////////////////////////////////////////////
const PropertyStateSet PropertyTable_YIterator::getState() const
{
	/*!	\todo	performance tuning: 
				to find out the state of a row/line at runtime 
				demands the call of this method.
				To avoid these permanent calls the state of the row/line 
				could be updated via an so called 
				derived attribute (an attribute storing the current state of the line)
				with the requirement to update this attribute 
				in several other methods in this class 
				implementation if the state is changed. */

	UInt32 unsetCount   = 0, 
		   unknownCount = 0,
		   cleanCount   = 0, 
		   insertCount  = 0, 
		   updateCount  = 0, 
		   deleteCount  = 0;

	const PropertyTable_XIterator itEnd = end();

	for (PropertyTable_XIterator xit = begin(); !(xit.isEqualInternal(itEnd)); xit.preincrementInteral() )
	{
		switch( xit.getStateInternal().getState() )
		{
		case SS_UNSET:		
			++unsetCount;
			break;

		case SS_UNKNOWN:	
			++unknownCount;
			break;

		case SS_CLEAN:		
			++cleanCount;
			break;

		case SS_INSERT:	
			++insertCount;
			break;

		case SS_UPDATE:	
			++updateCount;
			break;

		case SS_DELETE:	
			++deleteCount;
			break;

		default:		
			break;
		}
	}

//algorithm:
// if all xits (properties in xit) have state SS_UNSET, yit's state is SS_UNSET
// if xits have states SS_UNSET and SS_UNKNOWN only, yit's state is SS_UNKNOWN
// if xits have at least one state SS_CLEAN and no states SS_DELETE, SS_UPDATE or SS_INSERT, yit's state is SS_CLEAN
// if xits have states SS_DELETE, SS_UPDATE or SS_INSERT, yit's state is a combination of those (i.e. xits: SS_DELETE, SS_UPDATE -> yits state: SS_DELETE and SS_UPDATE)

	NumberColumns cols = m_Pt->getPropertyDescriptionList().size();

	// SS_UNSET: all xits have the state SS_UNSET;
	if ( cols == unsetCount )
		return SS_UNSET;
	
	// SS_UNKNOWN: SS_UNKNOWN >= 1 && rest SS_UNSET;
	if( (cols == (unknownCount + unsetCount)) && 
		(1    <= unknownCount) )
		return SS_UNKNOWN;

	// SS_CLEAN: SS_CLEAN >= 1;
	if( (1 <= cleanCount) && 
		(insertCount == 0 && updateCount == 0 && deleteCount == 0) )
		return SS_CLEAN;

	// if one of the following states (SS_INSERT, SS_UPDATE SS_DELETE) appears then it is relevant
	// which state appeared at first. This state determines the returned State;
	// SS_UNKNOWN hier extra auszuschliessen muesste nicht sein;
	// SupportedStateEnum eState = SS_UNKNOWN; // will never happen
	PropertyStateSet ps( static_cast<SupportedStateEnum>( 0 ) ); 

	if( 1 <= deleteCount )
		ps.insert( SS_DELETE );

	if( 1 <= updateCount )
		ps.insert( SS_UPDATE );

	if( 1 <= insertCount )
		ps.insert( SS_INSERT );

    return ps;
}
////////////////////////////////////////////////////////////////////////////////////////
/*!	\param	eState represents the state that has to be set in every single 
			column of the concerned row
	\throw	no-throw */
////////////////////////////////////////////////////////////////////////////////////////
void PropertyTable_YIterator::setState( const SupportedStateEnum eState )
{
	// caution: filtered switching is bypassed here 
	const PropertyTable_XIterator itEnd = end();

	for(PropertyTable_XIterator xit = begin(); xit != itEnd; ++xit )
	{ 
		if( SS_UNSET != xit.getStateInternal().getState() ){
			xit.m_It->setState( eState );
		}
	}
}
////////////////////////////////////////////////////////////////////////////////////////
void PropertyTable_YIterator::setInsertType( InsertTypeEnum type )
{
    m_It->first = type;
}
////////////////////////////////////////////////////////////////////////////////////////
void PropertyTable_YIterator::resetRowState()
{
    const PropertyTable_XIterator itEnd = end();
	
	for(PropertyTable_XIterator xit = begin(); !xit.isEqualInternal(itEnd); xit.preincrementInteral() )
	{ 
		xit.m_It->setState( SS_UNSET );
	}    
}

////////////////////////////////////////////////////////////////////////////////////////
/*!	\retval true if at least one column is in one of the given states 
	\param	pss is/are the possible state/s the state 
			of every column is checked against.
			Important: this method does not change 
			the state of the object it is called for
	\throw	no-throw */ 
////////////////////////////////////////////////////////////////////////////////////////
bool PropertyTable_YIterator::isInState( const PropertyStateSet & pss ) const
{    
    PropertyTable_XIterator       xit   = begin();
	const PropertyTable_XIterator itEnd = end  ();

	for(; xit != itEnd; ++xit )
	{
		if( pss.contains(xit.getStateInternal()) )
			break;
	}

	return (xit != itEnd);
}
////////////////////////////////////////////////////////////////////////////////////////
/*!	\retval true if column is in one of the given states 
	\param	rColName is the passed column name whose state is checked.
	\param	pss is/are the possible state/s the state the column identified 
			via the passed column name is checked against.
	\throw	no-throw */ 
////////////////////////////////////////////////////////////////////////////////////////
bool PropertyTable_YIterator::isInState( const ColumnName      & rColName, 
										 const PropertyStateSet& pss     ) const
{
	return pss.contains(at(getIndex(rColName)).getStateInternal());
}
////////////////////////////////////////////////////////////////////////////////////////
/*! \return PropertyStateSet containing the possible states a row can be in when 
			iterating over it
	\throw	no-throw */
////////////////////////////////////////////////////////////////////////////////////////
const PropertyStateSet& PropertyTable_YIterator::getFilter() const
{
	return m_Pss;
}
////////////////////////////////////////////////////////////////////////////////////////
/*!	\return type of given column
	\param	rColName is the passed column name for which the type is returned.
	\throw	UnknownColumnNameException */
////////////////////////////////////////////////////////////////////////////////////////
const PropertyType& PropertyTable_YIterator::getType( const ColumnName & rColName ) const
{
	checkPropTbl();

	// exception 'UnknownColumnNameException' will NOT be caught                
    return m_Pt->getPropertyDescriptionList().getType( rColName );
}

const PropertyType& PropertyTable_YIterator::getType( const ColumnIndex index ) const
{
	checkPropTbl();

	// exception 'UnknownColumnNameException' will NOT be caught                
    return m_Pt->getPropertyDescriptionList().getType( index );
}


////////////////////////////////////////////////////////////////////////////////////////
/*!	\return state of the given column
	\param	rColName is the passed column name for which the state is returned.
	\throw	UnknownColumnNameException */
////////////////////////////////////////////////////////////////////////////////////////
const PropertyState& PropertyTable_YIterator::getState( const ColumnName & rColName ) const
{
    // exception 'UnknownColumnNameException' will NOT be caught
	return getState(getIndex( rColName ));
}
////////////////////////////////////////////////////////////////////////////////////////
/*!	\return state of the given column
	\param	index is the passed number identifying the right column 
			for which the value is returned.
	\throw	UnknownColumnNameException */
////////////////////////////////////////////////////////////////////////////////////////
const PropertyState& PropertyTable_YIterator::getState( const ColumnIndex index ) const
{
    /*! \todo	open: check if the passed index is in the valid range 
				of the vector of xits (Propertyinternals) */
	return m_It->second[index].getState();
}
////////////////////////////////////////////////////////////////////////////////////////
/*!	\return const Int16&
	\param	rColName is the passed column name for which the value is returned.
	\throw	WrongPropertyTypeException, WrongOperationException, FatalErrorException */
////////////////////////////////////////////////////////////////////////////////////////
const Int16& PropertyTable_YIterator::getInt16( const ColumnName & rColName ) const
{
	return getInt16( getIndex(rColName));
}
////////////////////////////////////////////////////////////////////////////////////////
/*!	\return const Int16&
	\param	index is the passed number identifying the right column 
			for which the value is returned.
	\throw	WrongPropertyTypeException, WrongOperationException, FatalErrorException */
////////////////////////////////////////////////////////////////////////////////////////
const Int16& PropertyTable_YIterator::getInt16( const ColumnIndex index ) const
{
	return at( index ).getInt16();
}
////////////////////////////////////////////////////////////////////////////////////////
/*!	\param	rColName is the passed column name for which the value is set.
	\param	value value to be set
	\throw no-throw */
////////////////////////////////////////////////////////////////////////////////////////
void PropertyTable_YIterator::setInt16( const ColumnName & rColName, 
									    const Int16        value )
{
	setInt16( getIndex(rColName), value );
}
////////////////////////////////////////////////////////////////////////////////////////
/*!	\param	index is the passed number identifying the right column 
			for which the value is set.
	\param	value value to be set
	\throw	no-throw */
////////////////////////////////////////////////////////////////////////////////////////
void PropertyTable_YIterator::setInt16( const ColumnIndex index, 
									    const Int16       value )
{
	// no type checking 
	at( index ).setInt16( value );
}
////////////////////////////////////////////////////////////////////////////////////////
/*!	\return const Int32&
	\param	rColName is the passed column name for which the value is returned.
	\throw	WrongPropertyTypeException, WrongOperationException, FatalErrorException */
////////////////////////////////////////////////////////////////////////////////////////
const Int32& PropertyTable_YIterator::getInt32( const ColumnName& rColName ) const
{
 	return getInt32( getIndex(rColName));
}
////////////////////////////////////////////////////////////////////////////////////////
/*!	\return const Int32&
	\param	index is the passed number identifying the right column 
			for which the value is returned.
	\throw	WrongPropertyTypeException, WrongOperationException, FatalErrorException */
////////////////////////////////////////////////////////////////////////////////////////
const Int32& PropertyTable_YIterator::getInt32( const ColumnIndex index ) const
{
	return at( index ).getInt32();
}

////////////////////////////////////////////////////////////////////////////////////////
/*!	\param	rColName is the passed column name identifying the right column 
			for which the value is set.
	\param	value value to be set
	\throw	no-throw */
////////////////////////////////////////////////////////////////////////////////////////
void PropertyTable_YIterator::setInt32( const ColumnName & rColName, 
									    const Int32        value )
{
	setInt32( getIndex(rColName), value );
}
////////////////////////////////////////////////////////////////////////////////////////
/*!	\param	index is the passed number identifying the right column 
			for which the value is set.
	\param	value value to be set
	\throw	no-throw */
////////////////////////////////////////////////////////////////////////////////////////
void PropertyTable_YIterator::setInt32( const ColumnIndex index, 
									    const Int32       value )
{
	// no homologation testing arranged
	at( index ).setInt32( value );
}

////////////////////////////////////////////////////////////////////////////////////////
/*!	\return const Int32&
\param	rColName is the passed column name for which the value is returned.
\throw	WrongPropertyTypeException, WrongOperationException, FatalErrorException */
////////////////////////////////////////////////////////////////////////////////////////
const Int64 & PropertyTable_YIterator::getInt64(const ColumnName & rColName) const
{
    return getInt64(getIndex(rColName));
}

////////////////////////////////////////////////////////////////////////////////////////
/*!	\return const Int64&
\param	index is the passed number identifying the right column
for which the value is returned.
\throw	WrongPropertyTypeException, WrongOperationException, FatalErrorException */
////////////////////////////////////////////////////////////////////////////////////////
const Int64 & PropertyTable_YIterator::getInt64(const ColumnIndex index) const
{
    return at(index).getInt64();
}

////////////////////////////////////////////////////////////////////////////////////////
/*!	\param	rColName is the passed column name identifying the right column
for which the value is set.
\param	value value to be set
\throw	no-throw */
////////////////////////////////////////////////////////////////////////////////////////
void PropertyTable_YIterator::setInt64(const ColumnName & rColName, const Int64 value)
{
    setInt64(getIndex(rColName), value);
}

////////////////////////////////////////////////////////////////////////////////////////
/*!	\param	index is the passed number identifying the right column
for which the value is set.
\param	value value to be set
\throw	no-throw */
////////////////////////////////////////////////////////////////////////////////////////
void PropertyTable_YIterator::setInt64(const ColumnIndex index, const Int64 value)
{
    at(index).setInt64(value);
}
////////////////////////////////////////////////////////////////////////////////////////
/*!	\return const Float32&
	\param	rColName is the passed column name for which the value is returned.
	\throw	WrongPropertyTypeException, WrongOperationException, FatalErrorException */
////////////////////////////////////////////////////////////////////////////////////////
const Float32& PropertyTable_YIterator::getFloat32( const ColumnName & rColName ) const
{
	return getFloat32( getIndex(rColName));
}
////////////////////////////////////////////////////////////////////////////////////////
/*!	\return const Float32&
	\param	index is the passed number identifying the right column 
			for which the value is returned.
	\throw	WrongPropertyTypeException, WrongOperationException, FatalErrorException */
////////////////////////////////////////////////////////////////////////////////////////
const Float32& PropertyTable_YIterator::getFloat32( const ColumnIndex index ) const
{
	return at( index ).getFloat32();
}
////////////////////////////////////////////////////////////////////////////////////////
/*!	\param	rColName is the passed column name identifying the right column 
			for which the value is set.
	\param	value value to be set
	\throw	no-throw */
////////////////////////////////////////////////////////////////////////////////////////
void PropertyTable_YIterator::setFloat32( const ColumnName & rColName, 
										  const Float32      value )
{
	setFloat32( getIndex(rColName), value );
}
////////////////////////////////////////////////////////////////////////////////////////
/*!	\param	index is the passed number identifying the right column 
			for which the value is set.
	\param	value value to be set
	\throw	no-throw */
////////////////////////////////////////////////////////////////////////////////////////
void PropertyTable_YIterator::setFloat32( const ColumnIndex index, 
										  const Float32     value )
{
	at( index ).setFloat32( value );
}
////////////////////////////////////////////////////////////////////////////////////////
/*!	\return const Float64&
	\param	rColName is the passed column name for which the value is returned.
	\throw	WrongPropertyTypeException, WrongOperationException, FatalErrorException */
////////////////////////////////////////////////////////////////////////////////////////
const Float64& PropertyTable_YIterator::getFloat64( const ColumnName & rColName ) const
{
	return getFloat64( getIndex(rColName));
}
////////////////////////////////////////////////////////////////////////////////////////
/*!	\return const Float64&
	\param	index is the passed number identifying the right column 
			for which the value is returned.
	\throw	WrongPropertyTypeException, WrongOperationException, FatalErrorException */
////////////////////////////////////////////////////////////////////////////////////////
const Float64& PropertyTable_YIterator::getFloat64( const ColumnIndex index ) const
{
	return at( index ).getFloat64();
}
////////////////////////////////////////////////////////////////////////////////////////
/*!	\param	rColName is the passed column name identifying the right column 
			for which the value is set.
	\param	value value to be set
	\throw	no-throw */
////////////////////////////////////////////////////////////////////////////////////////
void PropertyTable_YIterator::setFloat64( const ColumnName & rColName, 
										  const Float64      value )
{
	setFloat64( getIndex(rColName), value );
}
////////////////////////////////////////////////////////////////////////////////////////
/*!	\param	index is the passed number identifying the right column 
			for which the value is set.
	\param	value value to be set
	\throw	no-throw */
////////////////////////////////////////////////////////////////////////////////////////
void PropertyTable_YIterator::setFloat64( const ColumnIndex index, 
										  const Float64     value )
{
	// no type checking
	at( index ).setFloat64( value );
}
////////////////////////////////////////////////////////////////////////////////////////
/*!	\return DateTime
	\param	rColName is the passed column name for which the value is returned.
	\throw	WrongPropertyTypeException, WrongOperationException, FatalErrorException */
////////////////////////////////////////////////////////////////////////////////////////
const DateTime & PropertyTable_YIterator::getDateTime( const ColumnName & rColName ) const
{
	return getDateTime( getIndex(rColName));
}
////////////////////////////////////////////////////////////////////////////////////////
/*!	\return DateTime
	\param	index is the number identifying the right column 
			for which the value is returned.
	\throw	WrongPropertyTypeException, WrongOperationException, FatalErrorException */
////////////////////////////////////////////////////////////////////////////////////////
const DateTime &	PropertyTable_YIterator::getDateTime( const ColumnIndex index ) const
{
	return at( index ).getDateTime();
}
////////////////////////////////////////////////////////////////////////////////////////
/*!	\param	rColName is the passed column name identifying the right column 
			for which the value is set.
	\param	value of type DateTime which has to be set.
	\throw	no-throw */
////////////////////////////////////////////////////////////////////////////////////////
void PropertyTable_YIterator::setDateTime( const ColumnName & rColName, 
										   const DateTime   & value )
{
	setDateTime( getIndex(rColName), value );
}
////////////////////////////////////////////////////////////////////////////////////////
/*!	\param	index is the passed number identifying the right column 
			for which the value is set.
	\param	value of type DateTime which has to be set.
	\throw	no-throw */
////////////////////////////////////////////////////////////////////////////////////////
void PropertyTable_YIterator::setDateTime( const ColumnIndex index, 
										   const DateTime  & value )
{
	at( index ).setDateTime( value );
}
////////////////////////////////////////////////////////////////////////////////////////
/*!	\return TimeSpan
	\param	rColName is the passed column name for which the value is returned.
	\throw	WrongPropertyTypeException, WrongOperationException, FatalErrorException */
////////////////////////////////////////////////////////////////////////////////////////
const TimeSpan & PropertyTable_YIterator::getTimeSpan( const ColumnName & rColName ) const
{
	return getTimeSpan( getIndex(rColName));
}
////////////////////////////////////////////////////////////////////////////////////////
/*!	\return TimeSpan
	\param	index is the number identifying the right column 
			for which the value is returned.
	\throw	WrongPropertyTypeException, WrongOperationException, FatalErrorException */
////////////////////////////////////////////////////////////////////////////////////////
const TimeSpan &	PropertyTable_YIterator::getTimeSpan( const ColumnIndex index ) const
{
	return at( index ).getTimeSpan();
}
////////////////////////////////////////////////////////////////////////////////////////
/*!	\param	rColName is the passed column name identifying the right column 
			for which the value is set.
	\param	value of type TimeSpan which has to be set.
	\throw	no-throw */
////////////////////////////////////////////////////////////////////////////////////////
void PropertyTable_YIterator::setTimeSpan( const ColumnName & rColName, 
										   const TimeSpan   & value )
{
	setTimeSpan( getIndex(rColName), value );
}
////////////////////////////////////////////////////////////////////////////////////////
/*!	\param	index is the passed number identifying the right column 
			for which the value is set.
	\param	value of type TimeSpan which has to be set.
	\throw	no-throw */
////////////////////////////////////////////////////////////////////////////////////////
void PropertyTable_YIterator::setTimeSpan( const ColumnIndex index, 
										   const TimeSpan  & value )
{
	at( index ).setTimeSpan( value );
}
////////////////////////////////////////////////////////////////////////////////////////
/*!	\return DateTime with the time value 0
	\param	rColName is the passed column name for which the value is returned.
	\throw	WrongPropertyTypeException, WrongOperationException, FatalErrorException */
////////////////////////////////////////////////////////////////////////////////////////
const Date & PropertyTable_YIterator::getDate( const ColumnName & rColName ) const
{
	return getDate( getIndex(rColName));
}
////////////////////////////////////////////////////////////////////////////////////////
/*!	\return DateTime with the time value 0
	\param	index is the number identifying the right column 
			for which the value is returned.
	\throw	WrongPropertyTypeException, WrongOperationException, FatalErrorException */
////////////////////////////////////////////////////////////////////////////////////////
const Date &	PropertyTable_YIterator::getDate( const ColumnIndex index ) const
{
	return at( index ).getDate();
}
////////////////////////////////////////////////////////////////////////////////////////
/*!	\param	rColName is the passed column name identifying the right column 
			for which the value is set.
	\param	value value to be set
	\throw	no-throw */
////////////////////////////////////////////////////////////////////////////////////////
void PropertyTable_YIterator::setDate( const ColumnName & rColName, 
									   const Date       & value )
{
	setDate( getIndex(rColName), value );
}
////////////////////////////////////////////////////////////////////////////////////////
/*!	\param	index is the passed number identifying the right column 
			for which the value is set.
	\param	value value to be set
	\throw	no-throw */
////////////////////////////////////////////////////////////////////////////////////////
void PropertyTable_YIterator::setDate( const ColumnIndex index, 
									   const Date      & value )
{
	at( index ).setDate( value );
}
////////////////////////////////////////////////////////////////////////////////////////
/*!	\return DateTime with the time value 0
	\param	rColName is the passed column name for which the value is returned.
	\throw	WrongPropertyTypeException, WrongOperationException, FatalErrorException */
////////////////////////////////////////////////////////////////////////////////////////
const Time & PropertyTable_YIterator::getTime( const ColumnName & rColName ) const
{
	return getTime( getIndex(rColName));
}
////////////////////////////////////////////////////////////////////////////////////////
/*!	\return DateTime with the time value 0
	\param	index is the number identifying the right column 
			for which the value is returned.
	\throw	WrongPropertyTypeException, WrongOperationException, FatalErrorException */
////////////////////////////////////////////////////////////////////////////////////////
const Time &	PropertyTable_YIterator::getTime( const ColumnIndex index ) const
{
	return at( index ).getTime();
}
////////////////////////////////////////////////////////////////////////////////////////
/*!	\param	rColName is the passed column name identifying the right column 
			for which the value is set.
	\param	value value to be set
	\throw	no-throw */
////////////////////////////////////////////////////////////////////////////////////////
void PropertyTable_YIterator::setTime( const ColumnName & rColName, 
									   const Time       & value )
{
	setTime( getIndex(rColName), value );
}
////////////////////////////////////////////////////////////////////////////////////////
/*!	\param	index is the passed number identifying the right column 
			for which the value is set.
	\param	value value to be set
	\throw	no-throw */
////////////////////////////////////////////////////////////////////////////////////////
void PropertyTable_YIterator::setTime( const ColumnIndex index, 
									   const Time      & value )
{
	at( index ).setTime( value );
}
////////////////////////////////////////////////////////////////////////////////////////
/*!	\return binary
\param	rColName is the passed column name for which the value is returned.
\throw	WrongPropertyTypeException, WrongOperationException, FatalErrorException */
////////////////////////////////////////////////////////////////////////////////////////
const Binary & PropertyTable_YIterator::getBinary(const ColumnName & rColName) const
{
    return getBinary( getIndex(rColName) );
}
////////////////////////////////////////////////////////////////////////////////////////
/*!	\return Decimal
\param	index is the passed number identifying the right column
for which the value is returned for.
\throw	WrongPropertyTypeException, WrongOperationException, FatalErrorException */
////////////////////////////////////////////////////////////////////////////////////////
const Binary & PropertyTable_YIterator::getBinary(const ColumnIndex index) const
{
    return at( index ).getBinary();
}
////////////////////////////////////////////////////////////////////////////////////////
/*!	\param	rColName is the passed column name identifying the right column
for which the value is set.
\param	value value to be set
\throw	no-throw */
////////////////////////////////////////////////////////////////////////////////////////
void PropertyTable_YIterator::setBinary(const ColumnName & rColName, const Binary & value)
{
    setBinary( getIndex( rColName ), value );
}
void PropertyTable_YIterator::setBinary(const ColumnIndex index, const Binary & value)
{
    at( index ).setBinary( value );
}
////////////////////////////////////////////////////////////////////////////////////////
/*!	\return Decimal
	\param	rColName is the passed column name for which the value is returned.
	\throw	WrongPropertyTypeException, WrongOperationException, FatalErrorException */
////////////////////////////////////////////////////////////////////////////////////////
const Decimal & PropertyTable_YIterator::getDecimal( const ColumnName & rColName ) const
{
	return getDecimal( getIndex(rColName));
}
////////////////////////////////////////////////////////////////////////////////////////
/*!	\return Decimal
	\param	index is the passed number identifying the right column 
			for which the value is returned for.
	\throw	WrongPropertyTypeException, WrongOperationException, FatalErrorException */
////////////////////////////////////////////////////////////////////////////////////////
const Decimal & PropertyTable_YIterator::getDecimal( const ColumnIndex index ) const
{
	return at( index ).getDecimal();
}
////////////////////////////////////////////////////////////////////////////////////////
/*!	\param	rColName is the passed column name identifying the right column 
			for which the value is set.
	\param	value value to be set
	\throw	no-throw */
////////////////////////////////////////////////////////////////////////////////////////
void PropertyTable_YIterator::setDecimal( const ColumnName & rColName, 
										  const Decimal    & value )
{
	setDecimal( getIndex(rColName), value );
}
////////////////////////////////////////////////////////////////////////////////////////
/*!	\param	index is the passed number identifying the right column 
			for which the value is set.
	\param	value value to be set
	\throw	no-throw */
////////////////////////////////////////////////////////////////////////////////////////
void PropertyTable_YIterator::setDecimal( const ColumnIndex index, 
										  const Decimal   & value	 )
{
	at( index ).setDecimal( value );
}
////////////////////////////////////////////////////////////////////////////////////////
/*!	\return I18nString
	\param	rColName is the passed column name for which the value is returned.
	\throw	WrongPropertyTypeException, WrongOperationException, FatalErrorException */
////////////////////////////////////////////////////////////////////////////////////////
const I18nString & PropertyTable_YIterator::getString( const ColumnName & rColName ) const
{
	return getString( getIndex(rColName));
}
////////////////////////////////////////////////////////////////////////////////////////
/*!	\return I18nString
	\param	index is the passed number identifying the right column 
			for which the value is returned.
	\throw	WrongPropertyTypeException, WrongOperationException, FatalErrorException */
////////////////////////////////////////////////////////////////////////////////////////
const I18nString & PropertyTable_YIterator::getString( const ColumnIndex index ) const
{
	return at( index ).getString();
}
////////////////////////////////////////////////////////////////////////////////////////
/*!	\param	rColName is the passed column name identifying the right column 
			for which the value is set.
	\param	value value to be set
	\throw	no-throw */
////////////////////////////////////////////////////////////////////////////////////////
void PropertyTable_YIterator::setString( const ColumnName & rColName, 
										 const I18nString & value )
{
	setString( getIndex(rColName), value );
}
////////////////////////////////////////////////////////////////////////////////////////
/*!	\param	index is the passed number identifying the right column 
			for which the value is set.
	\param	value value to be set
	\throw	no-throw */
////////////////////////////////////////////////////////////////////////////////////////
void PropertyTable_YIterator::setString( const ColumnIndex  index, 
										 const I18nString & value )
{
	at( index ).setString( value );
}

////////////////////////////////////////////////////////////////////////////////////////
/*!	\param	rColName is the passed column name identifying the right column 
			for which the value is set.
	\param	value value to be set
	\throw	no-throw */
////////////////////////////////////////////////////////////////////////////////////////
void PropertyTable_YIterator::setString( const ColumnName & rColName, 
										 ConstBuffer value )
{
	setString( getIndex(rColName), value );
}
////////////////////////////////////////////////////////////////////////////////////////
/*!	\param	index is the passed number identifying the right column 
			for which the value is set.
	\param	value value to be set
	\throw	no-throw */
////////////////////////////////////////////////////////////////////////////////////////
void PropertyTable_YIterator::setString( const ColumnIndex  index, 
										 ConstBuffer value )
{
	at( index ).setString( value );
}

////////////////////////////////////////////////////////////////////////////////////////
/*!	\return VarString
	\param	rColName is the passed column name for which the value is returned.
	\throw	WrongPropertyTypeException */
////////////////////////////////////////////////////////////////////////////////////////
const VarString PropertyTable_YIterator::getSQLString( const ColumnName & rColName ) const
{
	return at( getIndex(rColName)).getSQLString();
}
////////////////////////////////////////////////////////////////////////////////////////
/*!	\return index for given column
	\param	rColName is the passed column name for which the value is returned.
	\throw	UnknownColumnNameException, WrongTypeException */
////////////////////////////////////////////////////////////////////////////////////////
ColumnIndex PropertyTable_YIterator::getIndex( const ColumnName & rColName ) const
{
	checkPropTbl();
	return m_Pt->getPropertyDescriptionList().getIndex(rColName);
}

////////////////////////////////////////////////////////////////////////////////////////
/*!	\return InsertTypeEnum indicates the type the row/line was inserted
	\throw	no-throw */
////////////////////////////////////////////////////////////////////////////////////////
InsertTypeEnum PropertyTable_YIterator::getInsertType() const
{
	return m_It->first;
}
//------------------------------------------------------------------------------------//
// contains-methods
//------------------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////////
/*! \retval true if the passed column name is found
	\return otherwise returns false
	\param	rColName is the passed column name that is searched.
	\throw	no-throw */
////////////////////////////////////////////////////////////////////////////////////////
bool PropertyTable_YIterator::contains( const ColumnName & rColName ) const
{
	checkPropTbl();
	return m_Pt->getPropertyDescriptionList().contains( rColName );
}
////////////////////////////////////////////////////////////////////////////////////////
/*! \retval true if the passed column name with the given type is found
	\return otherwise returns false
	\param	rColName is the passed column name that is searched.
	\param	pt is the passed type which is searched.
	\throw	no-throw */
////////////////////////////////////////////////////////////////////////////////////////
bool PropertyTable_YIterator::contains( const ColumnName   & rColName, 
									    const PropertyType & pt      ) const
{
	return contains( std::make_pair(rColName, pt) );
}
////////////////////////////////////////////////////////////////////////////////////////
/*! \retval true if the passed column name with the given 
			type in the form of a pair is found
	\return otherwise returns false
	\param	p is the pair of the column name and the type which are searched.
	\throw	no-throw */
////////////////////////////////////////////////////////////////////////////////////////
bool PropertyTable_YIterator::contains( const std::pair<ColumnName, PropertyType> & p) const
{
	checkPropTbl();
	return m_Pt->getPropertyDescriptionList().contains( p );
}
////////////////////////////////////////////////////////////////////////////////////////
/*! \retval true if the state of the passed column name with the given 
			type is found in an amount of states
	\return otherwise returns false
	\param	rColName is column name which is searched.
	\param	pt is the type which is searched.
	\param	pss contains the possible states the column should be in.
	\throw	no-throw */
////////////////////////////////////////////////////////////////////////////////////////
bool PropertyTable_YIterator::contains( const ColumnName       & rColName, 
										const PropertyType     & pt      , 
										const PropertyStateSet & pss     ) const
{
	return contains( std::make_pair(rColName, pt), pss );
}
////////////////////////////////////////////////////////////////////////////////////////
/*! \retval true if the passed column name with the given 
			type in the form of a pair is found in an amount of states
	\return otherwise returns false
	\param	p is the pair of the column name and the type which is searched.
	\param	pss contains the possible states the column should be in.
	\throw	no-throw */
////////////////////////////////////////////////////////////////////////////////////////
bool PropertyTable_YIterator::contains( const std::pair<ColumnName, PropertyType> & p  , 
									    const PropertyStateSet                    & pss) const
{
	checkPropTbl();

	// PropertyType() == PropertyType::UNKNOWN --> ColumnName is compared only
	if( !(m_Pt->getPropertyDescriptionList().contains(p)) )
	{
		return false;
	}

	//! \todo here the propertytype is disregarded yet
	return at(getIndex(p.first)).isInStateInternal(pss);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\retval	true if given property is set and yiterator is not end 
	\return	otherwise returns false 
	\param	rColName property to check if it is set
	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
bool PropertyTable_YIterator::isNotEndAndSet( const ColumnName & rColName ) const
{
	return ( (!isEnd()) && 
		     (getState(rColName) != SS_UNSET) );
}
	
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\retval	true if given property is set and contained in yiterator
	\return	otherwise returns false 
	\param	rColName property to check
	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
bool PropertyTable_YIterator::isContainedAndSet( const ColumnName & rColName ) const
{
	return ( (contains(rColName)) && 
		     (getState(rColName) != SS_UNSET) );
}

//------------------------------------------------------------------------------------//
// uninitialized iterator - safe access methods to it
//------------------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////////
/*! \throw	NullReferenceException */
////////////////////////////////////////////////////////////////////////////////////////
void PropertyTable_YIterator::checkPropTbl() const
{
	FUNCTION_NAME( basar.property.PropertyTable_YIterator.checkPropTbl )

	if ( isNull() )
	{
		throw NullReferenceException( ExceptInfo( fun, 
												  "Iterator is not initialized/valid: "
												  "PropertyTable m_Pt is NULL!", 
												  __FILE__, __LINE__));
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \retval true if property table shared pointer is not initialzed 
	\return otherwise returns false
	\throw	no-throw */ 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool PropertyTable_YIterator::isNull() const
{
	return m_Pt.get() == 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \param	rColName passed column name referring to the xiterator.
	\param	eState the passed state which has to be set to the passed xiterator.
	\throw	basar exceptions */ 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PropertyTable_YIterator::setPropertyState( const ColumnName       & rColName, 
											    const SupportedStateEnum eState )
{
	setPropertyState(getIndex(rColName), eState );

    if (LoggerPool::propertyTable_YIterator().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
	    VarString msg;
	    msg.format("property '%s' set to state '%s'", 
            rColName.c_str(),
            PropertyState(eState).toString().c_str());
	    LoggerPool::propertyTable_YIterator().forcedLog(log4cplus::DEBUG_LOG_LEVEL,
			                                                  msg, 
															  __FILE__, __LINE__);
    }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \param	rColIndex index of the column
	\param	eState the passed state which has to be set to the passed xiterator.
	\throw	basar exceptions */ 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PropertyTable_YIterator::setPropertyState( const ColumnIndex      & rColIndex, 
												const SupportedStateEnum eState )
{
	at( rColIndex ).setPropertyState( eState );

    if (LoggerPool::propertyTable_YIterator().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
	    VarString msg;
	    msg.format("property no. '%d' set to state '%s'", 
            rColIndex ,
            PropertyState(eState).toString().c_str());
	    LoggerPool::propertyTable_YIterator().forcedLog(log4cplus::DEBUG_LOG_LEVEL, 
			                                                  msg, 
															  __FILE__, __LINE__);
    }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	number of steps (one step = one dataset) jumped while doing the filtered iteration
	\throw	no-throw */ 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
UInt32 PropertyTable_YIterator::getFilteredStep() const
{
	return m_FilteredStep;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \param	strm parameter is default initialized and could be overwritten by any std::ostream object,
			e.g. by the error channel or any object of a ofstream file.
  	\throw  no-throw */ 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PropertyTable_YIterator::toStream( std::ostream& strm /* = std::cout */ ) const
{
	if (isEnd())
		return;

	VarString                     value;
	const PropertyTable_XIterator itEnd = end();

	for (PropertyTable_XIterator xit = begin(); xit != itEnd; ++xit )
	{
		value = "";

		if( xit.getState().getState() != SS_UNSET )
			value = xit.getSQLString().c_str();


		if ( xit == begin())
		{
			VarString addr;
			addr.format("0x%0*lx", sizeof(size_t) * 2, this);

			strm << "--- PropertyTable_YIterator @ " << addr
				 << " -------------------"           << std::endl;
		}

		VarString no;
		no.format("#%-5d", xit.getIndex());
		
		strm << no
			 <<       " name : <" << xit.getName()			   << ">" << std::endl
			 << "       value: <" << value					   << ">" << std::endl
			 << "       state: <" << xit.getState().toString() << ">" << std::endl
			 << "       type : <" << xit.getType ().toString() << ">" << std::endl
			 << "------------------------------------------------"    << std::endl;
	}
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*! returns the X-iterator at columnindex i
\param columnindex columnindex
\throw  no-throw 
\return the X-iterator at ColumnIndex columnindex
*/ 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
PropertyTable_XIterator	PropertyTable_YIterator::getXIterator(const ColumnIndex columnindex) const
{
    PropertyTable_XIterator retval = begin();

    if (columnindex > size())
    {
        retval = end();
    }
    else if (columnindex > 0)
    {
       retval = retval.skip(columnindex);
    }

    return retval;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*! returns the X-iterator at column rColName
\param rColName columnname 
\throw  no-throw
\return the X-iterator at column rColName
*/ 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
PropertyTable_XIterator	PropertyTable_YIterator::getXIterator(const VarString & rColName) const
{
    return getXIterator(getIndex(rColName));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*! returns the Columnname for an index
\param index Columnindex 
\throw  no-throw
\return Columnname for ColumnIndex index
*/ 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
ColumnName PropertyTable_YIterator::getName(ColumnIndex index) const
{
    checkPropTbl();
    return m_Pt->getPropertyDescriptionList().getName( index );
}

//------------------------------------------------------------------------------------------------------------
} // End namespace property
} // End namespace basar
