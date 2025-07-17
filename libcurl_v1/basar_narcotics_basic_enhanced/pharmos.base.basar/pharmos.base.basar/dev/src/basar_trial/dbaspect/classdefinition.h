#include <memory>

class SQLStringBuilder;
class SQLQueryStringBuilder;
class SQLWriteStringBuilder;

class mySQLBuilder1 : public SQLStringBuilder
{
public:
	mySQLBuilder1()
	{}
	const boost::shared_ptr<SQLStringBuilder> create() const;
	std::pair<bool, basar::db::aspect::QueryTypeEnum> isQuery() const;
protected:
	virtual const bool isExecutable() const;
	virtual void buildSQLString();
};

class mySQLBuilderUpdate : public SQLWriteStringBuilder
{
public:
	const boost::shared_ptr<SQLStringBuilder> create() const;
protected:
	virtual const bool isExecutable() const;
	virtual void buildSQLString();
};

class mySQLBuilderQuery : public SQLQueryStringBuilder
{
public:
	const boost::shared_ptr<SQLStringBuilder> create() const;
protected:
	virtual const bool isExecutable() const;
	virtual void buildSQLString();
};

