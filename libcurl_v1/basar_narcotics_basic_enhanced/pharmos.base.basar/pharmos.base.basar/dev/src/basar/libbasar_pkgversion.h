//----------------------------------------------------------------------------
/*! \file
 *  \brief   definition of package version
 *  \author  Thomas Hoerath
 *  \date    15.07.2005 (created)
 *  \version 01.13.00.00 - 2019-12-20
 *
 *  \mainpage
 *
 *  \htmlinclude changelog.txt
 */
//----------------------------------------------------------------------------

#ifndef GUARD_PKG_VERSION_H
#define GUARD_PKG_VERSION_H

// don't forget to update doxygen\doxyfile and ..\ivy.xml and data\version.inc too

#define PRODUCT_MAJORNO            "01"		//!< major version number of basar library
#define PRODUCT_MINORNO            "14"		//!< minor version number of basar library
#define PRODUCT_EXTENSION          "01"		//!< extension version number of basar library
#define PRODUCT_BUILDNO            "02"		//!< build version number of basar library

#define PRODUCT_VERSION            1140101	//!< version as integer as follows: major w/o leading zero, minor, extension, build

/*! \ingroup DIFF_DBG_REL */
#ifdef NDEBUG
//! configuration
#define PRODUCT_CONFIG             " rel"
#else
//! configuration
#define PRODUCT_CONFIG             " dbg"
#endif

#if _MSC_VER == 1600
#define PRODUCT_BUILD_TYPE         " VC10"
#elif _MSC_VER == 1900
#define PRODUCT_BUILD_TYPE         " VC14"
#else
#define PRODUCT_BUILD_TYPE         ""
#endif

#define PRODUCT_NUMBER             PRODUCT_MAJORNO   "." \
                                   PRODUCT_MINORNO   "." \
                                   PRODUCT_EXTENSION "." \
                                   PRODUCT_BUILDNO       \
                                   PRODUCT_BUILD_TYPE    \
                                   PRODUCT_CONFIG				//!< version number string of basar library


#define PRODUCT_NAME              "Library Basar"				//!< package name

#define COMPANYNAME               "PHOENIX Group IT GmbH"		//!< company name
#define COPYRIGHT                 "Copyright © 2016-2023 by"	//!< copyright

#endif

