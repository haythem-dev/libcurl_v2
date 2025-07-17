#ifndef BASAR_TEST_UNIT_DBASPECT_FLOAT_BUILDER_H
#define BASAR_TEST_UNIT_DBASPECT_FLOAT_BUILDER_H

#include <libbasardbaspect_sqlstringbuilder.h>
#include "libbasarproperty_propertydescription.h"

CONST_PROPERTY_DESCRIPTION( PROPDEF_MYFLOAT, "myfloat", basar::FLOAT64 )
CONST_PROPERTY_DESCRIPTION( PROPDEF_MYSMALLFLOAT, "mysmallfloat", basar::FLOAT32 )

class MyFloatInsertBuilder : public basar::db::aspect::SQLWriteStringBuilder
{
    // Inherited via SQLWriteStringBuilder
    virtual const boost::shared_ptr<SQLStringBuilder> create() const /*override*/;
    virtual bool isExecutable() const /*override*/;
    virtual void buildSQLString() /*override*/;
};

#endif