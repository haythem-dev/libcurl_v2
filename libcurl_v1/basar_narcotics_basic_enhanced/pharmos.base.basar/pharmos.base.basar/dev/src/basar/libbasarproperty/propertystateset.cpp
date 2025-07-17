//////////////////////////////////////////////////////////////////////////////
/*! \file
 *  \brief  class PropertyStateSet describes the states 
			(SS_UNKNOWN, SS_CLEAN, SS_UNSET, SS_INSERT, SS_UPDATE, SS_DELETE) of a resultset row. 
 *  \author Bischof Björn 
 *  \date   25.07.2005
 */
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// includes
//////////////////////////////////////////////////////////////////////////////

#include "libbasarproperty_propertystateset.h"

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
// implemetation
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
/*!	\param supportedState represents the state the object is initialized with
	\throw IllegalPropertyStateException */
//////////////////////////////////////////////////////////////////////////////
PropertyStateSet::PropertyStateSet( const Int32 supportedState /*=basar::property::UNKNOWN*/)
{
	FUNCTION_NAME( basar.property.PropertyStateSet.PropertyStateSet(const Int32) )

	if( false == isValid(supportedState) ){
		VarString msg;
		msg.format(	"An unknown/invalid state (Hex: <%x>) was passed to initialize the object!", supportedState );
		throw IllegalPropertyStateException( ExceptInfo(fun, msg.c_str(), __FILE__, __LINE__) );
	}
	m_eState = static_cast<SupportedStateEnum>( supportedState );
}
//////////////////////////////////////////////////////////////////////////////
/*!	\retval		true if given state equals this
	\return		otherwise returns false
	\param pss	comparison of two objects of the same type 
				(PropertyStateSet). Searches internally for the same 
				value of the member (SupportedStateEnum) 
	\throw		no-throw */
//////////////////////////////////////////////////////////////////////////////
bool PropertyStateSet::operator==( const PropertyStateSet& pss ) const
{
    return pss.m_eState == m_eState;
}
//////////////////////////////////////////////////////////////////////////////
/*!	\retval		true if given state is unequal to this
	\return		otherwise returns false
	\param pss	comparison of two objects of the same type 
				(PropertyStateSet). Searches internally for the different 
				value of the member (SupportedStateEnum) 
	\throw		no-throw */
//////////////////////////////////////////////////////////////////////////////
bool PropertyStateSet::operator!=( const PropertyStateSet& pss ) const
{
    return !operator==( pss ) ;
}
//////////////////////////////////////////////////////////////////////////////
/*!	\return		SupportedStateName (string based state)
	\throw		no-throw */
//////////////////////////////////////////////////////////////////////////////
const SupportedStateName PropertyStateSet::toString() const
{
	basar::VarString s( "" );
	const UInt16 BITSHIFTED = 0x01;
	UInt16 numberElements = sizeof(s_SupportedStateNameColl)/sizeof(SupportedStateName);
	for( UInt16 i = 0; i < numberElements; ++i ){
		if( (m_eState & (BITSHIFTED << i) ) == (BITSHIFTED << i) ){
			s = s.append( s_SupportedStateNameColl[i] ); 
			s.append( ", ");
		}
	}
	s[ s.length()-2 ] = 0; // cut the last two signs (space and the comma)
	return s;
}
//////////////////////////////////////////////////////////////////////////////
/*!	\retval true if given state is valid and set successfully 
	\return otherwise returns false
	\param	eSupportedState holds the state intended to add 
	\throw		no-throw */
//////////////////////////////////////////////////////////////////////////////
bool PropertyStateSet::insert( const SupportedStateEnum eSupportedState )
{
	if( false == isValid(eSupportedState) ){
		return false;
	}
	// static_cast<> is required to assign an integer to an enum value:
	// (m_eState | eSupportedState): the "or"-ed is calculated on the base of integers,
	// so the result is also an integer and is assigned to an enum-value variable
	m_eState = static_cast<SupportedStateEnum>(m_eState | eSupportedState);
	return true;	
}
//////////////////////////////////////////////////////////////////////////////
/*!	\retval true if passed supported state is valid
	\return otherwise returns false
	\param	supportedState is the state checked for validity.
			This method is the help method called from c'tor and the 
			insert-method 
	\throw		no-throw*/
//////////////////////////////////////////////////////////////////////////////
bool PropertyStateSet::isValid( const Int32 supportedState ) const
{
	bool isValid = false;
	// the number 128 (decimal system) is represented by the 8th bit 
	// in the dual system. 
	// The sign '~' inverts the bits set. ~128 --> 0111 1111.
	// 0111 1111 represents the largest valid number of the possible 
	// Propertystates
	const UInt32 BITMASK = static_cast<UInt32>(~192);
	if( BITMASK == (static_cast<UInt32>(supportedState) | BITMASK) ){
		isValid = true;
	}
	return isValid;
}
//////////////////////////////////////////////////////////////////////////////
/*!	\retval true if state set contains the passed state 
	\param	eSupportedState is the state that is searched in the member of this object 
	\throw		no-throw*/
//////////////////////////////////////////////////////////////////////////////
bool PropertyStateSet::contains( const SupportedStateEnum eSupportedState ) const
{
	return checkContains( eSupportedState );
}
//////////////////////////////////////////////////////////////////////////////
/*!	\retval true if state set contains the passed state 
	\param	ps aggregate a state that is searched in the member of this object 
	\throw		no-throw*/
//////////////////////////////////////////////////////////////////////////////
bool PropertyStateSet::contains( const PropertyState& ps ) const
{
	return checkContains( ps.getState() );
}
//////////////////////////////////////////////////////////////////////////////
/*!	\retval true if state set contains the passed state 
	\return otherwise returns false
	\param	eSupportedState is the state checked if it is held 
			in the member of this object. 
			This method is the help method called from the contains-method 
	\throw		no-throw*/
//////////////////////////////////////////////////////////////////////////////
bool PropertyStateSet::checkContains( const SupportedStateEnum eSupportedState ) const
{
	if( (eSupportedState == (m_eState & eSupportedState))   || 
        (m_eState == SS_UNKNOWN)							){
		return true;
	}

	return false;
}
//////////////////////////////////////////////////////////////////////////////
/*!	todo	this contains differs from the other contains-methods in this class.
			It binary ANDs the inner state (member) with the passed parameter
			equal to the other contains-methods but does not compare the result
			with the set state, but only checks if one of the passed states
			is set. Or the inner state is UNKNOWN. */
/*!	\retval true if state set contains the passed state
	\return otherwise returns false
	\param	pss aggregated states that are searched in the member 
			of this object 
	\throw		no-throw*/
//////////////////////////////////////////////////////////////////////////////
bool PropertyStateSet::contains( const PropertyStateSet& pss ) const
{
	return (((m_eState & pss.getStates()) != 0) || // BB 09.09.2005
			(m_eState == SS_UNKNOWN));
}
//////////////////////////////////////////////////////////////////////////////
/*!	\return	SupportedStateEnum: possible contained states 
	\throw n no-throw*/
//////////////////////////////////////////////////////////////////////////////
SupportedStateEnum PropertyStateSet::getStates() const
{
	return m_eState;
}

} // End namespace property
} // End namespace basar
