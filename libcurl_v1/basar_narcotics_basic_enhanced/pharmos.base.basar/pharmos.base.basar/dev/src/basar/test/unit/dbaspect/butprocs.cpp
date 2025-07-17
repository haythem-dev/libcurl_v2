//----------------------------------------------------------------------------
/** $Id $
 *  \author Michael Eichenlaub
 *  \date   21.05.2008
*/
//----------------------------------------------------------------------------

#include <UnitTest++/UnitTest++.h>
#include "libbasardbaspect.h"

#include "butprocs.h"
#include "libbasarproperty.h"
#include "testnames.h"
#include "buttablepropertydescriptions.h"
#include "buttablepropertydescriptionvectors.h"

// -----------------------------------------------------------------------------------------------------------------
using basar::db::aspect::SQLStringBuilder;

// -----------------------------------------------------------------------------------------------------------------

	DONT_CHECK_PROPERTIES(SQLBuilderExecProc1)
    BUILDER_DEFINITION(SQLBuilderExecProc1)
    SQL_PATTERN(SQLBuilderExecProc1, "EXECUTE PROCEDURE but_proc1(4);")

	DONT_CHECK_PROPERTIES(SQLBuilderExecFunc2)
    BUILDER_DEFINITION(SQLBuilderExecFunc2)
    SQL_PATTERN(SQLBuilderExecFunc2, "EXECUTE FUNCTION but_func2(6);")

	DONT_CHECK_PROPERTIES(SQLBuilderExecFunc3)
    BUILDER_DEFINITION(SQLBuilderExecFunc3)
    SQL_PATTERN(SQLBuilderExecFunc3, "EXECUTE FUNCTION but_func3(13);")

    BUILDER_DEFINITION(SQLBuilderExecFunc2WithVariable)
    DONT_CHECK_PROPERTIES(SQLBuilderExecFunc2WithVariable)

    void SQLBuilderExecFunc2WithVariable::buildSQLString()
    {
        std::ostringstream os;
        os << "EXECUTE FUNCTION but_func2(" << PROPDEF_COLRET0.toSQLReplacementString() << ");";

        resolve( os.str() );
    }


namespace accessor_butprocs_storedprocs
{
	BEGIN_ACCESSOR_DEFINITION(ACCESSORNAME_ButProcs_Stored_Procs)

		PROPERTY_DESCRIPTION_LIST(propDescVecButProcsStoredProc)

		// ----------------------------------------------------------

		SQL_BUILDER_CREATE(SQLBuilderExecProc1)
        SQL_BUILDER_CREATE(SQLBuilderExecFunc2WithVariable)
		SQL_BUILDER_CREATE(SQLBuilderExecFunc2)
		SQL_BUILDER_CREATE(SQLBuilderExecFunc3)

		// ----------------------------------------------------------

		ACCESS_METHOD(ACCESSMETHOD_ButProcs_ExecProc1)
		SQL_BUILDER_PUSH_BACK(SQLBuilderExecProc1)

        ACCESS_METHOD(ACCESSMETHOD_ButProcs_ExecFunc2_WithVariable)
        SQL_BUILDER_PUSH_BACK(SQLBuilderExecFunc2WithVariable);

		ACCESS_METHOD(ACCESSMETHOD_ButProcs_ExecFunc2)
		SQL_BUILDER_PUSH_BACK(SQLBuilderExecFunc2)

		ACCESS_METHOD(ACCESSMETHOD_ButProcs_ExecFunc3)
		SQL_BUILDER_PUSH_BACK(SQLBuilderExecFunc3)

	END_ACCESSOR_DEFINITION
}

