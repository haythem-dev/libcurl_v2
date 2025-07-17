//------------------------------------------------------------------------------
/*! \file
 *  \brief  common header files include
 *  \author Thomas Hörath
 *  \date   10.02.2006
 */
//------------------------------------------------------------------------------

#ifndef GUARD_BUSINESSOBJECTLOCK_COMMONHEADER_H
#define GUARD_BUSINESSOBJECTLOCK_COMMONHEADER_H

//------------------------------------------------------------------------------
// displays stage of precompiling, so not fulfilled precompile conditions can be detected
#ifdef WIN32
	#ifndef PRECOMPILEDHDR__MESSAGE__COMMONHEADER_H
		#pragma message("    precompiling " __FILE__ " ...")
		#define PRECOMPILEDHDR__MESSAGE__COMMONHEADER_H
	#endif	
#endif

//-------------------------------------------------------------------------------------------------//
// includes
//-------------------------------------------------------------------------------------------------//
#include <libbasarcmnutil.h>
#include <libbasardbsql.h>
#include <libbasardbaspect.h>

//-------------------------------------------------------------------------------------------------//

#endif //GUARD_BUSINESSOBJECTLOCK_COMMONHEADER_H

//-------------------------------------------------------------------------------------------------//
