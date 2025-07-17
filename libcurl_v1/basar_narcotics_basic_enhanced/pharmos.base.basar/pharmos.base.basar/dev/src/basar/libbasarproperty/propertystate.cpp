//////////////////////////////////////////////////////////////////////////////
/*! \file
 *  \brief	class representing the possible states 
			(determines the database action, e.g update- or insert-action)
 *  \author Bischof Björn 
 *  \date   25.07.2005
 */
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// includes
//////////////////////////////////////////////////////////////////////////////

#include "libbasarproperty_propertystate.h"
#include "../macro.h"

//////////////////////////////////////////////////////////////////////////////
// namespaces
//////////////////////////////////////////////////////////////////////////////
namespace basar
{
namespace property
{
//////////////////////////////////////////////////////////////////////////////
// implementation
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
/*!	\param	eSupportedState indicates the state and is assigned to the member 
			of the same type after checking up on valid values. 
			An invalid value will cause an exception.
	\throw	IllegalSupportedStateException */
//////////////////////////////////////////////////////////////////////////////
PropertyState::PropertyState( const SupportedStateEnum eSupportedState /*=basar::property::UNSET*/)
{
	FUNCTION_NAME( basar.property.PropertyState.PropertyState(SupportedStateEnum) )

	switch( eSupportedState ){
        case SS_UNKNOWN:	// fall through
        case SS_CLEAN:     // fall through               
        case SS_UNSET:		// fall through
        case SS_INSERT:    // fall through
		case SS_UPDATE:    // fall through
        case SS_DELETE:	m_eState = eSupportedState;
                        break;
        default:        VarString msg;
						msg.format( "An unknown/invalid state <%d> was passed to initialize the object!", 
								    eSupportedState );
						throw IllegalPropertyStateException( 
								ExceptInfo( fun, msg.c_str(), __FILE__, __LINE__) );
                        break;
	}
}

//! assignment operator
PropertyState& PropertyState::operator= (const PropertyState& rhs)
{
    if (*this == rhs)
    {
        return *this;
    }
    
    m_eState = rhs.m_eState;
    
    return *this;
}

//! assignment overload for SupportedStateEnum
PropertyState& PropertyState::operator= (SupportedStateEnum eSupportedState)
{
    m_eState = eSupportedState;
    
    return *this;
}

//! copy c'tor
PropertyState::PropertyState(const PropertyState& source)
{
    m_eState = source.m_eState;
}

//////////////////////////////////////////////////////////////////////////////
/*!	\throw		no-throw
	\retval		true if states are equal 
	\return		otherwise returns false
	\param	ps	state to compare with; 
				comparison of two objects of the same type PropertyState;
				Checks internally if the value is the same as the member of the 
				type SupportedStateEnum*/
//////////////////////////////////////////////////////////////////////////////
bool PropertyState::operator==( const PropertyState& ps ) const
{
	return m_eState == ps.m_eState;
}
//////////////////////////////////////////////////////////////////////////////
/*!	\throw		no-throw
	\retval		true if states are unequal 
	\return		otherwise returns false
	\param	ps	state to compare with; 
				comparison of two objects of the same type PropertyState;
				Checks internally if the value is different to the member of the 
				type SupportedStateEnum*/
//////////////////////////////////////////////////////////////////////////////
bool PropertyState::operator!=( const PropertyState& ps ) const
{
    return !operator==( ps ) ;
}

bool PropertyState::operator==( SupportedStateEnum eSupportedState ) const
{
    return m_eState == eSupportedState;
}

bool PropertyState::operator!=( SupportedStateEnum eSupportedState ) const
{
    return !operator==( eSupportedState );
}

//////////////////////////////////////////////////////////////////////////////
/*! \return name of state as string 
	\throw	no-throw */
//////////////////////////////////////////////////////////////////////////////
const SupportedStateName PropertyState::toString() const
{
	const UInt16 BITSHIFTED = 0x01;
	UInt16 i = 0;
	for( ; ((BITSHIFTED << i) <= m_eState); ++i ){
		if( (m_eState & (BITSHIFTED << i) ) == m_eState ){
			break;
		}
	}
	return s_SupportedStateNameColl[i];
}
//////////////////////////////////////////////////////////////////////////////
/*!	\return SupportedStateEnum 
	\throw	no-throw */
//////////////////////////////////////////////////////////////////////////////
SupportedStateEnum PropertyState::getState() const
{
	return m_eState;
}

} // End namespace property
} // End namespace basar
