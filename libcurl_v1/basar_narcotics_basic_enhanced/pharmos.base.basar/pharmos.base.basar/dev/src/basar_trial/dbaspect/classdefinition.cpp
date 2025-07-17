#include "usings.h"
#include "classdefinition.h"


std::pair<bool, basar::db::aspect::QueryTypeEnum> mySQLBuilder1::isQuery() const
{
	return std::make_pair<bool, basar::db::aspect::QueryTypeEnum>( true, basar::db::aspect::MULTI_QUERY );
}

const bool mySQLBuilder1::isExecutable() const
{
	return true;
}

void mySQLBuilderUpdate::buildSQLString()
{
	literal("test ");
	value("test");
}

const bool mySQLBuilderUpdate::isExecutable() const
{
	return true;
}

void mySQLBuilderQuery::buildSQLString()
{
	resolve("test ?test?");
}

const bool mySQLBuilderQuery::isExecutable() const
{
	return true;
}

void mySQLBuilder1::buildSQLString()
{
	resolve("test ?test? test");
}

const boost::shared_ptr<SQLStringBuilder> mySQLBuilder1::create() const
{
	return boost::shared_ptr<SQLStringBuilder>(new mySQLBuilder1(*this));
}


const boost::shared_ptr<SQLStringBuilder> mySQLBuilderUpdate::create() const
{
	return boost::shared_ptr<SQLStringBuilder>(new mySQLBuilderUpdate(*this));
}


const boost::shared_ptr<SQLStringBuilder> mySQLBuilderQuery::create() const
{ 
	return boost::shared_ptr<SQLStringBuilder>(new mySQLBuilderQuery(*this));
}

