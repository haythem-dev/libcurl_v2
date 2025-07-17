//----------------------------------------------------------------------------------------//
/*! \file
 *  \brief  class PropertyTable_XIterator represents an iterator pointing 
			to a single propertyinternal (every column)
 *  \author Bischof Björn 
 *  \date   23.08.2005
 */
//----------------------------------------------------------------------------------------//

//----------------------------------------------------------------------------------------//
// includes
//----------------------------------------------------------------------------------------//

#include "libbasarproperty_propertytable_xiterator.h"

#include "libbasarproperty_propertystate.h"
#include "propertytable.h"
#include "propertyinternal.h"
#include "loggerpool.h"
#include "libbasarproperty_propertytype.h"
#include "libbasarproperty_exceptions.h"

#include "../macro.h"

//----------------------------------------------------------------------------------------//
// namespaces
//----------------------------------------------------------------------------------------//
namespace basar
{
namespace property
{
////////////////////////////////////////////////////////////////////////////////////////////
/*! \throw	no-throw */ 
////////////////////////////////////////////////////////////////////////////////////////////
PropertyTable_XIterator::PropertyTable_XIterator() : m_pInsertTypeCollColumn( 0 )
{
}

////////////////////////////////////////////////////////////////////////////////////////
/*! \param	rhs passed PropertyStateSet object with states that will be set to the 
			PropertyStateSet member m_Pss.
	\throw	no-throw */  
////////////////////////////////////////////////////////////////////////////////////////
PropertyTable_XIterator::PropertyTable_XIterator( const PropertyStateSet& rhs )
{
	m_Pss = rhs;
}
////////////////////////////////////////////////////////////////////////////////////////
/*! \param	rhs passed PropertyTable_XIterator object that will be assigned to the new 
			instantiated one with regard to the PropertyStateSet member of the passed rhs.
	\throw	no-throw */  
////////////////////////////////////////////////////////////////////////////////////////
PropertyTable_XIterator::PropertyTable_XIterator( const PropertyTable_XIterator& rhs )
{
	// assigns the members of the passed object
	assign( rhs );
}
////////////////////////////////////////////////////////////////////////////////////////
/*! \param	rhs passed PropertyTable_XIterator object that will be assigned to the new 
			instantiated one with regard to the PropertyStateSet member of the passed rhs.
	\param	pss afterwards overwrites the PropertyStateSet member of the new object.
	\throw	no-throw */  
////////////////////////////////////////////////////////////////////////////////////////
PropertyTable_XIterator::PropertyTable_XIterator( const PropertyTable_XIterator& rhs, 
												  const PropertyStateSet& pss )
{
	// assigns the members of the passed object
	assign( rhs );
	
	/*! \todo	reflecting if the passed PropertyStateSet object should be 
				simply assigned to the PropertyStateSet member (as active) or 
				should be inserted into the PropertyStateSet member or 
				there should be a check if the states of the passed PropertyTable_YIterator
				are contained in the passed PropertyStateSet object.*/
	m_Pss = pss;
}
////////////////////////////////////////////////////////////////////////////////////////
/*! \param	rhs passed PropertyTable_XIterator object that will be assigned to the left one 
			with regard to the PropertyStateSet member of the passed rhs.
    \return PropertyTable_XIterator which has been assigned			
	\throw	no-throw */  
////////////////////////////////////////////////////////////////////////////////////////
PropertyTable_XIterator& PropertyTable_XIterator::operator=( const PropertyTable_XIterator& rhs )
{
	// avoid self assignment
	if( &rhs == this ) 
		return *this; 

	// assigns the members of the passed object
	assign( rhs );
	
	// returning self reference
	return *this; 
}

////////////////////////////////////////////////////////////////////////////////////////
/*! \param	rhs passed PropertyTable_XIterator object whose values are set to this.
	\throw	no-throw */  
////////////////////////////////////////////////////////////////////////////////////////
void PropertyTable_XIterator::assign( const PropertyTable_XIterator& rhs )
{
	m_Pt                    = rhs.m_Pt;
    m_It                    = rhs.m_It;
	m_pInsertTypeCollColumn = rhs.m_pInsertTypeCollColumn;
	
	m_itPropertyInternalColumnEnd = rhs.m_itPropertyInternalColumnEnd;
	
    const basar::SupportedStateEnum rhsState = rhs.m_Pss.getStates();

	if( (rhsState != SS_UNKNOWN) && 
		(rhsState != SS_UNSET  )    )
	{
		m_Pss = rhs.m_Pss;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////
/*! \param	pt is the shared pointer to class aggregating the container.
	\param	p is a bald pointer to a pair of the insert type and the actual value.
			It collides with our claim to a safe memory management. 
			But in this context it is intention.
	\param	it is the pure/real iterator.
	\throw	no-throw. */ 
////////////////////////////////////////////////////////////////////////////////////////////
PropertyTable_XIterator::PropertyTable_XIterator( PropertyTableSharedPtr                     pt, 
												  PairInsertTypeCollPropertyInternalColumn * p, 
												  CollPropertyInternalColumnIter             it)
												: m_Pt                   ( pt ), 
												  m_pInsertTypeCollColumn( p  ), 
												  m_It                   ( it ),
												  m_itPropertyInternalColumnEnd(p->second.end())
{
}

////////////////////////////////////////////////////////////////////////////////////////////
/*! \retval true if xit is equal to this 
	\param	xit is the iterator compared with this. 
			Comparing criteria are the same container, same iterator and same value
	\throw	InvalidIteratorException, DifferentContainersIteratorException */ 
////////////////////////////////////////////////////////////////////////////////////////////
bool PropertyTable_XIterator::operator == ( const PropertyTable_XIterator& xit ) const
{
	FUNCTION_NAME(basar.property.PropertyTable_XIterator.operator==)	

	// checks if the iterator is valid/initialised
	checkInitialization();

    //don't check if xit is initialized, next if will throw except anyway

	// comparison: same container and same iterator
	if( xit.m_Pt != m_Pt )
	{
		VarString s;
		s.format( "Iterator of different container (property: '%s')!", getName().c_str() ); 
		ExceptInfo sInfo( fun, s, __FILE__, __LINE__ );
		throw DifferentContainersIteratorException(sInfo);
	}
    
    return isEqualInternal(xit);

}

bool PropertyTable_XIterator::isEqualInternal(const PropertyTable_XIterator& xit) const
{
	return  (&(xit.m_pInsertTypeCollColumn->second) == &(m_pInsertTypeCollColumn->second))	&& 
            ( xit.m_It == m_It );
}

////////////////////////////////////////////////////////////////////////////////////////////
/*! \retval true if xit is not equal to this 
	\param	rhp is the iterator compared for unequality
	\throw	no-throw */ 
////////////////////////////////////////////////////////////////////////////////////////////
bool PropertyTable_XIterator::operator != ( const PropertyTable_XIterator& rhp ) const
{
    return ! ( this->operator == (rhp) );
}
////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	PropertyTable_XIterator next iterator (via precrement)
	\throw	OutOfRangeIteratorException */ 
////////////////////////////////////////////////////////////////////////////////////////////
PropertyTable_XIterator& PropertyTable_XIterator::operator++()
{

    checkInitialization();

    return preincrementInteral();

}

PropertyTable_XIterator& PropertyTable_XIterator::preincrementInteral()
{
    FUNCTION_NAME( basar.property.PropertyTable_XIterator.preincrementInteral )

	//CollPropertyInternalColumn::const_iterator itEnd = m_pInsertTypeCollColumn->second.end();

	if( m_itPropertyInternalColumnEnd == m_It )
	{
		VarString s;
		s.format( "Current position of iterator is end() (property: '%s')!", 
			     getName().c_str() ); 
		throw OutOfRangeIteratorException( ExceptInfo( fun, s, __FILE__, __LINE__) );
    }

    ++m_It;

	while(	m_itPropertyInternalColumnEnd != m_It && (!isInStateInternal(m_Pss)) )
	{
		++m_It;	
	}

    return *this;    
}
 		
////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	PropertyTable_XIterator switched to the next iterator via postcrement
	\param	i post-crement
	\throw	OutOfRangeIteratorException*/ 
////////////////////////////////////////////////////////////////////////////////////////////
PropertyTable_XIterator PropertyTable_XIterator::operator ++ ( int )
{
    FUNCTION_NAME( basar.property.PropertyTable_XIterator.operator++(int) )

    checkInitialization();

	if( m_itPropertyInternalColumnEnd == m_It ) {
		VarString s;
		s.format( "Current position of iterator is end() (property: '%s')!", getName().c_str() ); 
        throw OutOfRangeIteratorException( ExceptInfo( fun, s, __FILE__, __LINE__) );
    }  

    PropertyTable_XIterator it( *this );
    ++m_It;                

	while(	m_itPropertyInternalColumnEnd != m_It && (!isInStateInternal(m_Pss)) )
	{
		++m_It;	
	}

    return it;
}
////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	PropertyTable_XIterator is the new iterator
	\param	difftype is the distance to jump
	\throw	no-throw*/ 
////////////////////////////////////////////////////////////////////////////////////////////
PropertyTable_XIterator PropertyTable_XIterator::skip( PropertyTable_XIterator::Difference_Type difftype ) const
{
	return this->operator+( difftype );
}
////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	PropertyTable_XIterator is the new iterator
	\param	difftype is the distance to jump
	\throw	no-throw */ 
////////////////////////////////////////////////////////////////////////////////////////////
PropertyTable_XIterator PropertyTable_XIterator::operator + ( PropertyTable_XIterator::Difference_Type difftype ) const
{	
    // return this + integer
	PropertyTable_XIterator tmp(*this);
    tmp.getIter() += difftype;
    return tmp;            
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \param	eState is the passed state which has to be checked if it is allowed to set it
	\throw	InvalidParameterException */ 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PropertyTable_XIterator::setPropertyState( const SupportedStateEnum eState )
{
	FUNCTION_NAME( basar.property.PropertyTable_XIterator.setPropertyState() )

    checkInitialization();

	PropertyState ps( m_It->getState().getState() );

	if (ps == eState)
	{	
		if (LoggerPool::propertyTable_XIterator().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
		{
			VarString msg;
			msg.format("property state <%s> unchanged", getState().toString().c_str());
			LoggerPool::propertyTable_XIterator().forcedLog(log4cplus::DEBUG_LOG_LEVEL, 
																  msg, 
																  __FILE__, __LINE__);
		}

		return;
	}

	switch( eState )
	{
		case SS_UNSET:		
			m_It->setState( eState );
			break;

		case SS_UNKNOWN:	// fall through
		case SS_CLEAN:
			{
				VarString errMsg; 
                errMsg.format( "Try to set an invalid state (unknown or clean) of the property '%s'!", 
						       getName().c_str() );
				throw InvalidParameterException( ExceptInfo(fun, errMsg, __FILE__, __LINE__) );
			}
            break;
		case SS_INSERT:		// fall through
		case SS_UPDATE:		// fall through
		case SS_DELETE:		
			if ((ps == SS_INSERT) || 
				(ps == SS_UPDATE) || 
				(ps == SS_DELETE) || 
				(ps == SS_CLEAN )    )
			{
				m_It->setState( eState );
			} 
			else 
			{
				// changes op 193, throws exception at states unset or unknown.
				VarString errMsg; 
				errMsg.format( "Try to change an invalid state <%s> of property '%s'!", 
							   (ps == SS_UNSET) ? "unset" : "unknown",
								getName().c_str() );

				throw InvalidParameterException( ExceptInfo(fun, errMsg, __FILE__, __LINE__) );
			}
			break;

		default:			
			break;
	}

	if (LoggerPool::propertyTable_XIterator().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format("property state set to <%s>", getState().toString().c_str());
		LoggerPool::propertyTable_XIterator().forcedLog(log4cplus::DEBUG_LOG_LEVEL, 
																msg, 
																__FILE__, __LINE__);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////
/*! \retval true if passed state is equal to current state
	\return otherwise returns false
	\param	pss is the passed state/s the actual state is compared with
	\throw	no-throw*/ 
////////////////////////////////////////////////////////////////////////////////////////////
bool PropertyTable_XIterator::isInState( const PropertyStateSet& pss ) const
{
	return pss.contains( getState() );
}

bool PropertyTable_XIterator::isInStateInternal( const PropertyStateSet& pss ) const
{
    return pss.contains( m_It->getState() );
}

////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	ColumnName is the name of column the iterator is pointing to.
	\throw	no-throw */ 
////////////////////////////////////////////////////////////////////////////////////////////
const ColumnName& PropertyTable_XIterator::getName() const
{
    return getPt()->getPropertyDescriptionList().getName( getIndex() );
}
////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	PropertyType is the datatype of the object the iterator is pointing to.
	\throw	no-throw */ 
////////////////////////////////////////////////////////////////////////////////////////////
const PropertyType& PropertyTable_XIterator::getType() const
{
    checkInitialization();
    return getTypeInternal();
}

const PropertyType& PropertyTable_XIterator::getTypeInternal() const
{
    return m_Pt->getPropertyDescriptionList().getType( getIndexInteral() );
}

////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	PropertyState represents the state of the object the iterator is pointing to.
	\throw	no-throw */ 
////////////////////////////////////////////////////////////////////////////////////////////
const PropertyState& PropertyTable_XIterator::getState() const
{
    return getIter()->getState();
}

////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	PropertyState represents the state of the object the iterator is pointing to. \n internal use only
	\throw	no-throw */ 
////////////////////////////////////////////////////////////////////////////////////////////
const PropertyState& PropertyTable_XIterator::getStateInternal() const
{
    return m_It->getState();
}


////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	ColumnIndex is the position the iterator points to in the propertydescriptionlist
	\throw	no-throw */ 
////////////////////////////////////////////////////////////////////////////////////////////
ColumnIndex PropertyTable_XIterator::getIndex() const
{
    checkInitialization();
    
    return getIndexInteral();
}

ColumnIndex PropertyTable_XIterator::getIndexInteral() const
{
    return m_It - m_pInsertTypeCollColumn->second.begin();
}

//---------------------------------------------------------------------------------------------------------------------------//
// data type dependent set-/get-methods
//---------------------------------------------------------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////////////
/*! \param	value set to the object the iterator is pointing to.
	\throw	WrongPropertyTypeException */ 
////////////////////////////////////////////////////////////////////////////////////////////
void PropertyTable_XIterator::setInt16( const Int16& value )
{
    FUNCTION_NAME( basar.property.PropertyTable_XIterator.setInt16(Int16) )
	
	checkInitialization();
	
	if( INT16 != getTypeInternal().getType() ){
		VarString s;
		s.format( "An INT-method was called for a not as INT-declared Property (property: '%s')!", 
			     getName().c_str() );
		throw WrongPropertyTypeException( ExceptInfo( fun, s, __FILE__, __LINE__) );
    }
	
	if( SS_CLEAN == m_It->getState().getState() )
	{
		if (value == m_It->getInt16()) 
			return;
	}

    determineState();

	m_It->setInt16( value );
}
////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	const Int16&
	\throw	WrongPropertyTypeException, WrongOperationException */ 
////////////////////////////////////////////////////////////////////////////////////////////
const Int16& PropertyTable_XIterator::getInt16() const
{
	FUNCTION_NAME( basar.property.PropertyTable_XIterator.getInt16() )
	
	checkInitialization();
	
	if( INT16 != getTypeInternal().getType() ){
		VarString s;
		s.format( "An INT-method was called for a not as INT-declared Property (property: '%s')!", 
			      getName().c_str() );
        throw WrongPropertyTypeException( ExceptInfo( fun, s, __FILE__, __LINE__) );
    }   
	
	SupportedStateEnum eState = m_It->getState().getState();
	if( SS_UNSET == eState ){
		VarString s;
		s.format( "Operation was executed on an unset state (property: '%s')!", getName().c_str() ); 
		throw WrongOperationException( ExceptInfo( fun, s, __FILE__, __LINE__) );
	}

	return m_It->getInt16();
}
////////////////////////////////////////////////////////////////////////////////////////////
/*! \param	value set to the object the iterator is pointing to.
	\throw	WrongPropertyTypeException */ 
////////////////////////////////////////////////////////////////////////////////////////////
void PropertyTable_XIterator::setInt32( const Int32& value )
{
    FUNCTION_NAME( basar.property.PropertyTable_XIterator.setInt32(Int32) )

    checkInitialization();

	SupportedTypeEnum type = getTypeInternal().getType();

	if ( INT32   != type && 
		 INTDATE != type && 
		 INTTIME != type)
	{
		VarString s;
		s.format( "An INT-method was called for a not as INT-declared property (property: '%s')!", 
			      getName().c_str() ); 
        throw WrongPropertyTypeException( ExceptInfo( fun, s, __FILE__, __LINE__) );
    }
	
	if( SS_CLEAN == m_It->getState().getState() )
	{
		if (value == m_It->getInt32())
			return;
	}

    determineState();

	m_It->setInt32( value );
}

////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	const Int64&
\throw	WrongPropertyTypeException, WrongOperationException */
////////////////////////////////////////////////////////////////////////////////////////////
const Int64 & PropertyTable_XIterator::getInt64() const
{
    FUNCTION_NAME(basar.property.PropertyTable_XIterator.getInt64())

    checkInitialization();

    SupportedTypeEnum type = getTypeInternal().getType();

    if (INT64 != type )
    {
        VarString s;
        s.format("An INT-method was called for a not as INT-declared property (property: '%s')!",
            getName().c_str());
        throw WrongPropertyTypeException(ExceptInfo(fun, s, __FILE__, __LINE__));
    }

    SupportedStateEnum eState = m_It->getState().getState();

    if (SS_UNSET == eState)
    {
        VarString s;
        s.format("Operation was executed on an unset state (property: '%s')!", getName().c_str());
        throw WrongOperationException(ExceptInfo(fun, s.c_str(), __FILE__, __LINE__));
    }

    return m_It->getInt64();
}

////////////////////////////////////////////////////////////////////////////////////////////
/*! \param	value set to the object the iterator is pointing to.
\throw	WrongPropertyTypeException */
////////////////////////////////////////////////////////////////////////////////////////////
void PropertyTable_XIterator::setInt64(const Int64 & value)
{
    FUNCTION_NAME(basar.property.PropertyTable_XIterator.setInt64(Int64))

    checkInitialization();

    SupportedTypeEnum type = getTypeInternal().getType();

    if (INT64 != type )
    {
        VarString s;
        s.format("An INT-method was called for a not as INT-declared property (property: '%s')!",
            getName().c_str());
        throw WrongPropertyTypeException(ExceptInfo(fun, s, __FILE__, __LINE__));
    }

    if (SS_CLEAN == m_It->getState().getState())
    {
        if (value == m_It->getInt64())
            return;
    }

    determineState();

    m_It->setInt64(value);
}

////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	const Int32&
	\throw	WrongPropertyTypeException, WrongOperationException */ 
////////////////////////////////////////////////////////////////////////////////////////////
const Int32& PropertyTable_XIterator::getInt32() const
{
	FUNCTION_NAME( basar.property.PropertyTable_XIterator.getInt32() )

    checkInitialization();
    
    SupportedTypeEnum type = getTypeInternal().getType();

	if ( INT32   != type && 
		 INTDATE != type && 
		 INTTIME != type)
	{
		VarString s;
		s.format( "An INT-method was called for a not as INT-declared property (property: '%s')!", 
				    getName().c_str() ); 
		throw WrongPropertyTypeException( ExceptInfo( fun, s, __FILE__, __LINE__) );
    }   
	
	SupportedStateEnum eState = m_It->getState().getState();

	if ( SS_UNSET == eState )
	{
		VarString s;
		s.format( "Operation was executed on an unset state (property: '%s')!", getName().c_str() ); 
		throw WrongOperationException( ExceptInfo( fun, s.c_str(), __FILE__, __LINE__) );
	}

	return m_It->getInt32();
}

////////////////////////////////////////////////////////////////////////////////////////////
/*! \param	value set to the object the iterator is pointing to.
	\throw	WrongPropertyTypeException */ 
////////////////////////////////////////////////////////////////////////////////////////////
void PropertyTable_XIterator::setFloat32( const Float32& value )
{
	FUNCTION_NAME( basar.property.PropertyTable_XIterator::setFloat32(Float32) )

    checkInitialization();

	if( FLOAT32 != getTypeInternal().getType() )
	{
		VarString s;
		s.format( "A FLOAT-method was called for a not as FLOAT-declared property (property: '%s')!",
				    getName().c_str() ); 
		throw WrongPropertyTypeException( ExceptInfo( fun, s, __FILE__, __LINE__) );
    }

	if( SS_CLEAN == m_It->getState().getState() )
	{
		if (value == m_It->getFloat32()) 
			return;
	}
	
	determineState();

	m_It->setFloat32( value );
}
////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	const Float32&
	\throw	WrongPropertyTypeException, WrongOperationException */ 
////////////////////////////////////////////////////////////////////////////////////////////
const Float32& PropertyTable_XIterator::getFloat32() const
{
	FUNCTION_NAME( basar.property.PropertyTable_XIterator::getFloat32 )

    checkInitialization();

	if( FLOAT32 != getTypeInternal().getType() )
	{
		VarString s;
		s.format( "A FLOAT-method was called for a not as FLOAT-declared property (property: '%s')!", 
			      getName().c_str() ); 
		throw WrongPropertyTypeException( ExceptInfo( fun, s, __FILE__, __LINE__) );
    }	
	
	SupportedStateEnum eState = m_It->getState().getState();

	if( SS_UNSET == eState )
	{
		VarString s;
		s.format( "Operation was executed on an unset state (property: '%s')!", 
			      getName().c_str() ); 
		throw WrongOperationException( ExceptInfo( fun, s, __FILE__, __LINE__) );
	}
	
	return m_It->getFloat32();
}
////////////////////////////////////////////////////////////////////////////////////////////
/*! \param	value set to the object the iterator is pointing to.
	\throw	WrongPropertyTypeException */ 
////////////////////////////////////////////////////////////////////////////////////////////
void PropertyTable_XIterator::setFloat64( const Float64& value )
{
	FUNCTION_NAME( basar.property.PropertyTable_XIterator.setFloat64(Float64) )

    checkInitialization();

	if( FLOAT64 != getTypeInternal().getType() )
	{
		VarString s;
		s.format( "A FLOAT-method was called for a not as FLOAT-declared Property (property: '%s')!", 
			      getName().c_str() ); 
		throw WrongPropertyTypeException( ExceptInfo( fun, 	s, __FILE__, __LINE__) );
    }

	if( SS_CLEAN == m_It->getState().getState() )
	{
		if (value == m_It->getFloat64()) 
			return;
	}

    determineState();

	m_It->setFloat64( value );
}
////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	const Float64&
	\throw	WrongPropertyTypeException, WrongOperationException */ 
////////////////////////////////////////////////////////////////////////////////////////////
const Float64& PropertyTable_XIterator::getFloat64() const
{
	FUNCTION_NAME( basar.property.PropertyTable_XIterator.getFloat64 )

    checkInitialization();

	if( FLOAT64 != getTypeInternal().getType() )
	{
        VarString s;
		s.format( "A FLOAT-method was called for a not as FLOAT-declared Property (property: '%s')!", 
			     getName().c_str() ); 
		throw WrongPropertyTypeException( ExceptInfo( fun, s, __FILE__, __LINE__) );
    }	
	
	SupportedStateEnum eState = m_It->getState().getState();

	if( SS_UNSET == eState )
	{
		VarString s;
		s.format( "Operation was executed on an unset state (property: '%s')!", 
			      getName().c_str() ); 
		throw WrongOperationException( ExceptInfo( fun, s, __FILE__, __LINE__) );
	}
	
	return m_It->getFloat64();
}
////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	DateTime
	\throw	WrongPropertyTypeException, WrongOperationException */ 
////////////////////////////////////////////////////////////////////////////////////////////
const DateTime& PropertyTable_XIterator::getDateTime() const
{
	FUNCTION_NAME( basar.property.PropertyTable_XIterator.getDateTime )

    checkInitialization();

	if( DATETIME != getTypeInternal().getType() )
	{
		VarString s;
		s.format( "A DATETIME-method was called for a not as DATETIME-declared Property (property: '%s')!", 
			      getName().c_str() ); 
		throw WrongPropertyTypeException( ExceptInfo( fun, s, __FILE__, __LINE__) );
    }	
	
	SupportedStateEnum eState = m_It->getState().getState();

	if( SS_UNSET == eState )
	{
		VarString s;
		s.format( "Operation was executed on an unset state (property: '%s')!", 
			      getName().c_str() ); 
		throw WrongOperationException( ExceptInfo( fun, s, __FILE__, __LINE__) );
	}
	
	return m_It->getDateTime();
}
////////////////////////////////////////////////////////////////////////////////////////////
/*! \param	value set a DateTime object to the object the iterator is pointing to.
	\throw	WrongPropertyTypeException */ 
////////////////////////////////////////////////////////////////////////////////////////////
void PropertyTable_XIterator::setDateTime( const DateTime& value )
{
	FUNCTION_NAME( basar.property.PropertyTable_XIterator.setDateTime() )

    checkInitialization();

	if( DATETIME != getTypeInternal().getType() )
	{
        VarString s;
		s.format( "A DATETIME-method was called for a not as DATETIME-declared Property (property: '%s')!", 
			      getName().c_str() ); 
		throw WrongPropertyTypeException( ExceptInfo( fun, s, __FILE__, __LINE__) );
    }

	if( SS_CLEAN == m_It->getState().getState() )
	{
		if( value == m_It->getDateTime() )
			return;
	}

    determineState();

	m_It->setDateTime( value );
}
////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	TimeSpan
	\throw	WrongPropertyTypeException, WrongOperationException */ 
////////////////////////////////////////////////////////////////////////////////////////////
const TimeSpan& PropertyTable_XIterator::getTimeSpan() const
{
	FUNCTION_NAME( basar.property.PropertyTable_XIterator.getTimeSpan )

    checkInitialization();

	if( TIMESPAN != getTypeInternal().getType() )
	{
		VarString s;
		s.format( "A TIMESPAN-method was called for a not as TIMESPAN-declared Property (property: '%s')!", 
			      getName().c_str() ); 
		throw WrongPropertyTypeException( ExceptInfo( fun, s, __FILE__, __LINE__) );
    }	
	
	SupportedStateEnum eState = m_It->getState().getState();

	if( SS_UNSET == eState )
	{
		VarString s;
		s.format( "Operation was executed on an unset state (property: '%s')!", 
			      getName().c_str() ); 
		throw WrongOperationException( ExceptInfo( fun, s, __FILE__, __LINE__) );
	}
	
	return m_It->getTimeSpan();
}
////////////////////////////////////////////////////////////////////////////////////////////
/*! \param	value set a TimeSpan object to the object the iterator is pointing to.
	\throw	WrongPropertyTypeException */ 
////////////////////////////////////////////////////////////////////////////////////////////
void PropertyTable_XIterator::setTimeSpan( const TimeSpan& value )
{
	FUNCTION_NAME( basar.property.PropertyTable_XIterator.setTimeSpan() )

    checkInitialization();

	if( TIMESPAN != getTypeInternal().getType() )
	{
        VarString s;
		s.format( "A TIMESPAN-method was called for a not as TIMESPAN-declared Property (property: '%s')!", 
			      getName().c_str() ); 
		throw WrongPropertyTypeException( ExceptInfo( fun, s, __FILE__, __LINE__) );
    }

	if( SS_CLEAN == m_It->getState().getState() )
	{
		if( value == m_It->getTimeSpan() )
			return;
	}

    determineState();

	m_It->setTimeSpan( value );
}
////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	DateTime
	\throw	WrongPropertyTypeException, WrongOperationException */ 
////////////////////////////////////////////////////////////////////////////////////////////
const Date& PropertyTable_XIterator::getDate() const
{
	FUNCTION_NAME( basar.property.PropertyTable_XIterator.getDate )

    checkInitialization();

	if( DATE != getTypeInternal().getType() )
	{
		VarString s;
		s.format( "A DATE-method was called for a not as DATE-declared property (property: '%s')!", 
			     getName().c_str() );
		throw WrongPropertyTypeException( ExceptInfo( fun, s, __FILE__, __LINE__) );
    }	
	
	SupportedStateEnum eState = m_It->getState().getState();

	if( SS_UNSET == eState )
	{
		VarString s;
		s.format( "Operation was executed on an unset state (property: '%s')!", 
			     getName().c_str() );
		throw WrongOperationException( ExceptInfo( fun, s, __FILE__, __LINE__) );
	}
		
	return m_It->getDate();
}
////////////////////////////////////////////////////////////////////////////////////////////
/*! \param	value set a DateTime object with time 0 to the object the iterator is pointing to.
	\throw	WrongPropertyTypeException */ 
////////////////////////////////////////////////////////////////////////////////////////////
void PropertyTable_XIterator::setDate( const Date& value )
{
	FUNCTION_NAME( basar.property.PropertyTable_XIterator.setDate() )

    checkInitialization();

	if( DATE != getTypeInternal().getType() )
	{
		VarString s;
		s.format( "A DATE-method was called for a not as DATE-declared property (property: '%s')!", 
			     getName().c_str() );
		throw WrongPropertyTypeException( ExceptInfo( fun, s, __FILE__, __LINE__) );
    }

	if( SS_CLEAN == m_It->getState().getState() )
	{
		if( value == m_It->getDate() )
			return;
	}

    determineState();

	m_It->setDate( value );	
}
////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	DateTime
	\throw	WrongPropertyTypeException, WrongOperationException */ 
////////////////////////////////////////////////////////////////////////////////////////////
const Time& PropertyTable_XIterator::getTime() const
{
	FUNCTION_NAME( basar.property.PropertyTable_XIterator.getTime )

    checkInitialization();

	if( TIME != getTypeInternal().getType() )
	{
		VarString s;
		s.format( "A TIME-method was called for a not as TIME-declared property (property: '%s')!", 
			     getName().c_str() );
		throw WrongPropertyTypeException( ExceptInfo( fun, s, __FILE__, __LINE__) );
    }	
	
	SupportedStateEnum eState = m_It->getState().getState();

	if( SS_UNSET == eState )
	{
		VarString s;
		s.format( "Operation was executed on an unset state (property: '%s')!", 
			      getName().c_str() );
		throw WrongOperationException( ExceptInfo( fun, s, __FILE__, __LINE__) );
	}
	
	return m_It->getTime();
}
////////////////////////////////////////////////////////////////////////////////////////////
/*! \param	value set a Binary to the object the iterator is pointing to.
\throw	WrongPropertyTypeException */
////////////////////////////////////////////////////////////////////////////////////////////
void PropertyTable_XIterator::setBinary(const Binary & value)
{
    FUNCTION_NAME(basar.property.PropertyTable_XIterator.setBinary())

    checkInitialization();

    if (BINARY != getTypeInternal().getType())
    {
        VarString s;
        s.format("A BINARY-method was called for a not as BINARY-declared property (property: '%s')!",
            getName().c_str());
        throw WrongPropertyTypeException(ExceptInfo(fun, s, __FILE__, __LINE__));
    }

    if (SS_CLEAN == m_It->getState().getState())
    {
        if (value == m_It->getBinary())
            return;
    }

    determineState();

    m_It->setBinary(value);
}
////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	binary 
	\throw	WrongPropertyTypeException, WrongOperationException */ 
////////////////////////////////////////////////////////////////////////////////////////////
const Binary & PropertyTable_XIterator::getBinary() const
{
    FUNCTION_NAME(basar.property.PropertyTable_XIterator.getDecimal)

    checkInitialization();

    if (BINARY != getTypeInternal().getType()) {
        VarString s;
        s.format("A BINARY-method was called for a not as BINARY-declared Property (property: '%s')!",
            getName().c_str());
        throw WrongPropertyTypeException(ExceptInfo(fun, s, __FILE__, __LINE__));
    }

    SupportedStateEnum eState = m_It->getState().getState();

    if (SS_UNSET == eState)
    {
        VarString s;
        s.format("Operation was executed on an unset state (property: '%s')!",
            getName().c_str());
        throw WrongOperationException(ExceptInfo(fun, s, __FILE__, __LINE__));
    }

    return m_It->getBinary();
}
////////////////////////////////////////////////////////////////////////////////////////////
/*! \param	value set a DateTime object with time 0 to the object the iterator is pointing to.
	\throw	WrongPropertyTypeException */ 
////////////////////////////////////////////////////////////////////////////////////////////
void PropertyTable_XIterator::setTime( const Time& value )
{
	FUNCTION_NAME( basar.property.PropertyTable_XIterator.setTime() )

    checkInitialization();

	if( TIME != getTypeInternal().getType() )
	{
		VarString s;
		s.format( "A TIME-method was called for a not as TIME-declared property (property: '%s')!", 
			      getName().c_str() );
		throw WrongPropertyTypeException( ExceptInfo( fun, s, __FILE__, __LINE__) );
    }

	if( SS_CLEAN == m_It->getState().getState() )
	{
		if( value == m_It->getTime() )
			return;
	}
	
	determineState();
	
	m_It->setTime( value );
}
////////////////////////////////////////////////////////////////////////////////////////////
/*! \param	value set Decimal to the object the iterator is pointing to.
	\throw	WrongPropertyTypeException */ 
////////////////////////////////////////////////////////////////////////////////////////////
void PropertyTable_XIterator::setDecimal( const Decimal& value )
{
	FUNCTION_NAME( basar.property.PropertyTable_XIterator.setDecimal(Decimal) )

    checkInitialization();

	if( DECIMAL != getTypeInternal().getType() )
	{
		VarString s;
		s.format( "A DECIMAL-method was called for a not as DECIMAL-declared property (property: '%s')!", 
			      getName().c_str() );
        throw WrongPropertyTypeException( ExceptInfo( fun, s, __FILE__, __LINE__) );
    }

	if( SS_CLEAN == m_It->getState().getState() )
	{
		if( value == m_It->getDecimal() )
			return;
	}

	determineState();
	
	m_It->setDecimal( value );
}
////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	Decimal 
	\throw	WrongPropertyTypeException, WrongOperationException */ 
////////////////////////////////////////////////////////////////////////////////////////////
const Decimal& PropertyTable_XIterator::getDecimal() const
{
	FUNCTION_NAME( basar.property.PropertyTable_XIterator.getDecimal )

    checkInitialization();

	if( DECIMAL != getTypeInternal().getType() ){
		VarString s;
		s.format( "A DECIMAL-method was called for a not as DECIMAL-declared Property (property: '%s')!", 
			      getName().c_str() );
        throw WrongPropertyTypeException( ExceptInfo( fun, s, __FILE__, __LINE__) );
    }	
	
	SupportedStateEnum eState = m_It->getState().getState();

	if( SS_UNSET == eState )
	{
		VarString s;
		s.format( "Operation was executed on an unset state (property: '%s')!", 
			      getName().c_str() ); 
		throw WrongOperationException( ExceptInfo( fun, s, __FILE__, __LINE__) );
	}
	
	return m_It->getDecimal();
}
////////////////////////////////////////////////////////////////////////////////////////////
/*! \param	value set to the object the iterator is pointing to.
	\throw	WrongPropertyTypeException */ 
////////////////////////////////////////////////////////////////////////////////////////////
void PropertyTable_XIterator::setString( const I18nString& value )
{
	FUNCTION_NAME( basar.property.PropertyTable_XIterator.setString() )

    checkInitialization();

	if( STRING != getTypeInternal().getType() )
	{
		VarString s;
		s.format( "A STRING-method was called for a not as STRING-declared Property (property: '%s')!", 
			     getName().c_str() ); 
		throw WrongPropertyTypeException( ExceptInfo( fun, s, __FILE__, __LINE__) );
    }

	if( SS_CLEAN == m_It->getState().getState() )
	{
		if (value == m_It->getString()) 
			return;
	}

    determineState();

	m_It->setString( value );
}

////////////////////////////////////////////////////////////////////////////////////////////
/*! \param	value set to the object the iterator is pointing to.
	\throw	WrongPropertyTypeException */ 
////////////////////////////////////////////////////////////////////////////////////////////
void PropertyTable_XIterator::setString( ConstBuffer value )
{
	FUNCTION_NAME( basar.property.PropertyTable_XIterator.setString() )

    checkInitialization();

	if( STRING != getTypeInternal().getType() )
	{
		VarString s;
		s.format( "A STRING-method was called for a not as STRING-declared Property (property: '%s')!", 
			     getName().c_str() ); 
		throw WrongPropertyTypeException( ExceptInfo( fun, s, __FILE__, __LINE__) );
    }

	if( SS_CLEAN == m_It->getState().getState() )
	{
		if (value == m_It->getString()) 
			return;
	}

    determineState();
	
	m_It->setString( value );
}

////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	I18nString
	\throw	WrongPropertyTypeException, WrongOperationException */ 
////////////////////////////////////////////////////////////////////////////////////////////
const I18nString& PropertyTable_XIterator::getString() const
{
	FUNCTION_NAME( basar.property.PropertyTable_XIterator.getString )

    checkInitialization();

	if( STRING != getTypeInternal().getType() )
	{
		VarString s;
		s.format( "A STRING-method was called for a not as STRING-declared Property (property: '%s')!", 
			      getName().c_str() ); 
        throw WrongPropertyTypeException( ExceptInfo( fun, s, __FILE__, __LINE__) );
    }	
	
	SupportedStateEnum eState = m_It->getState().getState();

	if( SS_UNSET == eState )
	{
		VarString s;
		s.format( "Operation was executed on an unset state (property: '%s')!", 
			      getName().c_str() ); 
		throw WrongOperationException( ExceptInfo( fun, s, __FILE__, __LINE__) );
	}
	
	return m_It->getString();
}
////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	I18nString with simple inverted commas that can be used in SQL-Strings
	\throw	FatalErrorException */ 
////////////////////////////////////////////////////////////////////////////////////////////
const SQLString PropertyTable_XIterator::getSQLString() const
{
	/*! \todo	This logic is duplicated. This logic is also used in the class/method 
				'const SQLString NoCaching::getSQLString() const'.
				But it should be used in one explicit method. */

	FUNCTION_NAME( basar.property.PropertyTable_XIterator.getSQLString )

    checkInitialization();

	VarString s;

	switch (getTypeInternal().getType())
	{
	case INT16:
		s.itos(m_It->getInt16());
		break;

	case INT32:
	case INTTIME:
	case INTDATE:
		s.itos(m_It->getInt32());
		break;

    case INT64:
        s.lltos(m_It->getInt64());
        break;

	case FLOAT32:
        //better usage would be std::numeric<float>::max_digits10() - but this requires C++11
		s.ftos(m_It->getFloat32(), 9);
		break;

	case FLOAT64:
        //better usage would be std::numeric<double>::max_digits10() - but this requires C++11
		s.ftos(m_It->getFloat64(), 17);
		break;

	case DECIMAL:
		s = m_It->getDecimal().toNonLocString();
		break;

	case STRING:
		// changes V1.0.5.0 the string value is no longer 
		// enclosed in SQL_VALUE_DELIM, as that is not
		// very practical for like statements

		// --- duplicate "'" if it occurs in string-value
		s = m_It->getString();
	    
		if (s.length() > 0)
		{
			VarString::size_type pos = s.find(SQL_VALUE_DELIM);

			while ( pos != VarString::npos )
			{
				s.insert(++pos, SQL_VALUE_DELIM);
				pos = s.find(SQL_VALUE_DELIM, ++pos);
			}
		}

		break;

	case DATETIME:
        s = m_It->getDateTime().toString(basar::cmnutil::FORMAT_SQL_DATETIME_INFORMIX);
		break;
    
    case TIMESPAN:
        s = m_It->getTimeSpan().toNormalizedString();
        break;

	case DATE:
		s = m_It->getDate().toString(basar::cmnutil::FORMAT_SQL_DATE_INFORMIX);
		break;

	case TIME:
		s = m_It->getTime().toString(basar::cmnutil::FORMAT_SQL_TIME_INFORMIX);
		break;

	default:
		{
			// this branch should never fire
			VarString msg;
			msg.format(	"An undefined type is set: property '%s'", getName().c_str() ); 
			throw FatalErrorException( ExceptInfo( fun, msg, __FILE__, __LINE__) );	
		}
	}

	return s;
}
////////////////////////////////////////////////////////////////////////////////////////////
/*! \throw	no-throw*/ 
////////////////////////////////////////////////////////////////////////////////////////////
void PropertyTable_XIterator::determineState()
{	
	SupportedStateEnum eState = m_It->getState().getState();

	switch( m_pInsertTypeCollColumn->first )
	{
		case FOR_UNKNOWN: 	
			if( SS_UNSET == eState )
				m_It->setState( SS_UNKNOWN );
			break;

		case FOR_CLEAN:		
			if( SS_UNSET == eState )
			{
				m_It->setState( SS_CLEAN );
			} 
			else if( SS_CLEAN == eState )
			{
				m_It->setState( SS_UPDATE );
			}
			break;

		case FOR_INSERT:	
			if( SS_UNSET == eState )
			{
				m_It->setState( SS_INSERT );
			} 
			else if( SS_CLEAN == eState )
			{
				//! \todo MP: what about the chanching of the primary key 
				m_It->setState( SS_UPDATE ); 
			}
			break;

		case FOR_UPDATE:	
			if( SS_UNSET == eState )
			{
				m_It->setState( SS_UPDATE );
			}
			else if( SS_CLEAN == eState )
			{
				m_It->setState( SS_UPDATE );
			}
			break;

		case FOR_DELETE:	
			if( SS_UNSET == eState )
			{
				m_It->setState( SS_DELETE );
			}
			break;

		default:
			break;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	states the iterator is traversing 
	\throw	no-throw*/ 
////////////////////////////////////////////////////////////////////////////////////////////
const PropertyStateSet& PropertyTable_XIterator::getFilter() const
{
    return m_Pss;
}
////////////////////////////////////////////////////////////////////////////////////////////
/*! \param	pss is/are the states set to the corresponding member.
	\throw	no-throw*/ 
////////////////////////////////////////////////////////////////////////////////////////////
void PropertyTable_XIterator::setFilter( const PropertyStateSet& pss )
{
    m_Pss = pss;
}
//----------------------------------------------------------------------------------------------------------//
// uninitialized iterator - safe access methods to it
//----------------------------------------------------------------------------------------------------------//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \throw	InvalidIteratorException*/ 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PropertyTable_XIterator::checkInitialization() const
{
	FUNCTION_NAME( basar.property.PropertyTable_XIterator.checkInitialization )

	if( isNull() )
	{
		throw InvalidIteratorException( ExceptInfo( fun, 
			                                        "Iterator is not initialized/valid: "
												    "PropertyTable m_Pt is NULL!", 
													__FILE__, __LINE__) );
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	PropertyTableSharedPtr& is a reference to the shared pointer of the container
			for non-const objects
	\throw	InvalidIteratorException*/ 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
PropertyTableSharedPtr& PropertyTable_XIterator::getPt()
{
	checkInitialization();
	return m_Pt;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	const PropertyTableSharedPtr& is a reference to the shared pointer of the container
			for const objects
	\throw	InvalidIteratorException*/ 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
const PropertyTableSharedPtr& PropertyTable_XIterator::getPt() const
{
	checkInitialization();
	return m_Pt;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	CollPropertyInternalColumnIter& is a reference to the pure iterator of the container
			for non-const objects
	\throw	InvalidIteratorException*/ 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
CollPropertyInternalColumnIter& PropertyTable_XIterator::getIter()
{
	checkInitialization();
	return m_It;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	const CollPropertyInternalColumnIter& is a reference to the pure iterator of the container
			for const objects
	\throw	InvalidIteratorException*/ 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
const CollPropertyInternalColumnIter& PropertyTable_XIterator::getIter() const
{
	checkInitialization();
	return m_It;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	PairInsertTypeCollPropertyInternalColumn* is a pointer to the pair containing the value 
			for non-const objects
	\throw	InvalidIteratorException*/ 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
PairInsertTypeCollPropertyInternalColumn* PropertyTable_XIterator::getInsertTypeColl()
{
	checkInitialization();
	return m_pInsertTypeCollColumn;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	const PairInsertTypeCollPropertyInternalColumn* is a pointer to the pair containing the value 
			for const objects
	\throw	InvalidIteratorException*/ 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
const PairInsertTypeCollPropertyInternalColumn* PropertyTable_XIterator::getInsertTypeColl() const
{
	checkInitialization();
	return m_pInsertTypeCollColumn;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*! checks the value
    \return true if XIterator's inner container is null, false otherwise
*/
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool PropertyTable_XIterator::isNull() const
{
	return ( m_Pt.get() == 0 );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!   checks if iterator points to begin() of the inner container
    \return true, if iterator points to begin() of inner container, false otherwise 
*/
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool PropertyTable_XIterator::isBegin() const
{
	checkInitialization();
	return (m_pInsertTypeCollColumn->second.begin() == m_It);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!   checks if iterator points to end() of the inner container 
    \return true, if iterator points to end() of inner container, false otherwise
*/
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool PropertyTable_XIterator::isEnd() const
{
	checkInitialization();
	return (m_pInsertTypeCollColumn->second.end() == m_It);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*! resets the value */
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PropertyTable_XIterator::reset()
{
	m_Pt.reset();
	m_pInsertTypeCollColumn = 0;
}

} // End namespace property
} // End namespace basar
