//------------------------------------------------------------------------------
/*! \file
 *  \brief  common header files include
 *  \author Michael Eichenlaub
 *  \date   22.02.2006
 */
//------------------------------------------------------------------------------
#ifndef GUARD_COMMONHEADER_H
#define GUARD_COMMONHEADER_H

//------------------------------------------------------------------------------
// displays stage of precompiling, so not fulfilled precompile conditions can be detected
#ifdef WIN32
	#ifndef PRECOMPILEDHDR__MESSAGE__COMMONHEADER_H
		#pragma message("    precompiling " __FILE__ " ...")
		#define PRECOMPILEDHDR__MESSAGE__COMMONHEADER_H
	#endif
#endif

//------------------------------------------------------------------------------
//includes
//------------------------------------------------------------------------------
#include <libbasarcmnutil.h>
#include <libbasardbsql.h>
#include <libbasardbaspect.h>
#include <libbasarproperty.h>
#include <libbasarlogin.h>
#include <libbasarappl.h>

#ifdef WIN32
	#pragma warning (push)
	#pragma warning(disable: 4127 4244 4311 4312 4481 4512 4800)
#endif
	#include <QtCore/QString>
	#include <QtCore/QStringList>
	#include <QtCore/QDir>
	#include <QtCore/QTextStream>
#ifdef WIN32
	#pragma warning (pop)
#endif



#include <signal.h>

#ifdef WIN32
#include <windows.h>
#endif


#endif	//GUARD_COMMONHEADER_H