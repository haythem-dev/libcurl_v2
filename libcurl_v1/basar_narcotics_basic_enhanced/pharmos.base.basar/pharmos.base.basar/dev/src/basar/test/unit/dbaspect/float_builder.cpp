#include "libbasardbaspect.h"

#include "float_builder.h"

#include <boost/make_shared.hpp>

const boost::shared_ptr<basar::db::aspect::SQLStringBuilder> MyFloatInsertBuilder::create() const
{
    return boost::make_shared<MyFloatInsertBuilder>( *this );
}

bool MyFloatInsertBuilder::isExecutable() const
{
    return true;
}

void MyFloatInsertBuilder::buildSQLString()
{
    std::ostringstream os;

    os << "insert into mytest(myfloat, mysmallfloat) values (";
    os << PROPDEF_MYFLOAT.toSQLReplacementString() << ", ";
    os << PROPDEF_MYSMALLFLOAT.toSQLReplacementString();
    os << ");";

    resolve(os.str());

}
