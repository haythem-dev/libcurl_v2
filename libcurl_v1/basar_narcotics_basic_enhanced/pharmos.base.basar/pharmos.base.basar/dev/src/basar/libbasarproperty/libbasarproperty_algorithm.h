#ifndef GUARD_LIBBASARPROPERTY_ALGORITHM_H
#define GUARD_LIBBASARPROPERTY_ALGORITHM_H
//--------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  basar::property global functions to check if some properties are set 
			or in a certain state when comparing a propertytable and a paremterlist
 *  \author Bischof Björn 
 *  \date   25.07.2005
 */
//--------------------------------------------------------------------------------------------//

//--------------------------------------------------------------------------------------------//
// dll imports and exports
//--------------------------------------------------------------------------------------------//
#ifndef LIBBASARPROPERTY_API
	#ifdef _WIN32
		#ifdef LIBBASARPROPERTY_EXPORTS
			#define LIBBASARPROPERTY_API __declspec(dllexport)	//!< dll exported
		#else
			#define LIBBASARPROPERTY_API __declspec(dllimport)	//!< dll imported
		#endif 
	#else 
		#define LIBBASARPROPERTY_API
	#endif 
#endif 

//--------------------------------------------------------------------------------------------//
#include "libbasar_definitions.h"

//--------------------------------------------------------------------------------------------//
// namespace section
//--------------------------------------------------------------------------------------------//
namespace basar    
{
	namespace cmnutil
	{
		class ParameterList;
	}

	namespace property 
	{

//--------------------------------------------------------------------------------------------//
// forward declaration
//--------------------------------------------------------------------------------------------//
class PropertyTable_YIterator;
class PropertyStateSet;

//--------------------------------------------------------------------------------------------//
// namespace global functions
//--------------------------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////////////////
/*! \brief	checks if all given properties in pl are found in the yiterator and 
			no attribute of the yiterator is in the state SS_UNSET. 
			\n no-throw */
LIBBASARPROPERTY_API bool arePropertiesSet( const PropertyTable_YIterator & yit, 
										    const cmnutil::ParameterList  & pl );
/*! \brief	checks if all given properties in pl are found in the yiterator and 
			no attribute of the yiterator is in the state SS_UNSET. 
			\n no-throw */
LIBBASARPROPERTY_API bool arePropertiesSet( const PropertyTable_YIterator & yit, 
										    const VarString               & pl );
////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////
/*! \brief	checks if all given properties in pl are found in the yiterator and 
			no attribute of the yiterator is in the state SS_UNSET and 
			all attributes of the yiterator have a state which is contained 
			in the propertystateset pss. 
			\n no-throw */
LIBBASARPROPERTY_API bool arePropertiesSetAndAllInState( const PropertyTable_YIterator & yit, 
														 const cmnutil::ParameterList  & pl, 
														 const PropertyStateSet        & pss );
/*! \brief	checks if all given properties in pl are found in the yiterator and 
			no attribute of the yiterator is in the state SS_UNSET and 
			all attributes of the yiterator have a state which is contained 
			in the propertystateset pss. 
			\n no-throw */
LIBBASARPROPERTY_API bool arePropertiesSetAndAllInState( const PropertyTable_YIterator & yit, 
														 const VarString               & pl, 
														 const PropertyStateSet        & pss );
////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////
/*! \brief	returns true, if all given properties in pl 
			are found and not in State Unset set in Row yit 
			AND at least one of the given properties is in one of the given states in pss
			\n no-throw */
LIBBASARPROPERTY_API bool arePropertiesSetAndOneMustBeInState( const PropertyTable_YIterator & yit, 
															   const cmnutil::ParameterList  & pl, 
															   const PropertyStateSet        & pss);
/*! \brief	returns true, if all given properties in pl 
			are found and not in State Unset set in Row yit 
			AND at least one of the given properties is in one of the given states in pss
			\n no-throw */
LIBBASARPROPERTY_API bool arePropertiesSetAndOneMustBeInState( const PropertyTable_YIterator & yit, 
															   const VarString               & pl, 
															   const PropertyStateSet        & pss);
////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////
/*! \brief	returns true, if all given properties in plShouldBeSet 
			are found and not in State Unset set in Row yit 
			AND at least one of the given properties in plOneMustBeInState
            is in one of the given states in pss
\n no-throw */
////////////////////////////////////////////////////////////////////////////////////////////////
LIBBASARPROPERTY_API bool are1stPropertiesSetAnd2ndOneInState( const PropertyTable_YIterator & yit, 
															   const VarString               & plShouldBeSet, 
															   const VarString               & plOneMustBeInState, 
															   const PropertyStateSet        & pss );
////////////////////////////////////////////////////////////////////////////////////////////////
/*! \brief	returns true, if all given properties in plShouldBeSet 
			are found and not in State Unset set in Row yit 
			AND at least one of the given properties in plOneMustBeInState
            is in one of the given states in pss
\n no-throw */
////////////////////////////////////////////////////////////////////////////////////////////////
LIBBASARPROPERTY_API bool are1stPropertiesSetAnd2ndOneInState(  const PropertyTable_YIterator & yit, 
																const cmnutil::ParameterList  & plShouldBeSet, 
																const cmnutil::ParameterList  & plOneMustBeInState, 
																const PropertyStateSet        & pss );

////////////////////////////////////////////////////////////////////////////////////////////////
/*! \brief	returns true, if  at least one of the properties in list
            is in one of the given states in pss
\n no-throw */
////////////////////////////////////////////////////////////////////////////////////////////////
LIBBASARPROPERTY_API bool isOnePropertyInState( const PropertyTable_YIterator & yit, 
												const cmnutil::ParameterList & list, 
												const PropertyStateSet       & pss);

////////////////////////////////////////////////////////////////////////////////////////////////
/*! \brief	returns true, if all the  properties that are found in yit are in
            one of the given states in pss 
            If none of the properties are found the value of acceptNoProperty is returned
\n no-throw */
////////////////////////////////////////////////////////////////////////////////////////////////
LIBBASARPROPERTY_API bool areKnownPropertiesInState( const PropertyTable_YIterator & yit, 
													 const cmnutil::ParameterList & list, 
													 const PropertyStateSet       & pss,
													 bool                           acceptNoProperty);

////////////////////////////////////////////////////////////////////////////////////////////////
/*! \brief	returns true, if all the  properties that are found in yit 
            and are not in State Unset are in one of the given states in pss   
            If none of the properties are found and/or set 
            the value of  acceptNoProperty is returned
\n no-throw */
////////////////////////////////////////////////////////////////////////////////////////////////
LIBBASARPROPERTY_API bool areKnownAndSetPropertiesInState( const PropertyTable_YIterator & yit, 
														   const cmnutil::ParameterList  & list, 
														   const PropertyStateSet        & pss,
														   bool                            acceptNoProperty);



} // End namespace property
} // End namespace basar

#endif // GUARD_LIBBASARPROPERTY_ALGORITHM_H
