
#include <libbasardbsql.h>

#include "testdbsql.h"

using basar::db::sql::ConnectionRef;
using basar::db::sql::StatementRef;
using basar::db::sql::PreparedStatementRef;
using basar::db::sql::ResultsetRef;
using basar::db::sql::SqlDiagnosticsRef;

//------------------------------------------------------------------------------
class TestImpl
{
public:
	TestImpl()
	{
	};

	~TestImpl()
	{
	};
};

//------------------------------------------------------------------------------
static void test_shared_ptr()
{
#ifdef XXX
	using boost::shared_ptr;

	shared_ptr<TestImpl> p1 (new TestImpl);
	shared_ptr<TestImpl> p2 (p1);

	p1.reset();

	shared_ptr<TestImpl> p3 (new TestImpl);

	p3.reset(new TestImpl);
#endif
}

//------------------------------------------------------------------------------
static void test_collerase()
{
#ifdef YYY
	printf("test collection erase\n");

	typedef std::deque <int> CollInt;
	
	CollInt collInt;

	collInt.push_back(0);
	collInt.push_back(1);
	collInt.push_back(2);
	collInt.push_back(3);
	collInt.push_back(4);

	CollInt::const_iterator citEnd = collInt.end();
	CollInt::iterator		itConn;

	CollInt::const_iterator citEnd2;
	CollInt::iterator		itConn2;

	citEnd = collInt.end();

	for (itConn = collInt.begin();
		 citEnd != itConn;
		 ++itConn)
	{
		collInt.erase(itConn);

		printf("\nerased\n");

		citEnd2 = collInt.end();
		for (itConn2 = collInt.begin();
			 citEnd2 != itConn2;
			++itConn2)
		{
			printf("item: %ld\n", *itConn2);
		}
	}
#endif
}

//------------------------------------------------------------------------------
void test_classes()
{
	StatementRef  objS1, 
				  objS2(objS1);
	objS2 = objS1;

	ConnectionRef objC1, 
		          objC2(objC1);
	objC2 = objC1;

	ResultsetRef  objR1, 
		          objR2(objR1);
	objR2 = objR1;

	SqlDiagnosticsRef objDi1, 
		              objDi2(objDi1);
	objDi2 = objDi1;
}

//------------------------------------------------------------------------------
void misc()
{
	test_shared_ptr	();
	test_collerase	();
	test_classes	();
}

