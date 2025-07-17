#include "version.h"
#include <libbasar_pkgversion.h>

const char BASAR_versioninfo[]  		 = PRODUCT_NAME " v " PRODUCT_MAJORNO "."PRODUCT_MINORNO "." PRODUCT_EXTENSION "." PRODUCT_BUILDNO PRODUCT_CONFIG;
const char BASAR_versioninfo_for_what[]  = "@(#)" PRODUCT_NAME " v " PRODUCT_MAJORNO "."PRODUCT_MINORNO "." PRODUCT_EXTENSION "." PRODUCT_BUILDNO PRODUCT_CONFIG;
const char versioninfo[]				 = PROGRAM_NAME " v " VERSION_NUMBER;
const char versioninfo_for_what[]		 = "@(#)" PROGRAM_NAME " v " VERSION_NUMBER;
