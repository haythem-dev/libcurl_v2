//----------------------------------------------------------------------------
/*! \file
 *  \brief  public library interface for common utilities
 *  \author Thomas Hörath
 *  \date   15.07.2005
 */
//----------------------------------------------------------------------------

#ifndef GUARD_LIBBASARCMNUTIL_H
#define GUARD_LIBBASARCMNUTIL_H

//----------------------------------------------------------------------------
// displays stage of compiling, so not fulfilled precompile conditions can be detected
#ifdef _WIN32
#ifndef COMPILEDHDR__MESSAGE__LIBBASARCMNUTIL_H
#pragma message("    compiling " __FILE__ " ...")
#define COMPILEDHDR__MESSAGE__LIBBASARCMNUTIL_H
#endif
#endif

//----------------------------------------------------------------------------
#include "libbasar_definitions.h"
#include "libbasarcmnutil_definitions.h"
#include "libbasarcmnutil_functions.h"
		
#ifndef LIBBASARCMNUTIL_INCLUDE_LEAN_AND_MEAN
	#include "libbasarcmnutil_locale.h"
	#include "libbasarcmnutil_i18nstring.h"
	#include "libbasarcmnutil_exceptions.h"
	#include "libbasarcmnutil_datetimeexception.h"
	#include "libbasarcmnutil_logging.h"
	#include "libbasarcmnutil_cmdline.h"
	#include "libbasarcmnutil_pattern.h"
	#include "libbasarcmnutil_datetime.h"
	#include "libbasarcmnutil_date.h"
	#include "libbasarcmnutil_datetimetools.h"
    #include "libbasarcmnutil_timespan.h"
	#include "libbasarcmnutil_parameterlist.h"
	#include "libbasarcmnutil_decimal.h"
	#include "libbasarcmnutil_pipedprocess.h"
	#include "libbasarcmnutil_print.h"
	#include "libbasarcmnutil_socket.h"
	#include "libbasarcmnutil_email.h"
	#include "libbasarcmnutil_attachment.h"
	#include "libbasarcmnutil_iattachmentptr.h"
	#include "libbasarcmnutil_iattachment.h"
	#include "libbasarcmnutil_codec.h"
#endif


//----------------------------------------------------------------------------
#endif

