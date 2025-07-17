//----------------------------------------------------------------------------
/** $Id: $
 *  \author Michael Eichenlaub
 *  \date   23.04.2008
*/
//----------------------------------------------------------------------------

#include <UnitTest++/UnitTest++.h>
#include "libbasardbaspect.h"

#include "libbasarproperty.h"

#include "testfixedstrings.h"

#include "testnames.h"
#include "buttablepropertydescriptionvectors.h"

// -----------------------------------------------------------------------------------------------------------------
using basar::VarString;
using basar::db::aspect::SQLStringBuilder;

//------------------------------------------------------------------------------

	VarString Sql_SQLBuilderQuery_TestFixedStrings_Select_a("SELECT FIRST 1 " 
															"NVL(m.colint, '  ')               as " + PROPDEF_COLNVL1.getName() + ", "
															"SUBSTR(NVL(m.colint, '  '), 1, 2) as " + PROPDEF_COLNVL2.getName() + " "
															"FROM but_detail d, OUTER but_master m "
															"WHERE d.colser = m.colser "
															"AND d.colser > 10;");
	DONT_CHECK_PROPERTIES(SQLBuilderQuery_TestFixedStrings_Select_a)
    BUILDER_DEFINITION(SQLBuilderQuery_TestFixedStrings_Select_a)
    SQL_PATTERN(SQLBuilderQuery_TestFixedStrings_Select_a, Sql_SQLBuilderQuery_TestFixedStrings_Select_a.c_str())

namespace accessor_buttable_fixedstrings
{
	BEGIN_ACCESSOR_DEFINITION(ACCESSORNAME_ButTable_FixedStrings)

		PROPERTY_DESCRIPTION_LIST(propertyDescriptionVectorButTableDescriptionFixedStrings)

		// ----------------------------------------------------------

		SQL_BUILDER_CREATE(SQLBuilderQuery_TestFixedStrings_Select_a)

		// ----------------------------------------------------------

		ACCESS_METHOD(ACCESSMETHOD_ButTables_FixedStrings_Select_a)
		SQL_BUILDER_PUSH_BACK(SQLBuilderQuery_TestFixedStrings_Select_a)

	END_ACCESSOR_DEFINITION
}
