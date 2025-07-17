//---------------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  definition narcotics version
 *  \author Juri Skuratovski
 *  \date   25.07.2023
 */
//---------------------------------------------------------------------------------------------------------//

#ifndef GUARD_NARCOTICS_ALERTING_VERSION_H
#define GUARD_NARCOTICS_ALERTING_VERSION_H

/*!
\page page2 narcoticsalerting

________________________________________________________________________________________
\version	01.00.00.00
\date		25.07.2023
\internal
- Initial Version.
\endinternal
 */


//! major version number of narcoticsalerting
#define VERSION_MAJORNO           "01"		

//! minor version number of narcoticsalerting
#define VERSION_MINORNO           "00"		

//! extension version number of narcoticsalerting
#define VERSION_EXTENSION         "00"		

//! build version number of narcoticsalerting
#define VERSION_BUILDNO           "00"		

//! version number string of narcoticsalerting
#define VERSIONNO				VERSION_MAJORNO "." VERSION_MINORNO "." VERSION_EXTENSION "." VERSION_BUILDNO


#ifdef NDEBUG
//! version number string release of narcoticsalerting
#define VERSION_NUMBER			VERSIONNO" rel"		
#else
//! version number string debug of narcoticsalerting
#define VERSION_NUMBER			VERSIONNO" dbg"		
#endif

//! narcoticsalerting name
#define PROGRAM_NAME			"narcoticsalerting"	

#endif //GUARD_NARCOTICS_ALERTING_VERSION_H
