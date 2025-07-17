//----------------------------------------------------------------------------
/*! \file
 *  \brief  standard library functions
 *  \author Roland Kiefert
 *  \date   15.07.2005
 */
//----------------------------------------------------------------------------

#include "libbasarguitie_functions.h"

#include "libbasar_pkgversion.h"

#pragma warning (push)
#pragma warning (disable: 4127 4512)
#include <QtWidgets/QDialog>
#pragma warning (pop)

/*----------------------------------------------------------------------------*/
//! component name
#define PROGRAM_NAME			"libbasarguitie"	

/*----------------------------------------------------------------------------*/
namespace basar {
namespace gui {
namespace tie {

/*----------------------------------------------------------------------------*/
//! in binary embedded full version string
static ConstString s_SCCSID =	"@(#) "			COMPANYNAME 
								" version: "	PROGRAM_NAME 
								 " "			PRODUCT_NUMBER 
								" built:   "	__DATE__ 
								" "				__TIME__;

//---------------------------------------------------------------------------
//! \return	string with package's version
//! \throw  no-throw
ConstString getVersion()
{
	return PRODUCT_NUMBER;
}

//---------------------------------------------------------------------------
//! \return	string with company, programname, package, component and builddate info
//! \throw  no-throw
ConstString getFullVersion()
{
	return s_SCCSID;
}

//---------------------------------------------------------------------------
//! \throw  no-throw
//! \return button that was pressed on dialog
const WidgetReturnEnum	getWidgetReturnType	(basar::Int32 retCode)
{
	if (retCode == QDialog::Accepted)
		return WIDGET_OK;
	else
		return WIDGET_CANCELED;
}

//---------------------------------------------------------------------------
}// tie
}// gui
}// basar
