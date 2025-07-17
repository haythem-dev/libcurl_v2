//------------------------------------------------------------------------------
/*! \file
 *  \brief  public library interface: export/import API
 *  \author Michael Eichenlaub
 *  \date   04.08.2010
 */
//------------------------------------------------------------------------------

#ifndef GUARD_LIBBASARDBSQL_API_H
#define GUARD_LIBBASARDBSQL_API_H

//------------------------------------------------------------------------------------------------------------//
#ifndef LIBBASARDBSQL_API
	#ifdef _WIN32
		#ifdef LIBBASARDBSQL_EXPORTS
			#define LIBBASARDBSQL_API __declspec(dllexport)	//!< dll exported
		#else
			#define LIBBASARDBSQL_API __declspec(dllimport)	//!< dll imported
		#endif 
	#else 
		#define LIBBASARDBSQL_API
	#endif
#endif

//------------------------------------------------------------------------------
#endif	// GUARD
