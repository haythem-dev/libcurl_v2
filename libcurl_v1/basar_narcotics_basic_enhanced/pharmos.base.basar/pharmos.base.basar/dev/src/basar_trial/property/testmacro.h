#ifndef __TESTMACRO_H__
#define __TESTMACRO_H__ __TESTMACRO_H__

#include <iostream>
#include <iomanip>
#include <string>
using std::cout;
using std::endl;
using std::string;
using std::left;
using std::setw;
using std::boolalpha;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PROJECT testproperty
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------//
// macros
//----------------------------------------------------------------------------//
#define FUNCTION_NAME( funname ) static const char* const fun = #funname;
#define __FUN__(funname) const char* const fun = /* (char*) */#funname;	

// NDEBUG: for debugging
#ifndef NDEBUG
	#define COUT( msg ) cout << left << setw(40) << #msg << ": " << endl << "\t >" << msg << "<" << endl;

	#define COUT_BOOL( msg ) cout	<< boolalpha << left << setw(40) \
									<< #msg << ": >" << msg << "<" << endl;

	#define DEBUG_ENTER( strm, fun ) strm	<< endl \
											<< setw(3) << setfill('=') << "=" << " ENTER " << fun \
											<< " (" << ":" <<  __LINE__ << ") " \
											<< setw(3) << "=" << endl << endl;

	#define DEBUG_LEAVE( strm, fun ) strm	<< endl \
											<< setw(3) << setfill('=') << "=" << " LEAVE " << fun \
											<< " (" << ":" <<  __LINE__ << ") " \
											<< setw(3) << "=" << endl << endl;

	#define DEBUG_RETURN(fun, fmt, val) printf( "\tRETURN %s (RC:>"fmt"<)\n", fun, val );	
#else 
	#define COUT( msg ) 
	#define COUT_BOOL( msg )    
	#define DEBUG_ENTER( strm, fun )
	#define DEBUG_LEAVE( strm, fun )
	#define DEBUG_RETURN(fun, fmt, val) 
#endif

#endif	//__TESTMACRO_H__
