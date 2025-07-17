//////////////////////////////////////////////////////////////////////////////
/*! \file	
 *  \brief  some help macros - except the 
			FUNCTION_NAME- and 
			DECLARE_NAMESPACE_CLASSNAME / DEFINE_NAMESPACE_CLASSNAME-
			macro every macros can be used in the debug case only.
 *  \author Bischof Björn 
 *  \date   25.07.2005
 */
//////////////////////////////////////////////////////////////////////////////

#ifndef GUARD_MACRO_H
#define GUARD_MACRO_H

//----------------------------------------------------------------------------//
// macros
//----------------------------------------------------------------------------//
#define FUNCTION_NAME( funname ) static const char* const fun = #funname;

//----------------------------------------------------------------------------//
#endif //GUARD_MACRO_H
