#include "libbasardbaspect.h"

#include "infxint8_builder.h"
#include <boost/make_shared.hpp>

const boost::shared_ptr<basar::db::aspect::SQLStringBuilder> MyInt8InsertBuilder::create() const
{
    return boost::make_shared<MyInt8InsertBuilder>( *this );
}

bool MyInt8InsertBuilder::isExecutable() const /*override*/
{
    return true;
}

void MyInt8InsertBuilder::buildSQLString() /*override*/
{
    std::ostringstream os;

    os << "insert into mytest(myint8) values (";
    os << PROPDEF_MYINT8.toSQLReplacementString();
    os << ");";

    resolve( os.str() );
}

//--------------------------------------------------------------------------

// Inherited via SQLQueryStringBuilder
const boost::shared_ptr<basar::db::aspect::SQLStringBuilder> MyInt8SelectBuilder::create() const /*override*/
{
    return boost::make_shared<MyInt8SelectBuilder>( *this );
}

bool MyInt8SelectBuilder::isExecutable() const /*override*/
{
    return true;
}

void MyInt8SelectBuilder::buildSQLString() /*override*/
{
    std::ostringstream os;

    os << "select first 1 myint8 as ";
    os << PROPDEF_MYINT8.getName() << " ";
    os << "from mytest;";

    resolve( os.str() );
}

//--------------------------------------------------------------------------

// Inherited via SQLQueryStringBuilder
const boost::shared_ptr<basar::db::aspect::SQLStringBuilder> MyInt8PreparedInsertBuilder::create() const /*override*/
{
    return boost::make_shared<MyInt8PreparedInsertBuilder>( *this );
}

bool MyInt8PreparedInsertBuilder::isExecutable() const /*override*/
{
    return true;
}

void MyInt8PreparedInsertBuilder::buildSQLString() /*override*/
{
    std::ostringstream os;

    os << "insert into mytest(myint8) values (";
    os << PROPDEF_MYINT8.toSQLReplacementString();
    os << ");";

    resolve( os.str() );
}

//--------------------------------------------------------------------------

// Inherited via SQLQueryStringBuilder
const boost::shared_ptr<basar::db::aspect::SQLStringBuilder> MyInt8PreparedSelectBuilder::create() const /*override*/
{
    return boost::make_shared<MyInt8PreparedSelectBuilder>( *this );
}

bool MyInt8PreparedSelectBuilder::isExecutable() const /*override*/
{
    return true;
}

void MyInt8PreparedSelectBuilder::buildSQLString() /*override*/
{
    std::ostringstream os;

    os << "select first 1 myint8 as ";
    os << PROPDEF_MYINT8.getName() << " ";
    os << "from mytest;";

    resolve( os.str() );
}

//--------------------------------------------------------------------------

// Inherited via SQLQueryStringBuilder
const boost::shared_ptr<basar::db::aspect::SQLStringBuilder> MyBigIntInsertBuilder::create() const /*override*/
{
    return boost::make_shared<MyBigIntInsertBuilder>( *this );
}

bool MyBigIntInsertBuilder::isExecutable() const /*override*/
{
    return true;
}

void MyBigIntInsertBuilder::buildSQLString() /*override*/
{
    std::ostringstream os;

    os << "insert into mytest(myint8) values (";
    os << PROPDEF_MYINT8.toSQLReplacementString();
    os << ");";

    resolve( os.str() );
}

//--------------------------------------------------------------------------

// Inherited via SQLQueryStringBuilder
const boost::shared_ptr<basar::db::aspect::SQLStringBuilder> MyBigIntSelectBuilder::create() const /*override*/
{
    return boost::make_shared<MyBigIntSelectBuilder>( *this );
}
bool MyBigIntSelectBuilder::isExecutable() const /*override*/
{
    return true;
}

void MyBigIntSelectBuilder::buildSQLString() /*override*/
{
    std::ostringstream os;

    os << "select first 1 myint8 as ";
    os << PROPDEF_MYINT8.getName() << " ";
    os << "from mytest;";

    resolve( os.str() );
}

//--------------------------------------------------------------------------

// Inherited via SQLQueryStringBuilder
const boost::shared_ptr<basar::db::aspect::SQLStringBuilder> MyBigIntPreparedInsertBuilder::create() const /*override*/
{
    return boost::make_shared<MyBigIntPreparedInsertBuilder>( *this );
}

bool MyBigIntPreparedInsertBuilder::isExecutable() const /*override*/
{
    return true;
}

void MyBigIntPreparedInsertBuilder::buildSQLString() /*override*/
{
    std::ostringstream os;

    os << "insert into mytest(myint8) values (";
    os << PROPDEF_MYINT8.toSQLReplacementString();
    os << ");";

    resolve( os.str() );
}

//--------------------------------------------------------------------------

// Inherited via SQLQueryStringBuilder
const boost::shared_ptr<basar::db::aspect::SQLStringBuilder> MyBigIntPreparedSelectBuilder::create() const /*override*/
{
    return boost::make_shared<MyBigIntPreparedSelectBuilder>( *this );
}
bool MyBigIntPreparedSelectBuilder::isExecutable() const /*override*/
{
    return true;
}

void MyBigIntPreparedSelectBuilder::buildSQLString() /*override*/
{
    std::ostringstream os;

    os << "select first 1 myint8 as ";
    os << PROPDEF_MYINT8.getName() << " ";
    os << "from mytest;";

    resolve( os.str() );
}