//----------------------------------------------------------------------------
/** $Id: $
 *  \author Michael Eichenlaub
 *  \date   23.04.2009
*/
//----------------------------------------------------------------------------

#include <UnitTest++/UnitTest++.h>
#include "libbasardbaspect.h"

#include "testi18n.h"

#include "libbasarproperty.h"

#include "testnames.h"
#include "buttablepropertydescriptionvectors.h"

// -----------------------------------------------------------------------------------------------------------------
using basar::VarString;

using basar::property::PropertyDescriptionArray;
using basar::property::PropertyDescriptionVector;

using basar::db::aspect::SQLStringBuilder;

//------------------------------------------------------------------------------
const PropertyDescriptionArray propDescArray_SelectByString =  
{
		PROPDEF_COLVCH3
};
const PropertyDescriptionVector propDescVec_SelectByString(1, propDescArray_SelectByString);

const PropertyDescriptionArray propDescArray_SelectByInt =  
{
		PROPDEF_COLINT
};
const PropertyDescriptionVector propDescVec_SelectByInt(1, propDescArray_SelectByInt);

//------------------------------------------------------------------------------
VarString Sql_SQLBuilderQuery_TestI18n_SelectByString(
	"SELECT colint, colvch3, "
  			"'\\x' || SUBSTR(HEX(ASCII(SUBSTR(colvch3,  1, 1))), 9, 2) || "
			"'\\x' || SUBSTR(HEX(ASCII(SUBSTR(colvch3,  2, 1))), 9, 2) || "
			"'\\x' || SUBSTR(HEX(ASCII(SUBSTR(colvch3,  3, 1))), 9, 2) || "
			"'\\x' || SUBSTR(HEX(ASCII(SUBSTR(colvch3,  4, 1))), 9, 2) || "
			"'\\x' || SUBSTR(HEX(ASCII(SUBSTR(colvch3,  5, 1))), 9, 2) || "
			"'\\x' || SUBSTR(HEX(ASCII(SUBSTR(colvch3,  6, 1))), 9, 2) || "
			"'\\x' || SUBSTR(HEX(ASCII(SUBSTR(colvch3,  7, 1))), 9, 2) || "
			"'\\x' || SUBSTR(HEX(ASCII(SUBSTR(colvch3,  8, 1))), 9, 2) || "
			"'\\x' || SUBSTR(HEX(ASCII(SUBSTR(colvch3,  9, 1))), 9, 2) || "
			"'\\x' || SUBSTR(HEX(ASCII(SUBSTR(colvch3, 10, 1))), 9, 2) || "
			"'\\x' || SUBSTR(HEX(ASCII(SUBSTR(colvch3, 11, 1))), 9, 2) || "
			"'\\x' || SUBSTR(HEX(ASCII(SUBSTR(colvch3, 12, 1))), 9, 2) as " + PROPDEF_COLSERVERSTRING.getName() + " "
	"FROM but_var "
	"WHERE " + PROPDEF_COLVCH3.getName() + " = '" + PROPDEF_COLVCH3.toSQLReplacementString() + "';");

ENSURE_PROPERTIES_ARE_SET(SQLBuilderQuery_TestI18n_SelectByString, propDescVec_SelectByString)
BUILDER_DEFINITION       (SQLBuilderQuery_TestI18n_SelectByString)
SQL_PATTERN              (SQLBuilderQuery_TestI18n_SelectByString, Sql_SQLBuilderQuery_TestI18n_SelectByString.c_str())

//------------------------------------------------------------------------------
VarString Sql_SQLBuilderQuery_TestI18n_SelectByInt(
	"SELECT colvch3, "
  			"'\\x' || SUBSTR(HEX(ASCII(SUBSTR(colvch3,  1, 1))), 9, 2) || "
			"'\\x' || SUBSTR(HEX(ASCII(SUBSTR(colvch3,  2, 1))), 9, 2) || "
			"'\\x' || SUBSTR(HEX(ASCII(SUBSTR(colvch3,  3, 1))), 9, 2) || "
			"'\\x' || SUBSTR(HEX(ASCII(SUBSTR(colvch3,  4, 1))), 9, 2) || "
			"'\\x' || SUBSTR(HEX(ASCII(SUBSTR(colvch3,  5, 1))), 9, 2) || "
			"'\\x' || SUBSTR(HEX(ASCII(SUBSTR(colvch3,  6, 1))), 9, 2) || "
			"'\\x' || SUBSTR(HEX(ASCII(SUBSTR(colvch3,  7, 1))), 9, 2) || "
			"'\\x' || SUBSTR(HEX(ASCII(SUBSTR(colvch3,  8, 1))), 9, 2) || "
			"'\\x' || SUBSTR(HEX(ASCII(SUBSTR(colvch3,  9, 1))), 9, 2) || "
			"'\\x' || SUBSTR(HEX(ASCII(SUBSTR(colvch3, 10, 1))), 9, 2) || "
			"'\\x' || SUBSTR(HEX(ASCII(SUBSTR(colvch3, 11, 1))), 9, 2) || "
			"'\\x' || SUBSTR(HEX(ASCII(SUBSTR(colvch3, 12, 1))), 9, 2) as " + PROPDEF_COLSERVERSTRING.getName() + " "
	"FROM but_var "
	"WHERE " + PROPDEF_COLINT.getName() + " = " + PROPDEF_COLINT.toSQLReplacementString() + ";");

ENSURE_PROPERTIES_ARE_SET(SQLBuilderQuery_TestI18n_SelectByInt, propDescVec_SelectByInt)
BUILDER_DEFINITION       (SQLBuilderQuery_TestI18n_SelectByInt)
SQL_PATTERN              (SQLBuilderQuery_TestI18n_SelectByInt, Sql_SQLBuilderQuery_TestI18n_SelectByInt.c_str())

//------------------------------------------------------------------------------
VarString Sql_SQLBuilderQuery_TestI18n_SelectLow(	
	"SELECT colint, colvch3, lower(colvch3) as low, "
  			"'\\x' || SUBSTR(HEX(ASCII(SUBSTR(colvch3,  1, 1))), 9, 2) || "
			"'\\x' || SUBSTR(HEX(ASCII(SUBSTR(colvch3,  2, 1))), 9, 2) || "
			"'\\x' || SUBSTR(HEX(ASCII(SUBSTR(colvch3,  3, 1))), 9, 2) || "
			"'\\x' || SUBSTR(HEX(ASCII(SUBSTR(colvch3,  4, 1))), 9, 2) || "
			"'\\x' || SUBSTR(HEX(ASCII(SUBSTR(colvch3,  5, 1))), 9, 2) || "
			"'\\x' || SUBSTR(HEX(ASCII(SUBSTR(colvch3,  6, 1))), 9, 2) || "
			"'\\x' || SUBSTR(HEX(ASCII(SUBSTR(colvch3,  7, 1))), 9, 2) || "
			"'\\x' || SUBSTR(HEX(ASCII(SUBSTR(colvch3,  8, 1))), 9, 2) || "
			"'\\x' || SUBSTR(HEX(ASCII(SUBSTR(colvch3,  9, 1))), 9, 2) || "
			"'\\x' || SUBSTR(HEX(ASCII(SUBSTR(colvch3, 10, 1))), 9, 2) || "
			"'\\x' || SUBSTR(HEX(ASCII(SUBSTR(colvch3, 11, 1))), 9, 2) || "
			"'\\x' || SUBSTR(HEX(ASCII(SUBSTR(colvch3, 12, 1))), 9, 2) as " + PROPDEF_COLSERVERSTRING.getName() + " "
	"FROM but_var "
	"WHERE " + PROPDEF_COLVCH3.getName() + " = '" + PROPDEF_COLVCH3.toSQLReplacementString() + "';");

ENSURE_PROPERTIES_ARE_SET(SQLBuilderQuery_TestI18n_SelectLow, propDescVec_SelectByString)
BUILDER_DEFINITION       (SQLBuilderQuery_TestI18n_SelectLow)
SQL_PATTERN              (SQLBuilderQuery_TestI18n_SelectLow, Sql_SQLBuilderQuery_TestI18n_SelectLow.c_str())

//------------------------------------------------------------------------------
VarString Sql_SQLBuilderQuery_TestI18n_SelectFormat(
	"SELECT  colint, "
			"colfl, colsmfl, coldec, colmon, "
			"coldate, coldt1, coldt2, coltime1, coltime2 "
	"FROM but_var "
	"WHERE " + PROPDEF_COLINT.getName() + " = " + PROPDEF_COLINT.toSQLReplacementString() + ";");

ENSURE_PROPERTIES_ARE_SET(SQLBuilderQuery_TestI18n_SelectFormat, propDescVec_SelectByInt)
BUILDER_DEFINITION       (SQLBuilderQuery_TestI18n_SelectFormat)
SQL_PATTERN              (SQLBuilderQuery_TestI18n_SelectFormat, Sql_SQLBuilderQuery_TestI18n_SelectFormat.c_str())

//------------------------------------------------------------------------------
namespace accessor_buttable_i18n
{
	BEGIN_ACCESSOR_DEFINITION(ACCESSORNAME_TestI18n)

		PROPERTY_DESCRIPTION_LIST(propDescVecButTableI18n)

		// ----------------------------------------------------------

		SQL_BUILDER_CREATE(SQLBuilderQuery_TestI18n_SelectByString)
		SQL_BUILDER_CREATE(SQLBuilderQuery_TestI18n_SelectByInt   )
		SQL_BUILDER_CREATE(SQLBuilderQuery_TestI18n_SelectLow     )
		SQL_BUILDER_CREATE(SQLBuilderQuery_TestI18n_SelectFormat  )

		// ----------------------------------------------------------

		ACCESS_METHOD        (ACCESSMETHOD_ButTables_I18n_SelectByString)
		SQL_BUILDER_PUSH_BACK(SQLBuilderQuery_TestI18n_SelectByString   )

		ACCESS_METHOD        (ACCESSMETHOD_ButTables_I18n_SelectByInt)
		SQL_BUILDER_PUSH_BACK(SQLBuilderQuery_TestI18n_SelectByInt   )

		ACCESS_METHOD        (ACCESSMETHOD_ButTables_I18n_SelectLow)
		SQL_BUILDER_PUSH_BACK(SQLBuilderQuery_TestI18n_SelectLow   )

		ACCESS_METHOD        (ACCESSMETHOD_ButTables_I18n_SelectFormat)
		SQL_BUILDER_PUSH_BACK(SQLBuilderQuery_TestI18n_SelectFormat   )

	END_ACCESSOR_DEFINITION
}
