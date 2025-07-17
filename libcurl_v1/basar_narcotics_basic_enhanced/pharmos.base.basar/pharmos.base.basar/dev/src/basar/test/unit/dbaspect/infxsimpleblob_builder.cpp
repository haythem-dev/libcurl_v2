#include "libbasardbaspect.h"

#include "infxsimpleblob_builder.h"

#include <boost/make_shared.hpp>

using basar::db::aspect::SQLStringBuilder;

// Inherited via SQLQueryStringBuilder
const boost::shared_ptr<SQLStringBuilder> MyTextInsertBuilder::create() const /*override*/
{
    return boost::make_shared<MyTextInsertBuilder>( *this );
}

bool MyTextInsertBuilder::isExecutable() const /*override*/
{
    return true;
}
void MyTextInsertBuilder::buildSQLString() /*override*/
{
    std::ostringstream os;

    os << "insert into mytest(mytext) values (";
    os << PROPDEF_MYTEXT.toSQLReplacementString();
    os << ");";

    resolve( os.str() );
}

//--------------------------------------------------------------------------------------------------

// Inherited via SQLQueryStringBuilder
const boost::shared_ptr<SQLStringBuilder> MyNonPreparedTextInsertBuilder::create() const /*override*/
{
    return boost::make_shared<MyNonPreparedTextInsertBuilder>( *this );
}

bool MyNonPreparedTextInsertBuilder::isExecutable() const /*override*/
{
    return true;
}

void MyNonPreparedTextInsertBuilder::buildSQLString() /*override*/
{
    std::ostringstream os;

    os << "insert into mytest(mytext) values (";
    os << PROPDEF_MYTEXT.toSQLReplacementString();
    os << ");";

    resolve( os.str() );
}

//--------------------------------------------------------------------------------------------------

// Inherited via SQLQueryStringBuilder
const boost::shared_ptr<SQLStringBuilder> MyTextSelectBuilder::create() const /*override*/
{
    return boost::make_shared<MyTextSelectBuilder>( *this );
}

bool MyTextSelectBuilder::isExecutable() const /*override*/
{
    return true;
}

void MyTextSelectBuilder::buildSQLString() /*override*/
{
    std::ostringstream os;

    os << "select first 1 mytext as ";
    os << PROPDEF_MYTEXT.getName() << " ";
    os << "from mytest;";

    resolve( os.str() );
}

//--------------------------------------------------------------------------------------------------

// Inherited via SQLQueryStringBuilder
const boost::shared_ptr<SQLStringBuilder> MyBinaryInsertBuilder::create() const /*override*/
{
    return boost::make_shared<MyBinaryInsertBuilder>( *this );
}

bool MyBinaryInsertBuilder::isExecutable() const /*override*/
{
    return true;
}

void MyBinaryInsertBuilder::buildSQLString() /*override*/
{
    std::ostringstream os;

    os << "insert into mytest(mybinary) values (";
    os << PROPDEF_MYBINARY.toSQLReplacementString();
    os << ");";

    resolve( os.str() );
}

//--------------------------------------------------------------------------------------------------

// Inherited via SQLQueryStringBuilder
const boost::shared_ptr<SQLStringBuilder> MyBinarySelectBuilder::create() const /*override*/
{
    return boost::make_shared<MyBinarySelectBuilder>( *this );
}

bool MyBinarySelectBuilder::isExecutable() const /*override*/
{
    return true;
}

void MyBinarySelectBuilder::buildSQLString() /*override*/
{
    std::ostringstream os;

    os << "select first 1 mybinary as ";
    os << PROPDEF_MYBINARY.getName() << " ";
    os << "from mytest;";

    resolve( os.str() );
}