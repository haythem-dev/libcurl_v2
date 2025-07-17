//------------------------------------------------------------------------------
/*! \file
 *  \brief  common header files include
 *  \author Thomas Hörath
 *  \date   10.02.2006
 */
//------------------------------------------------------------------------------

#ifndef GUARD_NARCOTICS_COMMONHEADER_H
#define GUARD_NARCOTICS_COMMONHEADER_H

//------------------------------------------------------------------------------
// displays stage of precompiling, so not fulfilled precompile conditions can be detected
#ifdef WIN32
	#ifndef PRECOMPILEDHDR__MESSAGE__COMMONHEADER_H
		#pragma message("    precompiling " __FILE__ " ...")
		#define PRECOMPILEDHDR__MESSAGE__COMMONHEADER_H
	#endif	
#endif

//------------------------------------------------------------------------------
#include <libbasar_definitions.h>
#include <libbasarcmnutil.h>
#include <libbasardbsql.h>
#include <libbasarproperty.h>
#include <libbasardbaspect.h>
#include <libbasarappl.h>
#include <libbasarlogin.h>

#ifdef WIN32
    #include <windows.h>
	
    #pragma warning (push)
	#pragma warning(disable: 4127 4244 4311 4312 4481 4512 4800)
#endif    
	#include <QtCore/QProcess>
	#include <QtCore/QDir>
	#include <QtCore/QTextStream>
	#include <QtCore/QFile>
	#include <QtCore/QDate>
	#include <QtCore/QObject>
#ifdef WIN32
	#pragma warning (pop)
#endif

#include <sstream>
#include <iomanip>

const std::string LOG4C_NARCOTICSMAIN = "transfer2bfarm";

#endif	//GUARD_NARCOTICS_COMMONHEADER_H